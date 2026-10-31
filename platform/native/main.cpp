/* main.cpp - the native runner's host side (phase 5 P6): XMen2Native.exe ROM [options].
 *
 * Reads the user's ROM (SHA-1 checked: R2, nothing ROM-derived ships; with none on the command line,
 * xmen2.ini's [rom] path, the ROM beside the program, or, played, a file picker: find_rom), powers the machine on and
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
 *   --no-render          (--benchmark) the sync points render no frame: a trace that compares no picture
 *   --no-pixels          (--benchmark) the run as asked, all of it but the pixels: no frame is drawn and
 *                        every picture hash (the [FBHASH] lines, present.csv, probe.csv) is 0. The state
 *                        files of a run with outputs, from a run that skips its pictures (--state-at:
 *                        trace-group's producer; the rest of a state is the same as the drawing run's)
 *   --hash-out F         the [FBHASH] lines to F instead of stderr
 *   --branch-plan F      (--benchmark) other runs that start with this one's pads, played from saves of
 *                        this run (`AT TOTAL DIR SCRIPT`: see run_branches)
 *   --state-at F         (--benchmark) states for other processes at runner frames (`AT PATH`: "State files")
 *   --state-load F       (--benchmark) start from such a state, as the branch from there would (with
 *                        --flip / --flip-scan: a state of a frame before the first flipped one)
 *   --options K=V,...    the enhancement options (phase 6, options.cpp; also xmen2.ini beside the
 *                        runner); --options --json lists them and exits
 *   --record-inputs F    (played, netplay too) the pads of every frame, as a replayable input script
 *                        (capture.cpp; also XMEN2_CAPTURE, XMEN2_STEM, XMEN2_SND_LOG there)
 * Environment: XMEN2_SYNC_CSV=path (sync rows from the export image), XMEN2_SYNC_DUMP=n,m +
 * XMEN2_DUMP_DIR (sNNNNNN.ram/.vram), XMEN2_STACK_LOW (as game/xmen2_spec.c), XMEN2_RENDER_US=path
 * (at exit: the render times behind the counters' render_us_p99, uint32 each, a loaded run's own).
 * At exit stderr gets `[xmen2] counters {...}` with the export's counts.
 */
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <system_error>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "host_sdl.h"
#include "native.h"
#include "sample_retime.h"
#include "state.h"
#include "xmen2_image.h"
#include "state_place.h"

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

extern "C" void xm_sha1(const uint8_t *data, size_t len, uint8_t out[20])
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

/* "" if PATH is the game (its bytes then in *out, if given), else why not */
static std::string rom_check(const char *path, std::vector<uint8_t> *out)
{
    FILE *f = fopen(path, "rb");
    if (!f) return std::string("cannot open the ROM ") + path;
    std::vector<uint8_t> rom;
    uint8_t buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) rom.insert(rom.end(), buf, buf + n);
    fclose(f);
    uint8_t d[20];
    xm_sha1(rom.data(), rom.size(), d);
    char hex[41];
    for (int i = 0; i < 20; i++) snprintf(hex + 2 * i, 3, "%02x", d[i]);
    if (strcmp(hex, ROM_SHA1))
        return std::string(path) + " is not X-Men 2: Clone Wars (USA/Europe): SHA-1 " + hex + ", expected " + ROM_SHA1 +
               ". A zipped ROM must be unzipped first; another version of the cartridge is refused.";
    if (out) out->swap(rom);
    return "";
}

/* played runs (no --benchmark) say what went wrong in a message box too: started from a file
   manager, there is no console to read */
static void rom_fatal(const std::string &why, bool played)
{
    fprintf(stderr, "[native] %s\n", why.c_str());
    if (played) host_message(why.c_str());
    exit(2);
}

static void load_rom(const char *path, bool played)
{
    std::vector<uint8_t> rom;
    std::string why = rom_check(path, &rom);
    if (!why.empty()) rom_fatal(why, played);
    memset(g_rom, 0, sizeof g_rom);
    memcpy(g_rom, rom.data(), rom.size() < sizeof g_rom ? rom.size() : sizeof g_rom);
}

/* No ROM on the command line (started by a double-click): xmen2.ini's [rom] path, then a file
   beside the program that is the game (by its size and SHA-1), then, played, a file picker; the
   one picked is remembered in xmen2.ini. "" if none. */
