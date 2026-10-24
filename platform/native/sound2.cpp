/* sound2.cpp - the audio-only sound machines: the second (phase 7 R9, xmen2disasm
 * docs/phases/P7-multiplayer.md), players 3 and 4's music parts, and music_voices' two, the music.
 * Each is its own Z80 with the same GEMS driver, YM2612 and PSG, fed the music commands the game
 * writes into the first machine's command ring (snd2_ring_write); the 68000 never sees them, and the
 * host mixes them with the first (sound_mix).
 *
 * The second machine (R9). The stage songs with player parts ($27, $25, $48, $64 by way of $5E/$5F)
 * start the parts from dispatcher tracks that test the driver's variables $B and $C, players 1 and
 * 2's characters (the part songs: kickoff notes section 3). With three or more players in the game it
 * gets play and stop of those songs, stop all, pause, resume, the variables, with $B/$C = players 3's
 * and 4's characters and $A = 2 (no sound effects), and plays the same song with its own (base)
 * tracks muted in their track slots, so only players 3's and 4's part songs sound there, at the base
 * song's tempo and start. It runs only while a player 3 or 4 is in the game. It is in the snapshot
 * ("snd2") and, with three or more players, in the canonical digest (snd2_digest_fields).
 *
 * music_voices (on by default; the original: 0). The GEMS driver takes a voice for each note and,
 * with none free, steals the lowest-priority one or drops the note: a stage song with two players'
 * parts wants up to 7 FM voices of the YM2612's 6 (kickoff notes section 4), and sound effects take
 * voices from the music. With the option on (in the session machine, where the chips run) the music
 * machine gets every command the game sends, as it is, and keeps the effects' tracks (capture.cpp's
 * classes: music is a song the game starts with a stop-all or in its survey list, or one the driver
 * starts from inside another) and the part songs' muted in its track slots: a muted track takes no
 * voice, but what it does besides notes is done (some effects start or stop music). The parts machine
 * gets the same and plays only players 1's and 2's part songs (every other track muted). So the
 * three drivers run the same songs; each sounds its own. The first machine runs as always (its RAM, chips'
 * registers and timers: the game and the canonical digest are the original's), and its music voices
 * are dropped on their way to its synthesis (capture_audio_write), so it sounds the effects. The two
 * are presentation only, like a seat's own 16:9 view (R12): each seat's own online, not in the
 * canonical digest or the sealed options; they are in the snapshot ("snd_music", "snd_parts"), so a
 * rollback or a savestate keeps them in step.
 *
 * The YM2612 timers and the Z80's frame are the first machine's (machine.cpp), copied: the same
 * driver sees the same machine. Every machine (and the first) counts its key-ons and the voices
 * other songs stole, by song (sound_counters_json).
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

extern "C" {
#include "external/superzazu/z80.h"
}
#include "audio/event_queue.h"

#include "native.h"
#include "state.h"

#define CHIP_DECLS(p)                                                                                     \
    void   p##_ym2612_init(void);                                                                           \
    void   p##_ym2612_advance(uint32_t cycles_master);                                                      \
    void   p##_ym2612_write(uint8_t port, uint8_t value);                                                   \
    size_t p##_ym2612_render(int16_t *out, size_t sample_count);                                            \
    size_t p##_ym2612_samples_available(void);                                                              \
    size_t p##_ym2612_rb_save(void *dst, size_t cap);                                                       \
    int    p##_ym2612_rb_load(const void *src, size_t len);                                                 \
    void   p##_psg_init(void);                                                                              \
    void   p##_psg_advance(uint32_t cycles_master);                                                         \
    void   p##_psg_write(uint8_t value);                                                                    \
    size_t p##_psg_render(int16_t *out, size_t sample_count);                                               \
    size_t p##_psg_samples_available(void);                                                                 \
    void   p##_psg_reset_leftover(void);                                                                    \
    size_t p##_psg_rb_save(void *dst, size_t cap);                                                          \
    int    p##_psg_rb_load(const void *src, size_t len);
extern "C" {
CHIP_DECLS(snd2)
CHIP_DECLS(mus)
CHIP_DECLS(prt)
extern uint8_t xm_region[0x10000];
int P34_MusicVoice(int k);   /* game/runtime/players34.cpp: Player_MusicVoice of player k (3, 4) */
}

