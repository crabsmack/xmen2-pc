/* capture.cpp - recording the port for showing it (xmen2disasm docs/RELEASE.md): development and
 * capture features, nothing the game sees.
 *
 *   --record-inputs F   (played, netplay too) every runner frame's four pads, written at exit as an
 *                       input script with a header that replays it: the session seed, the start
 *                       scene and the game options (`tools/capture.py` reads it). A frame run again
 *                       (a rollback, a savestate load) replaces what followed it, so the file is the
 *                       timeline that was shown.
 *   XMEN2_CAPTURE=F     every display period's picture (a frame shown twice, as two players at the
 *                       original 30 Hz are, is written twice: the video is in the sound's time),
 *                       lossless (FFV1 in Matroska), through an ffmpeg on PATH, on
 *                       a fixed canvas (XMEN2_CAPTURE_W, default 400 with widescreen, else 320, x 224):
 *                       narrower screens (the 4:3 Cerebro screens in 16:9) centred on black.
 *   XMEN2_STEM=music|sfx  the sound of only the music or only the sound effects: the chips' key-ons
 *                       (FM), DAC samples and PSG volumes of the other kind's voices are dropped on
 *                       their way to the synthesis. The driver, its RAM and the YM2612's timers run as
 *                       always, so the game and its picture are those of a run without it.
 *   XMEN2_SND_LOG=F     every play/stop command with the class it got, and a per-song summary at exit.
 *
 * Music or sound effect: GEMS plays both as songs. A song the game starts in the same frame as a
 * stop-all (Music_Play, Music_PlayStage, the Cerebro screens, the stage scripts that change the music)
 * is music, and so is every song a survey found that way (the list in capture_sound_command); every
 * other song the game starts is a sound effect. A song the game never started is one the driver
 * started from inside a song (the stage songs' player parts): music.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include "native.h"

extern "C" {
#include "video/genesis_machine.h"
}

std::string options_game_text(void);
int options_first_scene(void);

/* ---- sound: the class of each song, the stem filter ------------------------------------------------- */

enum { CLS_NONE = 0, CLS_MUSIC = 1, CLS_SFX = 2 };
extern "C" void capture_report(void);
static uint8_t  s_cls[256];
static uint64_t s_plays[256], s_first[256];
static uint64_t s_stopall_frame = ~0ull;
static int      s_stem = -1;   /* -1 off, CLS_MUSIC, CLS_SFX: the kind that sounds */
static FILE    *s_snd_log;

#define SLOTS      0x1B80u   /* the driver's 16 track slots, $20 bytes: +$0E song (sound2.cpp) */
#define FM_POOL    0x1772u   /* the voice pools, 7 bytes an entry: +0 channel (bit 7 free), +3 owner track */
#define PSG_POOL   0x179Du
#define NOISE_POOL 0x17B3u

static void snd_init(void)
{
    static bool done;
    if (done) return;
    done = true;
    const char *s = getenv("XMEN2_STEM");
    if (s && !strcmp(s, "music")) s_stem = CLS_MUSIC;
    else if (s && !strcmp(s, "sfx")) s_stem = CLS_SFX;
    else if (s && *s) { fprintf(stderr, "[capture] XMEN2_STEM=%s: music or sfx\n", s); exit(2); }
    const char *l = getenv("XMEN2_SND_LOG");
    if (l && *l && !(s_snd_log = fopen(l, "w"))) { fprintf(stderr, "[capture] cannot write %s\n", l); exit(2); }
    if (s_snd_log) {
        fprintf(s_snd_log, "frame,command,song,class\n");
        atexit(capture_report);
    }
}

static const char *cls_name(int c) { return c == CLS_MUSIC ? "music" : c == CLS_SFX ? "sfx" : "driver"; }

/* a song the game starts in frame f: music? */
static bool is_music(uint8_t id, uint64_t f)
{
    /* the songs the rule below found to be music in a survey of every stage, the title and the
       attract demos (6,000 frames each, 2026-10-23); some of them also start without a stop-all
       (the Brood's theme $30, Avalon's $00), so the list decides for them */
    static const uint8_t music[] = { 0x00, 0x01, 0x02, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1F, 0x23, 0x25, 0x27,
                                     0x30, 0x3E, 0x40, 0x44, 0x47, 0x48, 0x51, 0x5C, 0x5E, 0x5F, 0x6F, 0x70 };
    return memchr(music, id, sizeof music) != nullptr || f == s_stopall_frame;
}

