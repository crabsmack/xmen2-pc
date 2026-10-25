/* netplay.cpp - the native runner's rollback netplay (xmen2disasm docs/phases/P7-multiplayer.md R1,
 * R8, R10; M3, M5): the engine's libraries (recomp-net: sessions, transports, the rollback episode
 * driver; rbengine: the tick-keyed snapshot ring) with XMen2Native's own host table.
 *
 *   XMen2Native ROM --netplay lan|hub|ice --seat I --seats N --bind HOST:PORT [--peer HOST:PORT]
 *       [--input-delay D] [--netplay-json F] [--ice-signal-dir DIR] [--benchmark FRAMES]
 *   XMen2Native --netplay --json     the transports and seats this build has, then exit 0
 *
 * A tick is a runner frame: tick t is runner frame t+1 (machine_tick: its pads, its Z80 frame, the
 * game to its next sync point), and the snapshot keyed t is the state before it (at sync point t;
 * key 0 right after power-on). Seat i plays pad i+1 (seats 2 and 3: g_pad34). Every seat runs the
 * session machine with the host's seed. Seat 0 hosts: its game options (R10, R12: all but high_fps,
 * and widescreen unless widescreen_gameplay is 1), the seed and the start scene are sealed into the
 * session before tick 0 (a pre-match state transfer, then checked by the driver's mod-set handshake:
 * a seat with another set refuses the match); each seat keeps its own high_fps and presentation-only
 * 16:9 view (the canonical digest leaves the picture's state out). F10 is refused online only with
 * widescreen_gameplay=1.
 *
 * A replay (INLINE: inside poll_admit) is quiet: the ticks run, rows and digests are rewritten (a
 * seat's sync.csv and digest.csv are its final timeline, written when the session ends), nothing is
 * shown or heard. With --benchmark FRAMES (the harness: headless, paced at the NTSC rate) the seat
 * plays its script's column until every seat has agreed on the digests through FRAMES, drains the
 * match (rnet_rb_driver_request_quiesce) and exits 0.
 */
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>

#include "native.h"
#include "state.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")
#endif

extern "C" {
#include "recomp_net/config.h"
#include "recomp_net/ice.h"
#include "recomp_net/rb_driver.h"
#include "recomp_net/session.h"
#include "retcomm_rbengine/mono_ms.h"
#include "retcomm_rbengine/snap_ring.h"
}

/* main.cpp */
uint16_t main_local_pad(uint64_t frame, int col);
void     main_np_pads(const uint16_t rows[4]);
void     main_np_quiet(bool on);
void     main_np_slip(double us);
void     main_np_write_rows(uint64_t upto);

#define NP_PAD_MASK 0x0FFFu
#define NP_PERIOD_US 16688.9   /* NTSC: 262 lines of 3420 master clocks at 53.693175 MHz */
#define NP_VERSION "xmen2-native-netplay-3"   /* 2: the digest without the picture's state (R12); 3: v1.0.1, the
                                                 game with players 3-4 changed (enemies' targets, player 2's
                                                 pick, the camera with none in play) */

/* The input delay D is 5 ticks (83 ms), the rollback driver's own floor, and stays there: the
   scheduler's auto-delay (which would grow D with the link's latency, to 150 ms on the `mid`
   profile) is off unless RBE_RB_AUTO_DELAY says otherwise, so a link slower than the cushion is
   predicted and rolled back, not added to every player's input lag. */
static struct {
    bool on;
    std::string transport = "lan", bind, peer, json_path, sig_dir;
    int seat = 0, seats = 2, delay = 5;
    uint32_t session_id = 0x584D3200u;   /* "XM2" */
} N;

static RNetSession *s_session;
static RNetRbDriver *s_drv;
static RbeSnapRing *s_ring;
static uint16_t s_rows[4];
static int s_local_slot, s_slot_count;
static int s_input_delay, s_input_prediction;
static int s_in_resim;
static uint32_t s_replays, s_replay_ticks, s_max_replay, s_cur_replay;
static uint64_t s_late, s_frames;
static std::string s_seal;   /* the sealed configuration (the mod set's text) */
static double s_replay_us, s_replay_us_max, s_save_us, s_saves;   /* what a replayed tick and a snapshot cost */
static double now_us(void);

[[noreturn]] static void np_fail(const char *what)
{
    fprintf(stderr, "[netplay] %s\n", what);
    fflush(stderr);
    exit(6);
}

