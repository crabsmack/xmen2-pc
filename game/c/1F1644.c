/* XPlayer_Update - XPLAYER class slot +$4A, the player's frame: move, save the safe spot, animate,
 * count the timers down, run the state, end a camera look, the after-state hook, boxes, the
 * special meter, and the map tile hooks at the frame's hit points.
 *
 * self: the player.
 * Everything steps by 1, or 2 in two-player mode (the logic runs at half rate). flags 8 is set
 * when the animation stepped or the player moved. On the ground a riding player (flags $800)
 * or one on a moving segment (segment flags 4) follows it (Seg_ApplyObjPos); one not riding
 * is detached. The state runs when the delay (timer) is out ($FF and above: never counted
 * down); a player still present after it keeps to its partner (XPlayer_TetherToPartner).
 * Looking down (or up, playerFlags $40000, not in a crouching attack) ends when Down (Up) is
 * let go, the player moves or leaves the ground: unlatch it and the camera's EndLook.
 * The tile hooks (when on: g_TileHooksOn) run at the hit points of the frame shown; each hook
 * that fires halves the attack strength (playerFlags $20: once per attack). */
#include "xmen2.h"

void XPlayer_Update(XPlayer *self)
{
	short step;
	long x0, y0;
	short frame;
	char *points;

	if (g_TwoPlayerStep)
		step = 2;
	else
		step = 1;
	self->flags &= ~8;
	x0 = self->x;
	y0 = self->y;
#ifdef XMEN2_MPW
	if (!(self->flags & 0x1000) && (self->flags & 0x800 || self->seg->flags & 4))
#else
	/* a new player is on the ground with no segment: the 68000 reads ROM $000000 ($00) */
	if (!(self->flags & 0x1000) && (self->flags & 0x800 || (self->seg ? self->seg->flags : 0) & 4))
#endif
		Seg_ApplyObjPos(self);
	if (!(self->flags & 0x800) && self->spr.parent != 0xFF)
		Obj_Detach(self);
	XPlayer_Move(self, step);
	(*self->cls->method[39])(self);			/* SaveSafePos */
	if ((char)Anim_AdvanceByDistance(self, step, self->x - x0, self->y - y0))
		self->flags |= 8;
	if (self->countdown > 0) {
		self->countdown -= step;
		if (self->countdown < 0)
			self->countdown = 0;
	}
	if (self->stateTimer > 0) {
		self->stateTimer -= step;
		if (self->stateTimer < 0)
			self->stateTimer = 0;
	}
	if (self->actionTimer > 0) {
		self->actionTimer -= step;
		if (self->actionTimer < 0)
			self->actionTimer = 0;
	}
	if (self->ledgeTimer > 0) {
		self->ledgeTimer -= step;
		if (self->ledgeTimer < 0)
			self->ledgeTimer = 0;
	}
	if (self->hitCooldown <= 0)
		self->flags6 &= ~0x40;
	if (self->timer > 0) {
		if (self->timer < 0xFF) {
			self->timer -= step;
			if (self->timer < 0)
				self->timer = 0;
		}
	} else {
		(*self->state)(self);
		if (!(self->flags & 0x8000))
			return;
		XPlayer_TetherToPartner(self);
	}
	if (self->playerFlags & 0x40000 && !(self->playerFlags & 0x200000)) {
		if (g_Camera->camFlags & 8
		    && (!(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
			|| self->vx || self->flags & 0x1000)) {
			self->playerFlags &= ~0x40000;
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~g_PadMaskDown;
			else
				g_Pad2Latch = g_Pad2Latch & ~g_PadMaskDown;
			(*g_Camera->cls->method[11])(g_Camera);		/* EndLookDown */
		} else if (g_Camera->camFlags & 4
			   && (!(!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp)
			       || self->vx || self->flags & 0x1000)) {
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~g_PadMaskUp;
			else
				g_Pad2Latch = g_Pad2Latch & ~g_PadMaskUp;
			self->playerFlags &= ~0x40000;
			(*g_Camera->cls->method[10])(g_Camera);		/* EndLookUp */
		}
	}
	(*self->cls->method[27])(self);			/* AfterState */
	if (self->x != x0 || self->y != y0)
		self->flags |= 8;
	if (self->flags & 8)
		Obj_UpdateBoxes(self);
	if (self->hitCooldown > 0)
		self->hitCooldown--;
	(*self->cls->method[28])(self);			/* UpdateCharge */
	frame = self->spr.frame;
	{
		SprFrame *rec = SPR_FRAME(self->spr.def, frame);

		if (frame == 0xFF || !rec->hitPoints)
			points = 0;
		else
			points = (char *)self->spr.def + rec->hitPoints;
	}
	if (points && g_TileHooksOn && !(self->playerFlags & 0x20))
		while (self->attack > 0 && Tile_RunHooksAtPoints(self, points, self->x >> 8, self->y >> 8)) {
			self->attack >>= 1;
			self->playerFlags |= 0x20;
		}
}
