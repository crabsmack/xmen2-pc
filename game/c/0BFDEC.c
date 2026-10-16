/* SwampPlatform_Move - SWAMPPLATFORM Move (class slot +$4E; no velocity, the parent's isn't
 * called): bob up and down with the frame counter.
 *
 * self: the platform.
 * y = py minus a triangle wave of the frame counter (period 256 frames from phase, peak $80 px,
 * halved shift times), but never above the level's bottom minus $1A px (the FG map's height
 * in 8-px cells, g_LevelFgMap): an unsigned compare, as the ROM's. */
#include "xmen2.h"

/* The lowest y it may take (8 fraction bits), unsigned: MPW C promotes the u16 height to
   unsigned (the ROM compares with bls). */
#define FLOOR_Y		((((unsigned long)((MapHeader *)g_LevelFgMap)->height << 3) - 0x1A) << 8)

void SwampPlatform_Move(register SwampPlatform *self)
{
	register long d = (g_FrameCounter - self->phase) & 0xFF;

	if (d > 0x80)
		d = 0x100 - d;
	d >>= self->shift;
	self->y = (self->py - d) << 8;
	if (FLOOR_Y > self->y)
		self->y = FLOOR_Y;
}