/* a machine's chips: one compiled copy of the engine's wrappers (snd2_chips.cpp, mus_, prt_) */
struct Chips {
    void   (*ym_init)(void);
    void   (*ym_advance)(uint32_t);
    void   (*ym_write)(uint8_t, uint8_t);
    size_t (*ym_render)(int16_t *, size_t);
    size_t (*ym_available)(void);
    size_t (*ym_save)(void *, size_t);
    int    (*ym_load)(const void *, size_t);
    void   (*psg_init)(void);
    void   (*psg_advance)(uint32_t);
    void   (*psg_write)(uint8_t);
    size_t (*psg_render)(int16_t *, size_t);
    size_t (*psg_available)(void);
    void   (*psg_reset_leftover)(void);
    size_t (*psg_save)(void *, size_t);
    int    (*psg_load)(const void *, size_t);
};
#define CHIPS(p)                                                                                          \
    { p##_ym2612_init, p##_ym2612_advance, p##_ym2612_write, p##_ym2612_render,                            \
      p##_ym2612_samples_available, p##_ym2612_rb_save, p##_ym2612_rb_load, p##_psg_init, p##_psg_advance, \
      p##_psg_write, p##_psg_render, p##_psg_samples_available, p##_psg_reset_leftover, p##_psg_rb_save,   \
      p##_psg_rb_load }
static const Chips k_chips[3] = { CHIPS(snd2), CHIPS(mus), CHIPS(prt) };

#define MPL            3420u
#define LINES          262
#define MASTER_PER_Z80 15u
#define FM_SAMPLE      1008u
#define FRAME_END      895780u   /* host_audio's drain end (machine 1's) */
#define DRIVER         0x1FE66Cu /* Snd_Z80Driver .. Snd_Z80DriverEnd: the GEMS driver Z80_LoadDriver copies */
#define DRIVER_END     0x1FFED3u
#define RING           0x1B40u   /* the command ring (Snd_PutByte), its write index at $36 */
#define SLOTS          0x1B80u   /* the 16 track slots, $20 bytes: +6 flags (bit 1: muted), +$0E song */
#define FM_POOL        0x1772u   /* the voice pools, 7 bytes an entry: +0 channel (bit 7 free), +3 owner track */
#define PSG_POOL       0x179Du
#define NOISE_POOL     0x17B3u

/* the part songs (kickoff notes section 3): $27/$25's, $48's, $64's; every other song's tracks are
   base tracks */
static bool part_song(uint8_t s)
{
    return (s >= 0x28 && s <= 0x2F && s != 0x2E) || (s >= 0x49 && s <= 0x50 && s != 0x4F) || (s >= 0x65 && s <= 0x6B);
}
/* the songs whose dispatchers start parts (Snd_Play ids), and the base song the counters name */
static int dispatch_base(uint8_t id)
{
    switch (id) {
    case 0x27: case 0x25: case 0x48: return id;
    case 0x5E: case 0x5F: return 0x64;   /* CloneA1/A2: they start $64 */
    default: return -1;
    }
}

/* the song of the track that holds the voice of channel ch in a pool of Z80 RAM (-1: none found) */
static int owner_song(const uint8_t *ram, uint16_t pool, int entries, int ch)
{
    for (int i = 0; i < entries; i++) {
        const uint8_t *e = &ram[pool + 7 * i];
        if (e[0] == 0xFF) break;
        if ((e[0] & 0x80) || (e[0] & 0x07) != ch) continue;
        unsigned t = e[3] & 0x0F;
        return ram[SLOTS + 0x20 * t + 0x0E];
    }
    return -1;
}

/* ---- the voice counters (every machine, the first too): host counters, restored with a snapshot ---- */

/* Per song: FM key-ons and PSG notes (a volume on a silent channel) the driver wrote, and its voices
   stolen by another song's note (a pool entry given to a track of another song while busy: its +3
   owner rewritten with no free, +0 bit 7, since it was last given). The watch state is what
   decides them. */
struct VoiceStats {
    uint64_t frames;
    uint32_t fm[257], psg[257], stolen[256], stole[256];   /* [256]: no owner found */
    uint64_t fm_time[257], psg_time[257];   /* master clocks its FM channels were keyed, its PSG channels loud */
    uint64_t since[12];                     /* FM channel 0-7, PSG 8-11: keyed (loud) since, or ~0 */
    int16_t  owner[12];
    uint8_t  sel, att[4], freed[10], owned[10];
};
#define FRAME_CLOCKS ((uint64_t)262 * 3420)

/* a channel's time to now goes to its owner; then it is keyed (loud) by owner `song` from now, or not */
static void vs_time(VoiceStats *V, int c, uint64_t now, bool on, int song)
{
    if (V->since[c] != ~0ull) {
        uint64_t *t = c < 8 ? V->fm_time : V->psg_time;
        t[V->owner[c] < 0 ? 256 : V->owner[c]] += now - V->since[c];
    }
    V->since[c] = on ? now : ~0ull;
    if (song >= 0 || !on) V->owner[c] = (int16_t)song;
}

static void vs_ram_write(VoiceStats *V, const uint8_t *ram, uint32_t off, uint8_t v)
{
    int i, f;
    if (off >= FM_POOL && off < FM_POOL + 6 * 7) { i = (int)(off - FM_POOL) / 7; f = (int)(off - FM_POOL) % 7; }
    else if (off >= PSG_POOL && off < PSG_POOL + 3 * 7) { i = 6 + (int)(off - PSG_POOL) / 7; f = (int)(off - PSG_POOL) % 7; }
    else if (off >= NOISE_POOL && off < NOISE_POOL + 7) { i = 9; f = (int)(off - NOISE_POOL); }
    else return;
    const uint8_t *e = &ram[off - f];
    if (f == 0 && (v & 0x80)) V->freed[i] = 1;
    if (f != 3) return;
    uint8_t was = e[3] & 0x0F, now = v & 0x0F;
    if (V->owned[i] && !V->freed[i] && !(e[0] & 0x80)) {
        uint8_t victim = ram[SLOTS + 0x20 * was + 0x0E], by = ram[SLOTS + 0x20 * now + 0x0E];
        if (was != now && victim != by) { V->stolen[victim]++; V->stole[by]++; }
    }
    V->freed[i] = 0;
    V->owned[i] = 1;
}

/* a machine's chips and RAM start (power-on, a machine started): every PSG channel silent, no voice given */
static void vs_reset(VoiceStats *V)
{
    V->sel = 0;
    memset(V->att, 0x0F, sizeof V->att);
    memset(V->freed, 0, sizeof V->freed);
    memset(V->owned, 0, sizeof V->owned);
    for (int c = 0; c < 12; c++) { V->since[c] = ~0ull; V->owner[c] = -1; }
}

/* now: master clocks since the machine's counting began (its frames and the stamp in the frame) */
static void vs_ym_write(VoiceStats *V, const uint8_t *ram, unsigned port, uint8_t v, uint32_t stamp)
{
    if (port == 0) V->sel = v;
    else if (port == 1 && V->sel == 0x28) {
        int ch = v & 7, song = (v & 0xF0) ? owner_song(ram, FM_POOL, 6, ch) : -1;
        vs_time(V, ch, V->frames * FRAME_CLOCKS + stamp, (v & 0xF0) != 0, song);
        if (v & 0xF0) V->fm[song < 0 ? 256 : song]++;
    }
}

static void vs_psg_write(VoiceStats *V, const uint8_t *ram, uint8_t v, uint32_t stamp)
{
    if ((v & 0x90) != 0x90) return;
    int ch = v >> 5 & 3, att = v & 0x0F;
    int song = ch == 3 ? owner_song(ram, NOISE_POOL, 1, 3) : owner_song(ram, PSG_POOL, 3, ch);
    if (att != 0x0F && V->att[ch] == 0x0F) V->psg[song < 0 ? 256 : song]++;
    vs_time(V, 8 + ch, V->frames * FRAME_CLOCKS + stamp, att != 0x0F, song);
    V->att[ch] = (uint8_t)att;
}

static std::string vs_json(const VoiceStats &V)
{
    std::string s = "{\"frames\": " + std::to_string(V.frames) + ", \"songs\": {";
    bool first = true;
    for (int i = 0; i < 257; i++) {
        uint32_t st = i < 256 ? V.stolen[i] : 0, by = i < 256 ? V.stole[i] : 0;
        if (!V.fm[i] && !V.psg[i] && !st && !by && !V.fm_time[i] && !V.psg_time[i]) continue;
        char id[8], b[200];
        snprintf(id, sizeof id, i < 256 ? "%02X" : "none", i);
        /* the times in ms (master clock 53,693,175 Hz) */
        snprintf(b, sizeof b, "%s\"%s\": {\"fm\": %u, \"psg\": %u, \"stolen\": %u, \"stole\": %u, \"fm_ms\": %llu, \"psg_ms\": %llu}",
                 first ? "" : ", ", id, V.fm[i], V.psg[i], st, by, (unsigned long long)(V.fm_time[i] / 53693),
                 (unsigned long long)(V.psg_time[i] / 53693));
        s += b;
        first = false;
    }
    return s + "}}";
}

static VoiceStats s_m1;   /* the first machine's */

/* machine.cpp's z80_bus_write: the first machine's driver writes Z80 RAM (before the byte changes)
   or a chip */
extern "C" void snd_m1_ram_write(const uint8_t *ram, uint32_t off, uint8_t v)
{
    if (off >= FM_POOL && off < NOISE_POOL + 7 && machine_session()) vs_ram_write(&s_m1, ram, off, v);
}

extern "C" void snd_m1_audio_write(const uint8_t *ram, int port, uint8_t v, uint32_t stamp)
{
    if (!machine_session()) return;
    if (port == PLAT_AUDIO_PSG) vs_psg_write(&s_m1, ram, v, stamp);
    else vs_ym_write(&s_m1, ram, (unsigned)port, v, stamp);
}

static int arg_count(uint8_t c);

/* ---- a machine ------------------------------------------------------------------------------------- */

enum Role { ROLE_P34, ROLE_MUSIC, ROLE_PARTS };

/* Everything one machine is, one plain struct (the snapshot; the Z80's callbacks scrubbed). The ring
   parser's fields are the second machine's (the one parse feeds all three). */
struct Snd2 {
    z80      z;
    uint8_t  ram[0x2000];
    uint16_t bank;
    uint8_t  active, ei_shadow;
    uint32_t zclk, znow, zstamp;
    uint32_t fm_count, fm_start;
    int32_t  fm_ta, fm_tal, fm_tac, fm_tbl, fm_tbc;
    uint8_t  fm_mode, fm_status;
    uint16_t fm_addr;
    uint8_t  ym_reg[512], ym_sel[2];
    uint8_t  psg_latch, psg_att[4];
    uint8_t  forced[16];             /* slots this machine muted */
    int16_t  base;                   /* the dispatcher song playing (its base id), -1 none */
    /* what the first machine was told (its ring, parsed: snd2_ring_write), for a later start */
    uint8_t  npend, pend[4];         /* ring bytes written since the index moved (offsets; npend $FF: not commands) */
    uint8_t  pstate, cmd, need, nargs, args[12];
    uint8_t  have_init, init[12];
    uint8_t  param_set[4], param[32];
    int16_t  song;                   /* the song to start with the machine (P34: the dispatcher song), -1 none */
};

struct Ev { uint32_t stamp; uint8_t port, v; };

/* the second machine's counters (host counters: restored with a snapshot, not state) */
struct SongCount { int16_t id; uint64_t part, base; };
struct Counters { uint64_t frames; SongCount songs[8]; };

struct Machine {
    Role             role;
    const char      *name;   /* its snapshot section */
    const Chips     *chip;
    Snd2             S;
    std::vector<Ev>  ev;      /* the chips' writes not yet drained (mix) */
    VoiceStats       V;
    Counters         C;       /* ROLE_P34 */

    /* ---- the YM2612's timers, as machine.cpp has them ---- */
    void fm_update(uint32_t t)
    {
        if (t <= S.fm_count) return;
        uint32_t n = (t - S.fm_count + FM_SAMPLE - 1) / FM_SAMPLE;
        for (uint32_t i = 0; i < n; i++)
            if (S.fm_mode & 0x01) {
                if (--S.fm_tac <= 0) {
                    if (S.fm_mode & 0x04) S.fm_status |= 0x01;
                    S.fm_tac = S.fm_tal;
                }
            }
        if (S.fm_mode & 0x02) {
            S.fm_tbc -= (int)n;
            if (S.fm_tbc <= 0) {
                if (S.fm_mode & 0x08) S.fm_status |= 0x02;
                if (S.fm_tbl) S.fm_tbc += S.fm_tbl; else S.fm_tbc = S.fm_tbl;
            }
        }
        S.fm_count += n * FM_SAMPLE;
    }

    void fm_set_timers(uint8_t v)
    {
        if ((v & 1) && !(S.fm_mode & 1)) S.fm_tac = S.fm_tal;
        if ((v & 2) && !(S.fm_mode & 2)) S.fm_tbc = S.fm_tbl;
        S.fm_status &= (uint8_t)(~v >> 4);
        S.fm_mode = v;
    }

    void ym_timers(uint32_t t, unsigned port, uint8_t v)
    {
        if (port & 1) fm_update(t);
        switch (port & 3) {
        case 0: S.fm_addr = v; break;
        case 2: S.fm_addr = (uint16_t)(v | 0x100); break;
        default:
            switch (S.fm_addr) {
            case 0x24: S.fm_ta = (S.fm_ta & 0x03) | (v << 2); S.fm_tal = 1024 - S.fm_ta; break;
            case 0x25: S.fm_ta = (S.fm_ta & 0x3FC) | (v & 3); S.fm_tal = 1024 - S.fm_ta; break;
            case 0x26: S.fm_tbl = (256 - v) << 4; break;
            case 0x27: fm_set_timers(v); break;
            default: break;
            }
            break;
        }
    }

    /* ---- key-ons, by the track (and so the song) that owns the voice ---- */
    void count_keyon(int song)
    {
        if (role != ROLE_P34 || S.base < 0) return;
        SongCount *c = song_count(S.base);
        if (song >= 0 && part_song((uint8_t)song)) c->part++;
        else c->base++;
        static const bool dbg = getenv("XMEN2_SND2_DEBUG") != nullptr;
        if (dbg) fprintf(stderr, "[snd2] frame %llu key-on song $%02X (base $%02X)\n", (unsigned long long)machine_frame_no(),
                         song & 0xFF, S.base);
    }

    SongCount *song_count(int id)
    {
        for (SongCount &s : C.songs)
            if (s.id == id) return &s;
        for (SongCount &s : C.songs)
            if (s.id == 0) { s.id = (int16_t)id; return &s; }
        return &C.songs[7];
    }

    void ym_write(unsigned port, uint8_t v)
    {
        ym_timers(S.znow, port, v);
        vs_ym_write(&V, S.ram, port, v, S.zstamp);
        if (!(port & 1)) S.ym_sel[port >> 1 & 1] = v;
        else {
            unsigned r = (port >> 1 & 1) << 8 | S.ym_sel[port >> 1 & 1];
            S.ym_reg[r] = v;
            if (r == 0x28 && (v & 0xF0)) count_keyon(owner_song(S.ram, FM_POOL, 6, v & 7));
        }
        if (machine_session()) ev.push_back({ S.zstamp, (uint8_t)port, v });
    }

    void psg_write(uint8_t v)
    {
        vs_psg_write(&V, S.ram, v, S.zstamp);
        if (v & 0x80) S.psg_latch = (uint8_t)(v >> 4 & 7);
        if ((v & 0x90) == 0x90) {   /* a volume: a note sounds when its channel was silent */
            int ch = v >> 5 & 3, att = v & 0x0F;
            if (att != 0x0F && S.psg_att[ch] == 0x0F)
                count_keyon(ch == 3 ? owner_song(S.ram, NOISE_POOL, 1, 3) : owner_song(S.ram, PSG_POOL, 3, ch));
            S.psg_att[ch] = (uint8_t)att;
        }
        if (machine_session()) ev.push_back({ S.zstamp, (uint8_t)PLAT_AUDIO_PSG, v });
    }

    /* ---- the Z80's bus ---- */

    /* the songs whose tracks this machine keeps muted: the second and the parts machine play only part
       songs, the music machine the music but the parts: not the effects (capture.cpp's classes; a song
       the driver starts from inside another is music) (XMEN2_SOLO: only the songs it names) */
    bool muted_song(uint8_t s) const
    {
        switch (role) {
        case ROLE_P34: return !part_song(s);
        case ROLE_MUSIC: return part_song(s) || capture_song_effect(s) || !capture_solo_keeps(s);
        default: return !part_song(s) || !capture_solo_keeps(s);
        }
    }

    /* a track slot written: a muted song's track muted (+6 bit 1), kept muted; another as its own
       driver has it */
    void slot_written(uint16_t a)
    {
        unsigned t = (unsigned)(a - SLOTS) >> 5;
        uint8_t *slot = &S.ram[SLOTS + 0x20 * t];
        if (muted_song(slot[0x0E])) {
            slot[6] |= 2;
            S.forced[t] = 1;
        } else if (S.forced[t]) {
            slot[6] &= (uint8_t)~2;
            S.forced[t] = 0;
        }
    }

    uint8_t bus_read(uint16_t a)
    {
        if (a < 0x4000u) return S.ram[a & 0x1FFFu];
        if (a < 0x6000u) { fm_update(S.znow); return S.fm_status; }
        if (a < 0x8000u) return 0xFFu;
        uint32_t addr = ((uint32_t)S.bank << 15) + (uint32_t)(a - 0x8000u);
        uint16_t w = machine_peek16(addr & ~1u);
        return (uint8_t)(addr & 1 ? w : w >> 8);
    }

    void bus_write(uint16_t a, uint8_t v)
    {
        if (a < 0x4000u) {
            uint16_t o = a & 0x1FFFu;
            vs_ram_write(&V, S.ram, o, v);
            S.ram[o] = v;
            if (o >= SLOTS && o < SLOTS + 16 * 0x20) slot_written(o);
            return;
        }
        if (a < 0x6000u) { ym_write(a & 3, v); return; }
        if (a < 0x6100u) { S.bank = (uint16_t)(((S.bank >> 1) | ((v & 1) << 8)) & 0x1FF); return; }
        if (a == 0x7F11u) psg_write(v);
    }

    static uint8_t z_read(void *u, uint16_t a)            { return ((Machine *)u)->bus_read(a); }
    static void    z_write(void *u, uint16_t a, uint8_t v) { ((Machine *)u)->bus_write(a, v); }
    static uint8_t z_in(z80 *z, uint8_t p)                 { return ((Machine *)z->userdata)->bus_read((uint16_t)((z->b << 8) | p)); }
    static void    z_out(z80 *z, uint8_t p, uint8_t v)     { ((Machine *)z->userdata)->bus_write((uint16_t)((z->b << 8) | p), v); }

    void wire(void)
    {
        S.z.read_byte = z_read;
        S.z.write_byte = z_write;
        S.z.port_in = z_in;
        S.z.port_out = z_out;
        S.z.userdata = this;
    }

    void z_step(uint32_t target)
    {
        z80 *z = &S.z;
        while (S.zclk < target) {
            S.zstamp = S.zclk;
            if (!S.ei_shadow) {
                unsigned long c0 = z->cyc;
                if (z80_take_irq(z)) {
                    S.zclk += (uint32_t)(z->cyc - c0) * MASTER_PER_Z80;
                    if (S.zclk >= target) break;
                }
            }
            unsigned long c0 = z->cyc;
            S.znow = S.zclk + z80_base_cycles(z) * MASTER_PER_Z80;
            S.ei_shadow = (uint8_t)z80_exec_one(z);
            S.zclk += (uint32_t)(z->cyc - c0) * MASTER_PER_Z80;
        }
    }

    /* ---- the commands ---- */

    /* a byte into this machine's ring, as Snd_PutByte does it */
    void put(uint8_t b)
    {
        short pos = (signed char)S.ram[0x36];
        S.ram[(RING + pos) & 0x1FFF] = b;
        pos = (short)((pos & ~0xFF) | ((pos + 1) & 0x3F));
        S.ram[0x36] = (uint8_t)pos;
    }

    void cmd(uint8_t c, const uint8_t *a, int n)
    {
        put(0xFF);
        put(c);
        for (int i = 0; i < n; i++) put(a[i]);
    }

    /* a variable as this machine gets it: the second machine's $A = 2, $B/$C = players 3's and 4's
       characters; the music machines' as the game sets it */
    uint8_t param_value(uint8_t slot, uint8_t v) const
    {
        if (role != ROLE_P34) return v;
        if (slot == 0x0A) return 2;
        if (slot == 0x0B) return (uint8_t)P34_MusicVoice(3);
        if (slot == 0x0C) return (uint8_t)P34_MusicVoice(4);
        return v;
    }

    void send_param(uint8_t slot, uint8_t v)
    {
        uint8_t a[2] = { slot, param_value(slot, v) };
        cmd(0x1B, a, 2);
    }

    void start(void)
    {
        memset(&S.z, 0, sizeof S.z);
        z80_init(&S.z);
        wire();
        S.z.pc = 0; S.z.sp = 0; S.z.zf = 1; S.z.int_data = 0xFF;
        memset(S.ram, 0, sizeof S.ram);
        for (uint32_t a = DRIVER; a < DRIVER_END; a++) S.ram[a - DRIVER] = g_rom[a];
        S.bank = 0; S.ei_shadow = 0;
        S.zclk = S.znow = S.zstamp = 0;
        S.fm_count = S.fm_start = 0; S.fm_ta = 0; S.fm_tal = 1024; S.fm_tac = 0; S.fm_tbl = 256 << 4; S.fm_tbc = 0;
        S.fm_mode = 0x30; S.fm_status = 0; S.fm_addr = 0;
        memset(S.ym_reg, 0, sizeof S.ym_reg); memset(S.ym_sel, 0, sizeof S.ym_sel);
        S.psg_latch = 0; memset(S.psg_att, 0x0F, sizeof S.psg_att);
        memset(S.forced, 0, sizeof S.forced);
        S.active = 1;
        S.base = -1;
        if (machine_session()) { chip->ym_init(); chip->psg_init(); }
        ev.clear();
        vs_reset(&V);
        /* Snd_Init's bytes, the variables, the song playing */
        if (S.have_init) {
            put(0xFF);
            put(0x0B);
            for (int i = 0; i < 12; i++) put(S.init[i]);
        }
        for (int s = 0; s < 32; s++)
            if (S.param_set[s >> 3] >> (s & 7) & 1) send_param((uint8_t)s, S.param[s]);
        if (S.song >= 0) {
            uint8_t id = (uint8_t)S.song;
            cmd(0x10, &id, 1);
            S.base = (int16_t)dispatch_base(id);
        }
    }

    void stop(void)
    {
        S.active = 0;
        S.base = -1;
        ev.clear();
        if (machine_session()) { chip->ym_init(); chip->psg_init(); }
    }

    /* one command of the first machine's ring, complete. The second machine takes the dispatcher songs'
       play and stop, stop all, pause, resume and the variables. The music machines take every command
       as it is, the effects' too: an effect's tracks are muted there (muted_song), but what they do
       besides notes is done, as on the first machine (some start or stop music: $45 stops SavageA4's
       $42 and starts $43, $73 the Brood's $30 for $72; and the variables, the tempo) */
    void command(uint8_t c, const uint8_t *a)
    {
        if (role != ROLE_P34) {
            if (c == 0x0B) { memcpy(S.init, a, 12); S.have_init = 1; }
            if (c == 0x1B && a[0] < 32) { S.param[a[0]] = a[1]; S.param_set[a[0] >> 3] |= (uint8_t)(1 << (a[0] & 7)); }
            if (c == 0x10 && !capture_song_effect(a[0])) S.song = a[0];
            if (c == 0x16) S.song = -1;
            if (S.active) cmd(c, a, arg_count(c));
            return;
        }
        switch (c) {
        case 0x0B: memcpy(S.init, a, 12); S.have_init = 1; if (S.active) cmd(0x0B, a, 12); break;
        case 0x10: {
            int base = dispatch_base(a[0]);
            if (base < 0) break;   /* a sound effect or another song: not here */
            S.song = a[0];
            if (S.active) { cmd(0x10, a, 1); S.base = (int16_t)base; }
            break;
        }
        case 0x12:
            if (dispatch_base(a[0]) < 0) break;
            if (S.song == a[0]) S.song = -1;
            if (S.active) { cmd(0x12, a, 1); if (dispatch_base(a[0]) == S.base) S.base = -1; }
            break;
        case 0x16: S.song = -1; if (S.active) { cmd(0x16, a, 0); S.base = -1; } break;
        case 0x0C: case 0x0D: if (S.active) cmd(c, a, 0); break;
        case 0x1B:
            if (a[0] < 32) { S.param[a[0]] = a[1]; S.param_set[a[0] >> 3] |= (uint8_t)(1 << (a[0] & 7)); }
            if (S.active) send_param(a[0], a[1]);
            break;
        default: break;
        }
    }

    /* ---- the frame ---- */

    bool wanted(void) const;

    /* machine.cpp's z80_frame, after the first machine's: this machine's frame, while it is wanted */
    void frame(void)
    {
        bool want = wanted();
        if (want && !S.active) start();
        else if (!want && S.active) stop();
        if (!S.active) return;
        const uint32_t F = LINES * MPL;
        S.z.int_pending = 1;
        z_step(S.zclk + MPL);
        S.z.int_pending = 0;
        if (S.zclk < F) z_step(F);
        if (S.zclk < F) S.zclk = F;
        fm_update(F);
        uint32_t span = F > S.fm_start ? F - S.fm_start : 0;
        uint32_t k = (span + FM_SAMPLE - 1) / FM_SAMPLE;
        if (k == 0) k = 1;
        S.fm_count = S.fm_start = S.fm_start + k * FM_SAMPLE - F;
        S.zclk -= F;
        if (role == ROLE_P34) C.frames++;
        V.frames++;
    }

    /* host_audio: this machine's sound of the frame, added to the first machine's (the chips run muted too) */
    void mix(int16_t *fm, size_t fm_n, int16_t *psg, size_t psg_n, int mute)
    {
        if (!S.active) return;
        uint32_t fm_prev = 0, psg_prev = 0;
        size_t used = 0;
        for (; used < ev.size() && ev[used].stamp <= FRAME_END; used++) {
            const Ev &e = ev[used];
            if (e.port == PLAT_AUDIO_PSG) {
                if (e.stamp > psg_prev) chip->psg_advance(e.stamp - psg_prev);
                chip->psg_write(e.v);
                psg_prev = e.stamp;
            } else {
                if (e.stamp > fm_prev) chip->ym_advance(e.stamp - fm_prev);
                chip->ym_write(e.port, e.v);
                fm_prev = e.stamp;
            }
        }
        ev.erase(ev.begin(), ev.begin() + (ptrdiff_t)used);
        for (Ev &e : ev) e.stamp -= FRAME_END;
        if (FRAME_END > fm_prev) chip->ym_advance(FRAME_END - fm_prev);
        if (FRAME_END > psg_prev) chip->psg_advance(FRAME_END - psg_prev);
        chip->psg_reset_leftover();
        static int16_t fm2[8192 * 2], psg2[16384];
        size_t a = chip->ym_available(), b = chip->psg_available();
        size_t fn = chip->ym_render(fm2, a < 8192 ? a : 8192), pn = chip->psg_render(psg2, b < 16384 ? b : 16384);
        if (mute) return;
        for (size_t i = 0; i < 2 * (fn < fm_n ? fn : fm_n); i++) {
            int v = fm[i] + fm2[i];
            fm[i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
        for (size_t i = 0; i < (pn < psg_n ? pn : psg_n); i++) {
            int v = psg[i] + psg2[i];
            psg[i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
    }

    /* ---- snapshot ---- */

    size_t save(uint8_t *dst, size_t cap)
    {
        bool chips = machine_session();
        size_t ym = chips ? chip->ym_save(nullptr, 0) : 0, ps = chips ? chip->psg_save(nullptr, 0) : 0;
        size_t need = sizeof S + 3 * sizeof(uint32_t) + ev.size() * sizeof(Ev) + ym + ps;
        if (!dst) return need;
        if (cap < need) return 0;
        Snd2 copy = S;
        copy.z.read_byte = nullptr; copy.z.write_byte = nullptr; copy.z.port_in = nullptr; copy.z.port_out = nullptr;
        copy.z.userdata = nullptr;
        uint8_t *o = dst;
        memcpy(o, &copy, sizeof copy); o += sizeof copy;
        uint32_t n = (uint32_t)ev.size();
        memcpy(o, &n, 4); o += 4;
        if (n) memcpy(o, ev.data(), n * sizeof(Ev));
        o += n * sizeof(Ev);
        uint32_t y = (uint32_t)ym, p = (uint32_t)ps;
        memcpy(o, &y, 4); o += 4;
        if (y && chip->ym_save(o, y) != y) return 0;
        o += y;
        memcpy(o, &p, 4); o += 4;
        if (p && chip->psg_save(o, p) != p) return 0;
        o += p;
        return (size_t)(o - dst);
    }

    int load(const uint8_t *src, size_t len)
    {
        const uint8_t *i = src, *end = src + len;
        if (len < sizeof S + 12) return 0;
        memcpy(&S, i, sizeof S); i += sizeof S;
        wire();
        uint32_t n;
        memcpy(&n, i, 4); i += 4;
        if ((size_t)(end - i) < n * sizeof(Ev)) return 0;
        ev.assign((const Ev *)i, (const Ev *)i + n);
        i += n * sizeof(Ev);
        for (int k = 0; k < 2; k++) {
            uint32_t m;
            if (end - i < 4) return 0;
            memcpy(&m, i, 4); i += 4;
            if ((size_t)(end - i) < m) return 0;
            if (m && !(k == 0 ? chip->ym_load(i, m) : chip->psg_load(i, m))) return 0;
            i += m;
        }
        return i == end;
    }
};

static Machine M[3];
static Machine &P34 = M[ROLE_P34];

static bool in_game(void)
{
    if (g_opt.players <= 2 || !xm_region[0x0283]) return false;   /* g_P34InPlay (big-endian s16 at $282) */
    for (int k = 0; k < g_opt.players - 2; k++)
        if ((int8_t)xm_region[0x0200 + 40 * k + 12] != -1) return true;   /* g_PX[k].charIndex */
    return false;
}

/* the second machine while players 3-4 are in the game; the music machines in the session machine (the
   chips run) with the option on */
bool Machine::wanted(void) const
{
    return role == ROLE_P34 ? in_game() : g_opt.music_voices && machine_session();
}

extern "C" bool sound_music_voices(void) { return M[ROLE_MUSIC].S.active != 0; }

/* ---- the first machine's ring ------------------------------------------------------------------------ */

static void command(uint8_t c, const uint8_t *a)
{
    static const bool dbg = getenv("XMEN2_SND2_DEBUG") != nullptr;
    if (dbg) fprintf(stderr, "[snd2] frame %llu command $%02X %02X %02X (active %d)\n", (unsigned long long)machine_frame_no(),
                     c, a[0], a[1], P34.S.active);
    capture_sound_command(c, a);   /* capture.cpp: music or sound effect (the music machines, XMEN2_STEM, XMEN2_SND_LOG) */
    for (Machine &m : M) m.command(c, a);
}

static int arg_count(uint8_t c)
{
    switch (c) {
    case 0x0B: return 12;
    case 0x10: case 0x12: case 0x20: return 1;
    case 0x1B: return 2;
    default: return 0;
    }
}

static void ring_byte(uint8_t b)
{
    Snd2 &S = P34.S;
    switch (S.pstate) {
    case 0: if (b == 0xFF) S.pstate = 1; break;
    case 1:
        S.cmd = b; S.need = (uint8_t)arg_count(b); S.nargs = 0;
        if (!S.need) { command(b, S.args); S.pstate = 0; }
        else S.pstate = 2;
        break;
    default:
        S.args[S.nargs++] = b;
        if (S.nargs == S.need) { command(S.cmd, S.args); S.pstate = 0; }
        break;
    }
}

/* machine.cpp: the game wrote byte v at offset off of the first machine's Z80 RAM. Command bytes are
   ring writes the write index ($36) then moves past (Snd_PutCmd's $FF, Snd_PutByte's byte and the
   index); nothing else counts (Z80_LoadDriver's copy writes the ring too, then the slots after it). */
extern "C" void snd2_ring_write(const uint8_t *ram, uint32_t off, uint8_t v)
{
    Snd2 &S = P34.S;
    if (off >= RING && off < RING + 0x40) {
        if (S.npend < sizeof S.pend) S.pend[S.npend++] = (uint8_t)(off - RING);
        else S.npend = 0xFF;   /* not the game's commands */
        return;
    }
    if (off == 0x36 && S.npend != 0xFF && S.npend && ((S.pend[S.npend - 1] + 1) & 0x3F) == (v & 0x3F))
        for (unsigned i = 0; i < S.npend; i++) ring_byte(ram[RING + S.pend[i]]);
    S.npend = 0;
}

/* ---- the frame --------------------------------------------------------------------------------------- */

/* machine.cpp's z80_frame, after the first machine's */
extern "C" void snd2_frame(void)
{
    for (Machine &m : M) m.frame();
    if (machine_session()) s_m1.frames++;
}

/* host_audio: the machines' sound of the frame, added to the first machine's */
extern "C" void sound_mix(int16_t *fm, size_t fm_n, int16_t *psg, size_t psg_n, int mute)
{
    for (Machine &m : M) m.mix(fm, fm_n, psg, psg_n, mute);
}

/* ---- snapshot, digest, counters -------------------------------------------------------------------------- */

static size_t save_p34(uint8_t *d, size_t c)      { return M[ROLE_P34].save(d, c); }
static int    load_p34(const uint8_t *s, size_t n) { return M[ROLE_P34].load(s, n); }
static size_t save_mus(uint8_t *d, size_t c)      { return M[ROLE_MUSIC].save(d, c); }
static int    load_mus(const uint8_t *s, size_t n) { return M[ROLE_MUSIC].load(s, n); }
static size_t save_prt(uint8_t *d, size_t c)      { return M[ROLE_PARTS].save(d, c); }
static int    load_prt(const uint8_t *s, size_t n) { return M[ROLE_PARTS].load(s, n); }

extern "C" void snd2_state_init(void)
{
    static const char *names[3] = { "snd2", "snd_music", "snd_parts" };
    static const StateSaveFn saves[3] = { save_p34, save_mus, save_prt };
    static const StateLoadFn loads[3] = { load_p34, load_mus, load_prt };
    for (int r = 0; r < 3; r++) {
        Machine &m = M[r];
        m.role = (Role)r;
        m.name = names[r];
        m.chip = &k_chips[r];
        m.S.npend = 0;
        m.S.song = -1;
        m.S.base = -1;
        m.wire();
        m.chip->ym_init();
        m.chip->psg_init();
        state_section_custom(names[r], saves[r], loads[r]);
    }
    state_counters(&P34.C, sizeof P34.C);
    vs_reset(&s_m1);
    for (Machine &m : M) state_counters(&m.V, sizeof m.V);
    state_counters(&s_m1, sizeof s_m1);
}

/* the canonical digest's fields of the second machine (with three or more players): fixed widths,
   little-endian. The music machines are not in it (presentation only, R12's precedent). */
void snd2_digest_fields(std::vector<uint8_t> *out)
{
    const Snd2 &S = P34.S;
    auto u = [out](uint32_t v, int w) { for (int i = 0; i < w; i++) out->push_back((uint8_t)(v >> 8 * i)); };
    auto raw = [out](const void *p, size_t n) { out->insert(out->end(), (const uint8_t *)p, (const uint8_t *)p + n); };
    u(S.active, 1);
    if (!S.active) return;
    const z80 *z = &S.z;
    raw(S.ram, sizeof S.ram);
    u(S.bank, 2);
    u((uint32_t)z->cyc, 4);
    u(z->pc, 2); u(z->sp, 2); u(z->ix, 2); u(z->iy, 2); u(z->mem_ptr, 2);
    u(z->a, 1); u(z->b, 1); u(z->c, 1); u(z->d, 1); u(z->e, 1); u(z->h, 1); u(z->l, 1);
    u(z->a_, 1); u(z->b_, 1); u(z->c_, 1); u(z->d_, 1); u(z->e_, 1); u(z->h_, 1); u(z->l_, 1);
    u(z->f_, 1); u(z->i, 1); u(z->r, 1);
    u((unsigned)z->sf << 7 | (unsigned)z->zf << 6 | (unsigned)z->yf << 5 | (unsigned)z->hf << 4 |
      (unsigned)z->xf << 3 | (unsigned)z->pf << 2 | (unsigned)z->nf << 1 | (unsigned)z->cf, 1);
    u(z->iff_delay, 1); u(z->interrupt_mode, 1); u(z->int_data, 1);
    u((unsigned)z->iff1 << 4 | (unsigned)z->iff2 << 3 | (unsigned)z->halted << 2 |
      (unsigned)z->int_pending << 1 | (unsigned)z->nmi_pending, 1);
    u(S.ei_shadow, 1);
    u(S.zclk, 4); u(S.znow, 4); u(S.zstamp, 4);
    u(S.fm_count, 4); u(S.fm_start, 4); u((uint32_t)S.fm_ta, 4); u((uint32_t)S.fm_tal, 4);
    u((uint32_t)S.fm_tac, 4); u((uint32_t)S.fm_tbl, 4); u((uint32_t)S.fm_tbc, 4);
    u(S.fm_mode, 1); u(S.fm_status, 1); u(S.fm_addr, 2);
    raw(S.ym_reg, sizeof S.ym_reg); raw(S.ym_sel, sizeof S.ym_sel);
    u(S.psg_latch, 1); raw(S.psg_att, sizeof S.psg_att); raw(S.forced, sizeof S.forced);
    u((uint16_t)S.base, 2); u((uint16_t)S.song, 2);
}

std::string snd2_counters_json(void)
{
    std::string s = "{\"frames\": " + std::to_string(P34.C.frames) + ", \"songs\": {";
    bool first = true;
    for (const SongCount &c : P34.C.songs) {
        if (!c.id) continue;
        char b[96];
        snprintf(b, sizeof b, "%s\"%02X\": {\"part_keyons\": %llu, \"base_keyons\": %llu}", first ? "" : ", ", c.id & 0xFF,
                 (unsigned long long)c.part, (unsigned long long)c.base);
        s += b;
        first = false;
    }
    return s + "}}";
}

/* the counters line's "sound": every machine's key-ons and stolen voices by song (the session machine's) */
std::string sound_counters_json(void)
{
    return std::string("{\"music_voices\": ") + (g_opt.music_voices ? "1" : "0") + ", \"m1\": " + vs_json(s_m1) +
           ", \"music\": " + vs_json(M[ROLE_MUSIC].V) + ", \"parts\": " + vs_json(M[ROLE_PARTS].V) +
           ", \"snd2\": " + vs_json(P34.V) + "}";
}
