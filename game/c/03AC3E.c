/* RollingDiscs_ClassInit - per-level class setup of ROLLINGDISCS (class slot +$5E; the parent's
 * isn't called): its empty hook (RollingDiscs_NopStatic), then the level's rolling-disc bands:
 * the list at the offset in word +$12 of the BG map blob (g_LevelBgMap) gives their count
 * (g_DiscBandCount) and one entry each; the bands (g_DiscBands) are allocated on the heap
 * (Mem_StackAlloc, count x $14 bytes) and filled from them: position, scroll x << 8, y << 8,
 * speed and its size (the speed limit), acceleration, flag index, and whether its lines scroll.
 * No hurt cooldown.
 *
 * self: passed on to RollingDiscs_NopStatic (the class's index is the second argument, ignored). */
#include "xmen2.h"

void RollingDiscs_ClassInit(RollingDiscs *self)
{
	register DiscEntry *e;
	register s16 *list;
	register long n;
	register DiscBand *band;

	list = (s16 *)((u8 *)g_LevelBgMap + ((u16 *)g_LevelBgMap)[9]);
	e = (DiscEntry *)(list + 1);
	RollingDiscs_NopStatic(self);
	g_DiscBandCount = *list;
	g_DiscBands = (DiscBand *)Mem_StackAlloc(g_DiscBandCount * sizeof(DiscBand), STR(Str_Empty_03AD12, ""));
	for (n = g_DiscBandCount, band = g_DiscBands; n; n--, band++, e++) {
		band->x = e->x;
		band->y = e->y;
		band->scroll = e->x << 8;
		band->yFix = e->y << 8;
		band->speed = e->speed;
		band->maxSpeed = band->speed >= 0 ? e->speed : -e->speed;	/* |speed| */
		band->accel = e->accel;
		band->index = e->index;
		band->scrolls = e->still ? 0 : 1;
	}
	g_DiscHurtCooldown = 0;
}
