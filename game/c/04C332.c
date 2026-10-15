/* Apocalypse_StateVanish - APOCALYPSE defeated: hide (frame $FF), kill itself and spawn the
 * BEAMSCOOTER at (-10, -40) px that ends the fight (BeamScooter_Spawn).
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_StateVanish(register Apocalypse *self)
{
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: hide */
	(*self->cls->method[19])(self);			/* +$52 Kill */
	BeamScooter_Spawn(0xF600, -0x2800);
}
