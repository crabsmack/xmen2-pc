/* main.cpp - the native runner's host side (phase 5 P6): XMen2Native.exe ROM [options].
 *
 * Reads the user's ROM (SHA-1 checked: R2, nothing ROM-derived ships), powers the machine on and
 * calls the game's reset entry, which never returns: the game calls the platform at every sync
 * point (machine.cpp plat_sync_point), and the runner frame's host work happens there.
 *
 * With no --benchmark it is played: a window, the keyboard and game controllers, the sound
 * (host_sdl.cpp), at the NTSC frame rate.
 * Options (the recomp runners' names, as tools/xmen2check.py trace drives them):
 *   --benchmark N        play N runner frames headless, then print GENESISRECOMP_BENCHMARK {json}
 *   --quit-after N       (played) quit after N frames; --screenshot F: the last frame as a BMP first
 *   --input-script F     the pads per runner frame: `PADS p1 p2` (hex GPAD_* masks), `WAIT n`
 *   --hash-frames K      [FBHASH] frame=f w=W h=H hash=0x... every K runner frames (FNV-1a-64 of
 *                        the framebuffer's active region, as main.c framebuf_active_hash)
 *   --fresh-evidence     accepted (nothing is written beside the runner)
 *   --options K=V,...    the enhancement options (phase 6, options.cpp; also xmen2.ini beside the
 *                        runner); --options --json lists them and exits
 * Environment: XMEN2_SYNC_CSV=path (sync rows from the export image), XMEN2_SYNC_DUMP=n,m +
 * XMEN2_DUMP_DIR (sNNNNNN.ram/.vram), XMEN2_STACK_LOW (as game/xmen2_spec.c).
 * At exit stderr gets `[xmen2] counters {...}` with the export's counts.
 */
#include <chrono>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "host_sdl.h"
#include "native.h"
#include "xmen2_image.h"

extern "C" {
#include "audio/event_queue.h"
#include "audio/mixer.h"
#include "video/genesis_machine.h"
}

uint8_t g_rom[0x400000];

extern "C" void xm_export(XmImage *im);
extern "C" int xm_export_stats(char *buf, int n);
extern "C" int xm_mem_census(char *buf, int n);

/* ---- SHA-1 (the ROM check) ------------------------------------------------------------------ */

static void sha1(const uint8_t *data, size_t len, uint8_t out[20])
{
    uint32_t h[5] = { 0x67452301u, 0xEFCDAB89u, 0x98BADCFEu, 0x10325476u, 0xC3D2E1F0u };
    std::vector<uint8_t> m(data, data + len);
    uint64_t bits = (uint64_t)len * 8u;
    m.push_back(0x80);
    while (m.size() % 64 != 56) m.push_back(0);
    for (int i = 7; i >= 0; i--) m.push_back((uint8_t)(bits >> (8 * i)));
    for (size_t off = 0; off < m.size(); off += 64) {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
            w[i] = (uint32_t)m[off + 4 * i] << 24 | (uint32_t)m[off + 4 * i + 1] << 16 |
                   (uint32_t)m[off + 4 * i + 2] << 8 | m[off + 4 * i + 3];
        for (int i = 16; i < 80; i++) { uint32_t t = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]; w[i] = t << 1 | t >> 31; }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int i = 0; i < 80; i++) {
            uint32_t f, k;
            if (i < 20)      { f = (b & c) | (~b & d);          k = 0x5A827999u; }
            else if (i < 40) { f = b ^ c ^ d;                   k = 0x6ED9EBA1u; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDCu; }
            else             { f = b ^ c ^ d;                   k = 0xCA62C1D6u; }
            uint32_t t = (a << 5 | a >> 27) + f + e + k + w[i];
            e = d; d = c; c = b << 30 | b >> 2; b = a; a = t;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++) out[4 * i + j] = (uint8_t)(h[i] >> (24 - 8 * j));
}

static const char ROM_SHA1[] = "61409e6cf6065ab67d8952b891d8edcf47777193";

