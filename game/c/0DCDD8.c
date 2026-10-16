/* Blast_Draw - BLAST Draw (class slot +$1A): while latched (blastFlags bit 0) the zap flickers over
 * its target (Spr_EmitFlicker: g_BlastSprDef's tiles by Tbl_SprSizeTailTile, flicker mask 3,
 * palette 1, flipped by V-blank bits 2 and 4), else the parent's (ACTOR) Draw.
 *
 * self: the blast. */
#include "xmen2.h"

void Blast_Draw(register Blast *self)
{
	if (self->blastFlags & 1)
		Spr_EmitFlicker(self->target, (u8 *)CODEPTR(Tbl_SprSizeTailTile),
			&g_SpriteDefs[g_BlastSprDef], 1, 3, 1, 2, 4);
	else
		(*CLASS(Tbl_Blast_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
