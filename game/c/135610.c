/* FlyBrood_StateSwoopPatrol - FLYBROOD swoop patrol: at an end of its x range with the nearest
 * player outside the range, the next step now; otherwise swoop at the player (FlyBrood_MoveHoming
 * on Tbl_FlyBroodSwoopPath, 130 px beyond the player the way it heads, 27 px above the player's
 * second box) and wait for the swoop to end (FlyBrood_WaitSwoopDone).
 *
 * self: the object. Returns flags & 8 when it swoops (nothing in particular otherwise).
 * Swoop animation Tbl_Anim_134386 (Tbl_Anim_134390 in the raw-art scene). Never runs in the
 * corpus. */
#include "xmen2.h"

short FlyBrood_StateSwoopPatrol(FlyBrood *self)
{
	short x = self->x >> 8;
	XPlayer *p;
	short px;

	p = Player_Nearest((Actor *)self);
	px = p->x >> 8;
	if ((x == self->xMin || x == self->xMax) && (px < self->xMin || px > self->xMax)) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		self->mover = (FlyMoveFn)FlyBrood_MoveHoming;
		self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodSwoopPath);
		self->anchorX = !self->headingRight ? -0x82 : 0x82;
		self->anchorY = -0x1B;
		if (g_ScnUnused3B5C != g_SceneIndex)
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_134386));
		else
			Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_134390));
		SET_STATE(self, FlyBrood_WaitSwoopDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
