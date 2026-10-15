/* Magneto_HoverDamp - MAGNETO hovering: brake both speeds, bob up and down when nearly still,
 * and optionally turn to the held direction.
 *
 * self:     the player.
 * stopSlow: below speed $40 in x, stop dead in x (else leave the slow speed alone).
 * face:     Left held faces left, else Right held faces right.
 * From |speed| $40 the acceleration is -speed/8 (x and y); below it in y the acceleration is
 * 1 when rising faster than $30, -1 when falling faster than $30 (else unchanged). */
#include "xmen2.h"

void Magneto_HoverDamp(Magneto *self, char stopSlow, char face)
{
	register short vx = self->vx;
	register short vy = self->vy;

	if (ABS(vx) < 0x40) {
		if (stopSlow) {
			self->ax = 0;
			self->vx = 0;
		}
	} else
		self->ax = -(self->vx >> 3);
	if (ABS(vy) < 0x40) {
		if (vy < -0x30)
			self->ay = 1;
		else if (vy > 0x30)
			self->ay = -1;
	} else
		self->ay = -(self->vy >> 3);
	if (face) {
		if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
			self->spr.flags |= 0x10;
		else if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
			self->spr.flags &= ~0x10;
	}
}