static void load_rom(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "[native] cannot open the ROM %s\n", path); exit(2); }
    std::vector<uint8_t> rom;
    uint8_t buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) rom.insert(rom.end(), buf, buf + n);
    fclose(f);
    uint8_t d[20];
    sha1(rom.data(), rom.size(), d);
    char hex[41];
    for (int i = 0; i < 20; i++) snprintf(hex + 2 * i, 3, "%02x", d[i]);
    if (strcmp(hex, ROM_SHA1)) {
        fprintf(stderr, "[native] %s is not X-Men 2: Clone Wars (USA/Europe): SHA-1 %s, expected %s\n", path, hex, ROM_SHA1);
        exit(2);
    }
    memset(g_rom, 0, sizeof g_rom);
    memcpy(g_rom, rom.data(), rom.size() < sizeof g_rom ? rom.size() : sizeof g_rom);
}

/* ---- sync rows (as game/xmen2_spec.c: from the export image only) --------------------------- */

static uint32_t s_crc_table[256];

static void crc_init(void)
{
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        s_crc_table[i] = c;
    }
}

static uint32_t crc32_update(uint32_t c, const uint8_t *p, size_t n)
{
    while (n--) c = s_crc_table[(c ^ *p++) & 0xFF] ^ (c >> 8);
    return c;
}

static uint32_t crc32_bytes(const uint8_t *p, size_t n) { return crc32_update(0xFFFFFFFFu, p, n) ^ 0xFFFFFFFFu; }

/* CRAM / VSRAM words big-endian and masked, as the references' dumps hold them */
extern "C" void xmen2_words_be(uint8_t *out, const uint16_t *w, size_t n, uint16_t mask)
{
    for (size_t i = 0; i < n; i++) {
        uint16_t v = (uint16_t)(w[i] & mask);
        out[2 * i] = (uint8_t)(v >> 8);
        out[2 * i + 1] = (uint8_t)v;
    }
}

#define XMEN2_STACK_LOW 0x00FFAF1Au  /* lowest A7 of the corpus: the TAS */
#define XMEN2_STACK_TOP 0x00FFC518u  /* the reset SSP */
static uint32_t s_stack_low = XMEN2_STACK_LOW;
static FILE *s_sync;
static uint64_t s_sync_rows;
static XmImage s_image;

static uint32_t ram_crc_masked(const uint8_t *ram)
{
    static const uint8_t zeros[256] = { 0 };
    uint32_t lo = s_stack_low & 0xFFFFu, hi = XMEN2_STACK_TOP & 0xFFFFu;
    if ((s_stack_low & 0xFF0000u) != 0xFF0000u || hi < lo) hi = lo;
    uint32_t c = crc32_update(0xFFFFFFFFu, ram, lo);
    for (uint32_t n = hi - lo; n; ) {
        uint32_t k = n > sizeof zeros ? (uint32_t)sizeof zeros : n;
        c = crc32_update(c, zeros, k);
        n -= k;
    }
    c = crc32_update(c, ram + hi, 0x10000u - hi);
    return c ^ 0xFFFFFFFFu;
}

static void maybe_dump_sync(uint64_t sync)
{
    const char *list = getenv("XMEN2_SYNC_DUMP"), *dir = getenv("XMEN2_DUMP_DIR");
    if (!list || !dir) return;
    for (const char *s = list; *s; ) {
        char *e;
        unsigned long long n = strtoull(s, &e, 10);
        if (e == s) break;
        if (n == sync) {
            static uint8_t ram[0x10000];
            memcpy(ram, s_image.ram, sizeof ram);
            uint32_t lo = s_stack_low & 0xFFFFu, hi = XMEN2_STACK_TOP & 0xFFFFu;
            if ((s_stack_low & 0xFF0000u) == 0xFF0000u && hi > lo) memset(ram + lo, 0, hi - lo);
            char path[600];
            snprintf(path, sizeof path, "%s/s%06llu.ram", dir, n);
            if (FILE *f = fopen(path, "wb")) { fwrite(ram, 1, sizeof ram, f); fclose(f); }
            snprintf(path, sizeof path, "%s/s%06llu.vram", dir, n);
            if (FILE *f = fopen(path, "wb")) { fwrite(s_image.vram, 1, sizeof s_image.vram, f); fclose(f); }
        }
        s = *e ? e + 1 : e;
    }
}

