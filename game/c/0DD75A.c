/* ControlBox_Draw - CONTROLBOX Draw (class slot +$1A): while firing (stage 1), its light at the
 * pattern step shown (the g_ControlPanelSprDef2 sprite at the step's frame, the step's offset
 * from the box); otherwise the parent's (ACTOR) Draw.
 *
 * self: the box. */
#include "xmen2.h"

void ControlBox_Draw(ControlBox *self)
{
	SprInst *spr;
	register PanelStep *s;
	register long x, y;

	if (self->stage == 1) {
		spr = &g_SpriteDefs[g_ControlPanelSprDef2];
		s = &self->pattern->step[self->step];
		x = ((self->x >> 8) - g_CameraX) + s->dx;
		y = ((self->y >> 8) - g_CameraY) + s->dy;
		spr->frame = s->frame;
		Spr_Draw(spr, x, y);
	} else
		(*CLASS(Tbl_ControlBox_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
