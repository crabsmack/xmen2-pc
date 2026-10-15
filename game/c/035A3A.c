/* AvalonA4_InstallEventHooks - no camera zone held, and the Throne Room's six event hooks on
 * floor-segment types $15-$1A: AvalonA4_EventZoneFree0-5 (type $15 + n answers for zone n). */
#include "xmen2.h"

#define g_A4CamZones	(*(s16 *)g_SceneVar3B02)	/* scene variable: bit n = camera zone n held */

void AvalonA4_InstallEventHooks(void)
{
	g_A4CamZones = 0;
	g_SegTypeHooks[0x15] = AvalonA4_EventZoneFree0;
	g_SegTypeHooks[0x16] = AvalonA4_EventZoneFree1;
	g_SegTypeHooks[0x17] = AvalonA4_EventZoneFree2;
	g_SegTypeHooks[0x18] = AvalonA4_EventZoneFree3;
	g_SegTypeHooks[0x19] = AvalonA4_EventZoneFree4;
	g_SegTypeHooks[0x1A] = AvalonA4_EventZoneFree5;
}
