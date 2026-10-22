/* state.cpp - the native runner's savestates (xmen2disasm docs/phases/P7-multiplayer.md R2, M2).
 *
 * A snapshot is the game's whole state at a sync point, while the game's fiber is suspended there
 * (machine.cpp): named sections, each a list of memory ranges or a custom save/load (the fiber's
 * stack and context, the machine with its host pointers scrubbed, the sound chips). In-process
 * only: the fiber's stack holds host addresses (frame pointers, pointers to locals and into the
 * arena and the ROM image), valid because the stack and every static stay where they are. Peers
 * compare the canonical digest (machine.cpp machine_digest), never these bytes.
 *
 * Besides the sections, a snapshot carries the host's counters (the trace's measurements): they are
 * not the game's state (no section, not in the snapshot's hash), but a restore puts them back too,
 * so a probe's first pass or a rollback's replayed ticks are counted once, on the timeline kept.
 *
 * Every module registers its state at start (state_section_*, state_counters); after a load the
 * after-load hooks rebuild what is derived from the state (the palette cache, the screen cache).
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "native.h"
#include "state.h"

[[noreturn]] static void xm_state_fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[native] state: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    fflush(stderr);
    exit(5);
}

namespace {

struct Range { void *p; size_t n; };
struct Section {
    std::string name;
    std::vector<Range> ranges;
    StateSaveFn save = nullptr;   /* custom (instead of the ranges) */
    StateLoadFn load = nullptr;
};

/* never destroyed: the counters' report runs at exit (atexit), after function-local statics made
   later than its registration are gone */
std::vector<Section> &sections()
{
    static auto *s = new std::vector<Section>;
    return *s;
}

std::vector<Range> &counters()
{
    static auto *c = new std::vector<Range>;
    return *c;
}

std::vector<void (*)(void)> &after_load()
{
    static auto *a = new std::vector<void (*)(void)>;
    return *a;
}

Section &section(const char *name)
{
    for (Section &s : sections())
        if (s.name == name) return s;
    sections().push_back(Section{ name, {}, nullptr, nullptr });
    return sections().back();
}

}  // namespace

static std::vector<size_t> s_last_spans;   /* each section's size in the last snapshot saved (reported) */

extern "C" void state_section_range(const char *name, void *p, size_t n)
{
    section(name).ranges.push_back(Range{ p, n });
}

extern "C" void state_section_custom(const char *name, StateSaveFn save, StateLoadFn load)
{
    Section &s = section(name);
    s.save = save;
    s.load = load;
}

extern "C" void state_counters(void *p, size_t n) { counters().push_back(Range{ p, n }); }

extern "C" void state_after_load(void (*fn)(void)) { after_load().push_back(fn); }

static size_t section_size(const Section &s)
{
    if (s.save) return s.save(nullptr, 0);
    size_t n = 0;
    for (const Range &r : s.ranges) n += r.n;
    return n;
}

void state_save(Snapshot &snap)
{
    snap.spans.clear();
    s_last_spans.clear();
    size_t total = 0;
    for (const Section &s : sections()) total += section_size(s);
    snap.bytes.resize(total);
    size_t at = 0;
    for (const Section &s : sections()) {
        size_t n;
        if (s.save) {
            n = s.save(snap.bytes.data() + at, snap.bytes.size() - at);
            if (!n && section_size(s)) xm_state_fatal("section %s: its save failed", s.name.c_str());
        } else {
            n = 0;
            for (const Range &r : s.ranges) { memcpy(snap.bytes.data() + at + n, r.p, r.n); n += r.n; }
        }
        snap.spans.push_back({ at, n });
        s_last_spans.push_back(n);
        at += n;
    }
    snap.bytes.resize(at);
    size_t cn = 0;
    for (const Range &r : counters()) cn += r.n;
    snap.counters.resize(cn);
    cn = 0;
    for (const Range &r : counters()) { memcpy(snap.counters.data() + cn, r.p, r.n); cn += r.n; }
}

