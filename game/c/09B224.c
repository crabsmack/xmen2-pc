/* BroodClaw_Swing - BROODCLAW motion function (+$92, run by BroodClaw_Move): swing the claw out and
 * back along swingOffset by its speed vx (the whole pixels: vx & ~$FF), rising on a sine curve
 * as it goes out; while it swings back it pushes the boss (g_BroodBossOffsetX, mode 0) or
 * the scroll (g_SceneVar3B02 += vx >> 8, mode 1).
 *
 * self: the claw.
 * Out: at 0 or at swingLen << 8 it stops there, turns back (SWING_BACK 1), the boss is lowered
 * (g_BroodBossFlags $10) and the camera shakes (Tbl_BroodClawQuake). Back: at either end it turns
 * out again. A claw with a pair sets the boss arm frame by the offset (g_BroodArmFrame). Its x is
 * its home px plus the offset less the boss offset; its y is its home py raised by dy, which is
 * taken off the 8.8 y and so is in 1/256 px: going out, dy = sin(offset px / 2 degrees) x 32 px
 * (Math_SinDeg is 16.16, >> 3; about 13 px at the $32 swing length) + 4, and 0 on reaching an
 * end; going back, 0 (home height), and 4 (4/256 px up) on reaching an end. Once its scroll is
 * over (scrollEnded) and, in mode 1, the next step would take the scroll past g_BroodArenaLeft:
 * mode 0, and the camera follows normally again (Cam_DefaultUpdate, BroodClaw_CamApplyShake, no
 * shake or post hook, at the current scroll, XClone_CamSetNormal), g_BroodBossFlags $80, every
 * LINESEGKILLER removed.
 * The claw's y is read into a local never used, and the test for the ends going out also takes a
 * stop flag that is never set. */
#include "xmen2.h"

#define g_BroodScrollStep	(*(s16 *)g_SceneVar3B02)	/* scene variable: scroll x step of the boss scroll (CloneA5) */
#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */
#define g_BroodArmFrame		(*(s16 *)g_SceneVar3B10)	/* scene variable: frame of the boss arm */
#define g_BroodArenaLeft	(*(s32 *)g_SceneVar3B20)	/* scene variable: scroll x the claw swings the arena to */
#define SWING_BACK(o)		((o)->classParam)	/* ACTOR +$8B, for a claw: 0 swinging out, 1 swinging back (other classes use the byte differently) */

void BroodClaw_Swing(BroodClaw *self)
{
	register long y0 = self->y;
	register char shake;
	char stop;
	register short dy;

	shake = 0;
	stop = 0;
	dy = 0;
	if (!SWING_BACK(self)) {		/* swinging out */
		dy = 4;
		self->swingOffset += self->vx & ~0xFF;
		if (self->pair) {
			if (self->swingOffset > 0x2400)
				g_BroodArmFrame = 4;
			else if (self->swingOffset > 0x1800)
				g_BroodArmFrame = 0;
			else if (self->swingOffset > 0xA00)
				g_BroodArmFrame = 1;
			else
				g_BroodArmFrame = 2;
		}
		{
			register short angle = (self->swingOffset >> 8) >> 1;

			dy += Math_SinDeg(angle) >> 3;
		}
		if (self->swingOffset <= 0 || self->swingLen << 8 <= self->swingOffset || stop) {
			SWING_BACK(self) = 1;
			g_BroodBossFlags |= 0x10;
			shake = 1;
			dy = 0;
			if (self->swingOffset <= 0)
				self->swingOffset = 0;
			else
				self->swingOffset = self->swingLen << 8;
		}
	} else {
		dy = 0;
		self->swingOffset -= self->vx & ~0xFF;
		if (self->mode)
			g_BroodScrollStep += self->vx >> 8;
		else
			g_BroodBossOffsetX -= self->vx & ~0xFF;
		if (self->pair) {
			if (self->swingOffset < 0xD00)
				g_BroodArmFrame = 2;
			else if (self->swingOffset < 0x2200)
				g_BroodArmFrame = 3;
			else
				g_BroodArmFrame = 4;
		}
		if (self->swingOffset <= 0 || self->swingLen << 8 <= self->swingOffset) {
			SWING_BACK(self) = 0;
			dy = 4;
			if (self->swingOffset <= 0)
				self->swingOffset = 0;
			else
				self->swingOffset = self->swingLen << 8;
		}
	}
	self->x = (self->px << 8) + self->swingOffset - g_BroodBossOffsetX;
	self->y = (self->py << 8) - dy;
	if (shake)
		g_CamShake = CODEPTR(Tbl_BroodClawQuake);
	if (self->scrollEnded && g_FgScrollX + (self->vx >> 8) < g_BroodArenaLeft && self->mode) {
		self->mode = 0;
		g_Camera->update = Cam_DefaultUpdate;
		g_Camera->apply = BroodClaw_CamApplyShake;
		g_Camera->shake = 0;
		g_Camera->postHook = 0;
		g_Camera->x = g_CameraX;
		g_Camera->y = g_CameraY;
		g_CamModeSwitched = 1;
		g_Camera->offsetX = 0;
		g_Camera->offsetY = 0;
		g_Camera->look = 0;
		XClone_CamSetNormal(g_Camera);
		g_BroodBossFlags |= 0x80;
		LineSegKiller_KillAll();
	}
}