/* sound2.cpp's ring parser: one complete command the game sent the first machine */
extern "C" void capture_sound_command(uint8_t c, const uint8_t *a)
{
    snd_init();
    uint64_t f = machine_frame_no();
    if (c == 0x16) s_stopall_frame = f;
    if (c == 0x10) {
        uint8_t id = a[0];
        s_cls[id] = is_music(id, f) ? CLS_MUSIC : CLS_SFX;
        if (!s_plays[id]++) s_first[id] = f;
    }
    if (s_snd_log && (c == 0x10 || c == 0x12 || c == 0x16))
        fprintf(s_snd_log, "%llu,%02X,%02X,%s\n", (unsigned long long)f, c, c == 0x16 ? 0 : a[0],
                c == 0x10 ? cls_name(s_cls[a[0]]) : "");
}

/* XMEN2_BED=1 (a music bed, not a take): the sound effects never reach the driver, so none of them
   takes a voice from the music (the driver steals by priority, and a stolen note is lost in the
   music stem too). A play command of an effect, `$FF $10 id` in the ring, is made `$FF $12 id` (a
   stop of what isn't playing) as its id byte arrives: the game holds the Z80's bus through
   Snd_Play, so the driver reads none of it before. The bytes are followed in the order the game
   writes them (Snd_PutByte publishes the index after each one). The game may differ from the take's
   from then on: for the music alone. machine.cpp's plat_z80_ram_write8 calls this for each byte. */
#define RING 0x1B40u   /* the command ring (Snd_PutByte), its write index at $36 (sound2.cpp) */
static bool is_music(uint8_t id, uint64_t frame);
extern "C" void capture_ring_write(uint8_t *ram, uint32_t off, uint8_t v)
{
    static const bool bed = [] { const char *b = getenv("XMEN2_BED"); return b && *b == '1'; }();
    static int state;              /* 0: before a $FF, 1: the command byte next, 2: a play's id next */
    static uint32_t cmd_off;
    static uint64_t stopall = ~0ull;
    if (!bed || off < RING || off >= RING + 0x40) return;
    if (state == 0) { if (v == 0xFF) state = 1; return; }
    if (state == 1) {
        if (v == 0x16) stopall = machine_frame_no();
        cmd_off = off;
        state = v == 0x10 ? 2 : 0;   /* other commands' arguments are never $FF-led here: resync on the next $FF */
        return;
    }
    uint64_t f = machine_frame_no();
    if (f != stopall && !is_music(v, f)) ram[cmd_off] = 0x12;
    state = 0;
}

/* XMEN2_SOLO=27,2B (a multitrack music render): only those songs' tracks play. Every other track
   slot is kept muted in the driver (+6 bit 1, as the second sound machine mutes base tracks), so its
   note-ons are skipped and take no voice: a song alone is never short of voices (the stage songs
   with two player parts need more FM voices than the chip has, and the driver drops the
   lowest-priority notes). Render each layer with the same input and mix them. The game never reads
   the slots (Snd_ReadDriverByte reads the variables), so every pass is the same game.
   machine.cpp's z80_bus_write calls this when the driver writes a slot. */
static uint8_t s_solo[256];
static int s_solo_on = -1;
extern "C" void capture_slot_written(uint8_t *ram, uint32_t addr)
{
    if (s_solo_on < 0) {
        s_solo_on = 0;
        const char *s = getenv("XMEN2_SOLO");
        for (const char *p = s; p && *p; ) {
            char *e;
            unsigned long id = strtoul(p, &e, 16);
            if (e == p) { fprintf(stderr, "[capture] XMEN2_SOLO=%s: hex song ids, comma-separated\n", s); exit(2); }
            s_solo[id & 0xFF] = 1;
            s_solo_on = 1;
            p = *e ? e + 1 : e;
        }
    }
    if (!s_solo_on || addr < SLOTS || addr >= SLOTS + 16 * 0x20) return;
    static uint8_t forced[16];   /* slots this muted: a slot reused for a solo song is unmuted (as sound2.cpp) */
    unsigned t = (addr - SLOTS) >> 5;
    uint8_t *slot = &ram[SLOTS + 0x20 * t];
    if (!s_solo[slot[0x0E]]) {
        slot[6] |= 0x02;
        forced[t] = 1;
    } else if (forced[t]) {
        slot[6] &= (uint8_t)~0x02;
        forced[t] = 0;
    }
}

/* the class of the song whose track holds the voice on channel ch of a pool (CLS_NONE: no owner,
   which both stems keep) */