bool netplay_enabled(void) { return N.on; }

void netplay_print_json(void)
{
#if defined(RNET_ENABLE_ICE)
    printf("{\"transports\": [\"lan\", \"hub\", \"ice\"], \"max_seats\": 4}\n");
#else
    printf("{\"transports\": [\"lan\", \"hub\"], \"max_seats\": 4}\n");
#endif
    fflush(stdout);
}

bool netplay_arg(int argc, char **argv, int *i)
{
    const char *a = argv[*i];
    auto val = [&](void) -> const char * {
        if (*i + 1 >= argc) { fprintf(stderr, "[netplay] %s needs a value\n", a); exit(2); }
        return argv[++*i];
    };
    if (!strcmp(a, "--netplay")) {
        std::string t = val();
        if (t == "--json") { netplay_print_json(); exit(0); }
        if (t != "lan" && t != "hub" && t != "ice") { fprintf(stderr, "[netplay] unknown transport %s\n", t.c_str()); exit(2); }
        N.on = true;
        N.transport = t;
    } else if (!strcmp(a, "--seat")) N.seat = atoi(val());
    else if (!strcmp(a, "--seats")) N.seats = atoi(val());
    else if (!strcmp(a, "--bind")) N.bind = val();
    else if (!strcmp(a, "--peer")) N.peer = val();
    else if (!strcmp(a, "--input-delay")) N.delay = atoi(val());
    else if (!strcmp(a, "--netplay-json")) N.json_path = val();
    else if (!strcmp(a, "--ice-signal-dir")) N.sig_dir = val();
    else return false;
    return true;
}

/* ---- the seal (R10): the host's game options, seed and start scene -------------------------------- */

static std::string seal_text(void)
{
    char b[64];
    std::string s = options_game_text();
    snprintf(b, sizeof b, ";seed=%llu;scene=%d", (unsigned long long)machine_session_seed(), options_first_scene());
    return NP_VERSION ";" + s + b;
}

static bool seal_adopt(const std::string &t)
{
    size_t a = t.find(';'), b = t.find(";seed="), c = t.find(";scene=");
    if (a == std::string::npos || b == std::string::npos || c == std::string::npos || t.substr(0, a) != NP_VERSION) return false;
    if (!options_adopt_game(t.substr(a + 1, b - a - 1))) return false;
    machine_set_session(strtoull(t.c_str() + b + 6, nullptr, 10));
    options_set_first_scene(atoi(t.c_str() + c + 7));
    return true;
}

static int modset_check(const char *want, char *reason, uint32_t cap)
{
    if (want && s_seal == want) return 0;
    snprintf(reason, cap, "the session's configuration differs from the host's");
    return 1;
}

static uint32_t fnv32(const std::string &s)
{
    uint32_t h = 2166136261u;
    for (unsigned char ch : s) h = (h ^ ch) * 16777619u;
    return h;
}

/* ---- the session's host table (delay-sync callbacks; the driver publishes) ------------------------ */

static void encode(uint16_t buttons, RNetInputSample *out, rnet_u32 tick)
{
    memset(out, 0, sizeof *out);
    out->tick = tick;
    out->size = 4;
    out->bytes[0] = (rnet_u8)(buttons & 0xFF);
    out->bytes[1] = (rnet_u8)(buttons >> 8 & 0x0F);
    out->valid = 1;
}

/* The seat's own pad: sampled at sim tick t, it is played at tick t+D (the session's input delay;
   ticks 0..D-1 play neutral pads), so a script's column is read at that tick's runner frame,
   t+D+1; a played seat's pad is the device's now. */
static void sample_local(rnet_u32 tick, RNetInputSample *out, void *ctx)
{
    (void)ctx;
    uint64_t d = s_session ? rnet_session_committed_delay(s_session) : (uint64_t)N.delay;
    encode(main_local_pad((uint64_t)tick + d + 1, N.seat), out, tick);
}

static void session_publish(rnet_u32, const RNetInputSample *, int, void *) {}

/* ---- ICE signalling without a lobby (R8: no lobby server): files in --ice-signal-dir, seat i writing
   sig<i>.txt and reading its peer's; one signal a line, "type flag text" (the engine's
   ice_manual_signaling example) ------------------------------------------------------------------------ */

static FILE *s_sig_out;
static long s_sig_in_at;

