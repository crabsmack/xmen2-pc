/* FloatBio_Init - FLOATBIO Init (class slot +$12): the parent's (BABYBIO) Init, then stats
 * Tbl_FloatBioStats (and their health), speed $100, waits of 60 frames, and by its mode (variant
 * & 3, placement param 4) the AI script, from the first step on its first update (no script: the
 * default state):
 *   0 Tbl_FloatBio_Script0 (FloatBio_Script0: face the player, frames, fire);
 *   1 Tbl_FloatBio_Script1 (FloatBio_Script1 / _Part2: fire, then float up or down), its y range
 *     placement y - |param 0| to y + |param 1| (0: 48 each);
 *   2 Tbl_FloatBio_Script2 (FloatBio_Script2: fire, then float along the patrol range);
 *   3 Tbl_FloatBio_Script3 (FloatBio_Script3: frames, FloatBio_ActWaitParam, fire), the wait
 *     param 0 when there is a placement and it isn't 0.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on;
 * mode 1 reads it even when it is 0).
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): FloatBio_Script1_Part2.
 */
#include "xmen2.h"

void FloatBio_Init(FloatBio *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_FloatBio_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_FloatBioStats))->health;
	self->speed = 0x100;
	self->mode = self->variant & 3;
	self->waitTime = 60;
	switch (self->mode) {
	default:
	case 0:		/* FloatBio_Init_Case0 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_FloatBio_Script0));
		break;
	case 1:		/* FloatBio_Init_Case1 */
#ifndef XMEN2_MPW
		if (!pl)	/* no placement: the 68000 reads the vector table */
			pl = (Placement *)Vectors;
#endif
		self->ceilY = pl->y - ((v = pl->param[0]) ? ABS(v) : 0x30);
		self->floorY = pl->y + ((v = pl->param[1]) ? ABS(v) : 0x30);
		AiScript_Set(&self->ai, CODEPTR(Tbl_FloatBio_Script1));
		break;
	case 2:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FloatBio_Script2));
		break;
	case 3:
		if (pl && (v = pl->param[0]))
			self->waitTime = v;
		AiScript_Set(&self->ai, CODEPTR(Tbl_FloatBio_Script3));
		break;
	}
	/* FloatBio_Init_Tail */
	AI_FIRST_STEP(self, self->ai, 0);
}
