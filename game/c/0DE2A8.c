/* ControlPanel_StateDestroyed - CONTROLPANEL state (the fourth hit): an explosion at the centre
 * of its body box, no frame, out of the enemy body hit list, its switch's flag bit 0 (which
 * activates the switch's CONTROLBOX), its firing pattern started; ControlPanel_StateArm next.
 *
 * self: the panel.
 * Returns self->flags & 8. */
#include "xmen2.h"

short ControlPanel_StateDestroyed(register ControlPanel *self)
{
	register long cx = (self->boxRight + self->boxLeft) << 7;
	register long cy = (self->boxTop + self->boxBottom) << 7;

	Explosion_Spawn(cx, cy, 0);
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame: none */
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	(*self->cls->method[26])(self, 1);	/* +$6E ControlPanel_SetFlags */
	(*self->cls->method[27])(self);		/* +$72 ControlPanel_StartPattern */
	SET_STATE(self, ControlPanel_StateArm);
	return self->flags & 8;
}
