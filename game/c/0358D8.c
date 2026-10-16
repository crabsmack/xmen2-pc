/* AvalonA4_SetCameraZone - hold the camera in zone n (0-5): pan it to (x, y) at the given speeds
 * (CAMERA Cam_PanTo) with x locked there (except zone 3) and y free, and set bit n of the zone
 * mask (AvalonA4_EventZoneFree0-5 report a zone free while its bit is clear).
 *
 * n: the zone.  x, y: camera position (pixels).  speedX, speedY: pan speeds.
 * The switch cases are the fragments AvalonA4_SetCameraZone_case0 (zones 0-2), _case3, _case4
 * (zones 4, 5); _apply is the rest. A zone outside 0-5 or's in whatever bit held: on the 68000
 * the zone itself (n and bit share d7; the PC line says so). */
#include "xmen2.h"

#define g_A4CamZones	(*(s16 *)g_SceneVar3B02)	/* scene variable: bit n = camera zone n held */

void AvalonA4_SetCameraZone(register short n, short x, short y, short speedX, short speedY)
{
	register unsigned char lockX, lockY;
	register short bit;

	lockX = n == 3 ? 0 : 1;
	lockY = 0;
	(*g_Camera->cls->method[12])(g_Camera, x, y, speedX, speedY, lockX, lockY);	/* +$36 Cam_PanTo */
#ifndef XMEN2_MPW
	bit = n;	/* (the 68000 keeps n and bit in one register) */
#endif
	switch (n) {
	case 0: bit = 1; break;
	case 1: bit = 2; break;
	case 2: bit = 4; break;
	case 3: bit = 8; break;
	case 4: bit = 0x10; break;
	case 5: bit = 0x20; break;
	}
	g_A4CamZones |= bit;
}
