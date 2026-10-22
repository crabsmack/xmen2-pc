/* snd2_psg.c - the second sound machine's PSG (phase 7 R9, sound2.cpp): the engine's SN76489
 * wrapper compiled a second time under other names, so it has a state of its own. */
#define psg_init              snd2_psg_init
#define psg_advance           snd2_psg_advance
#define psg_write             snd2_psg_write
#define psg_render            snd2_psg_render
#define psg_samples_available snd2_psg_samples_available
#define psg_sample_rate       snd2_psg_sample_rate
#define psg_reset_leftover    snd2_psg_reset_leftover
#define psg_save_state        snd2_psg_save_state
#define psg_load_state        snd2_psg_load_state
#define psg_rb_save           snd2_psg_rb_save
#define psg_rb_load           snd2_psg_rb_load
#define psg_cosim_hash        snd2_psg_cosim_hash
#define psg_cosim_dump        snd2_psg_cosim_dump
#include "audio/sn76489.c"
