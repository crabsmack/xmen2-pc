/* Tusk_StateThrow - TUSK at the end of the throw animation: let go of the boulder it holds
 * (its first attached child; none: just choose again) with frame 3 and sound $AC, giving it
 * the velocity and gravity of its aim column (-6..5, the same both sides; columns -1..1 throw
 * downwards), mirrored for a column to the left; then the idle animation and choose again
 * after 20 frames (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0.
 * The throw is first set to column -3's and mirrored when TUSK faces left, then overwritten by
 * the column's (only a column outside -6..5 keeps it); the boulder's speed limits are made
 * positive. The empty if (0) at the end is a block the original compiled out: its end is a
 * label, where MPW C pops Obj_SetAnim's arguments. */
#include "xmen2.h"

short Tusk_StateThrow(Tusk *self)
{
	register Actor *held;

	held = self->spr.firstChild == 0xFF ? 0 : g_ObjTable[self->spr.firstChild];
	if (!held) {
		self->state = Tusk_StateChooseAttack;
		return 0;
	}
	(*self->cls->method[20])(self, 3);		/* +$56 SetFrame */
	Snd_Stop(0xAC);
	Snd_Play(0xAC);
	Obj_Detach(held);
	held->vx = 0x2BC;
	held->vy = -0x258;
	held->ay = 0x6E;
	if (self->spr.flags & 0x10)
		held->vx = -held->vx;
	switch (self->aimColumn) {
	case -6:
		held->vx = 0x424;
		held->vy = -0x3B6;
		held->ay = 0x64;
		break;
	case -5:
		held->vx = 0x424;
		held->vy = -0x3B6;
		held->ay = 0x64;
		break;
	case -4:
		held->vx = 0x384;
		held->vy = -0x258;
		held->ay = 0x6E;
		break;
	case -3:
		held->vx = 0x2BC;
		held->vy = -0x258;
		held->ay = 0x6E;
		break;
	case -2:
		held->vx = 0x258;
		held->vy = -0x2BC;
		held->ay = 0x6E;
		break;
	case -1:
		held->vx = 0x12C;
		held->vy = 0x2BC;
		held->ay = 0x6E;
		break;
	case 0:
		held->vx = 0x96;
		held->vy = 0x4EC;
		held->ay = 0x50;
		break;
	case 1:
		held->vx = 0x12C;
		held->vy = 0x2BC;
		held->ay = 0x6E;
		break;
	case 2:
		held->vx = 0x258;
		held->vy = -0x2BC;
		held->ay = 0x6E;
		break;
	case 3:
		held->vx = 0x2BC;
		held->vy = -0x258;
		held->ay = 0x6E;
		break;
	case 4:
		held->vx = 0x384;
		held->vy = -0x258;
		held->ay = 0x6E;
		break;
	case 5:
		held->vx = 0x424;
		held->vy = -0x3B6;
		held->ay = 0x64;
		break;
	}
	held->maxVx = ABS(held->maxVx);
	held->maxVy = ABS(held->maxVy);
	if (self->aimColumn < 0)
		held->vx = -held->vx;
	Obj_SetAnim(self, CODEPTR(Tbl_TuskAnim));
	self->countdown = 20;
	self->state = Tusk_StateChooseAttack;
	if (0) {				/* compiled out (see the header) */
	}
	return 0;
}
