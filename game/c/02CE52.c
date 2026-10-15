/* Exodus_NextPattern - EXODUS: go on to the next attack pattern and start it from step 0.
 *
 * self: the boss.
 * Patterns 1-5 are followed by the pattern in their placement parameter (param[0] after
 * pattern 1 ... param[4] after pattern 5); a 0 there means the default order 1, 2, 3, 4, 5, 1.
 * Pattern 0 stays. The placement entry is g_PlacementList[self->unk_14] (the byte Actor_SpawnFromRec
 * copies from $FFF633).
 * The cases are Exodus_NextPattern_part1 (1), _part2 (3) and the tail _part3 in the disassembly. */
#include "xmen2.h"

void Exodus_NextPattern(Exodus *self)
{
	register Placement *pl = &g_PlacementList[self->unk_14];

	switch (self->pattern) {
	default:
	case 0:
		break;
	case 1:		/* Exodus_NextPattern_part1 */
		self->pattern = pl->param[0] ? pl->param[0] : 2;
		break;
	case 2:
		self->pattern = pl->param[1] ? pl->param[1] : 3;
		break;
	case 3:		/* Exodus_NextPattern_part2 */
		self->pattern = pl->param[2] ? pl->param[2] : 4;
		break;
	case 4:
		self->pattern = pl->param[3] ? pl->param[3] : 5;
		break;
	case 5:
		self->pattern = pl->param[4] ? pl->param[4] : 1;
		break;
	}
	self->step = 0;		/* Exodus_NextPattern_part3 */
}
