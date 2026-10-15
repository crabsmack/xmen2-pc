/* BigAcolyte_Deactivate - BIGACOLYTE Deactivate (class slot +$46): the parent's (BIONPC), then the
 * camera is released (its UnlockAxes). */
#include "xmen2.h"

void BigAcolyte_Deactivate(BigAcolyte *self)
{
	(*CLASS(Tbl_BigAcolyte_vtable)->parent->method[16])(self);	/* BIONPC Deactivate */
	(*g_Camera->cls->method[13])(g_Camera);				/* +$3A UnlockAxes */
}
