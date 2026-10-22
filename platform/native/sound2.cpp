/* sound2.cpp - the second sound machine (phase 7 R9, xmen2disasm docs/phases/P7-multiplayer.md):
 * players 3 and 4's music parts.
 *
 * The stage songs with player parts ($27, $25, $48, $64 by way of $5E/$5F) start the parts from
 * dispatcher tracks that test the driver's variables $B and $C, players 1 and 2's characters (the
 * part songs: kickoff notes section 3). With three or more players in the game an audio-only second
 * machine (its own Z80 with the same GEMS driver, YM2612 and PSG) gets the first machine's music
 * commands as the game writes them into the first machine's command ring (snd2_ring_write: play and
 * stop of those songs, stop all, pause, resume, the variables, with $B/$C = players 3's and 4's
 * characters and $A = 2; no sound effects), and plays the same song with its own (base) tracks
 * muted in their track slots, so only players 3's and 4's part songs sound there, at the base
 * song's tempo and start. The 68000 never sees it. It runs only while a player 3 or 4 is in the
 * game; the host mixes the two (snd2_mix). It is in the snapshot ("snd2") and, with three or more
 * players, in the canonical digest (snd2_digest_fields).
 *
 * The YM2612 timers and the Z80's frame are the first machine's (machine.cpp), copied: the same
 * driver sees the same machine.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

extern "C" {
#include "external/superzazu/z80.h"
}
#include "audio/event_queue.h"

#include "native.h"
#include "state.h"

extern "C" {
void   snd2_ym2612_init(void);
void   snd2_ym2612_advance(uint32_t cycles_master);
void   snd2_ym2612_write(uint8_t port, uint8_t value);
size_t snd2_ym2612_render(int16_t *out, size_t sample_count);
size_t snd2_ym2612_samples_available(void);
size_t snd2_ym2612_rb_save(void *dst, size_t cap);
int    snd2_ym2612_rb_load(const void *src, size_t len);
void   snd2_psg_init(void);
void   snd2_psg_advance(uint32_t cycles_master);
void   snd2_psg_write(uint8_t value);
size_t snd2_psg_render(int16_t *out, size_t sample_count);
size_t snd2_psg_samples_available(void);
void   snd2_psg_reset_leftover(void);
size_t snd2_psg_rb_save(void *dst, size_t cap);
int    snd2_psg_rb_load(const void *src, size_t len);
extern uint8_t xm_region[0x10000];
int P34_MusicVoice(int k);   /* game/runtime/players34.cpp: Player_MusicVoice of player k (3, 4) */
}

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
   base tracks, muted here */
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

/* Everything the second machine is, one plain struct (the snapshot; the Z80's callbacks scrubbed) */
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
    uint8_t  forced[16];             /* slots this machine muted (their song a base song) */
    int16_t  base;                   /* the dispatcher song playing (its base id), -1 none */
    /* what the first machine was told (its ring, parsed: snd2_ring_write), for a later start */
    uint8_t  npend, pend[4];         /* ring bytes written since the index moved (offsets; npend $FF: not commands) */
    uint8_t  pstate, cmd, need, nargs, args[12];
    uint8_t  have_init, init[12];
    uint8_t  param_set[4], param[32];
    int16_t  song;                   /* the dispatcher song the first machine plays (Snd_Play id), -1 none */
};
static Snd2 S;

struct Ev { uint32_t stamp; uint8_t port, v; };
static std::vector<Ev> s_ev;        /* the chips' writes not yet drained (snd2_mix) */

/* the counters (host counters: restored with a snapshot, not state) */
struct SongCount { int16_t id; uint64_t part, base; };
static struct Counters { uint64_t frames; SongCount songs[8]; } C = { 0, {} };

static SongCount *song_count(int id)
{
    for (SongCount &s : C.songs)
        if (s.id == id) return &s;
    for (SongCount &s : C.songs)
        if (s.id == 0) { s.id = (int16_t)id; return &s; }
    return &C.songs[7];
}

static bool session_chips(void) { return machine_session() != 0; }

/* ---- the YM2612's timers, as machine.cpp has them ---------------------------------------------------- */

static void fm_update(uint32_t t)
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

static void fm_set_timers(uint8_t v)
{
    if ((v & 1) && !(S.fm_mode & 1)) S.fm_tac = S.fm_tal;
    if ((v & 2) && !(S.fm_mode & 2)) S.fm_tbc = S.fm_tbl;
    S.fm_status &= (uint8_t)(~v >> 4);
    S.fm_mode = v;
}