static void ice_on_signal(const RNetSignal *msg, void *ctx)
{
    (void)ctx;
    if (!s_sig_out || !msg) return;
    std::string t;   /* an SDP is several lines: they travel as \n, a backslash as \\ */
    for (const char *p = msg->text; *p; p++) {
        if (*p == '\n') t += "\\n";
        else if (*p == '\\') t += "\\\\";
        else if (*p != '\r') t += *p;
    }
    fprintf(s_sig_out, "%d %u %s\n", (int)msg->type, (unsigned)msg->flag, t.c_str());
    fflush(s_sig_out);
}

static void ice_pump_signals(void)
{
    if (N.transport != "ice" || !s_session) return;
    std::string path = N.sig_dir + "/sig" + std::to_string(1 - N.seat) + ".txt";
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) return;
    if (fseek(f, s_sig_in_at, SEEK_SET) == 0) {
        char line[2200];
        while (fgets(line, sizeof line, f)) {
            size_t n = strlen(line);
            if (!n || line[n - 1] != '\n') break;   /* a line still being written: next time */
            while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = 0;
            char *a = strchr(line, ' '), *b = a ? strchr(a + 1, ' ') : nullptr;
            if (!b) continue;
            *a = *b = 0;
            RNetSignal sig;
            memset(&sig, 0, sizeof sig);
            int type = atoi(line);   /* the peer's own (local) SDP and candidates are our remote ones */
            if (type == RNET_SIGNAL_LOCAL_SDP) type = RNET_SIGNAL_REMOTE_SDP;
            else if (type == RNET_SIGNAL_LOCAL_CANDIDATE) type = RNET_SIGNAL_REMOTE_CANDIDATE;
            sig.type = (RNetSignalType)type;
            sig.flag = (rnet_u8)strtoul(a + 1, nullptr, 10);
            sig.peer_slot = 0xFF;
            std::string t;
            for (const char *p = b + 1; *p; p++) {
                if (*p == '\\' && p[1] == 'n') { t += '\n'; p++; }
                else if (*p == '\\' && p[1] == '\\') { t += '\\'; p++; }
                else t += *p;
            }
            snprintf(sig.text, sizeof sig.text, "%s", t.c_str());
            rnet_session_push_signal(s_session, &sig);
            s_sig_in_at = ftell(f);
        }
    }
    fclose(f);
}

/* ---- the rollback host table ----------------------------------------------------------------------- */

static int ring_serialize(void *ctx, uint32_t tick, uint8_t **out, size_t *len)
{
    (void)ctx; (void)tick;
    static Snapshot snap;
    static std::vector<uint8_t> blob;
    state_save(snap);
    state_serialize(snap, blob);
    uint8_t *p = (uint8_t *)malloc(blob.size());
    if (!p) return 0;
    memcpy(p, blob.data(), blob.size());
    *out = p;
    *len = blob.size();
    return 1;
}

static int ring_deserialize(void *ctx, uint32_t tick, const uint8_t *data, size_t len)
{
    (void)ctx; (void)tick;
    static Snapshot snap;
    if (!state_deserialize(data, len, snap)) return 0;
    state_load(snap, nullptr);
    return 1;
}

static const RbeSnapVTable s_snap_vt = { nullptr, ring_serialize, ring_deserialize };

static int h_snap_save(void *c, uint32_t t)
{
    (void)c;
    double t0 = now_us();
    int ok = rbe_snap_ring_save(s_ring, t, &s_snap_vt);
    s_save_us += now_us() - t0;
    s_saves++;
    return ok;
}
static int h_snap_load(void *c, uint32_t t) { (void)c; return rbe_snap_ring_load(s_ring, t, &s_snap_vt); }
static int h_snap_has(void *c, uint32_t t) { (void)c; return rbe_snap_ring_has(s_ring, t); }
static int h_snap_oldest(void *c, uint32_t *o)
{
    (void)c;
    if (rbe_snap_ring_count(s_ring) == 0) return 0;
    *o = rbe_snap_ring_oldest_tick(s_ring);
    return 1;
}
static void h_snap_drop_after(void *c, uint32_t t) { (void)c; (void)rbe_snap_ring_drop_after(s_ring, t); }

static void h_publish(void *ctx, uint32_t tick, const RNetRbFrame *rows, int slots, int replay)
{
    (void)ctx; (void)tick; (void)replay;
    memset(s_rows, 0, sizeof s_rows);
    for (int i = 0; i < slots && i < 4; i++) s_rows[i] = (uint16_t)(rows[i].buttons & NP_PAD_MASK);
    main_np_pads(s_rows);
}

