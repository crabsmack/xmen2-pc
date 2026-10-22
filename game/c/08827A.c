/* CloneA5_CamUpdate - the boss arena camera's update (CAMERA +$04, CloneA5_CamSetHooks): when the
 * claw's swing scrolls the arena this frame (g_BroodScrollStep), each player standing (live, on
 * the ground) or a Magneto in Tbl_MagnetoAnim whose box's right edge is on the screen is carried
 * back with it (x and segPos minus the step, boxes recomputed); then the camera's own tracking,
 * easing (when it has one) and apply (CloneA5_CamApply).
 *
 * cam: the camera.
 * It loads the plane B scroll bases into a register it never uses (kept). */
#include "xmen2.h"

#define g_BroodScrollStep	(*(s16 *)g_SceneVar3B02)	/* scene variable (CloneA5): pixels the arena scrolls this frame (BroodClaw_Swing; CloneA5_CamApply adds it to the FG scroll and clears it) */

void CloneA5_CamUpdate(register Camera *cam)
{
	register short unused;
	register CamFn ease;

	unused = g_BgScrollBaseX;
	unused = g_BgScrollBaseY;
	if (g_BroodScrollStep) {
		if (((IS_ALIVE(&g_Player1) && !(g_Player1.flags & 0x1000))
		     || (g_Player1.cls == CLASS(Tbl_Magneto_vtable) && g_Player1.anim == CODEPTR(Tbl_MagnetoAnim)))
		    && g_Player1.boxRight < g_CameraX + 0x140) {
			g_Player1.segPos -= g_BroodScrollStep << 8;
			g_Player1.x -= g_BroodScrollStep << 8;
			Obj_UpdateBoxes(&g_Player1);
		}
		if (((IS_ALIVE(&g_Player2) && !(g_Player2.flags & 0x1000))
		     || (g_Player2.cls == CLASS(Tbl_Magneto_vtable) && g_Player2.anim == CODEPTR(Tbl_MagnetoAnim)))
		    && g_Player2.boxRight < g_CameraX + 0x140) {
			g_Player2.segPos -= g_BroodScrollStep << 8;
			g_Player2.x -= g_BroodScrollStep << 8;
			Obj_UpdateBoxes(&g_Player2);
		}
#if defined(XMEN2_NATIVE)
		P34_ScrollPush(g_BroodScrollStep, g_CameraX + 0x140);	/* phase 7: players 3-4 with the arena */
#endif
	}
	(*cam->track)(cam);
	if ((ease = cam->ease) != 0)
		(*ease)(cam);
	(*cam->apply)(cam);
}
