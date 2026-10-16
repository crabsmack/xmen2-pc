/* Wolverine_StateWallClimb - Wolverine climbs a wall one step at a time (Wolverine_StateWallCling
 * starts each step's animation): at the step's end he clings again; at the wall's top or bottom
 * he stops, or climbs onto the ceiling he touched (Wolverine_WallToCeiling, now).
 *
 * self: the player.
 * Step over (no animation): stop, the cling frame $1B or $20 by clawPhase, playerFlags $20
 * cleared, the claw sound $63, clinging in 14 frames. Blocked (playerFlags $3C000): Up/Down
 * unlatched (pad latch bits 0-1), no animation, the other claw leads, cling from the next frame.
 * Pushed in y (flags $80) or at an end (playerFlags $30000): the segment touched (g_Segs[pushSeg])
 * is classified (ClassifySeg, slot +$F2); Up/Down unlatched, no animation, the other claw leads;
 * a ceiling (1) is climbed onto; else he stops (vy, ay 0) and clings.
 * Returns self->flags & 8; nothing when Wolverine_WallToCeiling runs now. */
#include "xmen2.h"

short Wolverine_StateWallClimb(register Wolverine *self)
{
	register Seg *seg;
	register short kind;

	if (self->anim) {
		if (self->playerFlags & 0x3C000) {
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~3;
			else
				g_Pad2Latch = g_Pad2Latch & ~3;
			self->anim = 0;
			self->clawPhase = !self->clawPhase ? 1 : 0;
			SET_STATE(self, (StateFn)Wolverine_StateWallCling);
			return self->flags & 8;
		}
		if (self->flags & 0x80 || self->playerFlags & 0x30000) {
			seg = &g_Segs[self->pushSeg];
			kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
			if (!self->pad)
				g_Pad1Latch = g_Pad1Latch & ~3;
			else
				g_Pad2Latch = g_Pad2Latch & ~3;
			self->anim = 0;
			self->clawPhase = !self->clawPhase ? 1 : 0;
			if (kind == 1) {
				GOTO_STATE(self, (StateFn)Wolverine_WallToCeiling);
				RETURN_NO_VALUE;
			}
			self->vy = 0;
			self->ay = 0;
			SET_STATE(self, (StateFn)Wolverine_StateWallCling);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Wolverine_StateWallClimb);
		return self->flags & 8;
	}
	STOP_MOVING(self);
	if (!self->clawPhase)
		(*self->cls->method[20])(self, 0x1B);		/* SetFrame */
	else
		(*self->cls->method[20])(self, 0x20);
	self->playerFlags &= ~0x20;
	Snd_Stop(0x63);
	Snd_Play(0x63);
	SET_STATE_WAIT(self, (StateFn)Wolverine_StateWallCling, 0xE);
	return self->flags & 8;
}
