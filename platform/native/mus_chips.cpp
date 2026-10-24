/* mus_chips.cpp - the music machine's chips (music_voices, sound2.cpp): the engine's YM2612 (ymfm)
 * wrapper compiled once more under other names, so it has a state of its own (as snd2_chips.cpp). */
#define ym2612_init              mus_ym2612_init
#define ym2612_advance           mus_ym2612_advance
#define ym2612_write             mus_ym2612_write
#define ym2612_render            mus_ym2612_render
#define ym2612_samples_available mus_ym2612_samples_available
#define ym2612_sample_rate       mus_ym2612_sample_rate
#define ym2612_save_state        mus_ym2612_save_state
#define ym2612_load_state        mus_ym2612_load_state
#define ym2612_rb_save           mus_ym2612_rb_save
#define ym2612_rb_load           mus_ym2612_rb_load
#define ym2612_cosim_hash        mus_ym2612_cosim_hash
#include "audio/ym2612_ymfm.cpp"