static void ym_timers(uint32_t t, unsigned port, uint8_t v)
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

/* ---- key-ons, by the track (and so the song) that owns the voice ------------------------------------- */

/* the song of the track that holds the voice of channel ch in a pool (-1: none found) */
static int owner_song(uint16_t pool, int entries, int ch)
{
    for (int i = 0; i < entries; i++) {
        const uint8_t *e = &S.ram[pool + 7 * i];
        if (e[0] == 0xFF) break;
        if ((e[0] & 0x80) || (e[0] & 0x07) != ch) continue;
        unsigned t = e[3] & 0x0F;
        return S.ram[SLOTS + 0x20 * t + 0x0E];
    }
    return -1;
}

static void count_keyon(int song)
{
    if (S.base < 0) return;
    SongCount *c = song_count(S.base);
    if (song >= 0 && part_song((uint8_t)song)) c->part++;
    else c->base++;
    static const bool dbg = getenv("XMEN2_SND2_DEBUG") != nullptr;
    if (dbg) fprintf(stderr, "[snd2] frame %llu key-on song $%02X (base $%02X)\n", (unsigned long long)machine_frame_no(),
                     song & 0xFF, S.base);
}

static void ym_write(unsigned port, uint8_t v)
{
    ym_timers(S.znow, port, v);
    if (!(port & 1)) S.ym_sel[port >> 1 & 1] = v;
    else {
        unsigned r = (port >> 1 & 1) << 8 | S.ym_sel[port >> 1 & 1];
        S.ym_reg[r] = v;
        if (r == 0x28 && (v & 0xF0)) count_keyon(owner_song(FM_POOL, 6, v & 7));
    }
    if (session_chips()) s_ev.push_back({ S.zstamp, (uint8_t)port, v });
}

static void psg_write(uint8_t v)
{
    if (v & 0x80) S.psg_latch = (uint8_t)(v >> 4 & 7);
    if ((v & 0x90) == 0x90) {   /* a volume: a note sounds when its channel was silent */
        int ch = v >> 5 & 3, att = v & 0x0F;
        if (att != 0x0F && S.psg_att[ch] == 0x0F)
            count_keyon(ch == 3 ? owner_song(NOISE_POOL, 1, 3) : owner_song(PSG_POOL, 3, ch));
        S.psg_att[ch] = (uint8_t)att;
    }
    if (session_chips()) s_ev.push_back({ S.zstamp, (uint8_t)PLAT_AUDIO_PSG, v });
}

/* ---- the Z80's bus -------------------------------------------------------------------------------------- */

/* a track slot written: a base song's track muted (+6 bit 1), kept muted; a part song's as its own driver has it */
static void slot_written(uint16_t a)
{
    unsigned t = (unsigned)(a - SLOTS) >> 5;
    uint8_t *slot = &S.ram[SLOTS + 0x20 * t];
    if (!part_song(slot[0x0E])) {
        slot[6] |= 2;
        S.forced[t] = 1;
    } else if (S.forced[t]) {
        slot[6] &= (uint8_t)~2;
        S.forced[t] = 0;
    }
}

static uint8_t bus_read(uint16_t a)
{
    if (a < 0x4000u) return S.ram[a & 0x1FFFu];
    if (a < 0x6000u) { fm_update(S.znow); return S.fm_status; }
    if (a < 0x8000u) return 0xFFu;
    uint32_t addr = ((uint32_t)S.bank << 15) + (uint32_t)(a - 0x8000u);
    uint16_t w = machine_peek16(addr & ~1u);
    return (uint8_t)(addr & 1 ? w : w >> 8);
}

static void bus_write(uint16_t a, uint8_t v)
{
    if (a < 0x4000u) {
        uint16_t o = a & 0x1FFFu;
        S.ram[o] = v;
        if (o >= SLOTS && o < SLOTS + 16 * 0x20) slot_written(o);
        return;
    }
    if (a < 0x6000u) { ym_write(a & 3, v); return; }
    if (a < 0x6100u) { S.bank = (uint16_t)(((S.bank >> 1) | ((v & 1) << 8)) & 0x1FF); return; }
    if (a == 0x7F11u) psg_write(v);
}

