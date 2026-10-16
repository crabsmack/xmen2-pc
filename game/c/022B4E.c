/* AvalonA2_InstallEventHooks - the Falling Room's event hooks on floor-segment types $14
 * (AvalonA2_HookReturn0) and $15 (AvalonA2_HookEndOnEvent3: the exit). */
#include "xmen2.h"

void AvalonA2_InstallEventHooks(void)
{
	g_SegTypeHooks[0x14] = AvalonA2_HookReturn0;
	g_SegTypeHooks[0x15] = AvalonA2_HookEndOnEvent3;
}
