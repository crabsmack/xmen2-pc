/* SavageMediumBio_ClassInit - per-level class setup of SAVAGEMEDIUMBIO (class slot +$5E): only the
 * parent's (SAVAGEBIOBASE: SavageBioBase_ClassInit, which calls the empty SavageBio_NopStatic).
 *
 * self: 0, passed on. The class's index in the level's class list follows; it is not passed. */
#include "xmen2.h"

void SavageMediumBio_ClassInit(SavageMediumBio *self)
{
	(*CLASS(Tbl_SavageMediumBio_vtable)->parent->method[22])(self);	/* SAVAGEBIOBASE ClassInit */
}