/* phase 6's `enhance` counters (P6-enhance.md "The contract"), from the game's own state at each
   logic frame: a sync point inside the game's frame loops (g_InLevelLoop, $FFC942: Level_Run's
   Level_FrameLoop and PlayerSelect_Run; not loads and fades, where Level_Load leaves
   g_TwoPlayerStep at 1 in one-player games too): g_PlayersAlive ($FFC920), g_TwoPlayerStep ($FFC922) */
static uint64_t s_two_player_frames, s_two_player_step_frames;

extern "C" void plat_sync_row(uint64_t sync, uint64_t frame)
{
    s_sync_rows = sync;
    if (xm_ram_peek8(0xC942)) {   /* g_InLevelLoop: a pass of Level_FrameLoop / PlayerSelect_Run */
        if (xm_ram_peek8(0xC920) == 2) s_two_player_frames++;
        if (xm_ram_peek8(0xC922)) s_two_player_step_frames++;
    }
    if (!s_sync && !getenv("XMEN2_SYNC_DUMP")) return;   /* a played run compares nothing */
    xm_export(&s_image);
    maybe_dump_sync(sync);
    if (!s_sync) return;
    fprintf(s_sync, "%llu,%llu,%08x,%08x,%08x,%08x\n", (unsigned long long)sync, (unsigned long long)frame,
            ram_crc_masked(s_image.ram), crc32_bytes(s_image.vram, sizeof s_image.vram),
            crc32_bytes(s_image.cram, sizeof s_image.cram), crc32_bytes(s_image.vsram, sizeof s_image.vsram));
}

static void report_counters(void)
{
    if (s_sync) fflush(s_sync);
    char exported[640];
    xm_export_stats(exported, (int)sizeof exported);
    char memory[256];
    xm_mem_census(memory, (int)sizeof memory);
    char enhance[4096];
    std::string more;
    if (g_opt.high_fps || present_logging()) more += ", \"present\": " + present_counters_json();
    if (g_opt.widescreen) more += ", \"ws\": " + ws_counters_json();
    snprintf(enhance, sizeof enhance, "{\"two_player_frames\": %llu, \"two_player_step_frames\": %llu, "
             "\"frame_tick_waits\": %llu%s}", (unsigned long long)s_two_player_frames,
             (unsigned long long)s_two_player_step_frames, (unsigned long long)machine_frame_tick_waits(),
             more.c_str());
    /* no dispatch, no interpreter, no stand-ins: the native runner has no 68000 code to miss */
    fprintf(stderr, "[xmen2] counters {\"dispatch_misses\": 0, \"interp_instructions\": 0, \"stub_hits\": 0, "
                    "\"sync_points\": %llu, \"a7_min\": null, \"stack_low\": %u, \"exported\": %s, \"memory\": %s, "
                    "\"options\": %s, \"enhance\": %s}\n",
            (unsigned long long)s_sync_rows, (unsigned)(s_stack_low & 0xFFFFFFu), exported, memory,
            options_effective_json().c_str(), enhance);
    fflush(stderr);
}

/* ---- the picture ---------------------------------------------------------------------------- */

#define FB_W 512
#define FB_H 480
static uint32_t s_fb[FB_H][FB_W];
static uint32_t s_ifb[FB_H][FB_W];          /* high_fps: an in-between frame (present.cpp) */
static uint32_t (*s_target)[FB_W] = s_fb;
static uint32_t s_hash_frames;

extern "C" void plat_scanline(int row, const uint32_t *argb, int width)
{
    if (row < 0 || row >= FB_H) return;
    int n = width < FB_W ? width : FB_W;
    memcpy(s_target[row], argb, (size_t)n * sizeof(uint32_t));
}

extern "C" void plat_scan_target(uint32_t *fb) { s_target = fb ? (uint32_t (*)[FB_W])fb : s_fb; }

static uint64_t fb_hash(int w, int h)
{
    uint64_t x = 0xCBF29CE484222325ULL;
    for (int y = 0; y < h && y < FB_H; y++) {
        const uint8_t *p = (const uint8_t *)s_fb[y];
        for (int i = 0; i < w * 4; i++) { x ^= p[i]; x *= 0x100000001B3ULL; }
    }
    return x;
}

/* ---- playing: the window, input and sound (host_sdl.cpp); headless runs have none ----------- */

static int s_interactive;

extern "C" void plat_audio_write(uint32_t stamp, int port, uint8_t v)
{
    if (s_interactive) audio_event_push(stamp, (uint8_t)port, v);
}

