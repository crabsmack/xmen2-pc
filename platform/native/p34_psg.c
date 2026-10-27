/* p34_psg.c - the full-parts machine's PSG (full_parts: players 3-4's parts, sound2.cpp): the engine's SN76489
 * wrapper compiled once more under other names, so it has a state of its own (as snd2_psg.c). */
#define psg_init              p34_psg_init
#define psg_advance           p34_psg_advance
#define psg_write             p34_psg_write
#define psg_render            p34_psg_render
#define psg_samples_available p34_psg_samples_available
#define psg_sample_rate       p34_psg_sample_rate
#define psg_reset_leftover    p34_psg_reset_leftover
#define psg_save_state        p34_psg_save_state
#define psg_load_state        p34_psg_load_state
#define psg_rb_save           p34_psg_rb_save
#define psg_rb_load           p34_psg_rb_load
#define psg_cosim_hash        p34_psg_cosim_hash
#define psg_cosim_dump        p34_psg_cosim_dump
#include "audio/sn76489.c"
