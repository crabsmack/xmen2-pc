/* xmen2_image.h - the 68000 machine state a sync row is computed from (phase 4 sync points,
 * phase 5 export), in the layouts of the reference's dump files (xmen2disasm
 * emu/dump_state.py; game/xmen2_spec.c maybe_dump):
 *   ram    work RAM $FF0000-$FFFFFF, 68000 byte order (big-endian)
 *   vram   64 KB, 68000 byte order
 *   cram   64 bus-format words & $0EEE, big-endian
 *   vsram  40 words & $07FF, big-endian
 * The stepped and timed runners take it straight from the emulated machine; XMen2Native builds
 * it with xm_export (game/runtime/export.cpp) from where the game state lives, and its sync
 * rows (xmen2_spec.c) are computed from that image only.
 */
#ifndef XMEN2_IMAGE_H
#define XMEN2_IMAGE_H

#include <stddef.h>
#include <stdint.h>

#define XMEN2_IMAGE_CRAM_WORDS  64
#define XMEN2_IMAGE_VSRAM_WORDS 40

typedef struct XmImage {
    uint8_t ram[0x10000];
    uint8_t vram[0x10000];
    uint8_t cram[2 * XMEN2_IMAGE_CRAM_WORDS];
    uint8_t vsram[2 * XMEN2_IMAGE_VSRAM_WORDS];
} XmImage;

#ifdef __cplusplus
extern "C" {
#endif

/* xmen2_spec.c: n words, each & mask, big-endian (the reference's CRAM/VSRAM format). */
void xmen2_words_be(uint8_t *out, const uint16_t *w, size_t n, uint16_t mask);

/* export.cpp (XMen2Native only): the game state as the image; its counts as a JSON object. */
void xm_export(XmImage *im);
int  xm_export_stats(char *buf, int n);

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_IMAGE_H */