extern "C" void plat_frame_audio(uint64_t frame)
{
    if (s_interactive) host_audio(frame);
}

extern "C" int plat_realtime(void) { return s_interactive; }

extern "C" void plat_after_render(void)
{
    present_after_render(machine_frame_no(), s_interactive != 0, &s_ifb[0][0], FB_W);
}

/* ---- input script --------------------------------------------------------------------------- */

struct PadSpan { uint16_t pad[2]; uint64_t frames; };
static std::vector<PadSpan> s_script;

static void load_script(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[native] cannot open the input script %s\n", path); exit(2); }
    char line[512];
    uint16_t cur[2] = { 0, 0 };
    while (fgets(line, sizeof line, f)) {
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (!*s || *s == '#' || *s == '\n' || *s == '\r') continue;
        char op[16] = "";
        unsigned a = 0, b = 0;
        int k = sscanf(s, "%15s %x %x", op, &a, &b);
        if (!strcmp(op, "PADS") && k >= 2) { cur[0] = (uint16_t)a; cur[1] = (uint16_t)(k >= 3 ? b : 0); continue; }
        unsigned long long n = 0;
        if (!strcmp(op, "WAIT") && sscanf(s, "%15s %llu", op, &n) == 2) {
            s_script.push_back(PadSpan{ { cur[0], cur[1] }, n });
            continue;
        }
        fprintf(stderr, "[native] input script %s: unsupported line: %s", path, line);
        exit(2);
    }
    fclose(f);
}

static uint64_t s_flip;   /* --flip N: player 1's C button inverted in runner frames N to N+7 (phase 6 E4) */

extern "C" void plat_frame_begin(uint64_t frame)
{
    uint64_t at = 0;
    uint16_t pad[2] = { 0, 0 };
    if (s_interactive && s_script.empty()) {
        host_input(pad);
    } else if (!s_script.empty()) {   /* the span covering this frame; after the script, its last pads */
        pad[0] = s_script.back().pad[0];
        pad[1] = s_script.back().pad[1];
        for (const PadSpan &p : s_script) {
            if (frame <= at + p.frames) { pad[0] = p.pad[0]; pad[1] = p.pad[1]; break; }
            at += p.frames;
        }
    }
    if (s_flip && frame >= s_flip && frame < s_flip + 8) pad[0] ^= GPAD_C;   /* --flip: the latency probe */
    g_machine.bus.pad[0] = pad[0];
    g_machine.bus.pad[1] = pad[1];
}

/* ---- frames --------------------------------------------------------------------------------- */

static uint64_t s_benchmark, s_quit_after;
static const char *s_screenshot;
static std::chrono::steady_clock::time_point s_t0;

/* XMEN2_FRAME_TIMES=path: each runner frame's wall time (microseconds, the game's logic, the
   export, the Z80 and the render; with --benchmark nothing else), for finding slow scenes */
static FILE *s_ftimes;
static std::chrono::steady_clock::time_point s_tlast;

