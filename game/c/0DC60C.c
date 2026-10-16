/* Barrier_Nop6A - class slot +$6A of BARRIER: an empty hook (it only calls the empty
 * Barrier_NopStatic).
 *
 * self: the object. */
#include "xmen2.h"

void Barrier_Nop6A(Barrier *self)
{
	Barrier_NopStatic();
}
