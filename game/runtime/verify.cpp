/* verify.cpp - shadow differential tests of the hand-written C (phase 4, `xmen2check.py verify`;
 * xmen2disasm docs/phases/P4-decomp.md "Verified by verify").
 *
 * Built into XMen2Verify: the timed runner with ALL generated code, which plays a corpus
 * input exactly as the reference does, plus the hand-written C. At every call of a function
 * F that has C (the generated wrapper's XENTRY hook):
 *   1. snapshot RAM and the registers; run F's generated code, recording every hardware
 *      access and the instructions of F it executes;
 *   2. if an interrupt was taken meanwhile, the call is not comparable: keep the result;
 *   3. else roll RAM and registers back and run F's C through its slot entry, untimed, with
 *      the recorded hardware reads replayed in order and its hardware writes captured (not
 *      performed); whatever F calls runs as generated code ("nested calls run plain");
 *   4. compare: RAM outside the dead stack below the returned A7 (stack_low up to A7), the
 *      result in d0 (as wide as F's C return type), A7, the return target, the callee-saved
 *      registers d3-d7/a2-a6, and the hardware access sequence. Hand-written 68000 (asm):
 *      also the registers its convention hands back (bridge.py regs(...)) and SR's system
 *      byte, not the registers it marks clobbers(...); one with hand-written adapters
 *      (conv) every register but those;
 *   5. put the generated result back and continue: the game always runs the generated code.
 * A call that passes adds the instructions it executed to F's covered set.
 *
 * Nesting: a function G with C called inside F's generated run (step 1) is tested there the
 * same way, as part of F's run (up to MAX_DEPTH deep); never inside a C run or a synthetic one.
 * Otherwise a callee of a function that is tested at every call would never be tested.
 *
 * Synthetic cases (P4-decomp.md: "captured states mutated to reach other paths, run the same
 * way"): XMEN2_VERIFY_SYNTH=<dir> holds <ADDR>.syn files (format: load_synth below). At the
 * first call of F (or, `at any`, the first top-level call of any function: for functions
 * the corpus never runs) each case mutates that entry state (RAM, registers, SR; ROM too,
 * for a path a constant of this ROM rules out) and runs both versions
 * from it, untimed (no interrupts), with hardware reads supplied by the case (default 0) and
 * hardware writes captured, not performed; then everything is put back. Passing cases count
 * as checked calls and add coverage; failing ones as mismatches.
 *
 * XMEN2_VERIFY_OUT=<path> writes {"stopped_early": null or why, "frames", "functions_tested",
 * "calls_checked", "calls_refused", "functions": {"ADDR": {"calls_checked", "mismatches",
 * "irq_skipped", "synthetic_checked", "first_mismatch", "covered": [pc, ...]}}} at exit;
 * XMEN2_VERIFY_ONLY=ADDR,... restricts the tests to those functions.
 *
 * Testing that stops is loud (xmen2disasm LESSONS.md, kickoff item 6: two full verifies ran on
 * for ~1.5 h after testing had stopped, with 498 and 416 functions checked). `stopped_early` is
 * set when the run ended (a) on a fatal error (xm::fatal), (b) inside a C run or a synthetic
 * case (they are untimed: one still open at the end never returned, e.g. a case that took a
 * V-int and ran the game on inside it), or (c) after STOP_FRAMES or more frames in which every
 * call of a function under test was refused because no test could open (a C run or case open,
 * or MAX_DEPTH reached). xmen2check.py verify fails on it. XMEN2_VERIFY_DEV_STOP=<frame> (dev)
 * refuses every test from that frame on, to prove the check.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern "C" {
#include "genesis_runtime.h"
extern uint8_t g_rom[0x400000];
extern uint32_t g_xclk, g_xstart, g_xend, g_xrts;
extern int g_xtrace;
uint32_t xmen2_stack_low(void);
void xm_install_crash_report(void (*report)(unsigned long code, void *addr));   /* crash_win.cpp */
}

struct XmVerifyFn { uint32_t addr, end; void (*slot)(void); void (*gen)(void); int ret_bytes;
                    int kind; uint16_t out_mask, skip_mask; uint8_t out_bytes[16]; int arg_bytes; };
namespace xm {
extern bool g_retnv;       /* the C call just made returned no value (handc_rt.cpp) */
extern char g_fatal_msg[]; /* the fatal error ending the run, if one did (handc_rt.cpp) */
uint32_t fn_current(void); /* the C function running (the bridge's fn_enter/fn_leave) */
}

extern const XmVerifyFn xm_verify_fns[];
extern const int xm_verify_fn_count;