static int s_song = -1;            /* the song voice_class found (-1: none) */
static uint64_t s_keyons[256];     /* FM key-ons that sounded, per song (XMEN2_SND_LOG's summary) */
static int voice_class(uint16_t pool, int entries, int ch)
{
    const uint8_t *ram = g_machine.bus.z80_ram;
    s_song = -1;
    for (int i = 0; i < entries; i++) {
        const uint8_t *e = &ram[pool + 7 * i];
        if (e[0] == 0xFF) break;
        if ((e[0] & 0x80) || (e[0] & 0x07) != ch) continue;
        uint8_t song = ram[SLOTS + 0x20 * (e[3] & 0x0F) + 0x0E];
        s_song = song;
        return s_cls[song] == CLS_SFX ? CLS_SFX : CLS_MUSIC;
    }
    return CLS_NONE;
}
static bool other_kind(int k) { return k != CLS_NONE && k != s_stem; }

/* XMEN2_STEM_DEBUG: what the filter saw, by chip event and class (none, music, sfx), at exit */
static uint64_t s_dbg[3][3];   /* [key-on, DAC, PSG volume][class] */
static void stem_debug_report(void)
{
    static const char *ev[3] = { "fm key-on", "dac sample", "psg volume" };
    for (int e = 0; e < 3; e++)
        fprintf(stderr, "[capture] stem %s: none %llu music %llu sfx %llu\n", ev[e], (unsigned long long)s_dbg[e][0],
                (unsigned long long)s_dbg[e][1], (unsigned long long)s_dbg[e][2]);
}
static int counted(int e, int k)
{
    static const bool on = getenv("XMEN2_STEM_DEBUG") != nullptr;
    static bool reg;
    if (on && !reg) { reg = true; atexit(stem_debug_report); }
    if (on) s_dbg[e][k]++;
    return k;
}

/* A channel written after the driver freed its voice (a PSG note's release: the driver keeps writing
   its volume) belongs to the song that last held it. [FM, PSG][channel] */
static int s_last[2][8];
static int owner_or_last(int chip, int ch, int k)
{
    if (k != CLS_NONE) s_last[chip][ch & 7] = k;
    return k != CLS_NONE ? k : s_last[chip][ch & 7];
}

/* main.cpp's plat_audio_write: false = drop the write; *v may be changed */
extern "C" bool capture_audio_write(int port, uint8_t *v)
{
    snd_init();
    if (s_stem < 0) return true;
    static uint8_t sel[2];
    if (port >= 0 && port <= 3) {
        if (!(port & 1)) { sel[port >> 1] = *v; return true; }
        if (port == 1 && sel[0] == 0x28 && (*v & 0xF0)) {   /* key-on */
            int ch = *v & 7;
            if (other_kind(owner_or_last(0, ch, counted(0, voice_class(FM_POOL, 6, ch))))) *v &= 0x0F;   /* keys off instead */
            else if (s_song >= 0) s_keyons[s_song]++;
        } else if (port == 1 && sel[0] == 0x2A) {           /* a DAC sample: FM channel 6 */
            if (other_kind(owner_or_last(0, 6, counted(1, voice_class(FM_POOL, 6, 6))))) *v = 0x80;
        }
        return true;
    }
    if ((*v & 0x90) == 0x90) {   /* a PSG volume */
        int ch = *v >> 5 & 3;
        int k = counted(2, ch == 3 ? voice_class(NOISE_POOL, 1, 3) : voice_class(PSG_POOL, 3, ch));
        if (other_kind(owner_or_last(1, ch, k))) *v |= 0x0F;
    }
    return true;
}

/* host_audio: the second sound machine is players 3-4's music parts */
extern "C" bool capture_mute_snd2(void) { snd_init(); return s_stem == CLS_SFX; }

/* ---- the inputs ---------------------------------------------------------------------------------- */

static const char *s_rec_path;
static std::vector<std::array<uint16_t, 4>> *s_rec;

static void rec_write(void)
{
    if (!s_rec_path || !s_rec) return;
    FILE *f = fopen(s_rec_path, "w");
    if (!f) { fprintf(stderr, "[capture] cannot write %s\n", s_rec_path); return; }
    fprintf(f, "# xmen2 input recording: %zu runner frames\n", s_rec->size() > 0 ? s_rec->size() - 1 : 0);
    fprintf(f, "# session %llu\n", (unsigned long long)machine_session_seed());
    const char *env_scene = getenv("XMEN2_DEBUG_SCENE");
    fprintf(f, "# scene %d\n", options_first_scene() >= 0 ? options_first_scene() : env_scene && *env_scene ? atoi(env_scene) : -1);
    fprintf(f, "# options %s\n", options_game_text().c_str());
    size_t n = 0;
    for (size_t i = 1; i < s_rec->size(); i++) {
        const auto &p = (*s_rec)[i];
        bool last = i + 1 == s_rec->size();
        n++;
        if (last || (*s_rec)[i + 1] != p) {
            fprintf(f, "PADS %X %X %X %X\nWAIT %zu\n", p[0], p[1], p[2], p[3], n);
            n = 0;
        }
    }
    fclose(f);
    fprintf(stderr, "[capture] wrote %s (%zu frames)\n", s_rec_path, s_rec->size() > 0 ? s_rec->size() - 1 : 0);
}

