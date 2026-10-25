/* SavageBigBio_Draw - SAVAGEBIGBIO Draw (class slot +$1A): the body (the parent's, SAVAGEBIOBASE
 * Draw), then the tentacle when it is out: its tip and one sprite per segment, straight into the
 * sprite table, each also tested against both players (SavageBigBio_TentacleHit).
 *
 * self: the boss.
 * While it flashes (flags6 bit $40) on even g_LevelFrame frames the body is drawn with palette 0
 * and the flash bit off, both put back after (palette 3). The tentacle hangs from the body
 * (73 px above it, below when v-flipped); each segment is its path point (tentacle, tentacleLen
 * of them) moved by a step of the wave (Tbl_SavageBigBioTentacleWave, one step per 2 frames,
 * the next step for the next segment, wrapping at its end marker dx >= 1000); h-flipped, x is
 * mirrored by the piece's width. Tip: a 32 x 32 sprite of the tiles from $14 at the last point
 * (7 px up, wave step of the last segment). Segments: 16 x 16 sprites of the point's tiles and
 * attribute byte (its h-flip toggled when mirrored). Drawn only on screen; the hit tests use
 * map pixels and happen also off screen. */
#include "xmen2.h"

#define g_A4TentacleHits	(*(s16 *)g_SceneVar3B06)	/* scene variable: players the tentacle touched this frame (bits 1, 2) */
#define g_A4Hit1		(*(s32 *)g_SceneVar3B18)	/* scene variable: where the tentacle touched player 1: x | y << 16 */
#define g_A4Hit2		(*(s32 *)g_SceneVar3B1C)	/* scene variable: the same for player 2 */

#define ON_SCREEN(x, y)	((x) > VIEW_L(-0x20) && (x) < VIEW_R(0x140) && (y) > -0x20 && (y) < 0xE0)

void SavageBigBio_Draw(SavageBigBio *self)
{
	short bx, by;			/* where the tentacle hangs from, on screen */
	short w;			/* the piece's width (mirrored drawing) */
	char flash;
	register TentacleWave *wave;
	register PathPt *pt;
	short attr;
	short sx, sy;
	register short i;

	bx = self->x >> 8;
	by = self->y >> 8;
	flash = 0;
	g_A4TentacleHits = 0;
	g_A4Hit1 = 0;
	g_A4Hit2 = 0;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)
		g_P34A4Hit[0] = g_P34A4Hit[1] = 0;
#endif
	if (self->flags6 & 0x40 && (g_LevelFrame & 1) == 0) {
		flash = 1;
		self->flags6 &= ~0x40;
		self->spr.flags = self->spr.flags & ~3 | 0;
	}
	(*CLASS(Tbl_SavageBigBio_vtable)->parent->method[5])(self);	/* SAVAGEBIOBASE Draw */
	if (self->tentacle) {
		bx += !(self->spr.flags & 0x10) ? 0 : 0;
		by += !(self->spr.flags & 8) ? -0x49 : 0x49;
		bx -= g_CameraX;
		by -= g_CameraY;
		/* the tip */
		wave = (TentacleWave *)CODEPTR(Tbl_SavageBigBioTentacleWave);
		i = (g_FrameCounter >> 1 & 0x1F) + (self->tentacleLen - 1);
		if (i >= 0x20)
			i -= 0x20;
		wave += i;
		attr = g_SpriteDefs[g_SavageBioSprDef].tileBase + 0x14;
		attr |= (self->spr.flags & 3) << 13;
		pt = self->tentacle + (self->tentacleLen - 1);
		if (!(self->spr.flags & 0x10)) {
			sx = bx + pt->x + wave->dx;
			sy = by + pt->y + -7 + wave->dy;
			if (ON_SCREEN(sx, sy)) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0xF;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			}
			sx += g_CameraX;
			sy += g_CameraY;
			SavageBigBio_TentacleHit(&g_Player1, 1, &g_A4Hit1, sx, sy);
			SavageBigBio_TentacleHit(&g_Player2, 2, &g_A4Hit2, sx, sy);
#if defined(XMEN2_NATIVE)
			for (int k = 3; k <= g_opt.players; k++)	/* players 3-4 in play too (R11) */
				SavageBigBio_TentacleHit(PLAYER34(k), 4 << (k - 3), &g_P34A4Hit[k - 3], sx, sy);
#endif
		} else {
			w = 0x20;
			sx = bx - (pt->x + w + wave->dx);
			sy = by + pt->y + -7 + wave->dy;
			if (ON_SCREEN(sx, sy)) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0xF;
				g_SprWritePtr->attr = attr | 0x800;
				g_SprWritePtr->x = sx + 0x80;
			}
			sx += g_CameraX;
			sy += g_CameraY;
			SavageBigBio_TentacleHit(&g_Player1, 1, &g_A4Hit1, sx, sy);
			SavageBigBio_TentacleHit(&g_Player2, 2, &g_A4Hit2, sx, sy);
