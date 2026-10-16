/* LittleLeg_Update - LITTLELEG Update (class slot +$0E, run by its Timer every frame): while walking,
 * step the three sprites' animations; then place the leg at its home minus the boss offsets
 * (g_BroodBossOffsetX/Y), as the boss body moves.
 *
 * self: the leg.
 * An animation step lasts the script's delay in frames (counted 1 a frame, 2 in two-player
 * stepping); after the last step it starts again, or a one-shot script (flags bit 1) ends (anim
 * 0). A new frame whose tiles are not the uploaded ones (or none, $FF) clears the sprite's
 * uploaded flag (bit 5) so they are sent again. The script is read as bytes: step i's frame at
 * +7 + 2i (AnimScript header, steps from +6). */
#include "xmen2.h"

#define SCRIPT(s)	((AnimScript *)(u8 *)(s).anim)	/* the sprite's animation header */

/* One frame of a sprite's animation (the same code as ICandy_Update's, three times). */
#define STEP_ANIM(s) \
	if ((s).anim) { \
		(s).time += steps; \
		if ((s).time >= SCRIPT(s)->delay) { \
			(s).step++; \
			if (SCRIPT(s)->count <= (s).step) { \
				(s).step = 0; \
				if (SCRIPT(s)->flags & 2) \
					(s).anim = 0; \
			} \
			if ((s).anim) { \
				(s).spr.frame = *((s).anim + ((s).step << 1) + 7); \
				if ((s).spr.frame == 0xFF || (s).spr.frame != (s).spr.uploaded) \
					(s).spr.flags &= ~0x20; \
				(s).time = 0; \
			} \
		} \
	}

void LittleLeg_Update(LittleLeg *self)
{
	short steps = g_TwoPlayerStep ? 2 : 1;

	if (self->walking != 0) {
		STEP_ANIM(self->part[0])
		STEP_ANIM(self->part[1])
		STEP_ANIM(self->part[2])
	}
	self->px = self->x - (g_BroodBossOffsetX >> 8);
	self->py = self->y - (g_BroodBossOffsetY >> 8);
}