extern "C" void capture_record_start(const char *path)
{
    s_rec_path = path;
    s_rec = new std::vector<std::array<uint16_t, 4>>(1);
    atexit(rec_write);
}

/* netplay.cpp: only frames 1..frames are final (the rest ran on predicted inputs) */
extern "C" void capture_record_truncate(uint64_t frames)
{
    if (s_rec && s_rec->size() > frames + 1) s_rec->resize(frames + 1);
}

/* main.cpp's plat_frame_begin: the pads runner frame `frame` runs with (frames count from 1) */
extern "C" void capture_record_pads(uint64_t frame, const uint16_t pad[4])
{
    if (!s_rec || !frame) return;
    if (s_rec->size() > frame) s_rec->resize(frame);   /* run again: what followed is gone */
    while (s_rec->size() < frame) s_rec->push_back(s_rec->back());
    s_rec->push_back({ pad[0], pad[1], pad[2], pad[3] });
}

/* ---- the picture --------------------------------------------------------------------------------- */

static FILE *s_cap;
static int   s_cap_w, s_cap_h = 224;
static std::vector<uint32_t> s_canvas;

static void cap_close(void)
{
    if (!s_cap) return;
#ifdef _WIN32
    _pclose(s_cap);
#else
    pclose(s_cap);
#endif
    s_cap = nullptr;
}

/* main.cpp's plat_frame_end: a frame that is shown (fb is ARGB8888, `stride` pixels a row) */
extern "C" void capture_video_frame(const uint32_t *fb, int stride, int w, int h)
{
    static bool tried;
    if (!tried) {
        tried = true;
        const char *path = getenv("XMEN2_CAPTURE");
        if (!path || !*path) return;
        const char *cw = getenv("XMEN2_CAPTURE_W");
        s_cap_w = cw && *cw ? atoi(cw) : (g_opt.widescreen ? 400 : 320);
        s_canvas.assign((size_t)s_cap_w * s_cap_h, 0xFF000000u);
        char cmd[1200];
        snprintf(cmd, sizeof cmd,
                 "ffmpeg -hide_banner -loglevel error -y -f rawvideo -pix_fmt bgra -video_size %dx%d "
                 "-framerate 53693175/896040 -i - -c:v ffv1 -level 3 -g 1 -pix_fmt bgr0 \"%s\"",
                 s_cap_w, s_cap_h, path);
#ifdef _WIN32
        s_cap = _popen(cmd, "wb");
#else
        s_cap = popen(cmd, "w");
#endif
        if (!s_cap) { fprintf(stderr, "[capture] cannot start ffmpeg for %s\n", path); exit(2); }
        atexit(cap_close);
    }
    if (!s_cap) return;
    std::fill(s_canvas.begin(), s_canvas.end(), 0xFF000000u);
    int cw = w < s_cap_w ? w : s_cap_w, ch = h < s_cap_h ? h : s_cap_h;
    int x0 = (s_cap_w - cw) / 2, y0 = (s_cap_h - ch) / 2, sx = (w - cw) / 2;
    for (int y = 0; y < ch; y++)
        memcpy(&s_canvas[(size_t)(y0 + y) * s_cap_w + x0], fb + (size_t)y * stride + sx, (size_t)cw * 4);
    fwrite(s_canvas.data(), 4, s_canvas.size(), s_cap);
}

/* main.cpp's plat_frame_repeat: one more display period of the same picture (two players at the
   original 30 Hz show each frame twice), so the video keeps the sound's time */
extern "C" void capture_video_repeat(void)
{
    if (s_cap) fwrite(s_canvas.data(), 4, s_canvas.size(), s_cap);
}

/* ---- the summary ------------------------------------------------------------------------------- */

extern "C" void capture_report(void)
{
    if (!s_snd_log) return;
    fprintf(s_snd_log, "# song,class,plays,first_frame\n");
    for (int i = 0; i < 256; i++)
        if (s_plays[i])
            fprintf(s_snd_log, "# %02X,%s,%llu,%llu\n", i, cls_name(s_cls[i]), (unsigned long long)s_plays[i],
                    (unsigned long long)s_first[i]);
    fprintf(s_snd_log, "# keyons song,fm_keyons_heard (with XMEN2_STEM)\n");
    for (int i = 0; i < 256; i++)
        if (s_keyons[i]) fprintf(s_snd_log, "# keyons %02X,%llu\n", i, (unsigned long long)s_keyons[i]);
    fclose(s_snd_log);
    s_snd_log = nullptr;
}
