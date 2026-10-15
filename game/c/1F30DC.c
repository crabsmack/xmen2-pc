/* XPlayer_Land - XPLAYER class slot +$9A: the player lands: the scene's slow fall ends, the normal
 * maximum fall speed, on the ground (playerFlags $200; 2, 4 and $800000 cleared), a landing
 * sound, and a direction still held is latched again so walking goes on.
 *
 * self: the player.
 * The sound: landSound ($FF: none) when it fell faster than the jump speed less $250 (prevVy,
 * the speed before this frame's move), else lightLandSound. Tells the camera.
 * Returns 1 when |vx| is below the character's stop speed, else 0. */
#include "xmen2.h"

char XPlayer_Land(XPlayer *self)
{
	g_FloatFallSpeed = 0;
	self->maxVy = self->charRec->maxFallVy;
	self->playerFlags |= 0x200;
	self->playerFlags &= ~0x800006;
	if (self->prevVy + 0x250 > -self->charRec->jumpVy) {
		if (self->charRec->landSound != 0xFF) {
			Snd_Stop(self->charRec->landSound);
			Snd_Play(self->charRec->landSound);
		}
	} else {
		Snd_Stop(self->charRec->lightLandSound);
		Snd_Play(self->charRec->lightLandSound);
	}
	g_CamModeSwitched = 1;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
		if (!self->pad)
			g_Pad1Latch |= g_PadMaskLeftRight;
		else
			g_Pad2Latch |= g_PadMaskLeftRight;
	}
	if (ABS(self->vx) < self->charRec->stopSpeed)
		return 1;
	return 0;
}
