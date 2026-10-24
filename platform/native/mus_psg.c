/* mus_psg.c - the music machine's PSG (music_voices, sound2.cpp): the engine's SN76489 wrapper
 * compiled once more under other names, so it has a state of its own (as snd2_psg.c). */
#define psg_init              mus_psg_init
#define psg_advance           mus_psg_advance
#define psg_write             mus_psg_write
#define psg_render            mus_psg_render
#define psg_samples_available mus_psg_samples_available
#define psg_sample_rate       mus_psg_sample_rate
#define psg_reset_leftover    mus_psg_reset_leftover
#define psg_save_state        mus_psg_save_state
#define psg_load_state        mus_psg_load_state
#define psg_rb_save           mus_psg_rb_save
#define psg_rb_load           mus_psg_rb_load
#define psg_cosim_hash        mus_psg_cosim_hash
#define psg_cosim_dump        mus_psg_cosim_dump
#include "audio/sn76489.c"
