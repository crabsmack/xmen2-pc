/* Tusk_HookIsEvent3 - floor-segment type 20 hook while TUSK keeps its arena locked
 * (Tusk_LockArena): 1 for message 3, else 0.
 *
 * msg: the hook's message (Seg_ComputeLength sends 4 first; the other arguments are unused).
 * (MPW C 3.3 turns this if/return pair into a materialized condition, seq/neg in d3 then
 * sne/andi into d0: `return msg == 3;` compiles differently.) */
#include "xmen2.h"

short Tusk_HookIsEvent3(short msg)
{
	if (msg == 3)
		return 1;
	return 0;
}
