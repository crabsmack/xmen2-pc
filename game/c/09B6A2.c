/* BroodClaw_StateDropClone - BROODCLAW state at the drop point: release an evil clone, then go.
 *
 * self: the claw.
 * A claw with a pair waits until no live player's body box centre is right of x $71E; then the
 * segment hooks $15 and $1A, both claws' motion and speed and the camera shake are cleared and
 * the camera pans to ($5EA, $5A0) (CAMERA slot +$36 PanTo, speed 2, both axes locked). A claw
 * without one waits until a clone-maker kill was counted (g_CloneMakerKills). The clone
 * (XClone_SpawnByType: a placement 40 px left of and 28 px below the claw, type 0: the clone of
 * the player's character; for a pair player 2 when alive, else player 1 when alive) faces that
 * player and starts unhittable (XClone_ActUnhittableScript); if none could be made the wave's
 * kill count goes up instead. The pair drops too (this state, from its next update), and the claw
 * kills itself (slot +$52).
 * Returns self->flags & 8 while waiting; nothing in particular after the drop. */
#include "xmen2.h"

short BroodClaw_StateDropClone(BroodClaw *self)
{
	char go;
	short cx;
	XPlayer *p;
	XClone *clone;
	Placement place;

	go = 0;
	if (self->pair) {
		go = 1;
		p = &g_Player1;
		if (IS_ALIVE(p)) {
			cx = (p->boxLeft + p->boxRight) >> 1;
			if (cx > 0x71E)
				go = 0;
		}
		p = &g_Player2;
		if (IS_ALIVE(p)) {
			cx = (p->boxLeft + p->boxRight) >> 1;
			if (cx > 0x71E)
				go = 0;
		}
		if (go) {
			g_SegTypeHooks[0x15] = 0;
			g_SegTypeHooks[0x1A] = 0;
			self->mode = 0;
			self->vx = 0;
			self->motion = 0;
			self->pair->vx = 0;
			self->pair->motion = 0;
			g_CamShake = 0;
			(*CLASS(Tbl_Camera_vtable)->method[12])(g_Camera, 0x5EA, 0x5A0, 2, 2, 1, 1);	/* +$36 Cam_PanTo */
		}
	} else if (g_CloneMakerKills > 0)
		go = 1;
	if (!go) {
		SET_STATE(self, BroodClaw_StateDropClone);
		return self->flags & 8;
	}
	p = 0;
	Mem_Set(&place, 0, sizeof place);
	place.x = (self->x >> 8) + -0x28;
	place.y = self->py - -0x1C;
	if (self->pair) {
		if (IS_ALIVE(&g_Player2))
			p = &g_Player2;
		else
			p = &g_Player1;
	} else {
		if (IS_ALIVE(&g_Player1))
			p = &g_Player1;
		else
			p = &g_Player2;
	}
	if (clone = XClone_SpawnByType(&place, p)) {
		if (p) {
			if (p->x < clone->x)
				clone->spr.flags |= 0x10;
			else
				clone->spr.flags &= ~0x10;
		}
		XClone_ActUnhittableScript(clone);
	} else
		g_CloneWaveKills++;
	if (self->pair) {
		self->pair->state = (StateFn)BroodClaw_StateDropClone;
		self->pair->timer = 0;
		self->pair = 0;
	}
	(*self->cls->method[19])(self);		/* +$52 Kill */
	NO_RETURN_VALUE
}