namespace {

const int MAX_DEPTH = 16;
/* refused calls this many frames in a row, up to the end of the run: testing stopped */
const uint64_t STOP_FRAMES = 60;

struct Stat {
    uint64_t checked = 0, mismatches = 0, irq = 0, seen = 0, synth = 0;
    bool synth_done = false;
    std::string first;
    std::unordered_set<uint32_t> covered;
};
struct Hw {
    uint32_t addr; int size; int write; uint16_t v;
    bool operator==(const Hw &o) const { return addr == o.addr && size == o.size && write == o.write && v == o.v; }
};

/* One synthetic case: assignments to the entry state, hardware read values to supply. */
struct Assign { int what; int size; std::string addr; std::string value; };   /* what: 0 memory, 1 register, 2 SR */
/* `poke N` in a case: a RAM write once the run has made N hardware reads (resolved at the case's
   start); both runs make the same reads in the same order, so both see it at the same point */
struct Poke { size_t after; uint32_t addr; int size; uint32_t value; };
struct SynCase {
    std::string name;
    std::vector<Assign> sets;
    std::vector<uint16_t> reads;
    std::vector<std::pair<size_t, Assign>> pokes;
};
struct SynSpec { bool at_any = false; std::vector<SynCase> cases; };

/* One test in progress (a nesting level). */
struct Ctx {
    const XmVerifyFn *fn;
    int mode;                      /* 1 record (generated run), 2 replay (C run), 3 synthetic generated run */
    uint8_t pre[0x10000], post[0x10000];
    std::vector<Hw> logA, logB;
    size_t readpos;
    std::vector<uint32_t> cov;
    const std::vector<uint16_t> *supply;
    size_t supply_pos;
    const std::vector<Poke> *pokes;   /* a synthetic case's (else null) */
    size_t nreads;                 /* hardware reads made so far in this run (for pokes) */
    uint64_t irq0;                 /* g_xirq_taken when the test started */
    uint64_t frame0;               /* g_frame_count when the test started */
    bool doomed;                   /* its generated run took an interrupt: it won't be compared */
};

std::unordered_map<uint32_t, const XmVerifyFn *> s_fns;
std::unordered_map<uint32_t, Stat> s_stats;
std::unordered_map<uint32_t, SynSpec> s_synth;
Ctx *s_ctx[MAX_DEPTH];
int s_depth;
uint32_t s_entering;              /* the function whose generated code is being started */
bool s_any_done;
/* calls of functions under test that no test could take (a C run or synthetic case open, or
   MAX_DEPTH): how many, and 1 + the frame of the first since the last call that could (0: none) */
uint64_t s_refused, s_refused_since;
uint64_t s_dev_stop;              /* XMEN2_VERIFY_DEV_STOP: 1 + the frame testing stops at (dev) */

/* A generated run that has taken an interrupt can't be compared (its test is skipped as soon as
   it ends): stop collecting for it. A long-running function (a scene, the level loop) is such a
   test for minutes, and the per-frame code tested inside it would otherwise add to its logs. */
bool live(Ctx &c)
{
    if (!c.doomed && c.mode == 1 && g_xirq_taken != c.irq0) {
        c.doomed = true;
        std::vector<Hw>().swap(c.logA);
        std::vector<uint32_t>().swap(c.cov);
    }
    return !c.doomed;
}

/* A synthetic case's pokes due after this run's hardware read number t.nreads (RAM only). */
void apply_pokes(Ctx &t)
{
    if (!t.pokes) return;
    for (const Poke &p : *t.pokes) {
        if (p.after != t.nreads) continue;
        for (int i = 0; i < p.size; i++)
            g_ram[(p.addr + (uint32_t)i) & 0xFFFFu] = (uint8_t)(p.value >> (8 * (p.size - 1 - i)));
    }
}

int hw_hook(uint32_t a, int size, int kind, uint16_t v, uint16_t *out)
{
    if (!s_depth) return 0;
    Ctx &t = *s_ctx[s_depth - 1];
    if (t.mode == 1) {
        /* a generated run: part of every enclosing generated run too */
        for (int d = 0; d < s_depth; d++) {
            if (s_ctx[d]->mode != 1 || !live(*s_ctx[d])) continue;
            if (kind == 1) s_ctx[d]->logA.push_back({ a, size, 0, v });
            else if (kind == 2) s_ctx[d]->logA.push_back({ a, size, 1, v });
        }
        return 0;
    }
    if (t.mode == 2) {
        if (kind == 0) {
            /* the next read the generated run made (its value), whatever this one is:
               the sequence comparison reports any difference */
            uint16_t r = 0;
            while (t.readpos < t.logA.size() && t.logA[t.readpos].write) t.readpos++;
            if (t.readpos < t.logA.size()) r = t.logA[t.readpos++].v;
            *out = r;
            t.logB.push_back({ a, size, 0, r });
            t.nreads++;
            apply_pokes(t);
            return 1;
        }
        if (kind == 2) { t.logB.push_back({ a, size, 1, v }); return 1; }
        return 0;
    }
    if (t.mode == 3) {
        if (kind == 0) {
            uint16_t r = t.supply && t.supply_pos < t.supply->size() ? (*t.supply)[t.supply_pos++] : 0;
            *out = r;
            t.logA.push_back({ a, size, 0, r });
            t.nreads++;
            apply_pokes(t);
            return 1;
        }
        if (kind == 2) { t.logA.push_back({ a, size, 1, v }); return 1; }
    }
    return 0;
}

void insn_hook(uint32_t pc)
{
    for (int d = 0; d < s_depth; d++) {
        Ctx &c = *s_ctx[d];
        if (c.mode == 1 && !live(c)) continue;
        if ((c.mode == 1 || c.mode == 3) && pc >= c.fn->addr && pc < c.fn->end) c.cov.push_back(pc);
    }
}

std::unordered_set<uint32_t> s_only;
std::unordered_map<uint32_t, size_t> s_insns;   /* each function's instruction count */

void load_only()
{
    const char *e = getenv("XMEN2_VERIFY_ONLY");
    if (e) {
        for (const char *s = e; *s;) {
            char *end;
            unsigned long v = strtoul(s, &end, 16);
            if (end == s) break;
            s_only.insert((uint32_t)v);
            s = *end ? end + 1 : end;
        }
    }
    if (const char *d = getenv("XMEN2_VERIFY_DEV_STOP")) s_dev_stop = 1u + strtoull(d, nullptr, 0);
    /* XMEN2_VERIFY_INSNS=<path>: lines "ADDR COUNT" (hex address, decimal count of the
       function's instructions): the probing of sampled-out calls below stops once a function's
       checked calls have covered that many */
    const char *ip = getenv("XMEN2_VERIFY_INSNS");
    FILE *f = ip ? fopen(ip, "r") : nullptr;
    if (!f) return;
    unsigned a, n;
    while (fscanf(f, "%x %u", &a, &n) == 2) s_insns[a] = n;
    fclose(f);
}

/* <ADDR>.syn: '#' comments; `at any` (run at the first top-level call of any function, for
   functions the corpus never runs; default: at F's own first top-level call); `case NAME`
   starts a case; in a case, `b|w|l ADDR = VALUE` (memory), `reg dN|aN = VALUE`, `sr = VALUE`,
   `hw V V ...` (hardware read values, in order), `poke N b|w|l ADDR = VALUE` (a RAM write once
   the run has made N hardware reads: input a frame later, e.g. a pad's V-int copy). ADDR and VALUE are sums/differences of terms:
   $hex, decimal, dN, aN (register values at entry), argN (the long in argument slot N:
   A7+4+4N), [EXPR] (the long at EXPR). */
void load_synth()
{
    const char *dir = getenv("XMEN2_VERIFY_SYNTH");
    if (!dir) return;
    for (int i = 0; i < xm_verify_fn_count; i++) {
        char path[1024];
        snprintf(path, sizeof path, "%s/%06X.syn", dir, (unsigned)xm_verify_fns[i].addr);
        FILE *f = fopen(path, "r");
        if (!f) continue;
        SynSpec spec;
        char line[512];
        while (fgets(line, sizeof line, f)) {
            std::string l = line;
            size_t h = l.find('#');
            if (h != std::string::npos) l.resize(h);
            while (!l.empty() && (l.back() == '\n' || l.back() == '\r' || l.back() == ' ' || l.back() == '\t')) l.pop_back();
            size_t b = l.find_first_not_of(" \t");
            if (b == std::string::npos) continue;
            l = l.substr(b);
            if (l == "at any") { spec.at_any = true; continue; }
            if (l.rfind("case", 0) == 0) { spec.cases.push_back({ l.size() > 5 ? l.substr(5) : "", {}, {} }); continue; }
            if (spec.cases.empty()) spec.cases.push_back({ "", {}, {} });
            SynCase &c = spec.cases.back();
            if (l.rfind("hw ", 0) == 0) {
                const char *s = l.c_str() + 3;
                while (*s) {
                    while (*s == ' ' || *s == '\t') s++;
                    if (!*s) break;
                    char *e;
                    unsigned long v = *s == '$' ? strtoul(s + 1, &e, 16) : strtoul(s, &e, 0);
                    c.reads.push_back((uint16_t)v);
                    s = e;
                }
                continue;
            }
            size_t after = 0;
            bool poke = l.rfind("poke ", 0) == 0;
            if (poke) {   /* `poke N b|w|l ADDR = VALUE`: a RAM write after N hardware reads */
                char *e;
                after = strtoul(l.c_str() + 5, &e, 0);
                l = e;
                size_t nb = l.find_first_not_of(" \t");
                if (nb == std::string::npos) continue;
                l = l.substr(nb);
            }
            size_t eq = l.find('=');
            if (eq == std::string::npos) continue;
            std::string lhs = l.substr(0, eq), rhs = l.substr(eq + 1);
            while (!lhs.empty() && lhs.back() == ' ') lhs.pop_back();
            Assign a{};
            a.value = rhs;
            if (lhs == "sr") { a.what = 2; }
            else if (lhs.rfind("reg ", 0) == 0) { a.what = 1; a.addr = lhs.substr(4); }
            else { a.what = 0; a.size = lhs[0] == 'b' ? 1 : lhs[0] == 'w' ? 2 : 4; a.addr = lhs.substr(2); }
            if (poke) {
                if (a.what == 0) c.pokes.push_back({ after, a });
                continue;
            }
            c.sets.push_back(a);
        }
        fclose(f);
        s_synth[xm_verify_fns[i].addr] = spec;
    }
}

/* Evaluate an ADDR/VALUE expression against the current (entry) state. */
uint32_t eval(const char *&s);
uint32_t term(const char *&s)
{
    while (*s == ' ') s++;
    if (*s == '[') { s++; uint32_t a = eval(s); while (*s == ' ') s++; if (*s == ']') s++; return m68k_read32(a); }
    if (*s == '$') { char *e; uint32_t v = (uint32_t)strtoul(s + 1, &e, 16); s = e; return v; }
    if ((s[0] == 'd' || s[0] == 'a') && s[1] >= '0' && s[1] <= '7') { uint32_t v = s[0] == 'd' ? g_cpu.D[s[1] - '0'] : g_cpu.A[s[1] - '0']; s += 2; return v; }
    if (strncmp(s, "arg", 3) == 0) { char *e; long n = strtol(s + 3, &e, 10); s = e; return m68k_read32(g_cpu.A[7] + 4u + 4u * (uint32_t)n); }
    char *e; long v = strtol(s, &e, 0); s = e; return (uint32_t)v;
}
uint32_t eval(const char *&s)
{
    while (*s == ' ') s++;
    bool neg = false;
    if (*s == '-') { neg = true; s++; }
    uint32_t v = term(s);
    if (neg) v = 0u - v;
    for (;;) {
        while (*s == ' ') s++;
        if (*s == '+') { s++; v += term(s); }
        else if (*s == '-') { s++; v -= term(s); }
        else break;
    }
    return v;
}
uint32_t eval_str(const std::string &x) { const char *s = x.c_str(); return eval(s); }

std::string json_str(const std::string &s)
{
    std::string o = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') o += '\\';
        o += (unsigned char)c < 0x20 ? ' ' : c;
    }
    return o + "\"";
}

