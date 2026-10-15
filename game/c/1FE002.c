/* VDP_WriteWords - copy n words from memory to the VDP data port (address already set).
 *
 * src: the words.  n: how many; a dbra counts it, so only its low word matters.
 * Hand-written 68000; callers set the VDP address first (Level_LoadMaps, Spr_PreloadTiles,
 * Spr_UploadFrame, Art_LoadToVramRecolor). */
#include "xmen2.h"

void VDP_WriteWords(void *src, long n)
{
	s16 *p = (s16 *)src;
	unsigned short k = (unsigned short)n;

	while (k) {
		VDP_DATA = *p++;
		k--;
	}
}