static std::string find_rom(bool played)
{
    std::string ini = options_ini_rom();
    if (!ini.empty() && rom_check(ini.c_str(), nullptr).empty()) return ini;
    std::error_code ec;
    for (const auto &e : std::filesystem::directory_iterator(options_exe_dir(), ec)) {
        std::error_code ec2;
        if (!e.is_regular_file(ec2) || e.file_size(ec2) != 0x200000) continue;
        std::string p = e.path().string();
        if (rom_check(p.c_str(), nullptr).empty()) return p;
    }
    if (!played) return "";
    char path[4096];
    for (;;) {
        if (!host_pick_rom(path, sizeof path)) {
            host_message("X-Men 2 needs your ROM of the game (USA/Europe). Put it in the game's folder, or start "
                         "the game with it: drag the ROM onto the program, or run  XMen2.exe path\\to\\xmen2.bin  "
                         "(Linux: ./xmen2 path/to/xmen2.bin).");
            return "";
        }
        std::string why = rom_check(path, nullptr);
        if (why.empty()) {
            if (!options_remember_rom(path)) fprintf(stderr, "[native] could not write the ROM's path to xmen2.ini\n");
            return path;
        }
        host_message(why.c_str());   /* and the picker again */
    }
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

/* Phase 6 R8 (--game-rows FILE): the game's state at each sync point, less what only the picture
   depends on, so a run with the 16:9 view presentation only can be compared with the 4:3 run.
   `sync,game_crc32`: work RAM (the stack area masked as the sync rows) less the picture's state
   (machine_mask_picture), CRAM, VSRAM and VRAM. */
static FILE *s_game_rows;

static uint32_t game_crc(const XmImage *im, uint64_t sync)
{
    static uint8_t ram[0x10000], vram[0x10000];
    memcpy(ram, im->ram, sizeof ram);
    uint32_t lo = s_stack_low & 0xFFFFu, hi = XMEN2_STACK_TOP & 0xFFFFu;
    if ((s_stack_low & 0xFF0000u) == 0xFF0000u && hi > lo) memset(ram + lo, 0, hi - lo);
    memcpy(vram, im->vram, sizeof vram);
    machine_mask_picture(ram, vram);
    if (const char *list = getenv("XMEN2_GAME_DUMP"), *dir = getenv("XMEN2_DUMP_DIR"); list && dir) {   /* diagnosing: the state as compared */
        for (const char *q = list; *q; ) {
            char *e;
            unsigned long long n = strtoull(q, &e, 10);
            if (e == q) break;
            if (n == sync) {
                char path[600];
                snprintf(path, sizeof path, "%s/g%06llu.ram", dir, n);
                if (FILE *f = fopen(path, "wb")) { fwrite(ram, 1, sizeof ram, f); fclose(f); }
                snprintf(path, sizeof path, "%s/g%06llu.vram", dir, n);
                if (FILE *f = fopen(path, "wb")) { fwrite(vram, 1, sizeof vram, f); fclose(f); }
            }
            q = *e ? e + 1 : e;
        }
    }
    uint32_t c = crc32_update(0xFFFFFFFFu, ram, sizeof ram);
    c = crc32_update(c, vram, sizeof vram);
    c = crc32_update(c, (const uint8_t *)im->cram, sizeof im->cram);
    c = crc32_update(c, (const uint8_t *)im->vsram, sizeof im->vsram);
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

/* ---- the state probe (phase 7 M2: --state-probe P:L) and the digest (--digest) -------------------
 * Every P-th sync point (when L more fit in the run): save, run L sync points (pass 1), restore, save
 * again, run them again (pass 2: the run goes on from there). probe.csv has both passes' rows, as the
 * sync rows plus the picture's hash; probe_snap.csv the snapshot saved and the one saved right after
 * restoring it. Pass 1 is invisible to the rest: no sync row, no picture, no frame end; the counters
 * come back with the snapshot. --state-skip NAME leaves that section out of every restore. */
static uint64_t s_probe_every, s_probe_len, s_probe_at;
static int s_probe_pass;                 /* 0 the run, 1 the first pass after a save, 2 after the restore */
static const char *s_state_skip;
static FILE *s_probe_rows, *s_probe_snap, *s_digest;
static size_t s_state_bytes;

static uint64_t fb_hash(int w, int h);

/* Netplay (netplay.cpp): a tick may run again (a rollback's replay), so a seat's rows and digests
   are kept by sync point, each the last run's, and written when the session ends (the timeline
   every seat agreed on); replays are quiet (nothing shown, heard or ended). */
static bool s_np_buffer, s_np_quiet;
static std::vector<std::string> s_np_rows, s_np_digests;

/* The latency probe (phase 6 E4). --flip N: player 1's C button inverted in runner frames N to N+7.
   --flip-scan LO:HI:L (with --present-log, high_fps off): before each runner frame N in [LO, HI),
   save, run frames N to N+L with that flip (what `--flip N --frames N+L` runs from there), restore
   and go on unflipped. The flipped frames' present rows go to flip_scan.csv (flip,sync,... as
   present.csv); like the probe's first pass they show, hear, end and count nothing.
   --flip-scan-first: stop flipping after the first N (a gameplay sync point) whose pass shows another
   picture than the run's own unflipped rows (present.cpp); the run goes on to its --frames. */
static uint64_t s_flip, s_scan_lo, s_scan_hi, s_scan_len, s_scan_flip;
static FILE *s_scan_out;
static bool quiet(void) { return s_probe_pass == 1 || s_np_quiet || s_scan_flip; }

/* --branch-plan FILE (headless traces of runs that start with this run's input; the plan's
   lines `AT TOTAL DIR SCRIPT`): when runner frame AT is done, the run is saved and branches:
   DIR's run plays SCRIPT up to its frame TOTAL, as `--benchmark TOTAL --input-script SCRIPT` from
   power-on would (same pads up to AT), writing its files in DIR (sync.csv, present.csv, game.csv,
   digest.csv, probe.csv, probe_snap.csv, fbhash.log as asked for this run: this run's rows up to AT,
   then its own), its counters to DIR/runner.log and its benchmark line to DIR/runner.stdout; then
   everything is put back (the savestate with its counters, the presentation's state, the widescreen
   counters' strings, the export's counts, the stack's paint, the reported snapshot sizes) and this run
   goes on. A branch point inside a state probe's window moves back to the probe's start (same pads
   there too). A branch's fiber_stack_peak can differ from its run's from power-on: the game's fiber
   also runs the host's allocator, whose depth depends on the heap's history. */
struct Branch { uint64_t at, total; std::string dir, script; };
static std::vector<Branch> s_plan;
static size_t s_plan_next;
static bool s_branch_on, s_branch_done;
static uint64_t s_branch_at;
static std::chrono::steady_clock::time_point s_branch_t0;
static FILE *s_branch_log, *s_branch_stdout;
static std::string s_branch_json;   /* the branch's own fields for its benchmark line */
static bool branch_running(void) { return s_branch_on; }
static void run_branches(void);
static void save_states(void);   /* --state-at (see "State files" below run_branches) */
static void state_file_load(const char *path);
static void load_states(const char *path);
static uint64_t s_loaded_at;
static std::string s_loaded_json;

extern "C" void plat_sync_row(uint64_t sync, uint64_t frame)
{
    s_sync_rows = sync;
    players_sync_row();   /* phase 7: the mp counters */
    if (xm_ram_peek8(0xC942)) {   /* g_InLevelLoop: a pass of Level_FrameLoop / PlayerSelect_Run */
        if (xm_ram_peek8(0xC920) == 2) s_two_player_frames++;
        if (xm_ram_peek8(0xC922)) s_two_player_step_frames++;
    }
    if (s_scan_flip) return;
    char line[96];
    if (s_digest && s_probe_pass != 1) {
        snprintf(line, sizeof line, "%llu,%016llx\n", (unsigned long long)sync, (unsigned long long)machine_digest());
        if (s_np_buffer) {
            if (s_np_digests.size() <= sync) s_np_digests.resize(sync + 1);
            s_np_digests[sync] = line;
        } else
            fputs(line, s_digest);
    }
    bool live = s_probe_pass != 1;
    if (!s_probe_pass && !s_sync && !s_game_rows && !getenv("XMEN2_SYNC_DUMP")) return;   /* a played run compares nothing */
    xm_export(&s_image);
    uint32_t crc[4] = { ram_crc_masked(s_image.ram), crc32_bytes(s_image.vram, sizeof s_image.vram),
                        crc32_bytes(s_image.cram, sizeof s_image.cram), crc32_bytes(s_image.vsram, sizeof s_image.vsram) };
    if (s_probe_pass)
        fprintf(s_probe_rows, "%llu,%d,%llu,%08x,%08x,%08x,%08x,%016llX\n", (unsigned long long)s_probe_at, s_probe_pass,
                (unsigned long long)sync, crc[0], crc[1], crc[2], crc[3],
                (unsigned long long)fb_hash(machine_screen_width(), machine_screen_height()));
    if (!live) return;
    if (!s_np_buffer) maybe_dump_sync(sync);
    if (s_game_rows && !s_np_buffer) fprintf(s_game_rows, "%llu,%08x\n", (unsigned long long)sync, game_crc(&s_image, sync));
    if (!s_sync) return;
    snprintf(line, sizeof line, "%llu,%llu,%08x,%08x,%08x,%08x\n", (unsigned long long)sync, (unsigned long long)frame,
             crc[0], crc[1], crc[2], crc[3]);
    if (s_np_buffer) {
        if (s_np_rows.size() <= sync) s_np_rows.resize(sync + 1);
        s_np_rows[sync] = line;
    } else
        fputs(line, s_sync);
}

/* netplay.cpp's hooks */
void main_np_begin(void) { s_np_buffer = true; }
void main_np_quiet(bool on) { s_np_quiet = on; }
void main_np_write_rows(uint64_t upto)   /* the rows and digests of sync points 1..upto */
{
    for (uint64_t s = 1; s <= upto; s++) {
        if (s_sync && s < s_np_rows.size()) fputs(s_np_rows[s].c_str(), s_sync);
        if (s_digest && s < s_np_digests.size()) fputs(s_np_digests[s].c_str(), s_digest);
    }
    s_np_buffer = false;
}

static uint64_t s_benchmark, s_quit_after;

static void at_sync_work(uint64_t sync);

extern "C" void plat_at_sync(uint64_t sync)
{
    if (!s_branch_on) save_states();   /* --state-at: before a branch of the same frame */
    if (!s_branch_on && s_plan_next < s_plan.size()) run_branches();   /* --branch-plan: before this run's own work */
    at_sync_work(sync);
}

static void at_sync_work(uint64_t sync)
{
    if (!s_state_bytes) {   /* the snapshot's size, for the counters */
        Snapshot t;
        state_save(t);
        s_state_bytes = t.bytes.size();
    }
    if (uint64_t n = machine_frame_no() + 1;
        s_scan_hi && !s_scan_flip && n >= s_scan_lo && n < s_scan_hi && !present_scan_found()) {
        static Snapshot at;
        state_save(at);
        s_flip = s_scan_flip = n;
        present_scan(s_scan_out, n);
        for (uint64_t i = 0; i <= s_scan_len; i++) machine_tick();
        present_scan(nullptr, 0);
        s_flip = s_scan_flip = 0;
        state_load(at, nullptr);
    }
    if (!s_probe_every || sync % s_probe_every || (s_benchmark && sync + s_probe_len > s_benchmark)) return;
    static Snapshot saved, resaved;
    state_save(saved);
    s_state_bytes = saved.bytes.size();
    std::string a = state_sha1(saved);
    s_probe_at = sync;
    s_probe_pass = 1;
    for (uint64_t i = 0; i < s_probe_len; i++) machine_tick();
    state_load(saved, s_state_skip);
    state_save(resaved);
    fprintf(s_probe_snap, "%llu,%s,%s\n", (unsigned long long)sync, a.c_str(), state_sha1(resaved).c_str());
    s_probe_pass = 2;
    for (uint64_t i = 0; i < s_probe_len; i++) machine_tick();
    s_probe_pass = 0;
}

static void report_counters_to(FILE *out)
{
    if (s_sync) fflush(s_sync);
    if (s_digest) fflush(s_digest);
    if (s_probe_rows) fflush(s_probe_rows);
    if (s_probe_snap) fflush(s_probe_snap);
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
    /* phase 7: the savestate's sections and size, and how deep the game's fiber stack went */
    char peak[64];
    snprintf(peak, sizeof peak, ", \"fiber_stack_peak\": %llu}", (unsigned long long)machine_stack_peak());
    std::string state = state_sections_json(s_state_bytes) + peak;
    /* no dispatch, no interpreter, no stand-ins: the native runner has no 68000 code to miss */
    fprintf(out, "[xmen2] counters {\"dispatch_misses\": 0, \"interp_instructions\": 0, \"stub_hits\": 0, "
                 "\"sync_points\": %llu, \"a7_min\": null, \"stack_low\": %u, \"exported\": %s, \"memory\": %s, "
                 "\"options\": %s, \"enhance\": %s, \"state\": %s, \"session\": %s, \"mp\": %s, \"sound\": %s}\n",
            (unsigned long long)s_sync_rows, (unsigned)(s_stack_low & 0xFFFFFFu), exported, memory,
            options_effective_json().c_str(), enhance, state.c_str(), machine_session() ? "true" : "false",
            players_counters_json().c_str(), sound_counters_json().c_str());
    fflush(out);
}

static void report_counters(void)
{
    report_counters_to(stderr);
    if (const char *p = getenv("XMEN2_RENDER_US"); p && *p) present_render_us_write(p);
}

/* ---- the picture ---------------------------------------------------------------------------- */

#define FB_W 512
#define FB_H 480
static uint32_t s_fb[FB_H][FB_W];
static uint32_t s_ifb[FB_H][FB_W];          /* high_fps: an in-between frame (present.cpp) */
static uint32_t (*s_target)[FB_W] = s_fb;
static uint32_t s_hash_frames;
static FILE *s_hash_out;   /* the [FBHASH] lines: stderr, or --hash-out FILE (a branch plan's runs have their own) */

extern "C" void plat_scanline(int row, const uint32_t *argb, int width)
{
    if (row < 0 || row >= FB_H) return;
    int n = width < FB_W ? width : FB_W;
    memcpy(s_target[row], argb, (size_t)n * sizeof(uint32_t));
}

extern "C" void plat_scan_target(uint32_t *fb) { s_target = fb ? (uint32_t (*)[FB_W])fb : s_fb; }

static uint64_t fb_hash(int w, int h)
{
    if (!machine_pixels()) return 0;   /* --no-pixels: nothing drawn */
    uint64_t x = 0xCBF29CE484222325ULL;
    for (int y = 0; y < h && y < FB_H; y++) {
        const uint8_t *p = (const uint8_t *)s_fb[y];
        for (int i = 0; i < w * 4; i++) { x ^= p[i]; x *= 0x100000001B3ULL; }
    }
    return x;
}

/* ---- playing: the window, input and sound (host_sdl.cpp); headless runs have none ----------- */

static int s_interactive;

/* the sound chips run in the session machine (played runs, netplay, trace --session); only a played
   run has a device to hear them. With sample_retime or sample_smooth the FM writes wait for the frame's
   end (g_fm_m1), its DAC samples taken out and put back in at their steady times, with their ramps
   (sample_retime.cpp) */
extern "C" void plat_audio_write(uint32_t stamp, int port, uint8_t v)
{
    if (!machine_session() || !capture_audio_write(port, &v)) return;
    if (dac_on() && port != PLAT_AUDIO_PSG) {
        if (!dac_take(&g_dac_m1, "m1", stamp, (unsigned)port, v)) g_fm_m1.push_back({ stamp, (uint8_t)port, v });
        return;
    }
    audio_event_push(stamp, (uint8_t)port, v);
}

extern "C" void plat_frame_audio(uint64_t frame)
{
    if (!machine_session()) return;
    if (dac_on()) {
        static std::vector<ChipEv> merged;
        merged.clear();
        dac_merge(&g_dac_m1, g_fm_m1.data(), g_fm_m1.size(), &merged);
        g_fm_m1.clear();
        for (const ChipEv &e : merged) audio_event_push(e.stamp, e.port, e.v);
    }
    host_audio(frame, quiet());   /* quiet: the chips run, nothing is heard */
}

extern "C" int plat_realtime(void) { return s_interactive; }
void main_np_slip(double us) { if (s_interactive) host_slip(us); }   /* netplay.cpp: the scheduler held a frame */

extern "C" void plat_after_render(void)
{
    if (quiet()) return;
    present_after_render(machine_frame_no(), s_interactive != 0, &s_ifb[0][0], FB_W);
}

/* ---- input script --------------------------------------------------------------------------- */

/* `PADS p1 [p2 [p3 [p4]]]`: phase 7's stage inputs have four pad columns (players 3 and 4: the
   platform's pads beyond the 68000's two ports, g_pad34) */
struct PadSpan { uint16_t pad[4]; uint64_t frames; };
static std::vector<PadSpan> s_script;
extern "C" uint16_t g_pad34[2];
uint16_t g_pad34[2];

static void load_script(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[native] cannot open the input script %s\n", path); exit(2); }
    char line[512];
    uint16_t cur[4] = { 0, 0, 0, 0 };
    while (fgets(line, sizeof line, f)) {
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (!*s || *s == '#' || *s == '\n' || *s == '\r') continue;
        char op[16] = "";
        unsigned p[4] = { 0, 0, 0, 0 };
        int k = sscanf(s, "%15s %x %x %x %x", op, &p[0], &p[1], &p[2], &p[3]);
        if (!strcmp(op, "PADS") && k >= 2) {
            for (int i = 0; i < 4; i++) cur[i] = (uint16_t)(i < k - 1 ? p[i] : 0);
            continue;
        }
        unsigned long long n = 0;
        if (!strcmp(op, "WAIT") && sscanf(s, "%15s %llu", op, &n) == 2) {
            s_script.push_back(PadSpan{ { cur[0], cur[1], cur[2], cur[3] }, n });
            continue;
        }
        fprintf(stderr, "[native] input script %s: unsupported line: %s", path, line);
        exit(2);
    }
    fclose(f);
}

/* the script's pads of a runner frame (the span covering it; after the script, its last pads) */
static void script_pads(uint64_t frame, uint16_t pad[4])
{
    uint64_t at = 0;
    memcpy(pad, s_script.back().pad, 4 * sizeof pad[0]);
    for (const PadSpan &p : s_script) {
        if (frame <= at + p.frames) { memcpy(pad, p.pad, 4 * sizeof pad[0]); break; }
        at += p.frames;
    }
}

/* Netplay: a seat's own pad for a runner frame, column `col` of its script, or (played) the
   keyboard and first controller; the published rows of every seat for the frame being run. */
static bool s_np_pads;
static uint16_t s_np_pad[4];
uint16_t main_local_pad(uint64_t frame, int col)
{
    uint16_t pad[4] = { 0, 0, 0, 0 };
    if (!s_script.empty()) script_pads(frame, pad);
    else if (s_interactive) { host_input(pad); return pad[0]; }
    return pad[col & 3];
}
void main_np_pads(const uint16_t rows[4]) { s_np_pads = true; memcpy(s_np_pad, rows, sizeof s_np_pad); }

extern "C" void plat_frame_begin(uint64_t frame)
{
    uint16_t pad[4] = { 0, 0, 0, 0 };
    if (s_np_pads) memcpy(pad, s_np_pad, sizeof pad);
    else if (s_interactive && s_script.empty()) host_input(pad);
    else if (!s_script.empty()) {
        if (s_interactive) {   /* a window replaying a script: its events still read (else Windows calls it
                                  "Not Responding"; Esc and the hotkeys work), its pads the script's */
            uint16_t live[4];
            host_input(live);
        }
        script_pads(frame, pad);
    }
    if (s_flip && frame >= s_flip && frame < s_flip + 8) pad[0] ^= GPAD_C;   /* --flip: the latency probe */
    capture_record_pads(frame, pad);   /* --record-inputs */
    g_machine.bus.pad[0] = pad[0];
    g_machine.bus.pad[1] = pad[1];
    g_pad34[0] = pad[2];
    g_pad34[1] = pad[3];
}

/* ---- frames --------------------------------------------------------------------------------- */

static const char *s_screenshot;
static std::chrono::steady_clock::time_point s_t0;

/* XMEN2_FRAME_TIMES=path: each runner frame's wall time (microseconds, the game's logic, the
   export, the Z80 and the render; with --benchmark nothing else), for finding slow scenes */
static FILE *s_ftimes;
static std::chrono::steady_clock::time_point s_tlast;

extern "C" void plat_frame_repeat(void)
{
    if (!quiet()) capture_video_repeat();   /* XMEN2_CAPTURE: the video keeps the sound's time */
    if (!s_interactive || quiet()) return;
    /* its period is covered by the frames in between; pacing=display: the screen keeps the picture */
    if (g_opt.high_fps || host_vsync()) present_repeat();
    else host_present(&s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
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

extern "C" void xm_install_crash_trace(void);   /* game/runtime/crash_win.cpp */
extern "C" void xm_crash_test(uint64_t frame);

extern "C" void plat_frame_end(uint64_t frame)
{
    xm_crash_test(frame);   /* XMEN2_CRASH_FRAME */
    if (s_scan_flip) {   /* --flip-scan's flipped frames: their present rows only */
        present_whole(frame, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height(), false);
        return;
    }
    if (quiet()) return;   /* the probe's first pass, a rollback's replay: nothing is shown or ends */
    maybe_shot(frame);
    capture_video_frame(&s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());   /* XMEN2_CAPTURE */
    if (s_ftimes) {
        auto now = std::chrono::steady_clock::now();
        fprintf(s_ftimes, "%llu,%lld\n", (unsigned long long)frame,
                (long long)std::chrono::duration_cast<std::chrono::microseconds>(now - s_tlast).count());
        s_tlast = now;
    }
    if (s_hash_frames && frame % s_hash_frames == 0) {
        int w = machine_screen_width(), h = machine_screen_height();
        fprintf(s_hash_out ? s_hash_out : stderr, "[FBHASH] frame=%llu w=%d h=%d hash=0x%016llX\n",
                (unsigned long long)frame, w, h, (unsigned long long)fb_hash(w, h));
    }
    /* the present clock (--present-log, high_fps: present.cpp shows the frame at its time) */
    bool shown = present_whole(frame, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height(), s_interactive != 0);
    if (s_interactive && !shown) host_present(&s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
    if (s_quit_after && frame >= s_quit_after) {   /* a played run's smoke test */
        if (s_screenshot) host_screenshot(s_screenshot, &s_fb[0][0], FB_W, machine_screen_width(), machine_screen_height());
        exit(0);
    }
    if (s_benchmark && frame >= s_benchmark) {
        bool branch = branch_running();
        double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - (branch ? s_branch_t0 : s_t0)).count();
        uint64_t played = frame - (branch ? s_branch_at : s_loaded_at);   /* a branch's (a loaded run's) own frames, timed */
        FILE *o = branch ? s_branch_stdout : stdout;
        fprintf(o, "GENESISRECOMP_BENCHMARK {\"game\":\"xmen2\",\"frames\":%llu,\"seconds\":%.9f,\"fps\":%.3f,"
                "\"ms_per_frame\":%.6f,\"requested_frames\":%llu,\"complete\":true,\"runner\":\"native\"%s}\n",
                (unsigned long long)frame, s, s > 0 ? (double)played / s : 0.0, played ? s * 1000.0 / (double)played : 0.0,
                (unsigned long long)s_benchmark, branch ? s_branch_json.c_str() : s_loaded_json.c_str());
        fflush(o);
        if (branch) {   /* --branch-plan: the run ends here; its process goes on with the plan */
            report_counters_to(s_branch_log);
            s_branch_done = true;
            return;
        }
        save_states();   /* --state-at: this run's last frame */
        if (s_plan_next < s_plan.size()) run_branches();   /* the plan's branches at this run's last frame */
        exit(0);
    }
}

/* ---- --branch-plan (see the declarations above plat_sync_row) --------------------------------- */

extern "C" void xm_export_branch(int save);
extern "C" size_t xm_export_counts(void *buf, int get);
extern "C" void xm_mem_counts(unsigned long long v[2], int get);
extern "C" uint8_t *xm_arena;
extern "C" void xm_mem_branch(int save);

/* this run's files, by path, so a branch's copies start with its rows so far */
static std::string s_sync_path, s_present_path, s_game_rows_path, s_digest_path, s_probe_dir, s_hash_path;

static FILE *branch_file(FILE *mine, const std::string &from, const std::string &to)
{
    if (!mine) return nullptr;
    fflush(mine);
    FILE *in = fopen(from.c_str(), "rb"), *out = fopen(to.c_str(), "wb");
    if (!in || !out) { fprintf(stderr, "[native] --branch-plan: cannot copy %s to %s\n", from.c_str(), to.c_str()); exit(2); }
    char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, in)) > 0) fwrite(buf, 1, n, out);
    fclose(in);
    fclose(out);
    FILE *f = fopen(to.c_str(), "a");   /* text mode, as this run's own files */
    if (!f) { fprintf(stderr, "[native] --branch-plan: cannot write %s\n", to.c_str()); exit(2); }
    setvbuf(f, nullptr, _IOFBF, 1 << 20);
    return f;
}

static void branch_close(FILE *&branch, FILE *mine)
{
    if (branch && branch != mine) fclose(branch);
    branch = mine;
}

static void run_branches(void)
{
    uint64_t f = machine_frame_no();
    if (s_plan[s_plan_next].at != f) return;
    Snapshot snap;
    std::vector<size_t> spans = state_reported_spans();
    state_save(snap);
    state_set_reported_spans(spans);   /* this save is not the run's own */
    present_branch_save();
    ws_branch(1);
    xm_export_branch(1);
    xm_mem_branch(1);
    uint64_t peak = machine_stack_peak(), benchmark = s_benchmark;
    size_t state_bytes = s_state_bytes;
    std::vector<PadSpan> script = s_script;
    FILE *sync = s_sync, *game = s_game_rows, *digest = s_digest, *prows = s_probe_rows, *psnap = s_probe_snap,
         *hash = s_hash_out;
    while (s_plan_next < s_plan.size() && s_plan[s_plan_next].at == f) {
        const Branch &b = s_plan[s_plan_next++];
        const std::string d = b.dir + "/";
        s_sync = branch_file(sync, s_sync_path, d + "sync.csv");
        s_game_rows = branch_file(game, s_game_rows_path, d + "game.csv");
        s_digest = branch_file(digest, s_digest_path, d + "digest.csv");
        s_probe_rows = branch_file(prows, s_probe_dir + "/probe.csv", d + "probe.csv");
        s_probe_snap = branch_file(psnap, s_probe_dir + "/probe_snap.csv", d + "probe_snap.csv");
        s_hash_out = branch_file(hash, s_hash_path, d + "fbhash.log");
        FILE *plog = present_logging() ? present_log_file(nullptr) : nullptr;
        if (plog) present_log_file(branch_file(plog, s_present_path, d + "present.csv"));
        s_branch_log = fopen((d + "runner.log").c_str(), "w");
        s_branch_stdout = fopen((d + "runner.stdout").c_str(), "w");
        if (!s_branch_log || !s_branch_stdout) { fprintf(stderr, "[native] --branch-plan: cannot write in %s\n", b.dir.c_str()); exit(2); }
        s_script.clear();
        load_script(b.script.c_str());
        s_benchmark = b.total;
        s_branch_at = f;
        s_branch_json = ",\"branched_at\":" + std::to_string(f);
        s_branch_on = true;
        s_branch_done = false;
        s_branch_t0 = std::chrono::steady_clock::now();
        at_sync_work(s_sync_rows);   /* as plat_at_sync after frame AT in a run of its own */
        while (!s_branch_done) {
            machine_tick();
            if (!s_branch_done) at_sync_work(s_sync_rows);
        }
        s_branch_on = false;
        fprintf(stderr, "[branch] %s: frames %llu-%llu\n", b.dir.c_str(), (unsigned long long)f + 1, (unsigned long long)b.total);
        branch_close(s_sync, sync);
        branch_close(s_game_rows, game);
        branch_close(s_digest, digest);
        branch_close(s_probe_rows, prows);
        branch_close(s_probe_snap, psnap);
        branch_close(s_hash_out, hash);
        if (plog) fclose(present_log_file(plog));
        fclose(s_branch_log);
        fclose(s_branch_stdout);
        s_branch_log = s_branch_stdout = nullptr;
        s_script = script;
        s_benchmark = benchmark;
        state_load(snap, nullptr);
        state_set_reported_spans(spans);
        s_state_bytes = state_bytes;
        present_branch_restore();
        ws_branch(0);
        xm_export_branch(0);
        xm_mem_branch(0);
        machine_stack_repaint(peak);
    }
}

/* the plan: `AT TOTAL DIR SCRIPT` lines, in AT order (as moved for the state probe) */
static void load_plan(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[native] cannot open the branch plan %s\n", path); exit(2); }
    char line[2048];
    while (fgets(line, sizeof line, f)) {
        unsigned long long at = 0, total = 0;
        int used = 0;
        if (sscanf(line, "%llu %llu %n", &at, &total, &used) != 2) continue;
        std::string rest = line + used;
        while (!rest.empty() && (rest.back() == '\n' || rest.back() == '\r')) rest.pop_back();
        size_t sp = rest.find(' ');
        if (!at || total <= at || sp == std::string::npos) {
            fprintf(stderr, "[native] branch plan %s: bad line: %s", path, line);
            exit(2);
        }
        if (s_probe_every && at % s_probe_every) {   /* not inside a probe's window (no plat_at_sync there): its start */
            uint64_t p = at - at % s_probe_every;
            if (p && at <= p + s_probe_len) at = p;
        }
        s_plan.push_back(Branch{ at, total, rest.substr(0, sp), rest.substr(sp + 1) });
    }
    fclose(f);
    std::stable_sort(s_plan.begin(), s_plan.end(), [](const Branch &a, const Branch &b) { return a.at < b.at; });
}

/* ---- --state-at FILE, --state-load FILE (state files: a branch point for another process) -----------
 * `--state-at FILE`: lines `AT PATH`; when runner frame AT is done (where --branch-plan would save,
 * and before it; inside a state probe's window: at its start, as a branch point), everything a
 * branch starts from goes to PATH (written as PATH.tmp, then renamed):
 * the savestate with its counters, what run_branches keeps outside it, and where this run's files
 * stand (path and length; flushed). `--state-load FILE`: this run starts there, as the branch would:
 * its files begin with a copy of the saving run's up to that point, then its own rows; the run plays
 * on to --benchmark. The fiber's stack holds host addresses: a file loads only in a process laid out
 * as the one that saved it (state_place.h: the same executable at its preferred base, the same /GS
 * cookie, the stack and the arena at their fixed places), with the same options and files, which
 * the load checks. The benchmark line has the branch's `branched_at`. */
struct StateAt { uint64_t at; std::string path; };
static std::vector<StateAt> s_states;
static size_t s_states_next;

namespace {
struct Out {
    std::vector<uint8_t> b;
    void raw(const void *p, size_t n) { b.insert(b.end(), (const uint8_t *)p, (const uint8_t *)p + n); }
    void u64(uint64_t v) { raw(&v, 8); }
    void bytes(const void *p, size_t n) { u64(n); raw(p, n); }
    void str(const std::string &s) { bytes(s.data(), s.size()); }
};
struct In {
    const uint8_t *p, *end;
    bool ok;
    void raw(void *d, size_t n) { if ((size_t)(end - p) < n) { ok = false; memset(d, 0, n); return; } memcpy(d, p, n); p += n; }
    uint64_t u64() { uint64_t v; raw(&v, 8); return v; }
    const uint8_t *bytes(size_t *n) {
        uint64_t k = u64();
        if (!ok || k > (uint64_t)(end - p)) { ok = false; *n = 0; return p; }
        const uint8_t *at = p; p += k; *n = (size_t)k; return at;
    }
    std::string str() { size_t n; const uint8_t *at = bytes(&n); return std::string((const char *)at, n); }
};
const char STATE_MAGIC[8] = { 'X', 'N', 'S', 'T', 'A', 'T', 'E', '1' };
}  // namespace

/* this run's files, by kind: the open file (a reference, so a load can replace it) and its path */
struct RunFile { const char *kind; FILE **f; std::string path; };
static std::vector<RunFile> run_files(void)
{
    static FILE *present;
    present = present_logging() ? present_log_file(nullptr) : nullptr;   /* taken out for now: put back by the caller */
    return { { "sync", &s_sync, s_sync_path }, { "game", &s_game_rows, s_game_rows_path }, { "digest", &s_digest, s_digest_path },
             { "probe", &s_probe_rows, s_probe_dir + "/probe.csv" }, { "probe_snap", &s_probe_snap, s_probe_dir + "/probe_snap.csv" },
             { "hash", &s_hash_out, s_hash_path }, { "present", &present, s_present_path } };
}
static void run_files_done(std::vector<RunFile> &files)
{
    for (RunFile &r : files)
        if (!strcmp(r.kind, "present") && *r.f) present_log_file(*r.f);
}

static XmPlace place_here(void)
{
    uintptr_t lo = 0, top = 0;
    machine_fiber_range(&lo, &top);
    XmPlace p;
    xm_place_get(&p, lo, top, (uintptr_t)xm_arena);
    return p;
}

static void state_file_write(const std::string &path)
{
    Out o;
    o.raw(STATE_MAGIC, sizeof STATE_MAGIC);
    XmPlace place = place_here();
    o.raw(&place, sizeof place);
    o.str(options_effective_json());
    o.u64(machine_frame_no());
    Snapshot snap;
    std::vector<size_t> spans = state_reported_spans();
    state_save(snap);
    state_set_reported_spans(spans);   /* this save is not the run's own */
    std::vector<uint8_t> blob;
    state_serialize(snap, blob);
    o.bytes(blob.data(), blob.size());
    o.u64(spans.size());
    for (size_t s : spans) o.u64(s);
    o.u64(s_state_bytes);
    o.u64(machine_stack_peak());
    std::vector<uint8_t> part;
    present_state_save(part);
    o.bytes(part.data(), part.size());
    part.clear();
    capture_state_save(part);
    o.bytes(part.data(), part.size());
    std::string ws[3];
    ws_strings(ws, 1);
    for (const std::string &s : ws) o.str(s);
    part.resize(xm_export_counts(nullptr, 1));
    xm_export_counts(part.data(), 1);
    o.bytes(part.data(), part.size());
    unsigned long long mem[2];
    xm_mem_counts(mem, 1);
    o.u64(mem[0]);
    o.u64(mem[1]);
    std::vector<RunFile> files = run_files();
    o.u64(files.size());
    for (RunFile &r : files) {
        o.str(r.kind);
        long long len = -1;
        if (*r.f) {
            fflush(*r.f);
            FILE *in = fopen(r.path.c_str(), "rb");
            if (in && fseek(in, 0, SEEK_END) == 0) len = (long long)ftell(in);
            if (in) fclose(in);
            if (len < 0) { fprintf(stderr, "[state] cannot measure %s\n", r.path.c_str()); exit(2); }
        }
        o.str(r.path);
        o.u64((uint64_t)len);
    }
    run_files_done(files);
    if (int fi = state_section_index("fiber"); fi >= 0 && snap.spans[(size_t)fi].second > 48) {   /* XMEN2_STATE_SCAN */
        const uint8_t *fb = snap.bytes.data() + snap.spans[(size_t)fi].first;   /* fiber_compat.c's header, context, span */
        uint64_t span_lo, span_len;
        uint32_t ctx;
        memcpy(&span_lo, fb + 24, 8);
        memcpy(&span_len, fb + 32, 8);
        memcpy(&ctx, fb + 40, 4);
        if (48 + (size_t)ctx + span_len <= snap.spans[(size_t)fi].second) {
            xm_place_scan(fb + 48, ctx, 0, &place, "context");
            xm_place_scan(fb + 48 + ctx, (size_t)span_len, span_lo, &place, "stack");
        }
    }
    std::string tmp = path + ".tmp";
    FILE *f = fopen(tmp.c_str(), "wb");
    bool ok = f && fwrite(o.b.data(), 1, o.b.size(), f) == o.b.size();
    if (f && fclose(f) != 0) ok = false;
    remove(path.c_str());
    if (!ok || rename(tmp.c_str(), path.c_str()) != 0) { fprintf(stderr, "[state] cannot write %s\n", path.c_str()); exit(2); }
    fprintf(stderr, "[state] %llu %s\n", (unsigned long long)machine_frame_no(), path.c_str());
    if (getenv("XMEN2_STATE_SECTIONS")) {   /* testing: each section's SHA-1 (a loaded run's later save vs the run's own) */
        std::vector<std::string> names = state_section_names();
        fprintf(stderr, "[state] sections %llu", (unsigned long long)machine_frame_no());
        for (size_t i = 0; i < snap.spans.size() && i < names.size(); i++) {
            uint8_t d[20];
            xm_sha1(snap.bytes.data() + snap.spans[i].first, snap.spans[i].second, d);
            fprintf(stderr, " %s=%02x%02x%02x%02x%02x%02x", names[i].c_str(), d[0], d[1], d[2], d[3], d[4], d[5]);
        }
        fprintf(stderr, "\n");
    }
}

/* the states due at this runner frame (plat_at_sync, the benchmark's end) */
static void save_states(void)
{
    uint64_t f = machine_frame_no();
    while (s_states_next < s_states.size() && s_states[s_states_next].at == f) state_file_write(s_states[s_states_next++].path);
}

[[noreturn]] static void state_refuse(const char *path, const std::string &why)
{
    fprintf(stderr, "[state] %s: %s\n", path, why.c_str());
    exit(2);
}

/* After machine_start (the fiber made at its place, painted), before the first tick */
static void state_file_load(const char *path)
{
    std::vector<uint8_t> file;
    if (FILE *f = fopen(path, "rb")) {
        uint8_t buf[65536];
        size_t n;
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) file.insert(file.end(), buf, buf + n);
        fclose(f);
    } else state_refuse(path, "cannot read it");
    In in{ file.data(), file.data() + file.size(), true };
    char magic[8];
    in.raw(magic, sizeof magic);
    if (!in.ok || memcmp(magic, STATE_MAGIC, sizeof magic)) state_refuse(path, "not a state file of this runner");
    XmPlace saved, mine = place_here();
    in.raw(&saved, sizeof saved);
    if (const char *why = xm_place_mismatch(&saved, &mine)) {
        if (!getenv("XMEN2_STATE_PLACE_UNCHECKED")) state_refuse(path, why);   /* dev: what the check prevents */
        fprintf(stderr, "[state] %s: %s (XMEN2_STATE_PLACE_UNCHECKED: loading it anyway)\n", path, why);
    }
    if (in.str() != options_effective_json()) state_refuse(path, "saved with other options");
    uint64_t frame = in.u64();
    size_t n;
    const uint8_t *blob = in.bytes(&n);
    Snapshot snap;
    if (!in.ok || !state_deserialize(blob, n, snap)) state_refuse(path, "a malformed savestate");
    state_load(snap, s_state_skip);   /* --state-skip: the negative control */
    std::vector<size_t> spans((size_t)in.u64());
    for (size_t &s : spans) s = (size_t)in.u64();
    state_set_reported_spans(spans);
    s_state_bytes = (size_t)in.u64();
    machine_stack_set_peak(in.u64());
    const uint8_t *part = in.bytes(&n);
    if (!in.ok || !present_state_load(part, n)) state_refuse(path, "a malformed presentation state");
    part = in.bytes(&n);
    if (!in.ok || !capture_state_load(part, n)) state_refuse(path, "a malformed sound class table");
    std::string ws[3];
    for (std::string &s : ws) s = in.str();
    ws_strings(ws, 0);
    part = in.bytes(&n);
    if (!in.ok || n != xm_export_counts(nullptr, 1)) state_refuse(path, "the export's counts are of another build");
    xm_export_counts((void *)part, 0);
    unsigned long long mem[2];
    mem[0] = in.u64();
    mem[1] = in.u64();
    xm_mem_counts(mem, 0);
    std::vector<RunFile> files = run_files();
    uint64_t nfiles = in.u64();
    if (nfiles != files.size()) state_refuse(path, "another set of files");
    for (RunFile &r : files) {
        std::string kind = in.str(), from = in.str();
        long long len = (long long)in.u64();
        if (!in.ok || kind != r.kind) state_refuse(path, "another set of files");
        if ((len >= 0) != (*r.f != nullptr)) state_refuse(path, std::string("the run that saved it ") + (len >= 0 ? "wrote" : "did not write") + " the " + kind + " file, this one " + (len >= 0 ? "doesn't" : "does"));
        if (len < 0) continue;
        fprintf(stderr, "[state] prefix %s %lld\n", kind.c_str(), len);   /* trace-group --segment-frames: where the own rows start */
        /* this run's file: the saving run's first `len` bytes (its header and rows so far), then its own */
        fclose(*r.f);
        FILE *src = fopen(from.c_str(), "rb"), *dst = fopen(r.path.c_str(), "wb");
        if (!src || !dst) state_refuse(path, "cannot copy " + from + " to " + r.path);
        char buf[65536];
        for (long long left = len; left > 0; ) {
            size_t k = fread(buf, 1, left < (long long)sizeof buf ? (size_t)left : sizeof buf, src);
            if (!k) state_refuse(path, from + " is shorter than when the state was saved");
            fwrite(buf, 1, k, dst);
            left -= (long long)k;
        }
        fclose(src);
        fclose(dst);
        *r.f = fopen(r.path.c_str(), "a");   /* text mode, as this run's own files */
        if (!*r.f) state_refuse(path, "cannot write " + r.path);
        setvbuf(*r.f, nullptr, _IOFBF, 1 << 20);
    }
    run_files_done(files);
    if (!in.ok || in.p != in.end) state_refuse(path, "malformed");
    if (machine_frame_no() != frame) state_refuse(path, "the savestate is not at its frame");
    s_loaded_at = frame;
    s_loaded_json = ",\"branched_at\":" + std::to_string(frame);
    fprintf(stderr, "[state] loaded %s: frame %llu\n", path, (unsigned long long)frame);
}