/* Why testing stopped before the end of the run, or "" (the header comment's (a)-(c)). */
std::string stopped_early()
{
    char b[400];
    if (xm::g_fatal_msg[0]) return std::string("fatal error: ") + xm::g_fatal_msg;
    for (int d = 0; d < s_depth; d++) {
        const Ctx &c = *s_ctx[d];
        if (c.mode == 1) continue;
        snprintf(b, sizeof b, "the run ended inside the %s of $%06X opened at frame %llu (untimed: it never "
                              "returned), so nothing was tested after it",
                 c.mode == 2 ? "C run" : "synthetic case", (unsigned)c.fn->addr, (unsigned long long)c.frame0);
        return b;
    }
    if (s_refused_since && g_frame_count + 1 >= s_refused_since + STOP_FRAMES) {
        snprintf(b, sizeof b, "no test could open from frame %llu to the end (%llu frames%s; %d tests open, the "
                              "innermost $%06X mode %d)", (unsigned long long)(s_refused_since - 1),
                 (unsigned long long)(g_frame_count + 1 - s_refused_since),
                 s_dev_stop ? ", XMEN2_VERIFY_DEV_STOP" : "", s_depth,
                 s_depth ? (unsigned)s_ctx[s_depth - 1]->fn->addr : 0u, s_depth ? s_ctx[s_depth - 1]->mode : 0);
        return b;
    }
    return "";
}

