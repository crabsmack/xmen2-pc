/* SyncActor_MemberInWindow - SYNCHRONIZEDACTOR slot +$6E: the parent's (ACTOR) InActivateWindow
 * test (slot +$3A), for the SYNCHRONIZER (Synchronizer_WaitMembers): the member's own slot +$3A
 * is SyncActor_InActivateWindow.
 *
 * self: the object.  Returns the parent's answer. */
#include "xmen2.h"

char SyncActor_MemberInWindow(Synchronizedactor *self)
{
	return (char)(*CLASS(Tbl_SyncActor_vtable)->parent->method[13])(self);	/* ACTOR InActivateWindow */
}
