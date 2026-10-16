/* ToxicCan_Deactivate - TOXICCAN Deactivate (class slot +$46): leave the player-side body hit
 * list, then the parent's (ACTOR) Deactivate.
 *
 * self: the can. */
#include "xmen2.h"

void ToxicCan_Deactivate(register ToxicCan *self)
{
	List_Unlink(self->playerBodyNode, &g_HitListPlayerBody);
	self->playerBodyNode = 0;
	(*CLASS(Tbl_ToxicCan_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
}