void write_report(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return;
    uint64_t tested = 0, checked = 0;
    for (const auto &kv : s_stats) {
        tested += kv.second.checked + kv.second.mismatches > 0;
        checked += kv.second.checked;
    }
    std::string stop = stopped_early();
    if (!stop.empty()) fprintf(stderr, "[verify] testing stopped early: %s\n", stop.c_str());
    fprintf(f, "{\"stopped_early\": %s, \"frames\": %llu, \"functions_tested\": %llu, \"calls_checked\": %llu, "
               "\"calls_refused\": %llu,\n\"functions\": {", stop.empty() ? "null" : json_str(stop).c_str(),
            (unsigned long long)g_frame_count, (unsigned long long)tested, (unsigned long long)checked,
            (unsigned long long)s_refused);
    bool firstf = true;
    for (int i = 0; i < xm_verify_fn_count; i++) {
        uint32_t a = xm_verify_fns[i].addr;
        auto it = s_stats.find(a);
        Stat empty;
        const Stat &st = it == s_stats.end() ? empty : it->second;
        std::string first;
        for (char c : st.first) {
            if (c == '"' || c == '\\') first += '\\';
            first += c;
        }
        fprintf(f, "%s\n\"%06X\": {\"calls_checked\": %llu, \"mismatches\": %llu, \"irq_skipped\": %llu, "
                   "\"synthetic_checked\": %llu, \"first_mismatch\": %s%s%s, \"covered\": [",
                firstf ? "" : ",", (unsigned)a, (unsigned long long)st.checked, (unsigned long long)st.mismatches,
                (unsigned long long)st.irq, (unsigned long long)st.synth, st.first.empty() ? "null" : "\"",
                first.c_str(), st.first.empty() ? "" : "\"");
        bool fc = true;
        std::vector<uint32_t> cov(st.covered.begin(), st.covered.end());
        std::sort(cov.begin(), cov.end());
        for (uint32_t pc : cov) { fprintf(f, "%s%u", fc ? "" : ",", (unsigned)pc); fc = false; }
        fprintf(f, "]}");
        firstf = false;
    }
    fprintf(f, "\n}}\n");
    fclose(f);
}

