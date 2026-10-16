/* Irq_VBlank - the V-int handler (the hook at $210 that Irq_VBlankVector, the level-6 vector,
 * calls with d0-d2/a0-a1 saved): read the pads, count the frame, run the system hook
 * (g_VBlankHook: VDP_FrameUpdate) and the game's (g_VBlankGameHook, unless g_Unk38C0 or
 * g_Unk359E is set), then clear g_VBlankFrameDone.
 *
 * g_FrameUpdated is read first and never used. */
#include "xmen2.h"

/* the game hook as the function it is */
#define g_GameHook	(*(FrameHookFn *)&g_VBlankGameHook)

void Irq_VBlank(void)
{
	register char updated = g_FrameUpdated;

	Pad_ReadBoth();
	++g_VBlankCount;
	if (g_VBlankHook)
		(*g_VBlankHook)();
	if (!g_Unk38C0 && !g_Unk359E && g_GameHook)
		(*g_GameHook)();
	g_VBlankFrameDone = 0;
	if (0)
		(void)updated;
}
