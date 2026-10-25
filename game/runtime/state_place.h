/* state_place.h - the process layout a state file depends on (state_place.cpp) */
#ifndef XMEN2_STATE_PLACE_H
#define XMEN2_STATE_PLACE_H

#include <stddef.h>
#include <stdint.h>

/* Where the game's fiber stack and XMen2Native's arena are mapped in every runner process
   (fiber_set_placement, native_rt.cpp map_twice): far from what the OS hands out by default. */
#define XM_FIBER_BASE 0x3D0000000000ull
#define XM_ARENA_BASE 0x3D0010000000ull

#ifdef __cplusplus
extern "C" {
#endif

/* What a saved fiber stack's addresses depend on, recorded in a state file and compared before
   one loads. */
typedef struct XmPlace {
    uint64_t build;        /* the executable: its PE timestamp (/Brepro: a hash of its contents) and image size */
    uint64_t image_base;   /* where it is loaded (the harness's runner copies: at their preferred base) */
    uint64_t cookie;       /* a hash of the /GS cookie (XMEN2_GS_COOKIE: the same in every process of a group) */
    uint64_t fiber_lo, fiber_top;
    uint64_t arena;        /* XMen2Native's arena (0 in the engine runners) */
} XmPlace;

void        xm_place_get(XmPlace *p, uintptr_t fiber_lo, uintptr_t fiber_top, uintptr_t arena);
const char *xm_place_mismatch(const XmPlace *saved, const XmPlace *mine);   /* why they differ, or NULL */

/* XMEN2_STATE_SCAN=1 (diagnosing): the 8-byte words of a saved stack span (its first byte at
   address `at`) that look like addresses outside the executable, the fiber stack and the arena
   (another module, private memory: the heap), on stderr; those below 4 GB (mostly 68000 values)
   only counted, with XMEN2_STATE_SCAN=2 listed too. Dead slots show up as well; a live one is what
   breaks a load in another process. System DLLs sit at one base per boot in every process. Windows
   only. */
void        xm_place_scan(const uint8_t *bytes, size_t n, uint64_t at, const XmPlace *p, const char *what);

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_STATE_PLACE_H */
