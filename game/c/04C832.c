/* Apocalypse_Draw - APOCALYPSE Draw (class slot +$1A; the parent's isn't called, so no hit
 * blink): the second sprite takes the body's h-flip, then each of the body (+$2E) and the second
 * sprite (+$92) is drawn at its position on screen unless its frame is $FF; a
 * distance-driven animation draws both animDist behind, as ACTOR Draw does.
 *
 * self: the boss.
 * The original had a register variable used only in dead code (an if (0) block): MPW C still
 * saves d7 for it, and the block makes it reload spr from the stack, as the ROM does. */
#include "xmen2.h"

#define DRAW_X(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x20 \
			? (((o)->x - (o)->animDist) >> 8) - g_CameraX : ((o)->x >> 8) - g_CameraX)
#define DRAW_Y(o)	((o)->anim && (*(u8 *)(o)->anim & 0x30) == 0x30 \
			? (((o)->y - (o)->animDist) >> 8) - g_CameraY : ((o)->y >> 8) - g_CameraY)

void Apocalypse_Draw(register Apocalypse *self)
{
	register SprInst *spr2 = self->spr2;
	SprInst *spr = &self->spr;
	register short unused;

	if (0)
		unused = 0;
	spr2->flags = spr->flags & 0x10 | spr2->flags & ~0x10;
	if (spr->frame != 0xFF)
		Spr_Draw(spr, DRAW_X(self), DRAW_Y(self));
	if (spr2->frame != 0xFF)
		Spr_Draw(spr2, DRAW_X(self), DRAW_Y(self));
}
