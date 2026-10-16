/* Nightcrawler_StateWallCling - NIGHTCRAWLER holding on to a wall (climbSeg): C leaps off, a
 * ceiling touched is climbed onto, Up/Down climb, B slashes.
 *
 * self: the player.
 * C latched: upright again (upside down, the box is mirrored in y), no animation, the jump frame
 * $21, vy -$834 (0 with playerFlags $10000), the air speed and gravity, no climb state, flags6
 * $2000 off, then the control state (slot +$A6) from the next update.
 * Pushed in y (flags $80) by a segment (g_Segs[pushSeg], ClassifySeg slot +$F2) that is a ceiling
 * (1): that becomes climbSeg and the climb state, stop, the ceiling frame $8C turned towards the
 * wall with the box's wall side kept in place, the box's top on the ceiling, and in 7 frames
 * Nightcrawler_WallToCeiling (tells the camera). Pushed by anything else: stop and cling.
 * Up held below the wall's top (Player_TopAboveSeg -5) without playerFlags $10000: climb up at
 * $2BC; upside down he first turns upright about the box's centre (Tbl_Anim_1A5FE2) and climbs
 * (Nightcrawler_StateWallClimb). Down the same, below: climb down, turning upside down; (bottom:
 * Player_BottomBelowSeg 5, playerFlags $20000). B latched (unlatched): the slash
 * (Tbl_Anim_1A6008, ground-B strength [4], attack sound), stopped, Nightcrawler_WallAttackEnd at
 * its end. Else stop and cling. The ROM has the last "cling" once more after the if/else chain
 * (never reached: Nightcrawler_StateWallCling_Ret, the shared epilogue, follows it).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_StateWallCling(register Nightcrawler *self)
{
	if (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC) {
		if (self->spr.flags & 8) {
			register short dy = self->boxTop - (self->y >> 8);

			self->y = (dy + self->boxBottom) << 8;
		}
		self->anim = 0;
		self->spr.flags &= ~8;
		(*self->cls->method[20])(self, 0x21);		/* SetFrame */
		self->vy = !(self->playerFlags & 0x10000) ? -0x834 : 0;
		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		self->climbFlags = self->climbFlags & ~3 | 0;
		self->flags6 &= ~0x2000;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	} else if (self->flags & 0x80) {
		register short px = self->x >> 8;
		register Seg *seg = self->climbSeg;	/* dead: overwritten below (the original's, kept) */
		register short kind;

		seg = &g_Segs[self->pushSeg];
		kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
		self->anim = 0;
		if (kind == 1) {
			register short edge;

			self->climbSeg = seg;
			self->climbFlags = self->climbFlags & ~3 | kind;
			self->flags6 &= ~0x2000;
			STOP_MOVING(self);
			g_CamModeSwitched = 1;
			if (self->spr.flags & 0x10) {
				edge = self->boxLeft;
				(*self->cls->method[20])(self, 0x8C);	/* SetFrame */
				self->spr.flags &= ~0x10;
				Obj_UpdateBox(self);
				self->x = (edge + px - self->boxLeft) << 8;
			} else {
				edge = self->boxRight;
				(*self->cls->method[20])(self, 0x8C);	/* SetFrame */
				self->spr.flags |= 0x10;
				Obj_UpdateBox(self);
				self->x = (edge + px - self->boxRight) << 8;
			}
			self->y = (seg->y2 + (self->y >> 8) - self->boxTop) << 8;
			SET_STATE_WAIT(self, Nightcrawler_WallToCeiling, 7);
			return self->flags & 8;
		}
		self->vy = 0;
		self->ay = 0;
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	} else if ((!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp)
		   && !Player_TopAboveSeg(self, -5, self->climbSeg)
		   && !(self->playerFlags & 0x10000)) {
		self->vy = -0x2BC;
		if (self->spr.flags & 8) {
			register short mid = (self->boxTop + self->boxBottom) >> 1;
			register short dy = mid - (self->y >> 8);

			self->spr.flags &= ~8;
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FE2));
			self->y = (mid + dy) << 8;
			SET_STATE(self, Nightcrawler_StateWallClimb);
			return self->flags & 8;
		}
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	} else if ((!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
		   && !Player_BottomBelowSeg(self, 5, self->climbSeg)
		   && !(self->playerFlags & 0x20000)) {
		self->vy = 0x2BC;
		if (!(self->spr.flags & 8)) {
			register short mid = (self->boxTop + self->boxBottom) >> 1;
			register short dy = mid - (self->y >> 8);

			self->spr.flags |= 8;
			Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FE2));
			self->y = (mid + dy) << 8;
			SET_STATE(self, Nightcrawler_StateWallClimb);
			return self->flags & 8;
		}
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	} else if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->attack = self->charRec->strength[4];
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A6008));
		Snd_Stop(self->charRec->attackSound);
		Snd_Play(self->charRec->attackSound);
		self->vy = 0;
		self->ay = 0;
		SET_STATE_WAIT(self, Nightcrawler_WallAttackEnd, 0xFF);
		return self->flags & 8;
	} else {
		self->vy = 0;
		self->ay = 0;
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateWallCling);	/* never reached */
	return self->flags & 8;
}