static uint8_t z_read(void *u, uint16_t a)            { (void)u; return bus_read(a); }
static void    z_write(void *u, uint16_t a, uint8_t v) { (void)u; bus_write(a, v); }
static uint8_t z_in(z80 *z, uint8_t p)                 { return bus_read((uint16_t)((z->b << 8) | p)); }
static void    z_out(z80 *z, uint8_t p, uint8_t v)     { bus_write((uint16_t)((z->b << 8) | p), v); }

static void wire(void)
{
    S.z.read_byte = z_read;
    S.z.write_byte = z_write;
    S.z.port_in = z_in;
    S.z.port_out = z_out;
    S.z.userdata = nullptr;
}

static void z_step(uint32_t target)
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

/* ---- the commands -------------------------------------------------------------------------------------- */

/* a byte into this machine's ring, as Snd_PutByte does it */
static void put(uint8_t b)
{
    short pos = (signed char)S.ram[0x36];
    S.ram[(RING + pos) & 0x1FFF] = b;
    pos = (short)((pos & ~0xFF) | ((pos + 1) & 0x3F));
    S.ram[0x36] = (uint8_t)pos;
}

static void cmd(uint8_t c, const uint8_t *a, int n)
{
    put(0xFF);
    put(c);
    for (int i = 0; i < n; i++) put(a[i]);
}

/* a variable as this machine gets it: $A = 2, $B/$C = players 3's and 4's characters */
static uint8_t param_value(uint8_t slot, uint8_t v)
{
    if (slot == 0x0A) return 2;
    if (slot == 0x0B) return (uint8_t)P34_MusicVoice(3);
    if (slot == 0x0C) return (uint8_t)P34_MusicVoice(4);
    return v;
}

static void send_param(uint8_t slot, uint8_t v)
{
    uint8_t a[2] = { slot, param_value(slot, v) };
    cmd(0x1B, a, 2);
}

static void start(void)
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
    if (session_chips()) { snd2_ym2612_init(); snd2_psg_init(); }
    s_ev.clear();
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

static void stop(void)
{
    S.active = 0;
    S.base = -1;
    s_ev.clear();
    if (session_chips()) { snd2_ym2612_init(); snd2_psg_init(); }
}

/* one command of the first machine's ring, complete */
static void command(uint8_t c, const uint8_t *a)
{
    static const bool dbg = getenv("XMEN2_SND2_DEBUG") != nullptr;
    if (dbg) fprintf(stderr, "[snd2] frame %llu command $%02X %02X %02X (active %d)\n", (unsigned long long)machine_frame_no(),
                     c, a[0], a[1], S.active);
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
    if (off >= RING && off < RING + 0x40) {
        if (S.npend < sizeof S.pend) S.pend[S.npend++] = (uint8_t)(off - RING);
        else S.npend = 0xFF;   /* not the game's commands */
        return;
    }
    if (off == 0x36 && S.npend != 0xFF && S.npend && ((S.pend[S.npend - 1] + 1) & 0x3F) == (v & 0x3F))
        for (unsigned i = 0; i < S.npend; i++) ring_byte(ram[RING + S.pend[i]]);
    S.npend = 0;
}

/* ---- the frame ------------------------------------------------------------------------------------------ */

static bool in_game(void)
{
    if (g_opt.players <= 2 || !xm_region[0x0283]) return false;   /* g_P34InPlay (big-endian s16 at $282) */
    for (int k = 0; k < g_opt.players - 2; k++)
        if ((int8_t)xm_region[0x0200 + 40 * k + 12] != -1) return true;   /* g_PX[k].charIndex */
    return false;
}

/* machine.cpp's z80_frame, after the first machine's: this machine's frame, while players 3-4 are in */
extern "C" void snd2_frame(void)
{
    bool want = in_game();
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
    C.frames++;
}