/* skip: the negative control (--state-skip): that section is left as it is */
void state_load(const Snapshot &snap, const char *skip)
{
    std::vector<Section> &all = sections();
    if (snap.spans.size() != all.size()) xm_state_fatal("a snapshot of %zu sections for %zu", snap.spans.size(), all.size());
    for (size_t i = 0; i < all.size(); i++) {
        const Section &s = all[i];
        if (skip && s.name == skip) continue;
        const uint8_t *src = snap.bytes.data() + snap.spans[i].first;
        size_t n = snap.spans[i].second;
        if (s.load) {
            if (!s.load(src, n)) xm_state_fatal("section %s: its load failed", s.name.c_str());
            continue;
        }
        size_t k = 0;
        for (const Range &r : s.ranges) { memcpy(r.p, src + k, r.n); k += r.n; }
    }
    size_t cn = 0;
    for (const Range &r : counters()) { memcpy(r.p, snap.counters.data() + cn, r.n); cn += r.n; }
    for (auto fn : after_load()) fn();
}

int state_has_section(const char *name)
{
    for (const Section &s : sections())
        if (s.name == name) return 1;
    return 0;
}

std::string state_sections_json(size_t bytes)
{
    std::string o = "{\"sections\": [";
    for (size_t i = 0; i < sections().size(); i++) o += (i ? ", \"" : "\"") + sections()[i].name + "\"";
    o += "], \"section_bytes\": {";
    for (size_t i = 0; i < sections().size(); i++)
        o += (i ? ", \"" : "\"") + sections()[i].name + "\": " + std::to_string(s_last_spans.size() > i ? s_last_spans[i] : 0);
    char b[96];
    snprintf(b, sizeof b, "}, \"bytes\": %zu", bytes);
    return o + b;
}

/* ---- one blob (netplay's snapshot ring stores blobs): counts, spans, counters, the sections ---- */

void state_serialize(const Snapshot &snap, std::vector<uint8_t> &out)
{
    uint64_t head[2] = { snap.spans.size(), snap.counters.size() };
    out.resize(sizeof head + snap.spans.size() * 16 + snap.counters.size() + snap.bytes.size());
    uint8_t *o = out.data();
    memcpy(o, head, sizeof head); o += sizeof head;
    for (const auto &sp : snap.spans) {
        uint64_t v[2] = { sp.first, sp.second };
        memcpy(o, v, 16); o += 16;
    }
    memcpy(o, snap.counters.data(), snap.counters.size()); o += snap.counters.size();
    memcpy(o, snap.bytes.data(), snap.bytes.size());
}

bool state_deserialize(const uint8_t *p, size_t n, Snapshot &snap)
{
    uint64_t head[2];
    if (n < sizeof head) return false;
    memcpy(head, p, sizeof head);
    size_t fixed = sizeof head + head[0] * 16 + head[1];
    if (head[0] > 64 || fixed > n) return false;
    const uint8_t *i = p + sizeof head;
    snap.spans.resize(head[0]);
    for (auto &sp : snap.spans) {
        uint64_t v[2];
        memcpy(v, i, 16); i += 16;
        sp = { (size_t)v[0], (size_t)v[1] };
    }
    snap.counters.assign(i, i + head[1]); i += head[1];
    snap.bytes.assign(i, p + n);
    for (const auto &sp : snap.spans)
        if (sp.first + sp.second > snap.bytes.size()) return false;
    return true;
}

/* ---- SHA-1 (snapshot identity in probe_snap.csv) ------------------------------------------------ */

std::string state_sha1(const Snapshot &snap)
{
    uint8_t d[20];
    xm_sha1(snap.bytes.data(), snap.bytes.size(), d);
    char hex[41];
    for (int i = 0; i < 20; i++) snprintf(hex + 2 * i, 3, "%02x", d[i]);
    return hex;
}