#if defined(XMEN2_NATIVE)
			for (int k = 3; k <= g_opt.players; k++)	/* players 3-4 in play too (R11) */
				SavageBigBio_TentacleHit(PLAYER34(k), 4 << (k - 3), &g_P34A4Hit[k - 3], sx, sy);
#endif
		}
		/* the segments */
		wave = (TentacleWave *)CODEPTR(Tbl_SavageBigBioTentacleWave);
		wave += g_FrameCounter >> 1 & 0x1F;
		attr = g_SpriteDefs[g_SavageBioSprDef].tileBase | (self->spr.flags & 3) << 13;
		pt = self->tentacle;
		if (!(self->spr.flags & 0x10)) {
			for (i = 0; i < self->tentacleLen; i++, pt++) {
				sx = bx + pt->x + wave->dx;
				sy = by + pt->y + wave->dy;
				if (ON_SCREEN(sx, sy)) {
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = sy + 0x80;
					g_SprWritePtr->size = 5;
					g_SprWritePtr->attr = (short)((pt->tile << 2) + attr) | pt->attr << 8;
					g_SprWritePtr->x = sx + 0x80;
				}
				sx += g_CameraX;
				sy += g_CameraY;
				SavageBigBio_TentacleHit(&g_Player1, 1, &g_A4Hit1, sx, sy);
				SavageBigBio_TentacleHit(&g_Player2, 2, &g_A4Hit2, sx, sy);
#if defined(XMEN2_NATIVE)
				for (int k = 3; k <= g_opt.players; k++)	/* players 3-4 in play too (R11) */
					SavageBigBio_TentacleHit(PLAYER34(k), 4 << (k - 3), &g_P34A4Hit[k - 3], sx, sy);
#endif
				wave++;
				if (wave->dx >= 1000)
					wave = (TentacleWave *)CODEPTR(Tbl_SavageBigBioTentacleWave);
			}
		} else {
			for (i = 0; i < self->tentacleLen; i++, pt++) {
				w = ((pt->size >> 2 & 3) + 1) << 3;
				sx = bx - (pt->x + w + wave->dx);
				sy = by + pt->y + wave->dy;
				if (ON_SCREEN(sx, sy)) {
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = sy + 0x80;
					g_SprWritePtr->size = 5;
					g_SprWritePtr->attr = (short)((pt->tile << 2) + attr) | (pt->attr & 0xF0) << 8
						| (pt->attr & 8 ? 0 : 0x800);
					g_SprWritePtr->x = sx + 0x80;
				}
				sx += g_CameraX;
				sy += g_CameraY;
				SavageBigBio_TentacleHit(&g_Player1, 1, &g_A4Hit1, sx, sy);
				SavageBigBio_TentacleHit(&g_Player2, 2, &g_A4Hit2, sx, sy);
#if defined(XMEN2_NATIVE)
				for (int k = 3; k <= g_opt.players; k++)	/* players 3-4 in play too (R11) */
					SavageBigBio_TentacleHit(PLAYER34(k), 4 << (k - 3), &g_P34A4Hit[k - 3], sx, sy);
#endif
				wave++;
				if (wave->dx >= 1000)
					wave = (TentacleWave *)CODEPTR(Tbl_SavageBigBioTentacleWave);
			}
		}
	}
	if (flash) {
		self->flags6 |= 0x40;
		self->spr.flags = self->spr.flags & ~3 | 3;
	}
}
