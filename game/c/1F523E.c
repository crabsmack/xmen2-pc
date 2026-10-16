/* Scroll_RequestColumnVs - have the next V-blank upload the whole H-scroll and the 40 columns'
 * vertical scroll (g_ColumnVsReq, VDP_UpdateScroll) instead of the plain scroll update (the dirty
 * flags cleared). */
#include "xmen2.h"

void Scroll_RequestColumnVs(void)
{
	g_FgScrollDirty = 0;
	g_BgScrollDirty = 0;
	g_ColumnVsReq = 1;
}
