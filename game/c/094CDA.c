/* EvilDrop_Nop6A - class slot +$6A of EVILDROP: only calls the empty EvilDrop_NopStatic.
 *
 * self: the object (unused). Never runs in the corpus. */
#include "xmen2.h"

void EvilDrop_Nop6A(EvilDrop *self)
{
	EvilDrop_NopStatic();
}
