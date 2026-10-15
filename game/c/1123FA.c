/* Deathbird_Draw - DEATHBIRD Draw (class slot +$1A; the parent's isn't called): blinking (flags6
 * $40) only on odd g_LevelFrame; the second sprite when it is shown with a frame, else the body,
 * at the position on screen (animDist behind for a distance-driven animation, as ACTOR Draw).
 *
 * self: the boss.
 * The original had a register variable used only in dead code (an if (0) block): MPW C still
 * saves d7 for it (as in Apocalypse_Draw). */
#include "xmen2.h"

#define DRAW_X(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x20 \
			? (((o)->x - (o)->animDist) >> 8) - g_CameraX : ((o)->x >> 8) - g_CameraX)
#define DRAW_Y(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x30 \
			? (((o)->y - (o)->animDist) >> 8) - g_CameraY : ((o)->y >> 8) - g_CameraY)

void Deathbird_Draw(register Deathbird *self)
{
	register short unused;

	if (0)
		unused = 0;
	if (self->flags6 & 0x40 && (g_LevelFrame & 1) == 0)
		return;
	if (self->spr2Shown && self->spr2->frame != 0xFF)
		Spr_Draw(self->spr2, DRAW_X(self), DRAW_Y(self));
	else
		Spr_Draw(&self->spr, DRAW_X(self), DRAW_Y(self));
}
