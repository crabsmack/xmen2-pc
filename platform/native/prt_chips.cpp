/* prt_chips.cpp - the parts machine's chips (music_voices, sound2.cpp): the engine's YM2612 (ymfm)
 * wrapper compiled once more under other names, so it has a state of its own (as snd2_chips.cpp). */
/* the engine's file is never a source of the target itself (snd1_chips.cpp says why) */
#define ym2612_init              prt_ym2612_init
#define ym2612_advance           prt_ym2612_advance
#define ym2612_write             prt_ym2612_write
#define ym2612_render            prt_ym2612_render
#define ym2612_samples_available prt_ym2612_samples_available
#define ym2612_sample_rate       prt_ym2612_sample_rate
#define ym2612_save_state        prt_ym2612_save_state
#define ym2612_load_state        prt_ym2612_load_state
#define ym2612_rb_save           prt_ym2612_rb_save
#define ym2612_rb_load           prt_ym2612_rb_load
#define ym2612_cosim_hash        prt_ym2612_cosim_hash
#include "audio/ym2612_ymfm.cpp"
