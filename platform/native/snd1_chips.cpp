/* snd1_chips.cpp - the first sound machine's chips: the engine's YM2612 (ymfm) wrapper under its own
 * names. It is compiled through this file, not listed as a source of its own, because the other
 * machines' copies (snd2_chips.cpp, ...) include it: MSBuild's tracker credits every file read
 * after a compile opens one of the target's sources to that source, so the copies had no
 * dependencies and kept a stale chip after an engine change (native.cmake). snd1_psg.c: the PSG. */
#include "audio/ym2612_ymfm.cpp"
