/* AvalonB1_InitCamera - the AvalonB1 camera with its intro: palette line 0 saved (for
 * AvalonB1_PalFadeStep / AvalonB1_RestorePalette), a new CAMERA (g_Camera) running the intro hook
 * chain from AvalonB1_CamHookWait2, camera y $430 (plane B scroll y $E0), palette lines 0 and 2
 * black, then the map rows redrawn there with the wide streaming margin (interrupts off).
 * MPW: one byte off (the first loop's g_PalBase index is copied with move.l, the ROM's with
 * move.w); verified by shadow tests. */
#include "xmen2.h"

#define g_AvalonB1Timer		(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (AvalonB1_FrameHook counts) */
#define g_AvalonB1FadeRow	(*(s16 *)g_SceneVar3B3A)	/* scene variable: fade-in step (row of Tbl_AvalonB1FadeInLevels) */
#define g_AvalonB1SavedPal	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: line 0 saved here (a buffer in AvalonB1_RunScene's frame) */

void AvalonB1_InitCamera(void)
{
	register s16 *p;
	register short i;

	for (i = 0; i < 16; i++)
		g_AvalonB1SavedPal[i] = g_PalBase[i];
	g_AvalonB1FadeRow = 0;
	g_AvalonB1Timer = 0;
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable), 0);	/* New */
	g_Camera->update = AvalonB1_CamHookWait2;
	g_CameraY = 0x430;
	g_FgScrollY = g_CameraY;
	g_BgScrollY = 0xE0;
	p = g_PalBase;
	for (i = 0; i < 16; i++)
		*p++ = 0;
	p = &g_PalBase[32];
	for (i = 0; i < 16; i++)
		*p++ = 0;
	g_PalBaseDirty = 1;
	g_FgStreamY += 0x100;
	g_MapStreamNarrow = 0;
	Irq_Disable();
	(*g_MapRowHook)();
	Irq_Enable();
}
