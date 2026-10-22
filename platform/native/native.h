/* native.h - inside the native runner (phase 5 P6): how its machine (machine.cpp), its host side
 * (main.cpp) and the game's memory layer (game/runtime/native_rt.cpp) talk. C linkage. */
#ifndef XMEN2_NATIVE_H
#define XMEN2_NATIVE_H

#include <stddef.h>
#include <stdint.h>

#include "xmen2_options.h"

#ifdef __cplusplus
#include <stdio.h>
#include <string>
void        options_init(int *argc, char **argv);   /* options.cpp: --options, xmen2.ini */
void        options_toggle_high_fps(void);          /* played: F9 */
void        options_toggle_widescreen(void);        /* played: F10 (from the next screen) */
void        options_set_widescreen(int on);
void        options_set_first_scene(int scene);     /* --scene N: a new game starts there */
int         options_first_scene(void);              /* -1: none */
std::string options_game_text(void);                /* netplay (R10): the game-changing options, k=v,... */
bool        options_adopt_game(const std::string &text);   /* a guest takes the host's */
/* netplay.cpp (phase 7 R1): rollback netplay of the release runner */
bool        netplay_arg(int argc, char **argv, int *i);      /* --netplay ... options; true when it took one */
bool        netplay_enabled(void);
void        netplay_run(uint64_t frames);           /* never returns: the netplay loop (frames: 0 = played) */
void        netplay_print_json(void);               /* --netplay --json */
void        ws_request_toggle(void);                /* widescreen.cpp */
void        present_reset(void);                    /* present.cpp: high_fps turned on again */
void        present_repeat(void);                   /* present.cpp: a played frame-tick V-int's period */
std::string options_effective_json(void);           /* {"opt": "value", ...} */
std::string ws_counters_json(void);                 /* widescreen.cpp: the `ws` counters */
/* present.cpp (phase 6 R4): the present clock and log, high_fps's in-between frames */
void        present_init(const char *log_path);
bool        present_logging(void);
void        present_scan(FILE *f, uint64_t flip);   /* --flip-scan: a flipped pass's rows (null: the log's) */
bool        present_whole(uint64_t k, const uint32_t *fb, int stride, int w, int h, bool interactive);
void        present_after_render(uint64_t k, bool interactive, uint32_t *ifb, int stride);
std::string present_counters_json(void);
#include <vector>
void        ws_digest_fields(std::vector<uint8_t> *out);   /* widescreen.cpp: its part of the canonical digest */
/* players.cpp (phase 7: four players on the platform side, the mp counters) */
void        players_spr_owner(const void *owner, int idx);
void        players_spr_dropped(int n);
void        players_sync_row(void);
void        players_state_init(void);
int         players_p34_running(void);
std::string players_counters_json(void);
bool        players_vram_write(uint16_t a, uint16_t w);   /* a player 3-4's VRAM port write, into its tiles */
void        players_video_init(void);
void        players_frame_setup(void);   /* before a frame is rendered: VRAM's sprite chain ended at 80 */
void        players_frame_restore(void);
/* sound2.cpp (phase 7 R9: the second sound machine, players 3-4's music parts) */
void        snd2_digest_fields(std::vector<uint8_t> *out);
std::string snd2_counters_json(void);
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
void     machine_run(void);              /* the game on its fiber, the host between its sync points; never returns */
void     machine_start(void);            /* the game's fiber, made (netplay runs the ticks itself) */
void     machine_set_render(int on);     /* 0: ticks render no frame (a rollback's replay) */
uint16_t machine_peek16(uint32_t a);     /* a word of the 68000 bus (ROM, work RAM, players 3-4's region) */
void     machine_tick(void);             /* the host: one more runner frame, to the game's next sync point */
void     machine_state_init(void);       /* register the snapshot's sections (state.cpp) */
uint64_t machine_digest(void);           /* the canonical digest of the state now (P7 "Definitions") */
void     machine_digest_parts(uint64_t out[3]);   /* the last digest's partitions: work RAM, the VDP, the rest */
uint64_t machine_stack_peak(void);       /* the game fiber's deepest stack use, bytes */
void     machine_set_session(uint64_t seed);   /* the session machine (P7 "Definitions") */
int      machine_session(void);
uint64_t machine_session_seed(void);
void     machine_audio_init(void);        /* the sound chips' mixer (the session machine runs them) */
void     xm_mem_state_init(void);        /* native_rt.cpp: the "arena" section */
void     ws_state_init(void);            /* widescreen.cpp: its statics in "platform" */
void     xm_sha1(const uint8_t *data, size_t len, uint8_t out[20]);   /* main.cpp */
uint64_t machine_frame_no(void);
int      machine_screen_width(void);
int      machine_screen_height(void);
uint64_t machine_frame_tick_waits(void); /* V-ints taken by IRQ_WAIT (plat_wait_vint), outside a sync point */
void     snd2_ring_write(const uint8_t *ram, uint32_t off, uint8_t v);   /* sound2.cpp: the game wrote Z80 RAM */
void     snd2_frame(void);                /* ... the first machine's Z80 frame is done: the second's */
void     snd2_mix(int16_t *fm, size_t fm_n, int16_t *psg, size_t psg_n, int mute);   /* host_audio */
void     snd2_state_init(void);

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
void plat_at_sync(uint64_t sync);        /* the host loop at a sync point, before the next frame (the probe) */

/* widescreen.cpp (phase 6): the renderer's margins before a frame is rendered, the checks after */
void ws_frame_setup(void);
void ws_frame_check(void);
void ws_mask_line(int row, uint32_t *argb, int width);   /* the bars (R6) on one rendered row */

/* present.cpp / main.cpp / machine.cpp (phase 6 R4) */
void machine_render(void);                  /* render the frame from the VDP's state now */
void plat_scan_target(uint32_t *fb);        /* where plat_scanline writes (null: the frame buffer) */
void plat_after_render(void);               /* the frame begun is rendered: high_fps's frames */
void present_note_render(double us);        /* a displayed frame's render time */

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_NATIVE_H */
