/* ToxicCan_Activate - TOXICCAN Activate (class slot +$3E): the parent's (ACTOR) Activate, then
 * join the player-side body hit list, so that enemy attacks hit the can too.
 *
 * self: the can. */
#include "xmen2.h"

void ToxicCan_Activate(register ToxicCan *self)
{
	(*CLASS(Tbl_ToxicCan_vtable)->parent->method[14])(self);	/* ACTOR Activate */
	self->playerBodyNode = HitList_Add(self, &g_HitListPlayerBody);
}
