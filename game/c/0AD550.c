/* SavageA2_CamUpdateFlood - the Perimeter camera's update (CAMERA +$04, SavageA2/A3): raise the
 * flood, then the default camera update (Cam_DefaultUpdate).
 *
 * cam: the camera.
 * The water rises on frames whose bits 1-2 (g_FrameCounter & 6) are both set, once the scene is
 * past frame $78 (SavageA2) / $1E0 (SavageA3), while the flood line is below the top of the map
 * (g_FloodLineY > 0): fast until frame $C8 / $230 ($200 a step, 8 fraction bits), then slowly
 * ($80 in SavageA2, $100 in SavageA3); twice that in two-player stepping. The water surface and
 * its splash line follow, 1 px below the flood line. */
#include "xmen2.h"

void SavageA2_CamUpdateFlood(Camera *cam)
{
	register char rise;
	register char fast;

	if (g_ScnSavageA2 == g_SceneIndex) {
		rise = g_FrameCounter > 0x78 ? (g_FrameCounter & 6) == 6 : 0;
		fast = g_FrameCounter < 0xC8;
	} else {
		rise = g_FrameCounter > 0x1E0 ? (g_FrameCounter & 6) == 6 : 0;
		fast = g_FrameCounter < 0x230;
	}
	if (rise && g_FloodLineY > 0) {
		if (fast) {
			g_FloodY88 -= g_TwoPlayerStep ? 0x400 : 0x200;
			g_FloodLineY = g_FloodY88 >> 8;
		} else if (g_ScnSavageA2 == g_SceneIndex) {
			g_FloodY88 -= g_TwoPlayerStep ? 0x100 : 0x80;
			g_FloodLineY = g_FloodY88 >> 8;
		} else {
			g_FloodY88 -= g_TwoPlayerStep ? 0x200 : 0x100;
			g_FloodLineY = g_FloodY88 >> 8;
		}
		g_WaterSurfaceY = g_WaterSplashY = g_FloodLineY + 1;
	}
	Cam_DefaultUpdate(cam);
}
