/* SiberiaB1_InitWind - set up the Blizzard: the wind offset on, the wind at angle $1E strength
 * $600 and no change, its keyframes from the start of Tbl_SiberiaB1Wind (next step now); the
 * camera (Cam_New with SiberiaB1_InitCamera); the camera's maximum from the map size
 * (Lvl_SiberiaB1Bg's header, cells of 8 px, less the 320x224 view); colours $29-$2C. */
#include "xmen2.h"

#define g_WindAngle	(*(s16 *)g_SceneVar3B04)	/* scene variable: wind angle, degrees */
#define g_WindStrength	(*(s16 *)g_SceneVar3B06)	/* scene variable: wind strength, 8 fraction bits */
#define g_WindTimer	(*(s16 *)g_SceneVar3B08)	/* scene variable: frames to the next wind step */
#define g_WindKey	(*(PTR(WindKey) *)g_SceneVar3B14)	/* scene variable: the keyframe the wind moves to */
#define g_SiberiaB1Count	(*(s16 *)g_SceneVar3B32)	/* scene variable: a countdown (SiberiaB1_FrameHook; nothing else reads it) */
#define g_WindAngleStep	(*(s16 *)g_SceneVar3B40)	/* scene variable: angle change per step (-5..5) */
#define g_WindStrengthStep	(*(s16 *)g_SceneVar3B48)	/* scene variable: strength change per step ($80 / -$80) */

void SiberiaB1_InitWind(void)
{
	register void *map;
	register long hdr;

	g_BgOffsetOn = 1;
	g_WindStrengthStep = 0;
	g_WindAngleStep = 0;
	g_WindStrength = 0x600;
	g_WindAngle = 0x1E;
	g_WindKey = (WindKey *)CODEPTR(Tbl_SiberiaB1Wind);
	g_WindTimer = 0;
	g_SiberiaB1Count = 0;
	g_Camera = (Camera *)(*CLASS(Tbl_Camera_vtable)->method[1])(CLASS(Tbl_Camera_vtable),
		SiberiaB1_InitCamera);	/* Cam_New */
	map = CODEPTR(Lvl_SiberiaB1Bg);
	hdr = ADDR68(map);
	g_CamMaxX = g_CamMaxXTarget = (AT68(s16, hdr)[1] << 3) - 0x140;
	g_CamMaxY = g_CamMaxYTarget = (AT68(s16, hdr)[2] << 3) - 0xE0;
	g_PalBase[0x29] = 0xA8A;
	g_PalBase[0x2A] = 0xEEE;
	g_PalBase[0x2B] = 0xCCC;
	g_PalBase[0x2C] = 0x868;
	g_PalBaseDirty = 1;
}
