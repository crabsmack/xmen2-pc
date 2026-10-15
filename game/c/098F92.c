/* XClone_KillAndCount - kill an object (its class slot +$52) and count it in g_CloneMakerKills
 * (EvilMaker_StateWaitKills waits for 2).
 *
 * obj: the object to kill. */
#include "xmen2.h"

void XClone_KillAndCount(Actor *obj)
{
	(*obj->cls->method[19])(obj);		/* Kill */
	g_CloneMakerKills++;
}
