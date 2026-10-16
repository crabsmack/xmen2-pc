/* SyncActor_MemberOutsideWindow - SYNCHRONIZEDACTOR slot +$72: the parent's (ACTOR)
 * OutsideActiveWindow test (slot +$42), for the SYNCHRONIZER (Synchronizer_RunMembers).
 *
 * self: the object.  Returns the parent's answer. */
#include "xmen2.h"

char SyncActor_MemberOutsideWindow(Synchronizedactor *self)
{
	return (char)(*CLASS(Tbl_SyncActor_vtable)->parent->method[15])(self);	/* ACTOR OutsideActiveWindow */
}
