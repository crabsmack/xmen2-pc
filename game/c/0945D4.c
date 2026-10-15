/* EvilMaker_Nop6A - class slot +$6A of EVILMAKER: only calls the empty EvilMaker_NopStatic.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void EvilMaker_Nop6A(EvilMaker *self)
{
	EvilMaker_NopStatic();
}
