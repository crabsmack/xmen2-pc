/* Hologram_Explode - the hologram blows up: a chain explosion at its position (a plain ACTOR used
 * as the effect, Tbl_HologramDeathChain, harmless, 16 px left and 32 px down, 5 rounds), it is
 * hidden (pose $1B), its body segment is opened (bit $40), its sprite tiles are given back
 * (every live object after the players whose VRAM tiles lie above them moves down by as many
 * tiles and redraws, the VRAM tile cursor too) and the X-power power-ups spawn again.
 *
 * self: the hologram (Hologram_ExplodeWhenReady). */
#include "xmen2.h"

void Hologram_Explode(register Hologram *self)
{
	register short tileBase;
	register short tileCount;
	register short i;
	register Actor *o;

	o = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Actor_vtable), self->x, self->y, 0);
	if (o)
		ExplodeFx_ChainRepeat(o, CODEPTR(Tbl_HologramDeathChain), 0, -0x10, 0x20, 5);
	Hologram_SetPose(0x1B);
	self->bodySeg->flags |= 0x40;
	tileBase = self->spr.tileBase;
	tileCount = self->spr.tileCount;
	for (i = 2, o = g_ObjTable[2]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && tileBase < o->spr.tileBase) {
			Spr_InvalidateFrame(&o->spr);
			o->spr.tileBase -= tileCount;
		}
	}
	g_VramTileCursor -= tileCount;
	Hologram_SetXPowerSpawns(1);
}
