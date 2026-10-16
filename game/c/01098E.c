/* BigAcolyte_ActAppear - BIGACOLYTE intro script action (step 2): appear in the wall as a small
 * scaled picture, then the next step of the script on the next update.
 *
 * self: the boss.
 * Creates the wall's debris pool (36 pieces, floor at y $400) with its sprite definition
 * (BigAcolyte_LoadSprite, also re-run whenever sprites are reloaded); the boss's sprite takes
 * the $4D VRAM tiles after the debris tiles (streamed). Art_BigAcolytePicture becomes the
 * scaler source, scaled to its own size (width made even) in a 7x11 tile grid and rendered
 * into the boss's tiles (BigAcolyte_RenderScaled, first call). From now on it is drawn as the
 * picture. */
#include "xmen2.h"

void BigAcolyte_ActAppear(register BigAcolyte *self)
{
	register SprInst *spr;
	register short tile;
	SprInst *def;
	ScaleParams sp;

	spr = &self->spr;
	g_BigAcolyteDebris = Debris_CreatePool(0x24, 0, 0);
	Spr_ReloadDefs(BigAcolyte_LoadSprite, 0);
	g_BigAcolyteDebris->floorY = 0x400;
	def = &g_SpriteDefs[g_BigAcolyteDebris->sprDef];
	tile = def->tileBase + def->tileCount;
	spr->tileBase = tile;
	spr->tileCount = 0x4D;
	spr->flags |= 0x80;
	(void)(tile + 0x4D);		/* the end of its tiles, computed and unused */
	{
		register ScalePicture *pic = (ScalePicture *)CODEPTR(Art_BigAcolytePicture);

		g_ScaleSrcBitmap = pic;
		sp.srcW = pic->right - pic->left;
		if (sp.srcW & 1)
			sp.srcW++;
		sp.srcH = pic->bottom - pic->top;
		sp.srcX = 0;
		sp.srcY = 0;
		sp.rectW = sp.srcW;
		sp.rectH = sp.srcH;
		sp.tilesW = 7;
		sp.tilesH = 0xB;
		sp.outW = sp.srcW;
		sp.outH = sp.srcH;
		sp.clearMode = 1;
		sp.tableKey = 1;
		sp.hflip = 0;
		sp.vflip = 0;
		sp.transpose = 0;
		sp.direct = 1;
		sp.packed = 0;
		sp.rowsPerCall = 0;
		sp.unread_1E = 0;
		sp.unread_20 = 0;
		sp.unread_22 = 0;
		sp.unread_24 = 0;
		BigAcolyte_RenderScaled(pic, &sp, self->spr.tileBase << 5, 1);
	}
	self->picture = 1;
	AI_NEXT_STEP(self, self->ai, 0);
}