static int h_run_tick(void *ctx, uint32_t tick)
{
    (void)ctx; (void)tick;
    double t0 = now_us();
    machine_tick();
    double us = now_us() - t0;
    s_replay_us += us;
    if (us > s_replay_us_max) s_replay_us_max = us;
    s_replay_ticks++;
    s_cur_replay++;
    return 1;
}

static void h_resim_begin(void *ctx)
{
    (void)ctx;
    s_in_resim = 1;
    s_cur_replay = 0;
    main_np_quiet(true);
    machine_set_render(0);
}

static void h_resim_end(void *ctx)
{
    (void)ctx;
    s_in_resim = 0;
    main_np_quiet(false);
    machine_set_render(1);
    s_replays++;
    if (s_cur_replay > s_max_replay) s_max_replay = s_cur_replay;
}

static uint32_t fold32(uint64_t v) { return (uint32_t)(v ^ v >> 32); }

static uint32_t h_digest_master(void *ctx) { (void)ctx; return fold32(machine_digest()); }

static void h_digest_parts(void *ctx, RNetRbDigestParts *out)
{
    (void)ctx;
    out->master = fold32(machine_digest());
    uint64_t p[3];
    machine_digest_parts(p);
    for (int i = 0; i < 3; i++) out->part[i] = fold32(p[i]);
}

static void h_decode(void *ctx, int slot, const RNetInputSample *in, RNetRbFrame *out)
{
    (void)ctx; (void)slot;
    out->buttons = (uint16_t)((in->bytes[0] | (uint16_t)in->bytes[1] << 8) & NP_PAD_MASK);
    out->stick_x = out->stick_y = 0;
    out->analog = 0;
}

static void h_sanitize(void *ctx, int slot, RNetRbFrame *f)
{
    (void)ctx; (void)slot;
    f->buttons &= NP_PAD_MASK;
    f->stick_x = f->stick_y = 0;
    f->analog = 0;
}

static void h_neutral(void *ctx, int slot, RNetRbFrame *out) { (void)ctx; (void)slot; out->buttons = 0; }

static void h_boot_digest_noted(void *ctx)
{
    (void)ctx;
    uint64_t p[3];
    uint64_t m = machine_digest();
    machine_digest_parts(p);
    fprintf(stderr, "[netplay] boot digest %016llx (ram %016llx vdp %016llx rest %016llx)\n", (unsigned long long)m,
            (unsigned long long)p[0], (unsigned long long)p[1], (unsigned long long)p[2]);
}

static void h_return_to_lobby(void *ctx)
{
    (void)ctx;
    const char *why = s_drv ? rnet_rb_driver_refusal(s_drv) : nullptr;
    fprintf(stderr, "[netplay] the match was refused: %s\n", why ? why : "?");
    fflush(stderr);
    exit(7);
}

static uint32_t h_now_ms(void *ctx) { (void)ctx; return rbe_mono_ms(); }

/* ---- the loop ---------------------------------------------------------------------------------------- */