extern "C" void plat_frame_repeat(void)
{
    if (s_interactive) host_present(&s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
}

/* XMEN2_SHOTS=n,m,... + XMEN2_DUMP_DIR: the picture of those runner frames as fNNNNNN.bmp (headless
   too; for looking at what a trace did) */
static void maybe_shot(uint64_t frame)
{
    static const char *list = getenv("XMEN2_SHOTS"), *dir = getenv("XMEN2_DUMP_DIR");
    if (!list || !dir) return;
    for (const char *s = list; *s; ) {
        char *e;
        unsigned long long n = strtoull(s, &e, 10);
        if (e == s) break;
        if (n == frame) {
            char path[600];
            snprintf(path, sizeof path, "%s/f%06llu.bmp", dir, n);
            host_screenshot(path, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
        }
        s = *e ? e + 1 : e;
    }
}

extern "C" void plat_frame_end(uint64_t frame)
{
    maybe_shot(frame);
    if (s_ftimes) {
        auto now = std::chrono::steady_clock::now();
        fprintf(s_ftimes, "%llu,%lld\n", (unsigned long long)frame,
                (long long)std::chrono::duration_cast<std::chrono::microseconds>(now - s_tlast).count());
        s_tlast = now;
    }
    if (s_hash_frames && frame % s_hash_frames == 0) {
        int w = machine_screen_width(), h = machine_screen_height();
        fprintf(stderr, "[FBHASH] frame=%llu w=%d h=%d hash=0x%016llX\n", (unsigned long long)frame, w, h,
                (unsigned long long)fb_hash(w, h));
    }
    /* the present clock (--present-log, high_fps: present.cpp shows the frame at its time) */
    bool shown = present_whole(frame, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height(), s_interactive != 0);
    if (s_interactive && !shown) host_present(&s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
    if (s_quit_after && frame >= s_quit_after) {   /* a played run's smoke test */
        if (s_screenshot) host_screenshot(s_screenshot, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
        exit(0);
    }
    if (s_benchmark && frame >= s_benchmark) {
        double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - s_t0).count();
        printf("GENESISRECOMP_BENCHMARK {\"game\":\"xmen2\",\"frames\":%llu,\"seconds\":%.9f,\"fps\":%.3f,"
               "\"ms_per_frame\":%.6f,\"requested_frames\":%llu,\"complete\":true,\"runner\":\"native\"}\n",
               (unsigned long long)frame, s, s > 0 ? (double)frame / s : 0.0, frame ? s * 1000.0 / (double)frame : 0.0,
               (unsigned long long)s_benchmark);
        fflush(stdout);
        exit(0);
    }
}

int main(int argc, char **argv)
{
    const char *rom = nullptr, *script = nullptr, *present_log = nullptr;
    options_init(&argc, argv);   /* --options (phase 6), xmen2.ini; --options --json exits here */
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--benchmark") && i + 1 < argc) s_benchmark = strtoull(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--input-script") && i + 1 < argc) script = argv[++i];
        else if (!strcmp(argv[i], "--hash-frames") && i + 1 < argc) s_hash_frames = (uint32_t)strtoul(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--fresh-evidence")) {}
        else if (!strcmp(argv[i], "--quit-after") && i + 1 < argc) s_quit_after = strtoull(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--screenshot") && i + 1 < argc) s_screenshot = argv[++i];
        else if (!strcmp(argv[i], "--present-log") && i + 1 < argc) present_log = argv[++i];
        else if (!strcmp(argv[i], "--flip") && i + 1 < argc) s_flip = strtoull(argv[++i], nullptr, 10);
        else if (argv[i][0] != '-' && !rom) rom = argv[i];
        else { fprintf(stderr, "[native] unknown option %s\n", argv[i]); return 2; }
    }
    if (!rom) { fprintf(stderr, "usage: XMen2Native ROM [--benchmark N] [--input-script F] [--hash-frames K]\n"); return 2; }
    load_rom(rom);
    if (!s_benchmark) {   /* played: a window, the pads, the sound */
        if (!host_open("X-Men 2: Clone Wars")) return 3;
        s_interactive = 1;
        audio_mixer_init();
    }
    if (script) load_script(script);
    present_init(present_log);
    crc_init();
    if (const char *sl = getenv("XMEN2_STACK_LOW"); sl && *sl) s_stack_low = (uint32_t)strtoul(sl, nullptr, 0);
    if (const char *p = getenv("XMEN2_SYNC_CSV"); p && *p) {
        s_sync = fopen(p, "w");
        if (!s_sync) { fprintf(stderr, "[xmen2] cannot open XMEN2_SYNC_CSV %s\n", p); return 2; }
        setvbuf(s_sync, nullptr, _IOFBF, 1 << 20);
        fprintf(s_sync, "sync,frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32\n");
    }
    atexit(report_counters);
    xm_native_mem_init();
    g_machine.bus.pad_type[0] = g_machine.bus.pad_type[1] = 1;   /* 6-button pads, as the runners' settings */
    s_t0 = s_tlast = std::chrono::steady_clock::now();
    if (const char *p = getenv("XMEN2_FRAME_TIMES"); p && *p) {
        if ((s_ftimes = fopen(p, "w"))) fprintf(s_ftimes, "frame,us\n");
    }
    machine_power_on();
    g_machine.bus.pad_type[0] = g_machine.bus.pad_type[1] = 1;
    xm_entry_point();
    fprintf(stderr, "[native] the game's entry point returned\n");
    return 4;
}
