/* Deathbird_SetMoveMode - DEATHBIRD class slot +$72: set up a way of moving (velocity,
 * acceleration, entry point, moveKind for Deathbird_Move) and its animation.
 *
 * self: the boss.  mode: 0 stop (still); 1 fall when dead (gravity 4, vx & 9 kept with its sign,
 * Tbl_DeathbirdAnim5); 2 circle (speed $350, heading 305 degrees turning +5 a step, or 235
 * turning -5 when facing left; Tbl_DeathbirdAnim2); 3 stop and hover (Tbl_DeathbirdAnim);
 * 4 fly off up and ahead, speeding up (vx $400 / ax 10 by its facing, vy $90 / ay -20);
 * 5 swoop in from 100 px off the top corner of the side it waited at (vx $200 in, vy $200);
 * 6 wait off screen on the side it left by (moveKind 1 left / 2 right), 10 frames, hidden;
 * 7 dive in low from 50 px below the top on that side (vx $500 in, vy $264, ay -10,
 * Tbl_DeathbirdAnim4); 8 pull up (ay -40); 9 fly at the lift switch from 60 px below the top on
 * that side (vx $500 in, vy $270); 10 recoil after a hit (like 1 without gravity,
 * Tbl_DeathbirdAnim8); anything else: moveKind 3 only. Modes 1-5 and 7-10 fly with the wobble
 * (moveKind 3), 2 circles (4). The cases are Deathbird_SetMoveMode_Case0, _Case1, _Case2 (2, 3),
 * _Case4, _Case5 (5, 6), _Case7 (7, 8), _Case9, _Case10 and _Default. */
#include "xmen2.h"

#define g_DeathbirdHeading	(*(s32 *)&g_SceneVar3B20)	/* scene variable: circling heading, degrees, 8 fraction bits */
#define g_DeathbirdTurnRate	(*(s32 *)g_SceneVar3B24)	/* scene variable: added to the heading each step */
#define g_DeathbirdTurnAccel	(*(s32 *)g_SceneVar3B28)	/* scene variable: added to the turn rate each step */

void Deathbird_SetMoveMode(Deathbird *self, short mode)
{
	switch (mode) {
	case 0:				/* Deathbird_SetMoveMode_Case0 */
		STOP_MOVING(self);
		self->moveKind = 0;
		break;
	case 1:				/* Deathbird_SetMoveMode_Case1 */
		self->vy = 0;
		self->ay = 0;
		self->vx = self->vx > 0 ? self->vx & 9 : -(self->vx & 9);
		self->ax = 0;
		self->ay = 4;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim5));
		break;
	case 2:				/* Deathbird_SetMoveMode_Case2 */
		STOP_MOVING(self);
		if (!(self->spr.flags & 0x10)) {
			self->speed = 0x350;
			self->accel = 0;
			g_DeathbirdHeading = 0x13100;
			g_DeathbirdTurnRate = 0x500;
			g_DeathbirdTurnAccel = 0;
		} else {
			self->speed = 0x350;
			self->accel = 0;
			g_DeathbirdHeading = 0xEB00;
			g_DeathbirdTurnRate = -0x500;
			g_DeathbirdTurnAccel = 0;
		}
		self->moveKind = 4;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim2));
		break;
	case 3:
		STOP_MOVING(self);
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim));
		break;
	case 4:				/* Deathbird_SetMoveMode_Case4 */
		if (!(self->spr.flags & 0x10)) {
			self->vx = 0x400;
			self->ax = 10;
		} else {
			self->vx = -0x400;
			self->ax = -10;
		}
		self->vy = 0x90;
		self->ay = -20;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim));
		break;
	case 5:				/* Deathbird_SetMoveMode_Case5 */
		STOP_MOVING(self);
		if (self->moveKind == 1) {
			self->spr.flags &= ~0x10;
			self->x = (g_CameraX - 100) << 8;
			self->y = (g_CameraY - 100) << 8;
			self->vx = 0x200;
		} else {
			self->spr.flags |= 0x10;
			self->x = (g_CameraX + 0x1A4) << 8;
			self->y = (g_CameraY - 100) << 8;
			self->vx = -0x200;
		}
		self->vy = 0x200;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim));
		break;
	case 6:
		STOP_MOVING(self);
		if ((self->x >> 8) + 60 < g_CameraX)
			self->moveKind = 1;
		else
			self->moveKind = 2;
		self->countdown = 10;
		self->anim = 0;
		(*self->cls->method[20])(self, 0xFF);	/* SetFrame: hide */
		break;
	case 7:				/* Deathbird_SetMoveMode_Case7 */
		STOP_MOVING(self);
		if (self->moveKind == 1) {
			self->spr.flags &= ~0x10;
			self->x = (g_CameraX - 100) << 8;
			self->y = (g_CameraY + 50) << 8;
			self->vx = 0x500;
		} else {
			self->spr.flags |= 0x10;
			self->x = (g_CameraX + 0x1A4) << 8;
			self->y = (g_CameraY + 50) << 8;
			self->vx = -0x500;
		}
		self->vy = 0x264;
		self->ay = -10;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim4));
		break;
	case 8:
		self->ay = -40;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim));
		break;
	case 9:				/* Deathbird_SetMoveMode_Case9 */
		STOP_MOVING(self);
		if (self->moveKind == 1) {
			self->spr.flags &= ~0x10;
			self->x = (g_CameraX - 100) << 8;
			self->y = (g_CameraY + 60) << 8;
			self->vx = 0x500;
		} else {
			self->spr.flags |= 0x10;
			self->x = (g_CameraX + 0x1A4) << 8;
			self->y = (g_CameraY + 60) << 8;
			self->vx = -0x500;
		}
		self->vy = 0x270;
		self->ay = 0;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim));
		break;
	case 10:			/* Deathbird_SetMoveMode_Case10 */
		self->vy = 0;
		self->ay = 0;
		self->vx = self->vx > 0 ? self->vx & 9 : -(self->vx & 9);
		self->ax = 0;
		self->moveKind = 3;
		Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim8));
		break;
	default:			/* Deathbird_SetMoveMode_Default */
		self->moveKind = 3;
		break;
	}
}
