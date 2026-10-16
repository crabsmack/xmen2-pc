/* RollingDiscs_ReverseAtY - reverse the rolling-disc bands covering height y: each band whose
 * lines y is strictly inside (top < y < top + 128) gets its acceleration negated, or its speed
 * when it has none.
 *
 * y: map y, pixels (BaniMazaA1_SwitchTileHook: the switch's row * 32). Never runs in the corpus. */
#include "xmen2.h"

void RollingDiscs_ReverseAtY(short y)
{
	long n;
	DiscBand *b;

	for (n = g_DiscBandCount, b = g_DiscBands; n; n--, b++) {
		if (y > b->y && y < b->y + 128) {
			if (b->accel)
				b->accel = -b->accel;
			else
				b->speed = -b->speed;
		}
	}
}
