/* AvalonB1_InitIntroPalette - the palettes of the AvalonB1 intro (AvalonB1_CamHookWait2): line 0
 * mostly black (colours 7-9 $200 $220 $0E0, 14-15 $020 $028), line 2 the 16 colours of the
 * background map (Map_AvalonB1Bg); uploaded at the next V-blank. */
#include "xmen2.h"

/* The 16 colours of a map blob (MapHeader.palette is their offset from the header). MPW: the
   original's long arithmetic on the blob address. */
#ifdef XMEN2_MPW
#define MAP_PALETTE(m)	((s16 *)((long)(m) + *(u16 *)((long)(m) + 0x10)))
#else
#define MAP_PALETTE(m)	((s16 *)((char *)(m) + ((MapHeader *)(m))->palette))
#endif

void AvalonB1_InitIntroPalette(void)
{
	void *map;
	register s16 *p;
	register s16 *src;
	register short i;

	p = g_PalBase;
	map = CODEPTR(Map_AvalonB1Bg);
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0x200;
	*p++ = 0x220;
	*p++ = 0xE0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0x20;
	*p++ = 0x28;
	src = MAP_PALETTE(map);
	p = &g_PalBase[32];
	for (i = 0; i < 16; i++)
		*p++ = *src++;
	g_PalBaseDirty = 1;
}
