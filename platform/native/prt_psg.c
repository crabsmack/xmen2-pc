/* prt_psg.c - the parts machine's PSG (music_voices, sound2.cpp): the engine's SN76489 wrapper
 * compiled once more under other names, so it has a state of its own (as snd2_psg.c). */
#define psg_init              prt_psg_init
#define psg_advance           prt_psg_advance
#define psg_write             prt_psg_write
#define psg_render            prt_psg_render
#define psg_samples_available prt_psg_samples_available
#define psg_sample_rate       prt_psg_sample_rate
#define psg_reset_leftover    prt_psg_reset_leftover
#define psg_save_state        prt_psg_save_state
#define psg_load_state        prt_psg_load_state
#define psg_rb_save           prt_psg_rb_save
#define psg_rb_load           prt_psg_rb_load
#define psg_cosim_hash        prt_psg_cosim_hash
#define psg_cosim_dump        prt_psg_cosim_dump
#include "audio/sn76489.c"
