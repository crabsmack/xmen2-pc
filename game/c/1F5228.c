/* Scroll_RequestVsram - have the next V-blank upload both planes' vertical scroll (g_VsramReq,
 * VDP_UpdateScroll) instead of the plain scroll update (the dirty flags cleared). */
#include "xmen2.h"

void Scroll_RequestVsram(void)
{
	g_FgScrollDirty = 0;
	g_BgScrollDirty = 0;
	g_VsramReq = 1;
}
