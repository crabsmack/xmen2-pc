/* FlyBrood_Deactivate - FLYBROOD Deactivate (class slot +$46): the parent's (ACTOR) Deactivate,
 * then count g_FlyBroodCount down (nothing counts it up or reads it).
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_Deactivate(FlyBrood *self)
{
	(*CLASS(Tbl_FlyBrood_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
	g_FlyBroodCount--;
}
