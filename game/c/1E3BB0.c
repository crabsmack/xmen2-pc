/* Cyclops_Draw - CYCLOPS Draw (class slot +$1A): the beam (Cyclops_DrawBeam) under him, then the
 * parent's (XPLAYER) Draw, then the beam's hits (Cyclops_BeamHitEnemies) while it is active.
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_Draw(register Cyclops *self)
{
	Cyclops_DrawBeam(self, &self->beam);
	(*CLASS(Tbl_Cyclops_vtable)->parent->method[5])(self);	/* XPLAYER Draw */
	if (self->beam.active)
		Cyclops_BeamHitEnemies(self);
}
