/* BioNpc_DrawStunned - a stunned BIONPC's draw-list function (BioNpc_TakeDamage puts it in its
 * node): the psychic effect flickers over it (Spr_EmitFlicker: g_PsylockeFxSpr's tiles by
 * Tbl_SprSizeTailTile, flicker mask 3, palette 3, flipped by V-blank bits 2 and 4); the body
 * itself (class slot +$1A Draw) only while it has health or more than 15 stun frames are left.
 *
 * self: the object. */
#include "xmen2.h"

void BioNpc_DrawStunned(register BioNpc *self)
{
	register short left = self->stun & 0x3FFF;

	Spr_EmitFlicker((Actor *)self, (u8 *)CODEPTR(Tbl_SprSizeTailTile), g_PsylockeFxSpr, 1, 3, 3, 2, 4);
	if (self->health > 0 || left > 15)
		(*self->cls->method[5])(self);	/* +$1A Draw */
}
