/* snd2_chips.cpp - the second sound machine's chips (phase 7 R9, sound2.cpp): the engine's YM2612
 * (ymfm) wrapper compiled a second time under other names, so it has a state of its own (the
 * wrapper's state is all file-local). snd2_psg.c does the same for the PSG. */
#define ym2612_init              snd2_ym2612_init
#define ym2612_advance           snd2_ym2612_advance
#define ym2612_write             snd2_ym2612_write
#define ym2612_render            snd2_ym2612_render
#define ym2612_samples_available snd2_ym2612_samples_available
#define ym2612_sample_rate       snd2_ym2612_sample_rate
#define ym2612_save_state        snd2_ym2612_save_state
#define ym2612_load_state        snd2_ym2612_load_state
#define ym2612_rb_save           snd2_ym2612_rb_save
#define ym2612_rb_load           snd2_ym2612_rb_load
#define ym2612_cosim_hash        snd2_ym2612_cosim_hash
#include "audio/ym2612_ymfm.cpp"
