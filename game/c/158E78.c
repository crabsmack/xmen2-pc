/* FluxBomb_StateThrown - FLUXBOMB state of a thrown bomb: it flies on (its speed falling) and
 * goes no lower than y $16C (there its speed drops to $100); at speed $100 or less it stops
 * (frozen, flags6 $4000), plays the implode animation (Tbl_FluxBombAnim22) and implodes
 * (FluxBomb_StartImplode, then FluxBomb_StateImplode).
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_StateThrown(register FluxBomb *self)
{
	if (self->y > 0x16C00) {
		self->y = 0x16C00;
		self->speed = 0x100;
	}
	if (self->speed <= 0x100) {
		self->flags6 |= 0x4000;
		Obj_SetAnim(self, CODEPTR(Tbl_FluxBombAnim22));
		FluxBomb_StartImplode(self);
		SET_STATE(self, FluxBomb_StateImplode);
		return self->flags & 8;
	}
	SET_STATE(self, FluxBomb_StateThrown);
	return self->flags & 8;
}
