/* native.h - inside the native runner (phase 5 P6): how its machine (machine.cpp), its host side
 * (main.cpp) and the game's memory layer (game/runtime/native_rt.cpp) talk. C linkage. */
#ifndef XMEN2_NATIVE_H
#define XMEN2_NATIVE_H

#include <stdint.h>

#include "xmen2_options.h"

#ifdef __cplusplus
#include <string>
void        options_init(int *argc, char **argv);   /* options.cpp: --options, xmen2.ini */
std::string options_effective_json(void);           /* {"opt": "value", ...} */
std::string ws_counters_json(void);                 /* widescreen.cpp: the `ws` counters */
extern "C" {
#endif

extern uint8_t g_rom[0x400000];   /* the user's ROM, read at start (main.cpp; R2) */

/* the game's memory (native_rt.cpp) */
void     xm_native_mem_init(void);
uint8_t  xm_ram_peek8(uint32_t off);     /* the work-RAM byte at $FF0000 + off as the 68000 sees it */
void     xm_ram_image(uint8_t *out);     /* all 64 KB as the 68000 sees them (the export) */
uint32_t xm_frame_mark(void);            /* the stack region's mark (xmen2_mem.h xm::frame_*) */
void     xm_frame_set(uint32_t mark);
uint32_t xm_d0_get(void);                /* d0 of stored-pointer calls (bridge.py call_native) */
void     xm_d0_set(uint32_t v);

/* the game (main.cpp, over the bridge's C-visible names) */
void xm_entry_point(void);               /* EntryPoint: the reset vector; never returns */
void xm_vint_handler(void);              /* Irq_VBlankVector: the level-6 vector */

/* the machine (machine.cpp) */
void     machine_power_on(void);         /* power-on, then runner frame 1 begins */
uint64_t machine_frame_no(void);
int      machine_screen_width(void);
int      machine_screen_height(void);
uint64_t machine_frame_tick_waits(void); /* V-ints taken by IRQ_WAIT (plat_wait_vint), outside a sync point */

/* the host side (main.cpp), called by the machine */
#define PLAT_AUDIO_PSG 4                 /* audio/event_queue.h AUDIO_PORT_PSG; 0-3 the YM2612's ports */
void plat_audio_write(uint32_t stamp, int port, uint8_t v);
void plat_scanline(int row, const uint32_t *argb, int width);
void plat_frame_begin(uint64_t frame);   /* runner frame `frame` begins: its pads */
void plat_frame_audio(uint64_t frame);   /* the Z80 has run its frame: the sound of it */
void plat_frame_end(uint64_t frame);     /* it ends (at its sync point): its picture, the benchmark */
int  plat_realtime(void);                /* a played run (a window, paced): nothing is traced */
void plat_frame_repeat(void);            /* played: one more display period of the same picture */
void plat_sync_row(uint64_t sync, uint64_t frame);   /* a sync point: its row, from the export */

/* widescreen.cpp (phase 6): the renderer's margins before a frame is rendered, the checks after */
void ws_frame_setup(void);
void ws_frame_check(void);
void ws_mask_line(int row, uint32_t *argb, int width);   /* the bars (R6) on one rendered row */

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_NATIVE_H */