/* host_audio: this machine's sound of the frame, added to the first machine's (the chips run muted too) */
extern "C" void snd2_mix(int16_t *fm, size_t fm_n, int16_t *psg, size_t psg_n, int mute)
{
    if (!S.active) return;
    uint32_t fm_prev = 0, psg_prev = 0;
    size_t used = 0;
    for (; used < s_ev.size() && s_ev[used].stamp <= FRAME_END; used++) {
        const Ev &e = s_ev[used];
        if (e.port == PLAT_AUDIO_PSG) {
            if (e.stamp > psg_prev) snd2_psg_advance(e.stamp - psg_prev);
            snd2_psg_write(e.v);
            psg_prev = e.stamp;
        } else {
            if (e.stamp > fm_prev) snd2_ym2612_advance(e.stamp - fm_prev);
            snd2_ym2612_write(e.port, e.v);
            fm_prev = e.stamp;
        }
    }
    s_ev.erase(s_ev.begin(), s_ev.begin() + (ptrdiff_t)used);
    for (Ev &e : s_ev) e.stamp -= FRAME_END;
    if (FRAME_END > fm_prev) snd2_ym2612_advance(FRAME_END - fm_prev);
    if (FRAME_END > psg_prev) snd2_psg_advance(FRAME_END - psg_prev);
    snd2_psg_reset_leftover();
    static int16_t fm2[8192 * 2], psg2[16384];
    size_t a = snd2_ym2612_samples_available(), b = snd2_psg_samples_available();
    size_t fn = snd2_ym2612_render(fm2, a < 8192 ? a : 8192), pn = snd2_psg_render(psg2, b < 16384 ? b : 16384);
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

/* ---- snapshot, digest, counters -------------------------------------------------------------------------- */

static size_t snd2_save(uint8_t *dst, size_t cap)
{
    bool chips = session_chips();
    size_t ym = chips ? snd2_ym2612_rb_save(nullptr, 0) : 0, ps = chips ? snd2_psg_rb_save(nullptr, 0) : 0;
    size_t need = sizeof S + 3 * sizeof(uint32_t) + s_ev.size() * sizeof(Ev) + ym + ps;
    if (!dst) return need;
    if (cap < need) return 0;
    Snd2 copy = S;
    copy.z.read_byte = nullptr; copy.z.write_byte = nullptr; copy.z.port_in = nullptr; copy.z.port_out = nullptr;
    uint8_t *o = dst;
    memcpy(o, &copy, sizeof copy); o += sizeof copy;
    uint32_t n = (uint32_t)s_ev.size();
    memcpy(o, &n, 4); o += 4;
    if (n) memcpy(o, s_ev.data(), n * sizeof(Ev));
    o += n * sizeof(Ev);
    uint32_t y = (uint32_t)ym, p = (uint32_t)ps;
    memcpy(o, &y, 4); o += 4;
    if (y && snd2_ym2612_rb_save(o, y) != y) return 0;
    o += y;
    memcpy(o, &p, 4); o += 4;
    if (p && snd2_psg_rb_save(o, p) != p) return 0;
    o += p;
    return (size_t)(o - dst);
}

static int snd2_load(const uint8_t *src, size_t len)
{
    const uint8_t *i = src, *end = src + len;
    if (len < sizeof S + 12) return 0;
    memcpy(&S, i, sizeof S); i += sizeof S;
    wire();
    uint32_t n;
    memcpy(&n, i, 4); i += 4;
    if ((size_t)(end - i) < n * sizeof(Ev)) return 0;
    s_ev.assign((const Ev *)i, (const Ev *)i + n);
    i += n * sizeof(Ev);
    for (int k = 0; k < 2; k++) {
        uint32_t m;
        if (end - i < 4) return 0;
        memcpy(&m, i, 4); i += 4;
        if ((size_t)(end - i) < m) return 0;
        if (m && !(k == 0 ? snd2_ym2612_rb_load(i, m) : snd2_psg_rb_load(i, m))) return 0;
        i += m;
    }
    return i == end;
}

extern "C" void snd2_state_init(void)
{
    S.npend = 0;
    S.song = -1;
    S.base = -1;
    wire();
    snd2_ym2612_init();
    snd2_psg_init();
    state_section_custom("snd2", snd2_save, snd2_load);
    state_counters(&C, sizeof C);
}

/* the canonical digest's fields of this machine (with three or more players): fixed widths, little-endian */
void snd2_digest_fields(std::vector<uint8_t> *out)
{
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
    std::string s = "{\"frames\": " + std::to_string(C.frames) + ", \"songs\": {";
    bool first = true;
    for (const SongCount &c : C.songs) {
        if (!c.id) continue;
        char b[96];
        snprintf(b, sizeof b, "%s\"%02X\": {\"part_keyons\": %llu, \"base_keyons\": %llu}", first ? "" : ", ", c.id & 0xFF,
                 (unsigned long long)c.part, (unsigned long long)c.base);
        s += b;
        first = false;
    }
    return s + "}}";
}