void report()
{
    if (const char *path = getenv("XMEN2_VERIFY_OUT")) write_report(path);
}

/* Branch plans (the runner's --branch-plan; xmen2check.py verify): the testing's state at the
   branch point, put back after each branch, so a branch reports what the run it stands for
   reports when played from power-on. The tests open there keep their objects, and the functions
   their Stat (the game fiber's restored stack refers to both): their contents are copied back. */
struct Kept {
    std::unordered_map<uint32_t, Stat> stats;
    std::vector<Ctx> ctx;
    int depth;
    uint32_t entering;
    bool any_done;
    uint64_t refused, refused_since;
};
Kept *s_kept;

std::string describe(const char *what, uint32_t a, uint32_t g, uint32_t c, const std::string &call)
{
    char b[200];
    snprintf(b, sizeof b, "%s: %s %06X: generated %08X, C %08X", call.c_str(), what, (unsigned)a, (unsigned)g,
             (unsigned)c);
    return b;
}

/* Step 4: the C run's result (current state) against the generated run's (r1, c.post). */
std::string compare(const Ctx &c, const XmVerifyFn *fn, const M68KState &r1, uint32_t xrts1, const std::string &call)
{
    std::string bad;
    M68KState rc = g_cpu;
    /* dead stack: below the returned A7, and the function's own argument slots just above it,
       which its caller pops unread (P4-decomp.md decision 9: MPW keeps a by-value parameter
       there, and the original writes it where the C changes a host variable) */
    uint32_t lo = xmen2_stack_low() & 0xFFFFu, hi = (r1.A[7] & 0xFFFFu) + (uint32_t)fn->arg_bytes;
    if (hi < lo) hi = lo;
    for (uint32_t i = 0; i < 0x10000u && bad.empty(); i++) {
        if (i >= lo && i < hi) continue;
        if (g_ram[i] != c.post[i]) bad = describe("RAM", 0xFF0000u + i, c.post[i], g_ram[i], call);
    }
    if (bad.empty() && rc.A[7] != r1.A[7]) bad = describe("A7", 7, r1.A[7], rc.A[7], call);
    if (bad.empty() && g_xrts != xrts1) bad = describe("return to", 0, xrts1, g_xrts, call);
    if (bad.empty() && fn->ret_bytes && !xm::g_retnv) {   /* no value returned: d0 is not a result */
        uint32_t m = fn->ret_bytes >= 4 ? 0xFFFFFFFFu : fn->ret_bytes == 2 ? 0xFFFFu : 0xFFu;
        if ((r1.D[0] & m) != (rc.D[0] & m)) bad = describe("d0", 0, r1.D[0] & m, rc.D[0] & m, call);
    }
    /* registers: C functions keep d3-d7/a2-a6; asm hands back its outs (as wide as their C type)
       and keeps the callee-saved ones it doesn't clobber; conv reproduces all but clobbers */
    for (int r = 0; r < 15 && bad.empty(); r++) {
        bool saved = (r >= 3 && r <= 7) || (r >= 10 && r <= 14);
        uint32_t m = 0xFFFFFFFFu;
        if (fn->kind && (fn->skip_mask >> r & 1)) continue;
        if (fn->kind && (fn->out_mask >> r & 1)) {
            int b = fn->out_bytes[r];
            m = r >= 8 || b >= 4 ? 0xFFFFFFFFu : b == 2 ? 0xFFFFu : 0xFFu;
        } else if (!saved && fn->kind != 2) {
            continue;
        }
        uint32_t g = r < 8 ? r1.D[r] : r1.A[r - 8], cc = r < 8 ? rc.D[r] : rc.A[r - 8];
        if ((g & m) != (cc & m)) bad = describe(r < 8 ? "d" : "a", (uint32_t)(r & 7), g & m, cc & m, call);
    }
    if (bad.empty() && fn->kind && (r1.SR & 0xFF00u) != (rc.SR & 0xFF00u))
        bad = describe("SR", 0, r1.SR, rc.SR, call);
    if (bad.empty() && !(c.logA == c.logB)) {
        size_t k = 0;
        while (k < c.logA.size() && k < c.logB.size() && c.logA[k] == c.logB[k]) k++;
        char b[240];
        if (k < c.logA.size() && k < c.logB.size())
            snprintf(b, sizeof b, "%s: hardware access %zu: generated %s $%06X=%04X, C %s $%06X=%04X", call.c_str(), k,
                     c.logA[k].write ? "write" : "read", (unsigned)c.logA[k].addr, c.logA[k].v,
                     c.logB[k].write ? "write" : "read", (unsigned)c.logB[k].addr, c.logB[k].v);
        else
            snprintf(b, sizeof b, "%s: %zu hardware accesses generated, %zu in C", call.c_str(), c.logA.size(),
                     c.logB.size());
        bad = b;
    }
    return bad;
}

