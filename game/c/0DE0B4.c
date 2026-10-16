/* ControlPanel_Draw - CONTROLPANEL Draw (class slot +$1A): while firing (stage 5), its light at
 * the pattern step shown (the g_ControlPanelSprDef2 sprite at the step's frame, the step's
 * offset from the panel); otherwise the parent's (ACTOR) Draw.
 *
 * self: the panel.
 * The firing panel itself isn't drawn (it has no frame). */
#include "xmen2.h"

void ControlPanel_Draw(ControlPanel *self)
{
	SprInst *spr;
	register PanelStep *s;
	register long x, y;

	if (self->stage == 5) {
		spr = &g_SpriteDefs[g_ControlPanelSprDef2];
		s = &self->pattern->step[self->step];
		x = ((self->x >> 8) - g_CameraX) + s->dx;
		y = ((self->y >> 8) - g_CameraY) + s->dy;
		spr->frame = s->frame;
		Spr_Draw(spr, x, y);
	} else
		(*CLASS(Tbl_ControlPanel_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
