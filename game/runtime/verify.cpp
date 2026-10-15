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
 *      registers d3-d7/a2-a6, and the hardware access sequence;
 *   5. put the generated result back and continue: the game always runs the generated code.
 * A call that passes adds the instructions it executed to F's covered set.
 *
 * XMEN2_VERIFY_OUT=<path> writes {"functions": {"ADDR": {"calls_checked", "mismatches",
 * "irq_skipped", "first_mismatch", "covered": [pc, ...]}}} at exit; XMEN2_VERIFY_ONLY=ADDR,...
 * restricts the tests to those functions.
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
extern uint32_t g_xclk, g_xstart, g_xend, g_xrts;
extern int g_xtrace;
uint32_t xmen2_stack_low(void);
}

struct XmVerifyFn { uint32_t addr, end; void (*slot)(void); void (*gen)(void); int ret_bytes; };
namespace xm { extern bool g_retnv; }   /* the C call just made returned no value (handc_rt.cpp) */
extern const XmVerifyFn xm_verify_fns[];
extern const int xm_verify_fn_count;

namespace {

struct Stat {
    uint64_t checked = 0, mismatches = 0, irq = 0, seen = 0;
    std::string first;
    std::unordered_set<uint32_t> covered;
};
struct Hw {
    uint32_t addr; int size; int write; uint16_t v;
    bool operator==(const Hw &o) const { return addr == o.addr && size == o.size && write == o.write && v == o.v; }
};

std::unordered_map<uint32_t, const XmVerifyFn *> s_fns;
std::unordered_map<uint32_t, Stat> s_stats;
bool s_in_shadow;
const XmVerifyFn *s_cur;
std::vector<uint32_t> s_cov;
std::vector<Hw> s_logA, s_logB;
size_t s_readpos;
int s_mode;   /* 1: record (generated run), 2: replay (C run) */
uint8_t s_pre[0x10000], s_post[0x10000];

int hw_hook(uint32_t a, int size, int kind, uint16_t v, uint16_t *out)
{
    if (s_mode == 1) {
        if (kind == 1) s_logA.push_back({ a, size, 0, v });
        else if (kind == 2) s_logA.push_back({ a, size, 1, v });
        return 0;
    }
    if (s_mode == 2) {
        if (kind == 0) {
            /* the next read the generated run made (its value), whatever this one is:
               the sequence comparison reports any difference */
            uint16_t r = 0;
            while (s_readpos < s_logA.size() && s_logA[s_readpos].write) s_readpos++;
            if (s_readpos < s_logA.size()) r = s_logA[s_readpos++].v;
            *out = r;
            s_logB.push_back({ a, size, 0, r });
            return 1;
        }
        if (kind == 2) { s_logB.push_back({ a, size, 1, v }); return 1; }
    }
    return 0;
}

void insn_hook(uint32_t pc)
{
    if (s_cur && pc >= s_cur->addr && pc < s_cur->end) s_cov.push_back(pc);
}

std::unordered_set<uint32_t> s_only;

void load_only()
{
    const char *e = getenv("XMEN2_VERIFY_ONLY");
    if (!e) return;
    for (const char *s = e; *s;) {
        char *end;
        unsigned long v = strtoul(s, &end, 16);
        if (end == s) break;
        s_only.insert((uint32_t)v);
        s = *end ? end + 1 : end;
    }
}

void report()
{
    const char *path = getenv("XMEN2_VERIFY_OUT");
    if (!path) return;
    FILE *f = fopen(path, "w");
    if (!f) return;
    fprintf(f, "{\"functions\": {");
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
                   "\"first_mismatch\": %s%s%s, \"covered\": [",
                firstf ? "" : ",", (unsigned)a, (unsigned long long)st.checked, (unsigned long long)st.mismatches,
                (unsigned long long)st.irq, st.first.empty() ? "null" : "\"", first.c_str(), st.first.empty() ? "" : "\"");
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

std::string describe(const char *what, uint32_t a, uint32_t g, uint32_t c, uint64_t call)
{
    char b[160];
    snprintf(b, sizeof b, "call %llu: %s %06X: generated %08X, C %08X", (unsigned long long)call, what, (unsigned)a,
             (unsigned)g, (unsigned)c);
    return b;
}

}  /* namespace */

