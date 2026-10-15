/* Deathbird_StateFalling - dead DEATHBIRD falling (SetMoveMode 1): every 2 frames palette line 1
 * gets 32 bytes of ROM from the address g_FrameCounter (a flicker) and, on frames with
 * g_FrameCounter bits 0 and 1 set, an explosion goes off at a random spot up to 255 px right
 * and down of 13 px left, 28 px above it; once it is 80 px below the screen, hide it, end the
 * scene in 240 frames, send the lift up (Elevator_Toggle when going down, speed $78) with
 * acceleration $78, and flicker on (Deathbird_StateDeadFlicker).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateFalling(Deathbird *self)
{
	long x, y;

	if ((self->y >> 8) - 80 > g_CameraY + 0xE0) {
		self->anim = 0;
		(*self->cls->method[20])(self, 0xFF);	/* SetFrame: hide */
		Scene_EndAfter(0xF0);
		if (!g_Elevator->up) {
			(*g_Elevator->cls->method[3])(g_Elevator);	/* Elevator_Toggle */
			g_Elevator->speed = 0x78;
		}
		g_Elevator->accel = 0x78;
		SET_STATE(self, (StateFn)Deathbird_StateDeadFlicker);
		return self->flags & 8;
	}
	Mem_Copy(&g_PalBase[0x20], AT68(void, g_FrameCounter), 0x20);
	g_PalBaseDirty = 1;
	if (g_FrameCounter & 2 && g_FrameCounter & 1) {
		x = self->x + -0xD00 + (unsigned short)Rand_Next();
		y = self->y + -0x1C00 + (unsigned short)Rand_Next();
		Explosion_Spawn(x, y, 1);
	}
	SET_STATE_WAIT(self, (StateFn)Deathbird_StateFalling, 2);
	return self->flags & 8;
}
