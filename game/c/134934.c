/* FlyBrood_Init - FLYBROOD Init (class slot +$12): the parent's (ACTOR) Init, then the position,
 * sprite, range, flight parameters and AI script from the placement entry.
 *
 * self: the object.  x, y: passed on to the parent's Init.  pl: its placement entry (not
 * tested for 0).
 * Placement param 1 picks the script: 0 (and anything outside 0-7) hover forever, 1 / 2 / 3 wait
 * for the player to pass, then dive / dive low / sine fly-by (3 with shift 3), 4 enter from the
 * screen side nearer the placement (left when x is left of the screen centre) homing in, then
 * Tbl_FlyBroodAiScript, 5-7 Tbl_FlyBroodAiScript2-4 (7 with shift 3). Param 2: speed in pixels
 * ($200 when 0); param 3 nonzero: watch the right side. The x range is placement x -/+ 160, yMin
 * 128 px above it, both kept (clampFlags 3); it starts at FlyBrood_ActRestartScript, hurt state
 * FlyBrood_StateHit, flags6 |= $4080, facing right, MoveSine angle step 4. Sprite: the set
 * g_FlyBroodSprDef, or in the raw-art scene (g_ScnUnused3B5C) Art_FlyBroodNem with palette 2;
 * priority bit (2) from the live player (player 1 when neither is); flying animation
 * Tbl_Anim_13437C (raw-art scene Tbl_Anim_134390). */
#include "xmen2.h"

void FlyBrood_Init(FlyBrood *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_FlyBrood_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->x = pl->x << 8;
	self->y = pl->y << 8;
	self->vy = 0;
	self->ay = 0;
	if (g_ScnUnused3B5C != g_SceneIndex)
		Obj_SetSpriteDef((Actor *)self, g_FlyBroodSprDef);
	else {
		self->spr.def = CODEPTR(Art_FlyBroodNem);
		self->spr.flags = self->spr.flags & ~3 | 2;
	}
	self->flags6 |= 0x4080;
	self->state = (StateFn)FlyBrood_ActRestartScript;
	self->hurtState = (StateFn)FlyBrood_StateHit;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	AiScript_Set(&self->ai, 0);
	self->unread_96 = 1;
	self->unread_97 = 1;
	self->xMin = pl->x - 0xA0;
	self->xMax = pl->x + 0xA0;
	self->yMin = pl->y - 0x80;
	self->anchorX = 0;
	self->anchorY = 0;
	self->spr.flags &= ~0x10;
	self->headingRight = 0;
	self->mover = 0;
	self->path = 0;
	self->shift = 1;
	self->speed = 0x200;
	self->watchRight = 0;
	self->clampFlags = 3;
	self->angle = 0;
	self->angleStep = 4;
	switch (pl->param[1]) {
	default:
	case 0:				/* FlyBrood_Init_Case0 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBrood_ScriptA));	/* FlyBrood_ScrStepHoverForever */
		break;
	case 1:				/* FlyBrood_Init_Case1 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBrood_ScrStepsWaitPassedLen));	/* FlyBrood_ScrStepsWaitPassed */
		break;
	case 2:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBrood_ScrStepsWaitPassedLowLen));	/* FlyBrood_ScrStepsWaitPassedLow */
		break;
	case 3:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBrood_ScrStepsWaitPassedSineLen));	/* FlyBrood_ScrStepsWaitPassedSine */
		self->shift = 3;
		break;
	case 4: {			/* FlyBrood_Init_Case4 */
		short px = self->x >> 8;

		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBroodAiScript));
		if (px < g_CameraX + 0xA0)
			FlyBrood_EnterFromSide(self, 1);
		else
			FlyBrood_EnterFromSide(self, 0);
		self->state = self->defaultState = FlyBrood_WaitPathDone;
		self->mover = (FlyMoveFn)FlyBrood_MoveHoming;
		self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodHomingPath);
		break;
	}
	case 5:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBroodAiScript2));
		break;
	case 6:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBroodAiScript3));
		break;
	case 7:
		AiScript_Set(&self->ai, CODEPTR(Tbl_FlyBroodAiScript4));
		self->shift = 3;
		break;
	}
	/* FlyBrood_Init_Tail */
	if ((v = pl->param[2]))
		self->speed = v << 8;
	if ((v = pl->param[3]))
		self->watchRight = 1;
	if (g_ScnUnused3B5C != g_SceneIndex)
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_13437C));
	else
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_134390));
}
