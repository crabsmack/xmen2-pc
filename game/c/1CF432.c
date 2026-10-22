/* Gambit_UpdateCard - a flying card's frame: draw, move and hit-test it, then draw its sparkle
 * trail.
 *
 * self: the player.  card: the card (kind != -1).
 * On screen: one hardware sprite (size Tbl_GambitCardSize[kind], tiles cardTile +
 * Tbl_GambitCardTile[kind], centred on the card; palette 3 or 1 alternating every 2 frames,
 * priority, h-flip unless moving right, v-flip unless moving up). Moved by its velocity (twice
 * when g_TwoPlayerStep); off screen by up to 50 px it is only moved; further away it is freed.
 * A hit (Gambit_CardCollide) frees it too.
 * The trail: 1 sparkle (5 above 7 health) stepping back along the velocity from the card,
 * 8x8 sprites of the trail tiles (cardTile + $C), flips cycling 0, v, 0, h+v by frame; off-screen
 * steps are skipped, and the trail stops where it reaches Gambit (Gambit_PointNearBody).
 * The trail's flips are Gambit_UpdateCard_TrailLoop (a switch) and its loop step
 * Gambit_UpdateCard_TrailNext in the disassembly. */
#include "xmen2.h"

void Gambit_UpdateCard(register Gambit *self, register GambitCard *card)
{
	register short base = self->cardTile;
	short tile = base + *((s8 *)CODEPTR(Tbl_GambitCardTile) + card->kind);
	short trailTile = self->cardTile + 12;
	short count = self->health >= 0x701 ? 6 : 2;
	short right = g_CameraX + VIEW_R(0x140);
	register short bottom = g_CameraY + 0xE0;
	short vx = card->vx;
	short vy = card->vy;
	short hflip = vx > 0 ? 0 : 0x800;
	short vflip = vy < 0 ? 0 : 0x1000;
	register short x = card->x >> 8;
	register short y = card->y >> 8;

#if defined(XMEN2_NATIVE)
	/* phase 6 R8: what frees a card is the game's bound (the original 320 px, or R6's margin), not
	   the drawn window's, which the presentation-only view widens */
	if (!(x >= ACT_L(g_CameraX) - 50 && x <= g_CameraX + ACT_R(0x140) + 50 && y >= g_CameraY - 50 && y <= bottom + 50)) {
		card->kind = -1;
		return;
	}
#endif
	if (x >= VIEW_L(g_CameraX) && x <= right && y >= g_CameraY && y <= bottom) {
		register unsigned char size;
		short halfW, halfH, pal;

		size = ((u8 *)CODEPTR(Tbl_GambitCardSize))[card->kind];
		halfW = ((u8 *)CODEPTR(Tbl_SprSizeWidth))[size] >> 1;
		halfH = ((u8 *)CODEPTR(Tbl_SprSizeHeight))[size] >> 1;
		pal = g_FrameCounter >> 1 & 1 ? (short)0x6000 : (short)0x2000;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = (short)y - halfH - g_CameraY + 0x80;
		g_SprWritePtr->size = size;
		g_SprWritePtr->attr = tile | pal | 0x8000 | hflip | vflip;
		g_SprWritePtr->x = (short)x - halfW - g_CameraX + 0x80;
		card->x += card->vx;
		card->y += card->vy;
		if (g_TwoPlayerStep) {
			card->x += card->vx;
			card->y += card->vy;
		}
		if (Gambit_CardCollide(card)) {
			card->kind = -1;
			return;
		}
	} else if (x >= VIEW_L(g_CameraX) - 50 && x <= right + 50 && y >= g_CameraY - 50 && y <= bottom + 50) {
		card->x += card->vx;
		card->y += card->vy;
		if (g_TwoPlayerStep) {
			card->x += card->vx;
			card->y += card->vy;
		}
		if (Gambit_CardCollide(card)) {
			card->kind = -1;
			return;
		}
	} else {
		card->kind = -1;
		return;
	}
	{
		long sx, sy;
		register long n;

		sx = (x - g_CameraX) << 8;
		sy = (y - g_CameraY) << 8;
		for (n = count - 1; n; n--) {
			register short tx, ty;
			register short flip;

			sx -= vx;
			sy -= vy;
			tx = (short)(sx >> 8);
			ty = (short)(sy >> 8);
			if (tx >= VIEW_L(0) && tx <= VIEW_R(0x140) && ty >= 0 && ty <= 0xE0) {
				switch ((g_FrameCounter >> 1) + n & 3) {	/* Gambit_UpdateCard_TrailLoop */
				case 0:
					flip = 0;
					break;
				case 1:
					flip = 0x1000;
					break;
				case 2:
					flip = 0;
					break;
				case 3:
					flip = 0x1800;
					break;
				}
				if (Gambit_PointNearBody(self, card, tx + g_CameraX, ty + g_CameraY))
					return;
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = ty - 4 + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = trailTile | 0x2000 | 0x8000 | flip;
				g_SprWritePtr->x = tx - 4 + 0x80;
			}
			/* Gambit_UpdateCard_TrailNext */
			if (0) {	/* MPW: weights n, tx and ty over flip to give them the ROM's d7/d5/d6; no code */
				n = n + n + n + n;
				tx = (short)(tx + tx + tx);
				ty = (short)(ty + ty + ty);
			}
		}
	}
}