Ctx *push(const XmVerifyFn *fn, int mode)
{
    Ctx *c = s_ctx[s_depth];
    if (!c) c = s_ctx[s_depth] = new Ctx();
    c->fn = fn;
    c->mode = mode;
    c->logA.clear();
    c->logB.clear();
    c->cov.clear();
    c->readpos = 0;
    c->supply = nullptr;
    c->supply_pos = 0;
    c->pokes = nullptr;
    c->nreads = 0;
    c->irq0 = g_xirq_taken;
    c->frame0 = g_frame_count;
    c->doomed = false;
    s_depth++;
    g_xhw_hook = hw_hook;
    g_xinsn_hook = insn_hook;
    return c;
}

void pop()
{
    s_depth--;
    if (!s_depth) {
        g_xhw_hook = nullptr;
        g_xinsn_hook = nullptr;
    }
    /* a test can open again: the refusals so far were a C run's or case's own (brief) calls */
    if (!(s_depth && (s_ctx[s_depth - 1]->mode != 1 || s_depth >= MAX_DEPTH))
        && !(s_dev_stop && g_frame_count + 1 >= s_dev_stop))
        s_refused_since = 0;
}

void run_gen(const XmVerifyFn *fn)
{
    int trace0 = g_xtrace;
    g_xtrace = 1;
    s_entering = fn->addr;
    /* its XENTRY sees s_entering and lets the body run; in a dispatch frame of its own, so that
       a jmp to another function (a pending tail call) runs before the comparison */
    recomp_call_func(fn->gen);
    s_entering = 0;
    g_xtrace = trace0;
}

