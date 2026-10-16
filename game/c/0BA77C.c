/* SavageBigBio_Move - SAVAGEBIGBIO Move (class slot +$4E): its x speed kept within
 * +-g_A4BossMaxSpeed, then the parent's (SAVAGEBIOBASE) Move.
 *
 * self: the boss.  steps: passed on (1, or 2 in two-player stepping). */
#include "xmen2.h"

#define g_A4BossMaxSpeed	(*(s32 *)g_SceneVar3B24)	/* scene variable: SAVAGEBIGBIO's top x speed (SavageBigBio_Move) */

void SavageBigBio_Move(register SavageBigBio *self, short steps)
{
	if (self->vx > g_A4BossMaxSpeed)
		self->vx = g_A4BossMaxSpeed;
	else if (self->vx < -g_A4BossMaxSpeed)
		self->vx = -g_A4BossMaxSpeed;
	(*CLASS(Tbl_SavageBigBio_vtable)->parent->method[18])(self, steps);	/* SAVAGEBIOBASE Move */
}
