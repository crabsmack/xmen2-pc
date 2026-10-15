/* Hologram_Init - HOLOGRAM constructor (class slot +$12): the parent's (ACTOR) Init, then the
 * boss set up to wait for the players (Hologram_StateStart).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on;
 * not checked for null).
 * Sprite definition from slot g_HologramSprDef, the intro AI script
 * (Tbl_HologramAiScript_0EAEFC: Hologram_ScriptIntro, 1 step StartAttackScript), flags6 |= $5083,
 * rest/raised delays from placement parameters 0/1 (0 keeps the defaults 40/180), its body
 * segment (type $14, 60 x 200 px around it), and a 60-frame timer that then installs
 * Hologram_VBlankFlicker as the V-blank game hook (Timer_SetVBlankHookCb). The flicker is
 * armed with no sprite slot.
 * The two if (1) blocks stand for blocks under a constant condition in the original: their
 * ends are labels, where MPW C pops the pending call arguments (after Seg_Create and at the
 * end); plain blocks or do { } while (0) give other code. */
#include "xmen2.h"

void Hologram_Init(register Hologram *self, long x, long y, register Placement *pl)
{
	register Timer *t;

	(*CLASS(Tbl_Hologram_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	Obj_SetSpriteDef((Actor *)self, g_HologramSprDef);
	AiScript_Set(&self->ai, CODEPTR(Tbl_HologramAiScript_0EAEFC));
	self->state = self->defaultState = Hologram_StateStart;
	self->flags6 |= 0x5083;
	self->restDelay = 40;
	self->raisedDelay = 180;
	if (1) {
		register short left = self->px - 30;
		register short right = self->px + 30;
		register short top = self->py - 100;
		register short bottom = self->py + 100;

		self->bodySeg = Seg_Create(0x14, 0, left, top, right, bottom);
	}
	{
		register short v;

		if ((v = pl->param[0]) != 0)
			self->restDelay = v;
		if ((v = pl->param[1]) != 0)
			self->raisedDelay = v;
	}
	t = Timer_Create((TimerFn)Timer_SetVBlankHookCb, 60, FN_DATA(Hologram_VBlankFlicker), 1);
	Timer_Start(t);
	if (1) {
		g_HoloFlickerArmed = 1;
		g_HoloFlickerLinks[1] = 0;
	}
}
