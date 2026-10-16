/* BroodClaw_StateWaitMusic - BROODCLAW state after the boss: keep the claw parked $1A2 px right of
 * the camera (its pair $62 px right) until the sound driver's byte $13 is set (music cue,
 * Snd_ReadDriverByte), then come into view (BroodClaw_StateEnterView) from the next update.
 *
 * self: the claw.
 * Returns self->flags & 8 when it goes on; nothing in particular while waiting. The original read
 * the byte as a word (its d0 is zero-extended); muted (g_SndMute) the asm returns d0 as the call
 * left it, $13, so the claw goes on. */
#include "xmen2.h"

short BroodClaw_StateWaitMusic(BroodClaw *self)
{
	self->x = (g_CameraX + 0x1A2) << 8;
	if (self->pair)
		self->pair->x = (g_CameraX + 0x62) << 8;
#ifdef XMEN2_MPW
	if (((short (*)())Snd_ReadDriverByte)(0x13)) {
#else
	if (Snd_ReadDriverByte(0x13) || g_SndMute) {
#endif
		SET_STATE(self, BroodClaw_StateEnterView);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
