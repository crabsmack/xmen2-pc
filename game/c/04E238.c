/* ApocBomb_Update - APOCBOMB Update (class slot +$4A; the parent's isn't called and no state
 * runs): move (2 steps in two-player step mode, else 1); expired, it bursts (one live bomb less,
 * ExplodeFx_BurstSmall). Otherwise its look follows the orbit angle (its depth: 0 is nearest the
 * viewer, 180 the far side; ApocBomb_Move): frame 0 (near) to 4 (far), draw layer $10-$C0 and
 * shadow frame 10-14. The priority bit is cleared at 60-85 degrees and set at 275-300, and kept
 * in between, so as the angle advances it is low over the far side and high over the near side
 * (about 275 through 0 to 60). The hittable bomb (type $10) uses frames 5-9 and is hittable (no
 * hit cooldown) only at frame 5, the near side of the orbit (under 60 or from 300 degrees). It
 * is h-flipped (spr.flags $10) on the left half (angle < 180, where x < 256). A new frame goes
 * through SetFrame, and any change of frame, layer or priority re-sorts it
 * (DrawList_SetPriority); then its boxes (Obj_UpdateBoxes).
 *
 * self: the bomb. */
#include "xmen2.h"

#define g_ApocBombCount	(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */

void ApocBomb_Update(register ApocBomb *self)
{
	register SprInst *spr = &self->spr;
	register short frame = spr->frame;
	short steps;
	short oldFrame = frame;
	register char prio = spr->flags & 4;
	char oldPrio = prio;
	register unsigned char layer = self->layer;
	unsigned char oldLayer = layer;
	register short angle = self->angle >> 8;
	char changed = 0;

	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	(*self->cls->method[18])(self, steps);		/* +$4E Move */
	if (self->bombFlags & 4) {
		g_ApocBombCount--;
		ExplodeFx_BurstSmall(self);
		return;
	}
	if (angle < 60) {
		frame = 0;
		layer = 0x20;
	} else if (angle < 85) {
		frame = 1;
		layer = 0x40;
		prio = 0;
	} else if (angle < 105) {
		frame = 2;
		layer = 0x60;
	} else if (angle < 125) {
		frame = 3;
		layer = 0x80;
	} else if (angle < 180) {
		frame = 4;
		layer = 0xC0;
	} else if (angle < 235) {
		frame = 4;
		layer = 0xA0;
	} else if (angle < 255) {
		frame = 3;
		layer = 0x80;
	} else if (angle < 275) {
		frame = 2;
		layer = 0x60;
	} else if (angle < 300) {
		frame = 1;
		layer = 0x40;
		prio = 4;
	} else {
		frame = 0;
		layer = 0x10;
	}
	self->shadowFrame = frame + 10;
	if (self->type & 0x10) {
		frame += 5;
		if (frame == 5)
			self->hitCooldown = 0;
		else
			self->hitCooldown = 30;
	}
	if (angle < 180)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	if (frame != oldFrame) {
		(*self->cls->method[20])(self, frame);		/* +$56 SetFrame */
		changed = 1;
	}
	if (layer != oldLayer) {
		self->layer = layer;
		changed = 1;
	}
	if (prio != oldPrio) {
		if (prio)
			spr->flags |= 4;
		else
			spr->flags &= ~4;
		changed = 1;
	}
	if (changed)
		DrawList_SetPriority(self->drawNode, self->spr.flags & 4, self->layer, self->flags6 & 8);
	Obj_UpdateBoxes(self);
}
