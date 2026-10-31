/* p34_chips.cpp - the full-parts machine's chips (full_parts: players 3-4's parts, sound2.cpp): the engine's
 * YM2612 (ymfm) wrapper compiled once more under other names, so it has a state of its own (as snd2_chips.cpp). */
/* the engine's file is never a source of the target itself (snd1_chips.cpp says why) */
#define ym2612_init              p34_ym2612_init
#define ym2612_advance           p34_ym2612_advance
#define ym2612_write             p34_ym2612_write
#define ym2612_render            p34_ym2612_render
#define ym2612_samples_available p34_ym2612_samples_available
#define ym2612_sample_rate       p34_ym2612_sample_rate
#define ym2612_save_state        p34_ym2612_save_state
#define ym2612_load_state        p34_ym2612_load_state
#define ym2612_rb_save           p34_ym2612_rb_save
#define ym2612_rb_load           p34_ym2612_rb_load
#define ym2612_cosim_hash        p34_ym2612_cosim_hash
#define ym2612_set_model         p34_ym2612_set_model
#include "audio/ym2612_ymfm.cpp"