static double now_us(void)
{
    return (double)std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

/* the pre-match transfer of the seal: the host sends it, every guest adopts it (as the engine's SRAM
   barrier: nobody runs tick 0 before it has landed) */
static void seal_exchange(void)
{
    bool sent = false;
    double t0 = now_us();
    for (;;) {
        ice_pump_signals();
        rnet_session_pump(s_session);
        if (rnet_session_is_running(s_session)) {
            if (N.seat == 0 && !sent) {
                if (rnet_session_state_begin(s_session, RNET_STATE_OP_SRAM, 0, s_seal.data(), s_seal.size() + 1) != 0)
                    np_fail("cannot send the session's configuration");
                sent = true;
            }
            rnet_u8 op = 0, slot = 0;
            const void *data = nullptr;
            size_t size = 0;
            if (rnet_session_state_take_ready(s_session, &op, &slot, &data, &size)) {
                if (op == RNET_STATE_OP_SRAM && N.seat != 0) {
                    std::string t((const char *)data, size ? size - 1 : 0);
                    if (!seal_adopt(t)) {
                        fprintf(stderr, "[netplay] the host's configuration can't be taken: %s\n", t.c_str());
                        np_fail("refusing the match");
                    }
                    s_seal = seal_text();
                }
                rnet_session_state_finish(s_session, 0);
                if (op == RNET_STATE_OP_SRAM) return;
            }
        }
        if (now_us() - t0 > 60e6) np_fail("no session after 60 s (the other seats never came)");
        (void)rnet_session_wait_recv(s_session, 2);
    }
}

static void write_json(void)
{
    if (N.json_path.empty()) return;
    FILE *f = fopen(N.json_path.c_str(), "w");
    if (!f) return;
    fprintf(f, "{\"seat\": %d, \"options\": %s, \"rollbacks\": %u, \"max_rollback\": %u, \"replayed_ticks\": %u, "
               "\"late_frames\": %llu, \"frames\": %llu, \"episodes\": %u, \"desyncs\": %u, \"rtt_ms\": %u, "
               "\"replay_tick_us\": %.0f, \"replay_tick_us_max\": %.0f, \"snapshot_save_us\": %.0f, "
               "\"transport\": \"%s\", \"seal\": \"%s\"}\n",
            N.seat, options_effective_json().c_str(), s_replays, s_max_replay, s_replay_ticks,
            (unsigned long long)s_late, (unsigned long long)s_frames,
            s_drv ? rnet_rb_driver_episode_count(s_drv) : 0, s_drv ? rnet_rb_driver_desync_count(s_drv) : 0,
            s_drv ? rnet_rb_driver_rtt_estimate_ms(s_drv) : 0,
            s_replay_ticks ? s_replay_us / s_replay_ticks : 0.0, s_replay_us_max, s_saves ? s_save_us / s_saves : 0.0,
            N.transport.c_str(), s_seal.c_str());
    fclose(f);
}

static void start_transport(void)
{
    int rc;
    if (N.transport == "hub")
        rc = N.seat == 0 ? rnet_session_start_lan_hub(s_session, N.bind.c_str())
                         : rnet_session_start_lan(s_session, N.bind.c_str(), N.peer.c_str());
    else if (N.transport == "lan")
        rc = rnet_session_start_lan(s_session, N.bind.c_str(), N.peer.empty() ? nullptr : N.peer.c_str());
    else {
#if defined(RNET_ENABLE_ICE)
        if (N.seats != 2 || N.sig_dir.empty()) np_fail("ice: two seats and --ice-signal-dir DIR");
        RNetIceConfig ice;
        rnet_ice_config_init_defaults(&ice);   /* a public STUN server (R8) */
        ice.controlling = (rnet_u8)(N.seat == 0);
        rc = rnet_session_start_ice(s_session, &ice);
#else
        rc = -1;
#endif
    }
    if (rc != 0) np_fail("the transport didn't start");
}

/* --record-inputs in a played session (it ends at the window): the frames every remote seat's real
   input has reached, less a second for a correction still to come, are final */
static void record_cut(void)
{
    if (!s_session || !s_drv) return;
    uint64_t cut = rnet_rb_driver_sim_tick(s_drv);
    for (int i = 0; i < N.seats; i++) {
        rnet_u32 tip = 0;
        if (i != N.seat && rnet_session_remote_tip(s_session, i, &tip) && tip < cut) cut = tip;
    }
    capture_record_truncate(cut > 60 ? cut - 60 : 0);
}

void netplay_run(uint64_t frames)
{
    if (N.seats < 2 || N.seats > 4 || N.seat < 0 || N.seat >= N.seats) np_fail("--seat I --seats N: 0 <= I < N, 2 <= N <= 4");
    if (N.transport != "ice" && (N.bind.empty() || (N.seat && N.peer.empty())))
        np_fail("--bind HOST:PORT (and --peer HOST:PORT for a guest)");
    machine_start();
    if (!machine_session()) {   /* a guest takes the host's seed with the seal; the chips run anyway */
        machine_set_session(0);
        machine_audio_init();
    }

    RNetConfig rc;
    rnet_config_init_defaults(&rc);
    rc.slot_count = (rnet_u8)N.seats;
    rc.local_slot = (rnet_u8)N.seat;
    rc.input_delay = (rnet_u8)N.delay;
    rc.session_id = N.session_id;
    RNetHostVTable vt;
    memset(&vt, 0, sizeof vt);
    vt.sample_local = sample_local;
    vt.publish = session_publish;
    if (N.transport == "ice") {
        std::string p = N.sig_dir + "/sig" + std::to_string(N.seat) + ".txt";
        if (!(s_sig_out = fopen(p.c_str(), "ab"))) np_fail("cannot write the ICE signals");
        vt.on_signal = ice_on_signal;
    }
    s_session = rnet_session_create(&rc, &vt);
    if (!s_session) np_fail("cannot make the session");
    start_transport();
    s_local_slot = N.seat;
    s_slot_count = N.seats;
    s_input_delay = N.delay;
    s_seal = seal_text();
    seal_exchange();
    fprintf(stderr, "[netplay] seat %d of %d (%s), sealed: %s\n", N.seat, N.seats, N.transport.c_str(), s_seal.c_str());

    if (!getenv("RBE_RB_AUTO_DELAY")) {
        static char off[] = "RBE_RB_AUTO_DELAY=0";
        putenv(off);
    }
    s_drv = rnet_rb_driver_create();
    rnet_rb_driver_set_identity(s_drv, fnv32(NP_VERSION), fnv32(s_seal));
    rnet_rb_driver_set_modset(s_drv, s_seal.c_str(), modset_check, nullptr);
    s_ring = rbe_snap_ring_create(64);
    RNetRbDriverConfig cfg;
    memset(&cfg, 0, sizeof cfg);
    cfg.session = &s_session;
    cfg.local_slot = &s_local_slot;
    cfg.slot_count = &s_slot_count;
    cfg.input_delay = &s_input_delay;
    cfg.input_prediction = &s_input_prediction;
    cfg.replay_mode = RNET_RB_REPLAY_INLINE;
    cfg.part_names[0] = "ram";
    cfg.part_names[1] = "vdp";
    cfg.part_names[2] = "machine";
    cfg.snap_depth = 64;
    cfg.log_prefix = "xmen2_netplay";
    cfg.env_alias = "XMEN2_RB";
    cfg.inject_flip_bits = 0x00B0u;   /* B, C, Start: what the game reads in play and in menus */
    RNetRbHost host;
    memset(&host, 0, sizeof host);
    host.snap_save = h_snap_save;
    host.snap_load = h_snap_load;
    host.snap_has = h_snap_has;
    host.snap_oldest = h_snap_oldest;
    host.snap_drop_after = h_snap_drop_after;
    host.publish = h_publish;
    host.run_tick = h_run_tick;
    host.resim_begin = h_resim_begin;
    host.resim_end = h_resim_end;
    host.digest_master = h_digest_master;
    host.digest_parts = h_digest_parts;
    host.decode_sample = h_decode;
    host.sanitize_row = h_sanitize;
    host.neutral_row = h_neutral;
    host.boot_digest_noted = h_boot_digest_noted;
    host.request_return_to_lobby = h_return_to_lobby;
    host.now_ms = h_now_ms;
    if (!rnet_rb_driver_start(s_drv, &cfg, &host)) np_fail("the rollback driver didn't start");

    /* Pacing: live frames on the NTSC clock, as a played window presents them (a played seat waits in
       its present instead): the seat waits for a frame's time, then asks the scheduler for it. When
       the scheduler holds the frame (its timesync pacing slows the seat that runs ahead; its grace
       waits for a remote input before inventing it), the clock moves by the hold: the seat is slowed,
       as the scheduler means, instead of making the hold up on the next frames and keeping its lead
       (2026-10-22: the ahead seat then waited out a grace at most lost packets, 71 late frames of
       2,510 against the other seat's 2). A stall of more than 4 periods moves the clock as well. A
       frame shown more than a period after the one before (more than two periods between them) is
       late: a visible hitch, one per stall however long. */
    const bool paced_here = !plat_realtime();
#ifdef _WIN32
    timeBeginPeriod(1);   /* 1 ms waits (the default timer turns a 1 ms wait into 15.6 ms: a hitch) */
#endif
    double prev = 0, next = 0;
    double held = 0;   /* when the scheduler began holding this frame (0: it hasn't) */
    bool quiescing = false;
    /* XMEN2_NP_LATE=1 (diagnosing late frames): each one's gap, and what the loop did since the frame
       before: polls that admitted nothing, replay ticks, the time in poll_admit, the tick, the hold */
    static const bool dbg_late = getenv("XMEN2_NP_LATE") != nullptr;
    /* XMEN2_NP_F10=SEAT:TICK,... (testing R12 headless): that seat presses F10 before that live tick */
    const char *f10 = getenv("XMEN2_NP_F10");
    uint32_t d_waits = 0, d_replay0 = 0;
    double d_admit = 0, d_tick = 0;
    if (!frames) atexit(record_cut);   /* a played session ends at the window: --record-inputs keeps what is final */
    for (;;) {
        ice_pump_signals();
        rnet_session_pump(s_session);
        double a0 = now_us();
        if (paced_here && next && a0 < next) {   /* not the frame's time yet */
            (void)rnet_session_wait_recv(s_session, next - a0 > 1500 ? 1 : 0);
            continue;
        }
        RNetRbAdmit a = rnet_rb_driver_poll_admit(s_drv);
        if (dbg_late) d_admit += now_us() - a0;
        if (a == RNET_RB_ADMIT_LIVE) {
            double hold = held ? a0 - held : 0;
            held = 0;
            if (hold > 0 && next) {
                if (paced_here) next += hold;
                else main_np_slip(hold);
            }
            double k0 = dbg_late ? now_us() : 0;
            for (const char *q = f10; q && *q; ) {
                char *e;
                long seat = strtol(q, &e, 10), at = *e == ':' ? strtol(e + 1, &e, 10) : -1;
                if (seat == N.seat && at == (long)rnet_rb_driver_sim_tick(s_drv)) {
                    fprintf(stderr, "[netplay] F10 before tick %ld\n", at);
                    options_toggle_widescreen();
                }
                q = *e ? e + 1 : e;
            }
            machine_tick();
            rnet_rb_driver_finish_frame(s_drv);
            uint64_t tick = rnet_rb_driver_sim_tick(s_drv);
            double t = now_us();
            if (dbg_late) d_tick = t - k0;
            if (!next) next = t;
            next += NP_PERIOD_US;
            if (t > next + 4 * NP_PERIOD_US) next = t;   /* a long stall moves the clock */
            if (!frames || tick <= frames) {
                s_frames++;
                if (prev && t > prev + 2 * NP_PERIOD_US) {
                    s_late++;
                    if (dbg_late)
                        fprintf(stderr, "[netplay] late tick %llu: gap %.1f ms; since the frame before %u empty polls, "
                                "%u replay ticks, admit %.1f ms, tick %.1f ms, hold %.1f ms\n", (unsigned long long)tick,
                                (t - prev) / 1000, d_waits, s_replay_ticks - d_replay0, d_admit / 1000, d_tick / 1000,
                                hold / 1000);
                }
            }
            prev = t;
            d_waits = 0;
            d_replay0 = s_replay_ticks;
            d_admit = 0;
        } else {
            if (!held) held = a0;
            (void)rnet_session_wait_recv(s_session, 1);
            d_waits++;
        }
        if (!frames) continue;
        /* The end (the harness's sessions): the seat's rows through `frames` are final once it has run
           well past them on every seat's real inputs (each remote seat's input tip past them, no
           correction open); then the match is drained (every seat stops opening corrections) and the
           seat leaves. The margin keeps every seat past `frames` before any of them drains. */
        uint32_t sim = rnet_rb_driver_sim_tick(s_drv);
        if (!quiescing) {
            bool ready = sim >= frames + 60 && !rnet_rb_driver_episode_active(s_drv);
            for (int i = 0; ready && i < N.seats; i++) {
                rnet_u32 tip = 0;
                if (i != N.seat && (!rnet_session_remote_tip(s_session, i, &tip) || tip < frames + 30)) ready = false;
            }
            if (ready) {
                rnet_rb_driver_request_quiesce(s_drv);
                quiescing = true;
            }
        }
        RNetRbQuiesce q = rnet_rb_driver_quiesce_state(s_drv);
        if (q == RNET_RB_QUIESCE_DRAINED || q == RNET_RB_QUIESCE_TIMED_OUT) {
            if (sim < frames) {   /* drained by a peer before this seat got there: its rows are short */
                fprintf(stderr, "[netplay] drained at sim %u, before %llu\n", sim, (unsigned long long)frames);
                fflush(stderr);
                exit(8);
            }
            main_np_write_rows(frames);
            capture_record_truncate(frames);   /* --record-inputs: the final timeline */
            write_json();
            (void)rnet_session_send_bye(s_session);
            fprintf(stderr, "[netplay] done: %llu frames, %u replays (%u ticks, longest %u), %llu late; drain %s\n",
                    (unsigned long long)s_frames, s_replays, s_replay_ticks, s_max_replay, (unsigned long long)s_late,
                    q == RNET_RB_QUIESCE_DRAINED ? "complete" : "timed out");
            exit(0);
        }
    }
}