static void load_states(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[native] cannot open the state list %s\n", path); exit(2); }
    char line[2048];
    while (fgets(line, sizeof line, f)) {
        unsigned long long at = 0;
        int used = 0;
        if (sscanf(line, "%llu %n", &at, &used) != 1) continue;
        std::string p = line + used;
        while (!p.empty() && (p.back() == '\n' || p.back() == '\r')) p.pop_back();
        if (!at || p.empty()) {
            fprintf(stderr, "[native] state list %s: bad line: %s", path, line);
            exit(2);
        }
        if (s_probe_every && at % s_probe_every) {   /* as load_plan: not inside a probe's window, its start */
            uint64_t q = at - at % s_probe_every;
            if (q && at <= q + s_probe_len) at = q;
        }
        s_states.push_back(StateAt{ at, p });
    }
    fclose(f);
    std::stable_sort(s_states.begin(), s_states.end(), [](const StateAt &a, const StateAt &b) { return a.at < b.at; });
}

int main(int argc, char **argv)
{
    const char *rom = nullptr, *script = nullptr, *present_log = nullptr, *digest = nullptr, *probe_out = ".", *game_rows = nullptr;
    const char *plan = nullptr, *hash_out = nullptr, *states = nullptr, *state_in = nullptr;
    bool session = false, no_render = false, no_pixels = false, scan_first = false;
    uint64_t seed = 0;
    xm_install_crash_trace();   /* a host exception prints where and the stack (crash_win.cpp) */
    options_init(&argc, argv);   /* --options (phase 6), xmen2.ini; --options --json exits here */
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--benchmark") && i + 1 < argc) s_benchmark = strtoull(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--input-script") && i + 1 < argc) script = argv[++i];
        else if (!strcmp(argv[i], "--hash-frames") && i + 1 < argc) s_hash_frames = (uint32_t)strtoul(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--hash-out") && i + 1 < argc) hash_out = argv[++i];
        else if (!strcmp(argv[i], "--branch-plan") && i + 1 < argc) plan = argv[++i];
        else if (!strcmp(argv[i], "--state-at") && i + 1 < argc) states = argv[++i];
        else if (!strcmp(argv[i], "--state-load") && i + 1 < argc) state_in = argv[++i];
        else if (!strcmp(argv[i], "--fresh-evidence")) {}
        else if (!strcmp(argv[i], "--no-render")) no_render = true;
        else if (!strcmp(argv[i], "--no-pixels")) no_pixels = true;
        else if (!strcmp(argv[i], "--quit-after") && i + 1 < argc) s_quit_after = strtoull(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--screenshot") && i + 1 < argc) s_screenshot = argv[++i];
        else if (!strcmp(argv[i], "--present-log") && i + 1 < argc) present_log = argv[++i];
        else if (!strcmp(argv[i], "--flip") && i + 1 < argc) s_flip = strtoull(argv[++i], nullptr, 10);
        else if (!strcmp(argv[i], "--flip-scan-first")) scan_first = true;
        else if (!strcmp(argv[i], "--flip-scan") && i + 1 < argc) {
            unsigned long long lo = 0, hi = 0, l = 0;
            if (sscanf(argv[++i], "%llu:%llu:%llu", &lo, &hi, &l) != 3 || !lo || hi <= lo || !l) {
                fprintf(stderr, "[native] --flip-scan LO:HI:L\n");
                return 2;
            }
            s_scan_lo = lo;
            s_scan_hi = hi;
            s_scan_len = l;
        }
        /* phase 7 (P7-multiplayer.md "The contract") */
        else if (!strcmp(argv[i], "--session") && i + 1 < argc) { session = true; seed = strtoull(argv[++i], nullptr, 10); }
        else if (!strcmp(argv[i], "--scene") && i + 1 < argc) options_set_first_scene(atoi(argv[++i]));
        else if (!strcmp(argv[i], "--record-inputs") && i + 1 < argc) capture_record_start(argv[++i]);   /* capture.cpp */
        else if (!strcmp(argv[i], "--digest") && i + 1 < argc) digest = argv[++i];
        else if (!strcmp(argv[i], "--game-rows") && i + 1 < argc) game_rows = argv[++i];   /* phase 6 R8 */
        else if (!strcmp(argv[i], "--state-probe") && i + 1 < argc) {
            unsigned long long p = 0, l = 0;
            if (sscanf(argv[++i], "%llu:%llu", &p, &l) != 2 || !p || !l) { fprintf(stderr, "[native] --state-probe P:L\n"); return 2; }
            s_probe_every = p;
            s_probe_len = l;
        }
        else if (!strcmp(argv[i], "--probe-out") && i + 1 < argc) probe_out = argv[++i];
        else if (!strcmp(argv[i], "--state-skip") && i + 1 < argc) s_state_skip = argv[++i];
        else if (netplay_arg(argc, argv, &i)) {}   /* --netplay ... (netplay.cpp) */
        else if (argv[i][0] != '-' && !rom) rom = argv[i];
        else { fprintf(stderr, "[native] unknown option %s\n", argv[i]); return 2; }
    }
    if (const char *t = getenv("XMEN2_PAD_SELFTEST"); t && *t && strcmp(t, "0")) return host_pad_selftest();   /* host_sdl.cpp */
    std::string found;
    if (!rom) {
        found = find_rom(!s_benchmark);
        if (found.empty()) {
            fprintf(stderr, "usage: XMen2Native ROM [--benchmark N] [--input-script F] [--hash-frames K]\n"
                            "  (without ROM: xmen2.ini's [rom] path, the ROM beside the program, or a file picker)\n");
            return 2;
        }
        rom = found.c_str();
    }
    load_rom(rom, !s_benchmark);
    if (!s_benchmark) {   /* played: a window, the pads, the sound */
        if (!host_open("X-Men 2: Clone Wars")) return 3;
        s_interactive = 1;
    }
    /* the session machine: asked for, or a played run's, with a seed from the clock (as a console's
       power-on moment varies) */
    if (s_interactive && !session) {
        session = true;
        seed = (uint64_t)std::chrono::steady_clock::now().time_since_epoch().count();
    }
    if (session) {
        machine_set_session(seed);
        audio_mixer_init();
    }
    if (s_state_skip && !s_probe_every && !state_in) {
        fprintf(stderr, "[native] --state-skip needs --state-probe or --state-load\n");
        return 2;
    }
    if (s_probe_every) {
        std::string d = probe_out;
        s_probe_rows = fopen((d + "/probe.csv").c_str(), "w");
        s_probe_snap = fopen((d + "/probe_snap.csv").c_str(), "w");
        if (!s_probe_rows || !s_probe_snap) { fprintf(stderr, "[native] cannot write the probe's files in %s\n", probe_out); return 2; }
        fprintf(s_probe_rows, "at,pass,sync,ram_crc32,vram_crc32,cram_crc32,vsram_crc32,fb_fnv1a64\n");
        fprintf(s_probe_snap, "at,saved_sha1,resaved_sha1\n");
    }
    if (digest) {
        if (!(s_digest = fopen(digest, "w"))) { fprintf(stderr, "[native] cannot write %s\n", digest); return 2; }
        setvbuf(s_digest, nullptr, _IOFBF, 1 << 20);
        fprintf(s_digest, "sync,digest\n");
    }
    if (game_rows) {
        if (!(s_game_rows = fopen(game_rows, "w"))) { fprintf(stderr, "[native] cannot write %s\n", game_rows); return 2; }
        setvbuf(s_game_rows, nullptr, _IOFBF, 1 << 20);
        fprintf(s_game_rows, "sync,game_crc32\n");
    }
    if (script) load_script(script);
    present_init(present_log);
    if (s_scan_hi) {
        if (!present_log || g_opt.high_fps || s_flip || s_probe_every || netplay_enabled()) {
            fprintf(stderr, "[native] --flip-scan needs --present-log, high_fps off, and no --flip, --state-probe or --netplay\n");
            return 2;
        }
        std::string d = probe_out;
        if (!(s_scan_out = fopen((d + "/flip_scan.csv").c_str(), "w"))) {
            fprintf(stderr, "[native] cannot write flip_scan.csv in %s\n", probe_out);
            return 2;
        }
        setvbuf(s_scan_out, nullptr, _IOFBF, 1 << 20);
        fprintf(s_scan_out, "flip,sync,t_us,whole,gameplay,fb_fnv1a64\n");
        if (scan_first) present_scan_first(s_scan_len);
    } else if (scan_first) {
        fprintf(stderr, "[native] --flip-scan-first needs --flip-scan\n");
        return 2;
    }
    if (no_render) {   /* a trace that compares no picture: the sync points render none */
        const char *shots = getenv("XMEN2_SHOTS"), *cap = getenv("XMEN2_CAPTURE");
        if (!s_benchmark || s_hash_frames || present_log || s_flip || s_probe_every || netplay_enabled()
            || (shots && *shots) || (cap && *cap)) {
            fprintf(stderr, "[native] --no-render needs --benchmark, and no --hash-frames, --present-log, --flip, "
                            "--flip-scan, --state-probe, --netplay, XMEN2_SHOTS or XMEN2_CAPTURE\n");
            return 2;
        }
        machine_render_off();
    }
    if (no_pixels) {   /* a state producer: everything a render does but its pixels */
        const char *shots = getenv("XMEN2_SHOTS"), *cap = getenv("XMEN2_CAPTURE");
        if (!s_benchmark || no_render || s_flip || s_scan_hi || netplay_enabled() || (shots && *shots) || (cap && *cap)) {
            fprintf(stderr, "[native] --no-pixels needs --benchmark, and no --no-render, --flip, --flip-scan, --netplay, "
                            "XMEN2_SHOTS or XMEN2_CAPTURE\n");
            return 2;
        }
        machine_pixels_off();
    }
    crc_init();
    if (const char *sl = getenv("XMEN2_STACK_LOW"); sl && *sl) s_stack_low = (uint32_t)strtoul(sl, nullptr, 0);
    if (const char *p = getenv("XMEN2_SYNC_CSV"); p && *p) {
        s_sync = fopen(p, "w");
        if (!s_sync) { fprintf(stderr, "[xmen2] cannot open XMEN2_SYNC_CSV %s\n", p); return 2; }
        setvbuf(s_sync, nullptr, _IOFBF, 1 << 20);
        fprintf(s_sync, "sync,frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32\n");
        s_sync_path = p;
    }
    if (hash_out) {
        if (!(s_hash_out = fopen(hash_out, "w"))) { fprintf(stderr, "[native] cannot write %s\n", hash_out); return 2; }
        setvbuf(s_hash_out, nullptr, _IOFBF, 1 << 20);
        s_hash_path = hash_out;
    }
    if (plan) {   /* --branch-plan: headless traces only, every file of the run by path */
        if (!s_benchmark || !script || s_flip || s_scan_hi || netplay_enabled() || (s_hash_frames && !hash_out)) {
            fprintf(stderr, "[native] --branch-plan needs --benchmark and --input-script, --hash-out with --hash-frames, "
                            "and no --flip, --flip-scan or --netplay\n");
            return 2;
        }
        load_plan(plan);
        for (const Branch &b : s_plan)
            if (b.at > s_benchmark) {
                fprintf(stderr, "[native] branch plan: %s branches at %llu, after this run's %llu frames\n", b.dir.c_str(),
                        (unsigned long long)b.at, (unsigned long long)s_benchmark);
                return 2;
            }
    }
    if (states || state_in) {   /* state files: as a branch plan (see "State files") */
        if (!s_benchmark || !script || (states && (s_flip || s_scan_hi)) || netplay_enabled() || (s_hash_frames && !hash_out)) {
            fprintf(stderr, "[native] --state-at and --state-load need --benchmark and --input-script, --hash-out with "
                            "--hash-frames, and no --netplay; --state-at no --flip or --flip-scan\n");
            return 2;
        }
        if (states) load_states(states);
    }
    s_present_path = present_log ? present_log : "";   /* every file of the run by path (branches, state files) */
    s_game_rows_path = game_rows ? game_rows : "";
    s_digest_path = digest ? digest : "";
    s_probe_dir = probe_out;
    atexit(report_counters);
    xm_native_mem_init();
    g_machine.bus.pad_type[0] = g_machine.bus.pad_type[1] = 1;   /* 6-button pads, as the runners' settings */
    s_t0 = s_tlast = std::chrono::steady_clock::now();
    if (const char *p = getenv("XMEN2_FRAME_TIMES"); p && *p) {
        if ((s_ftimes = fopen(p, "w"))) fprintf(s_ftimes, "frame,us\n");
    }
    machine_state_init();
    players_state_init();
    snd2_state_init();
    players_video_init();
    state_counters(&s_sync_rows, sizeof s_sync_rows);
    state_counters(&s_two_player_frames, sizeof s_two_player_frames);
    state_counters(&s_two_player_step_frames, sizeof s_two_player_step_frames);
    machine_power_on();
    g_machine.bus.pad_type[0] = g_machine.bus.pad_type[1] = 1;
    if (netplay_enabled()) {   /* phase 7: the netplay loop runs the ticks; it ends the session itself */
        uint64_t frames = s_benchmark;
        s_benchmark = 0;
        main_np_begin();
        netplay_run(frames);
    }
    if (state_in) {   /* from a saved frame: as the branch from there (run_branches) */
        machine_start();
        state_file_load(state_in);
        if (s_loaded_at >= s_benchmark || (!s_plan.empty() && s_plan.front().at < s_loaded_at)
            || (!s_states.empty() && s_states.front().at <= s_loaded_at)) {
            fprintf(stderr, "[native] %s is of frame %llu: past this run's --benchmark, branches or states\n", state_in,
                    (unsigned long long)s_loaded_at);
            return 2;
        }
        /* phase 6 E4's probes: a run that differs from the saving one only from its flip on (the
           first flipped frame, the scan's first) starts from a frame before that */
        if ((s_flip && s_flip <= s_loaded_at) || (s_scan_hi && s_scan_lo <= s_loaded_at)) {
            fprintf(stderr, "[native] %s is of frame %llu: not before this run's first flipped frame %llu\n", state_in,
                    (unsigned long long)s_loaded_at, (unsigned long long)(s_flip ? s_flip : s_scan_lo));
            return 2;
        }
        at_sync_work(s_sync_rows);
        machine_loop();
    }
    machine_run();   /* never returns: the game ends the process (the benchmark, a quit, a fatal) */
    return 4;
}
