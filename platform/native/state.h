/* state.h - the native runner's savestates (state.cpp; xmen2disasm docs/phases/P7-multiplayer.md R2) */
#ifndef XMEN2_STATE_H
#define XMEN2_STATE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <string>
#include <utility>
#include <vector>

struct Snapshot {
    std::vector<uint8_t> bytes;                       /* the sections, in table order */
    std::vector<std::pair<size_t, size_t>> spans;     /* each section's offset and length in bytes */
    std::vector<uint8_t> counters;                    /* the host's counters (not state, not hashed) */
};

void        state_save(Snapshot &snap);
void        state_load(const Snapshot &snap, const char *skip);   /* skip: a section left out (the negative control) */
int         state_has_section(const char *name);
std::string state_sections_json(size_t bytes);    /* {"sections": [...], "bytes": n  (unterminated: the caller adds fields) */
std::string state_sha1(const Snapshot &snap);
void        state_serialize(const Snapshot &snap, std::vector<uint8_t> &out);   /* one blob */
bool        state_deserialize(const uint8_t *p, size_t n, Snapshot &snap);
extern "C" {
#endif

/* custom sections: save(nullptr, 0) returns the bytes needed; save(dst, cap) writes them and returns
   the count (0 on failure); load(src, len) returns 1 on success */
typedef size_t (*StateSaveFn)(uint8_t *dst, size_t cap);
typedef int (*StateLoadFn)(const uint8_t *src, size_t len);

void state_section_range(const char *name, void *p, size_t n);    /* a range of memory of a section */
void state_section_custom(const char *name, StateSaveFn save, StateLoadFn load);
void state_counters(void *p, size_t n);          /* host counters: restored with a snapshot, not state */
void state_after_load(void (*fn)(void));         /* rebuild derived state after every load */

#define STATE_VAR(section, v) state_section_range((section), (void *)&(v), sizeof(v))

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_STATE_H */
