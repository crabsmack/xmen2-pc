/* XClimber_CheckGrab - XCLIMBER CheckGrab (class slot +$96): in the air, should the player grab the
 * wall or ceiling segment just touched? Yes means the climbing state (slot +$AA) follows.
 *
 * self: the player.
 * On the ground: no. The climb state (climbFlags & 3) is cleared. B pressed keeps it from
 * grabbing for 20 frames (unk_A8 $14) and drops a queued wall jump (climbFlags $40); else, for a
 * character that can (climbFlags $100), C pressed queues one (unk_A8 10). Touching a segment
 * (flags $100 or $80): the last one touched (g_Segs[unk_7B]) is classified (ClassifySeg, slot
 * +$F2) into climbFlags bits 2-3, and a wall (2, 3), or a ceiling (1) with ceilings allowed
 * (climbFlags $80) and Up held, starts the grab window (stateTimer 20) with the segment held
 * (climbSeg); anything else closes it (XClimber_CheckGrab_Case0, XClimber_CheckGrab_Default).
 * With the window and unk_A8 both running it grabs unless playerFlags $10000 / $20000 limit it to
 * below / above the safe spot, a wall's ends are within 10 px of the player's top or bottom
 * (Player_TopAboveSeg / Player_BottomBelowSeg) or it falls at $1050 or faster, a ceiling's end
 * is within 10 px of its front (Player_FrontPastSeg). Grabbing sets the climb state from bits
 * 2-3. Returns 1 to grab, else 0. */
#include "xmen2.h"

char XClimber_CheckGrab(register XClimber *self)
{
	register short kind = 0;
	register Seg *seg;
	register long limits;
	register short y;

	if (!(self->flags & 0x1000))
		return 0;
	self->climbFlags = self->climbFlags & ~3 | 0;
	if (!self->pad ? g_Pad1Pressed & g_PadMaskB : g_Pad2Pressed & g_PadMaskB) {
		self->unk_A8 = 0x14;
		self->climbFlags &= ~0x40;
	} else if (self->climbFlags & 0x100) {
		if (!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC) {
			self->unk_A8 = 0xA;
			self->climbFlags |= 0x40;
		}
	}
	if (self->flags & 0x100 || self->flags & 0x80) {
		seg = &g_Segs[self->unk_7B];
		kind = (*self->cls->method[59])(self, seg);		/* ClassifySeg */
		self->climbFlags = self->climbFlags & ~0xC | kind << 2;
		switch (kind) {
		case 0:						/* XClimber_CheckGrab_Case0 */
			self->stateTimer = 0;
			break;
		case 2:
		case 3:
			self->stateTimer = 0x14;
			self->climbSeg = seg;
			break;
		case 1:
			if (self->climbFlags & 0x80
			    && (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp)) {
				self->stateTimer = 0x14;
				self->climbSeg = seg;
			} else
				self->stateTimer = 0;
			break;
		default:					/* XClimber_CheckGrab_Default */
			self->stateTimer = 0;
		}
	}
	if (self->stateTimer > 0 && self->unk_A8 > 0) {
		kind = (self->climbFlags & 0xC) >> 2;
		limits = self->playerFlags & 0x3C000;
		y = self->y >> 8;
		if (limits & 0x10000 && y < self->safeY)
			return 0;
		if (limits & 0x20000 && y > self->safeY)
			return 0;
		if (kind == 2 || kind == 3) {
			seg = self->climbSeg;
			if (!Player_TopAboveSeg(self, 10, seg) && !Player_BottomBelowSeg(self, 10, seg)
			    && ABS(self->vy) < 0x1050) {
				self->climbFlags = self->climbFlags & ~3 | kind;
				return 1;
			}
			return 0;
		}
		if (!Player_FrontPastSeg(self, 10, self->climbSeg)) {
			self->climbFlags = self->climbFlags & ~3 | kind;
			return 1;
		}
		return 0;
	}
	return 0;
}