extern "C" int xmen2_verify_entry(uint32_t addr)
{
    static bool init;
    if (!init) {
        init = true;
        for (int i = 0; i < xm_verify_fn_count; i++) s_fns[xm_verify_fns[i].addr] = &xm_verify_fns[i];
        load_only();
        atexit(report);
    }
    if (s_in_shadow) return 0;
    auto it = s_fns.find(addr);
    if (it == s_fns.end() || (!s_only.empty() && !s_only.count(addr))) return 0;
    const XmVerifyFn *fn = it->second;
    Stat &st = s_stats[addr];
    /* every call until 2000 have been checked, then one in 64 */
    if (st.checked >= 2000 && (st.seen++ & 63)) return 0;

    s_in_shadow = true;
    /* 1. the generated run */
    memcpy(s_pre, g_ram, 0x10000);
    M68KState r0 = g_cpu;
    uint32_t xrts0 = g_xrts;
    uint64_t irq0 = g_xirq_taken;
    int trace0 = g_xtrace;
    s_logA.clear();
    s_cov.clear();
    s_cur = fn;
    s_mode = 1;
    g_xhw_hook = hw_hook;
    g_xinsn_hook = insn_hook;
    g_xtrace = 1;
    fn->gen();                     /* its XENTRY sees s_in_shadow and lets the body run */
    g_xinsn_hook = nullptr;
    g_xtrace = trace0;
    s_mode = 0;
    M68KState r1 = g_cpu;
    uint32_t xrts1 = g_xrts;
    memcpy(s_post, g_ram, 0x10000);
    if (g_xirq_taken != irq0) {    /* 2. not comparable */
        st.irq++;
        g_xhw_hook = nullptr;
        s_cur = nullptr;
        s_in_shadow = false;
        return 1;
    }

    /* 3. the C run: untimed (no slice end can park it), hardware replayed */
    uint32_t clk = g_xclk, start = g_xstart, end = g_xend;
    g_xend = 0xFFFFFFFFu;
    memcpy(g_ram, s_pre, 0x10000);
    g_cpu = r0;
    g_xrts = xrts0;
    s_logB.clear();
    s_readpos = 0;
    s_mode = 2;
    fn->slot();
    s_mode = 0;
    g_xhw_hook = nullptr;
    M68KState rc = g_cpu;
    uint32_t xrtsc = g_xrts;

    /* 4. compare */
    uint64_t call = st.checked + st.mismatches + 1;
    std::string bad;
    uint32_t lo = xmen2_stack_low() & 0xFFFFu, hi = r1.A[7] & 0xFFFFu;
    if (hi < lo) hi = lo;
    for (uint32_t i = 0; i < 0x10000u && bad.empty(); i++) {
        if (i >= lo && i < hi) continue;   /* dead stack: below the returned A7 */
        if (g_ram[i] != s_post[i]) bad = describe("RAM", 0xFF0000u + i, s_post[i], g_ram[i], call);
    }
    if (bad.empty() && rc.A[7] != r1.A[7]) bad = describe("A7", 7, r1.A[7], rc.A[7], call);
    if (bad.empty() && xrtsc != xrts1) bad = describe("return to", 0, xrts1, xrtsc, call);
    if (bad.empty() && fn->ret_bytes && !xm::g_retnv) {   /* no value returned: d0 is not a result */
        uint32_t m = fn->ret_bytes >= 4 ? 0xFFFFFFFFu : fn->ret_bytes == 2 ? 0xFFFFu : 0xFFu;
        if ((r1.D[0] & m) != (rc.D[0] & m)) bad = describe("d0", 0, r1.D[0] & m, rc.D[0] & m, call);
    }
    for (int r = 3; r <= 7 && bad.empty(); r++)
        if (rc.D[r] != r1.D[r]) bad = describe("d", (uint32_t)r, r1.D[r], rc.D[r], call);
    for (int r = 2; r <= 6 && bad.empty(); r++)
        if (rc.A[r] != r1.A[r]) bad = describe("a", (uint32_t)r, r1.A[r], rc.A[r], call);
    if (bad.empty() && !(s_logA == s_logB)) {
        size_t k = 0;
        while (k < s_logA.size() && k < s_logB.size() && s_logA[k] == s_logB[k]) k++;
        char b[200];
        if (k < s_logA.size() && k < s_logB.size())
            snprintf(b, sizeof b, "call %llu: hardware access %zu: generated %s $%06X=%04X, C %s $%06X=%04X",
                     (unsigned long long)call, k, s_logA[k].write ? "write" : "read", (unsigned)s_logA[k].addr,
                     s_logA[k].v, s_logB[k].write ? "write" : "read", (unsigned)s_logB[k].addr, s_logB[k].v);
        else
            snprintf(b, sizeof b, "call %llu: %zu hardware accesses generated, %zu in C", (unsigned long long)call,
                     s_logA.size(), s_logB.size());
        bad = b;
    }
    if (bad.empty()) {
        st.checked++;
        st.covered.insert(s_cov.begin(), s_cov.end());
    } else {
        st.mismatches++;
        if (st.first.empty()) st.first = bad;
    }

    /* 5. the generated result stands */
    memcpy(g_ram, s_post, 0x10000);
    g_cpu = r1;
    g_xrts = xrts1;
    g_xclk = clk;
    g_xstart = start;
    g_xend = end;
    s_cur = nullptr;
    s_in_shadow = false;
    return 1;
}
