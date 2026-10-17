/* CloneA2_StartCamera - start the CloneA2 camera: its alternate mode (CloneA2_StartCamAltMode),
 * zone 0, then a pan (CAMERA Cam_PanTo, speeds 1000, x locked) to player 1's start, x rounded
 * down to 512 px. */
#include "xmen2.h"

void CloneA2_StartCamera(void)
{
	register unsigned char lockX = 1;
	register unsigned char lockY = 0;
#ifdef XMEN2_MPW
	register short x = g_P1StartRec->x;
	register short y = g_P1StartRec->y;
#else
	/* g_P1StartRec is 0 once the players are placed (Players_PlaceAtStart clears it): the 68000
	   reads the vector table */
	register short x = (g_P1StartRec ? (Placement *)g_P1StartRec : (Placement *)Vectors)->x;
	register short y = (g_P1StartRec ? (Placement *)g_P1StartRec : (Placement *)Vectors)->y;
#endif

	x &= 0xFE00;
	CloneA2_StartCamAltMode();
	CloneA2_SetCameraZone(0);
	(*g_Camera->cls->method[12])(g_Camera, x, y, 1000, 1000, lockX, lockY);	/* +$36 Cam_PanTo */
}
