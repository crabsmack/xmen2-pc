/* SwampBubbles_Draw - SWAMPBUBBLES Draw (class slot +$1A): draw its sprite once per bubble
 * (count 1-5), side by side around its position on screen.
 *
 * self: the bubbles.
 * 1: at x; 2: x -/+ 16; 3: x, x -/+ 32; 4: x -/+ 16, x -/+ 48; 5: x, x -/+ 64 (the 5 cases from
 * SwampBubbles_Draw_Multi in the disassembly). Any other count draws nothing; the common exit is
 * SwampBubbles_Draw_Done. Never runs in the corpus.
 * The ROM saves d6/d7 and reserves 8 bytes of stack that nothing uses: what the original kept
 * there was compiled out; the dead block below (two register variables both read and written,
 * two locals with their address taken) reproduces that frame. */
#include "xmen2.h"

void SwampBubbles_Draw(SwampBubbles *self)
{
	register long u1, u2;
	long sx, sy;			/* the position on screen */
	long s1, s2;

	if (0) {
		u1 = sx;
		u2 = sy;
		sx = u2;
		sy = u1;
		(void)&s1;
		(void)&s2;
	}
	sx = (self->x >> 8) - g_CameraX;
	sy = (self->y >> 8) - g_CameraY;
	switch (self->count) {
	case 1:		/* SwampBubbles_Draw_Multi */
		Spr_Draw(&self->spr, sx, sy);
		break;
	case 2:
		Spr_Draw(&self->spr, sx - 0x10, sy);
		Spr_Draw(&self->spr, sx + 0x10, sy);
		break;
	case 3:
		Spr_Draw(&self->spr, sx, sy);
		Spr_Draw(&self->spr, sx - 0x20, sy);
		Spr_Draw(&self->spr, sx + 0x20, sy);
		break;
	case 4:
		Spr_Draw(&self->spr, sx - 0x10, sy);
		Spr_Draw(&self->spr, sx + 0x10, sy);
		Spr_Draw(&self->spr, sx - 0x30, sy);
		Spr_Draw(&self->spr, sx + 0x30, sy);
		break;
	case 5:
		Spr_Draw(&self->spr, sx, sy);
		Spr_Draw(&self->spr, sx - 0x40, sy);
		Spr_Draw(&self->spr, sx + 0x40, sy);
		break;
	}
	/* SwampBubbles_Draw_Done */
}