/* One synthetic case of fn from the current state (an entry state); everything is put back. */
void run_case(const XmVerifyFn *fn, Stat &st, const SynCase &k)
{
    static uint8_t base[0x10000];
    memcpy(base, g_ram, 0x10000);
    M68KState r00 = g_cpu;
    uint32_t xrts00 = g_xrts, clk = g_xclk, start = g_xstart, end = g_xend;
    uint64_t irq0 = g_xirq_taken;
    /* the mutations, evaluated against the entry state. A ROM address is written in the ROM
       image (a path a constant of this ROM rules out) and put back after the case */
    std::vector<std::pair<uint32_t, uint8_t>> rom_saved;
    for (const Assign &a : k.sets) {
        uint32_t v = eval_str(a.value);
        if (a.what == 2) { g_cpu.SR = (uint16_t)v; continue; }
        if (a.what == 1) {
            int r = a.addr[1] - '0';
            if (a.addr[0] == 'd') g_cpu.D[r] = v; else g_cpu.A[r] = v;
            continue;
        }
        uint32_t at = eval_str(a.addr) & 0xFFFFFFu;
        if (at + (uint32_t)a.size <= 0x400000u) {
            for (int i = 0; i < a.size; i++) {
                rom_saved.push_back({ at + (uint32_t)i, g_rom[at + i] });
                g_rom[at + i] = (uint8_t)(v >> (8 * (a.size - 1 - i)));
            }
            continue;
        }
        if (a.size == 1) m68k_write8(at, (uint8_t)v);
        else if (a.size == 2) m68k_write16(at, (uint16_t)v);
        else m68k_write32(at, v);
    }
    /* the pokes, resolved against the same entry state (RAM addresses only) */
    std::vector<Poke> pokes;
    for (const auto &pk : k.pokes) {
        uint32_t at = eval_str(pk.second.addr) & 0xFFFFFFu;
        if (at >= 0xFF0000u) pokes.push_back({ pk.first, at, pk.second.size, eval_str(pk.second.value) });
    }
    g_xend = 0xFFFFFFFFu;   /* untimed: no slice, no interrupt */
    Ctx &c = *push(fn, 3);
    memcpy(c.pre, g_ram, 0x10000);
    M68KState r0 = g_cpu;
    uint32_t xrts0 = g_xrts;
    c.supply = &k.reads;
    c.pokes = &pokes;
    g_xirq_hold = 1;   /* untimed: an SR write takes no interrupt (exact_sched.c) */
    run_gen(fn);
    g_xirq_hold = 0;
    M68KState r1 = g_cpu;
    uint32_t xrts1 = g_xrts;
    memcpy(c.post, g_ram, 0x10000);
    /* XMEN2_VERIFY_TRACE_CASE=<name>: the path a case's generated run took (writing cases) */
    if (const char *tc = getenv("XMEN2_VERIFY_TRACE_CASE"); tc && k.name == tc) {
        fprintf(stderr, "[verify] case %s path:", k.name.c_str());
        for (uint32_t pc : c.cov) fprintf(stderr, " %06X", (unsigned)pc);
        fprintf(stderr, "\n");
    }
    memcpy(g_ram, c.pre, 0x10000);
    g_cpu = r0;
    g_xrts = xrts0;
    c.mode = 2;
    c.readpos = 0;
    c.nreads = 0;      /* the C run sees the pokes after the same reads */
    g_xirq_hold = 1;   /* untimed: an SR write takes no interrupt (exact_sched.c) */
    fn->slot();
    g_xirq_hold = 0;
    std::string bad = compare(c, fn, r1, xrts1, "synthetic case " + (k.name.empty() ? std::string("?") : k.name));
    if (g_xirq_taken != irq0) bad = "synthetic case " + k.name + ": an interrupt was taken";
    if (bad.empty()) {
        st.checked++;
        st.synth++;
        st.covered.insert(c.cov.begin(), c.cov.end());
    } else {
        st.mismatches++;
        if (st.first.empty()) st.first = bad;
    }
    pop();
    for (size_t i = rom_saved.size(); i-- > 0;) g_rom[rom_saved[i].first] = rom_saved[i].second;
    memcpy(g_ram, base, 0x10000);
    g_cpu = r00;
    g_xrts = xrts00;
    g_xclk = clk;
    g_xstart = start;
    g_xend = end;
}

void run_synth(uint32_t addr, bool self)
{
    for (auto &kv : s_synth) {
        if ((self && kv.first != addr) || (!self && !kv.second.at_any)) continue;
        Stat &st = s_stats[kv.first];
        if (st.synth_done) continue;
        st.synth_done = true;
        const XmVerifyFn *fn = s_fns[kv.first];
        if (!fn || (!s_only.empty() && !s_only.count(kv.first))) continue;
        for (const SynCase &k : kv.second.cases) run_case(fn, st, k);
    }
}

}  /* namespace */

/* A host exception (a C function dereferencing what the 68000 reads harmlessly): name the
   tests open and the C function running before the runner dies (crash_win.cpp). */
void crash_report(unsigned long code, void *where)
{
    fprintf(stderr, "[verify] host exception %08lX at %p; tests open (outermost first):", code, where);
    for (int d = 0; d < s_depth; d++)
        fprintf(stderr, " $%06X (mode %d)", (unsigned)s_ctx[d]->fn->addr, s_ctx[d]->mode);
    fprintf(stderr, "; C function running $%06X\n", (unsigned)xm::fn_current());
    fflush(stderr);
}

extern "C" void xmen2_verify_branch_save(void)
{
    if (!s_kept) s_kept = new Kept();
    Kept &k = *s_kept;
    k.stats = s_stats;
    k.ctx.clear();
    for (int d = 0; d < s_depth; d++) k.ctx.push_back(*s_ctx[d]);
    k.depth = s_depth;
    k.entering = s_entering;
    k.any_done = s_any_done;
    k.refused = s_refused;
    k.refused_since = s_refused_since;
}

extern "C" void xmen2_verify_branch_restore(void)
{
    const Kept &k = *s_kept;
    /* into the same map nodes: a test open across the branch point holds a reference to its
       function's Stat on the restored stack. A node made during the branch has no such holder. */
    for (auto it = s_stats.begin(); it != s_stats.end();) {
        auto kt = k.stats.find(it->first);
        if (kt == k.stats.end()) { it = s_stats.erase(it); continue; }
        it->second = kt->second;
        ++it;
    }
    for (int d = 0; d < k.depth; d++) *s_ctx[d] = k.ctx[(size_t)d];
    s_depth = k.depth;
    s_entering = k.entering;
    s_any_done = k.any_done;
    s_refused = k.refused;
    s_refused_since = k.refused_since;
    g_xhw_hook = s_depth ? hw_hook : nullptr;
    g_xinsn_hook = s_depth ? insn_hook : nullptr;
}

