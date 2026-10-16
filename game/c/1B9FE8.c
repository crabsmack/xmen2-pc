/* Psylocke_AfterState - PSYLOCKE AfterState (class slot +$72): only the parent's (XCLIMBER's,
 * reached through PSYLOCKE's descriptor) post-state hook.
 *
 * self: the player. */
#include "xmen2.h"

void Psylocke_AfterState(Psylocke *self)
{
	(*CLASS(Tbl_Psylocke_vtable)->parent->method[27])(self);
}
