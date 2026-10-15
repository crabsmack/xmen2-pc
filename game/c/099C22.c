/* XClone_SegHookArenaLocked - floor-segment type hook for the clone arena walls (type $19,
 * XClone_StartMakerMode).
 *
 * Returns 0 while the arena is locked (clone flags bit 0), else 1.
 * (MPW C 3.3 turns this if/return pair into seq/andi in d0; `return !(...)` compiles
 * differently.) */
#include "xmen2.h"

#define g_CloneFlags		(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */

short XClone_SegHookArenaLocked(void)
{
	if (g_CloneFlags & 1)
		return 0;
	return 1;
}