/* A branch's results: <dir of XMEN2_VERIFY_OUT>/<tag>.json */
extern "C" void xmen2_verify_branch_end(const char *tag)
{
    const char *out = getenv("XMEN2_VERIFY_OUT");
    if (!out) return;
    std::string dir(out);
    size_t slash = dir.find_last_of("/\\");
    dir = slash == std::string::npos ? std::string() : dir.substr(0, slash + 1);
    write_report((dir + tag + ".json").c_str());
}

extern "C" int xmen2_verify_entry(uint32_t addr)
{
    static bool init;
    if (!init) {
        init = true;
        for (int i = 0; i < xm_verify_fn_count; i++) s_fns[xm_verify_fns[i].addr] = &xm_verify_fns[i];
        load_only();
        load_synth();
        atexit(report);
        xm_install_crash_report(crash_report);
    }
    if (s_entering == addr) { s_entering = 0; return 0; }   /* the generated run under test */
    auto it = s_fns.find(addr);
    if (it == s_fns.end() || (!s_only.empty() && !s_only.count(addr))) return 0;
    /* no test can open inside a C run or a synthetic case (they are brief: untimed), or past
       MAX_DEPTH; refusals that last frames mean testing has stopped (report: stopped_early) */
    if ((s_depth && (s_ctx[s_depth - 1]->mode != 1 || s_depth >= MAX_DEPTH))
        || (s_dev_stop && g_frame_count + 1 >= s_dev_stop)) {
        s_refused++;
        if (!s_refused_since) s_refused_since = g_frame_count + 1;
        return 0;
    }
    s_refused_since = 0;
    const XmVerifyFn *fn = it->second;
    for (int d = 0; d < s_depth; d++)
        if (s_ctx[d]->fn == fn) return 0;   /* recursion: the outer test covers it */
    /* synthetic cases from an entry state the corpus reaches: the never-run functions' at the
       first top-level call of any function, a function's own at its first call (nested in
       another test too: everything is put back) */
    if (!s_depth && !s_any_done) { s_any_done = true; run_synth(addr, false); }
    run_synth(addr, true);
    Stat &st = s_stats[addr];
    /* every call until 2000 have been checked, then one in 64. A call the sampling skips is
       still a probe while the function's checked calls haven't covered all its instructions
       (XMEN2_VERIFY_INSNS): its generated run is traced, and it is compared only if it reached an
       instruction not covered yet (the rare paths are what sampling misses: Obj_CollideSegs'
       stuck player ran in 9 corpus runs, in no sampled call) */
    bool probe = false;
    if (st.checked >= 2000 && (st.seen++ & 63)) {
        auto n = s_insns.find(addr);
        if (n == s_insns.end() || st.covered.size() >= n->second) return 0;
        probe = true;
    }

    /* 1. the generated run */
    Ctx &c = *push(fn, 1);
    memcpy(c.pre, g_ram, 0x10000);
    M68KState r0 = g_cpu;
    uint32_t xrts0 = g_xrts;
    uint64_t irq0 = g_xirq_taken;
    run_gen(fn);
    M68KState r1 = g_cpu;
    uint32_t xrts1 = g_xrts;
    memcpy(c.post, g_ram, 0x10000);
    if (g_xirq_taken != irq0) {    /* 2. not comparable */
        if (!probe) st.irq++;
        pop();
        return 1;
    }
    if (probe) {
        bool fresh = false;
        for (uint32_t pc : c.cov)
            if (!st.covered.count(pc)) { fresh = true; break; }
        if (!fresh) {               /* nothing new: the generated run stands, as unsampled */
            pop();
            return 1;
        }
    }

    /* 3. the C run: untimed (no slice end can park it), hardware replayed */
    uint32_t clk = g_xclk, start = g_xstart, end = g_xend;
    g_xend = 0xFFFFFFFFu;
    memcpy(g_ram, c.pre, 0x10000);
    g_cpu = r0;
    g_xrts = xrts0;
    c.mode = 2;
    c.readpos = 0;
    g_xirq_hold = 1;   /* untimed: an SR write takes no interrupt (exact_sched.c) */
    fn->slot();
    g_xirq_hold = 0;

    /* 4. compare */
    char call[48];
    snprintf(call, sizeof call, "call %llu", (unsigned long long)(st.checked + st.mismatches + 1));
    std::string bad = compare(c, fn, r1, xrts1, call);
    if (bad.empty()) {
        st.checked++;
        st.covered.insert(c.cov.begin(), c.cov.end());
    } else {
        st.mismatches++;
        if (st.first.empty()) st.first = bad;
    }

    /* 5. the generated result stands */
    memcpy(g_ram, c.post, 0x10000);
    g_cpu = r1;
    g_xrts = xrts1;
    g_xclk = clk;
    g_xstart = start;
    g_xend = end;
    pop();
    return 1;
}
