/* FluxBomb_Update - FLUXBOMB Update (class slot +$4A): step the particles of the phase it is in
 * (implosion FluxBomb_StepImplode, burst FluxBomb_StepBurst), twice in the two-player step mode,
 * then the parent's (RTHETAOBJECT) Update.
 *
 * self: the bomb. */
#include "xmen2.h"

void FluxBomb_Update(register FluxBomb *self)
{
	register short n = g_TwoPlayerStep + 1;

	while (n--) {
		switch (self->phase) {
		case 1:
			FluxBomb_StepImplode(self);
			break;
		case 3:
			FluxBomb_StepBurst(self);
			break;
		}
	}
	(*CLASS(Tbl_FluxBomb_vtable)->parent->method[17])(self);	/* RTHETAOBJECT Update */
}
