/* Cyclops_DrawBeam - CYCLOPS draw the optic beam: its pieces straight into the hardware sprite
 * list, cut short at the first wall, its two piece lists one after the other (shown on alternate
 * V-blanks by Cyclops_BeamVBlankFlicker).
 *
 * self: the player.  beam: his beam (&self->beam).
 * No beam (not active, no definition or no level): the flicker is told to stop (g_BeamFlickerNew
 * [3] = 0; pending again if it was on). Else the origin on screen is the player's position plus
 * the level's offset (mirrored facing left). Not probed yet (cutX 0): from the origin, in steps
 * along the beam's direction (dir 0 level 20 px, 1 up, 2 down 20 px; 3/4 the diagonals 10 px
 * each way; x mirrored facing left), up to the level's length, the map is tested for solid tiles
 * (Tile_TestAt, $7800, hooks run); a wall sets playerFlags $20 and the cut, the distance reached
 * (in x for level beams, in y for vertical ones, both for diagonals; -1: no cut). Then each list
 * (cases Cyclops_DrawBeam_StepLevel/_StepDown/_StepDiag set the step, Cyclops_DrawBeam_CutVertical
 * /_CutBoth the cut, Cyclops_DrawBeam_Emit writes the pieces, Cyclops_DrawBeam_None is the exit
 * without a beam): pieces up to the list's count or a negative tile; at the first piece of each
 * group (flags bits 0-2) the list stops when the piece's x is past cutX or |y + h/2| past cutY
 * (unsigned); pieces on screen take the next slot of the sprite list (g_SprWritePtr, until slot
 * $50: full), the beam's attribute word plus the piece's tile, its flips (and h-flip facing
 * left). count1/count2 = the pieces gone through; g_BeamFlickerNew = {slot before the beam, its
 * first slot, the second list's first slot, $FF}; after the second list one more empty sprite
 * (y and x -1) ends it, and g_BeamFlickerSpr[3] keeps the link to it. Pending is set.
 * Not MPW C's code (no byte match): it keeps variables in d2/a2 and jumps through its own case
 * tables (Tbl_BeamStepCases, Tbl_BeamCutCases): hand-written 68000 in an MPW frame. */
#include "xmen2.h"

void Cyclops_DrawBeam(Cyclops *self, CyclopsBeam *beam)
{
	BeamLevel *level;
	BeamPiece *piece;
	HwSprite *spr;
	char flip, second;
	unsigned char lastGroup;
	short ox, oy, attr, cutX, cutY, count, n;
	short stepX, stepY, step, x, y, x0, w, h, dist, length;
	unsigned char size;
	char fg;

	if (!beam->active || !beam->def || !beam->level) {
		/* Cyclops_DrawBeam_None */
		g_BeamFlickerNew[3] = 0;
		if (g_BeamFlickerOn)
			g_BeamFlickerPending = 0xFF;
		return;
	}
	level = beam->level;
	flip = self->spr.flags & 0x10;
	attr = beam->attr;
	if (!flip) {
		ox = (short)(self->x >> 8) + level->dx;
		oy = (short)(self->y >> 8) + level->dy;
	} else {
		ox = (short)(self->x >> 8) - level->dx;
		oy = (short)(self->y >> 8) + level->dy;
	}
	ox -= g_CameraX;
	oy -= g_CameraY;
	cutY = beam->cutY;
	cutX = beam->cutX;
	if (!cutX) {
		/* the wall probe (Tbl_BeamStepCases by direction) */
		switch (level->dir) {
		default:
		case 0:			/* Cyclops_DrawBeam_StepLevel */
			stepX = 20;
			stepY = 0;
			step = 20;
			break;
		case 1:
			stepX = 0;
			stepY = -20;
			step = 20;
			break;
		case 2:			/* Cyclops_DrawBeam_StepDown */
			stepX = 0;
			stepY = 20;
			step = 20;
			break;
		case 3:			/* Cyclops_DrawBeam_StepDiag */
			stepX = 10;
			stepY = -10;
			step = 10;
			break;
		case 4:
			stepX = 10;
			stepY = 10;
			step = 10;
			break;
		}
		if (flip)
			stepX = -stepX;
		x = ox + g_CameraX;
		y = oy + g_CameraY;
		fg = !g_SolidMapIsBg;
		length = level->length;
		dist = 0;
		for (;;) {
			if ((char)Tile_TestAt(self, x, y, fg, 0x7800, 1)) {
				self->playerFlags |= 0x20;
				switch (level->dir) {	/* Tbl_BeamCutCases */
				default:
				case 0:
					cutX = dist;
					cutY = -1;
					break;
				case 1:
				case 2:		/* Cyclops_DrawBeam_CutVertical */
					cutY = dist;
					cutX = -1;
					break;
				case 3:
				case 4:		/* Cyclops_DrawBeam_CutBoth */
					cutY = dist;
					cutX = dist;
					break;
				}
				break;
			}
			x += stepX;
			y += stepY;
			dist += step;
			if (length < dist) {
				cutX = -1;
				cutY = -1;
				break;
			}
		}
		beam->cutX = cutX;
		beam->cutY = cutY;
	}
	/* Cyclops_DrawBeam_Emit */
	if (flip)
		attr |= 0x800;
	spr = g_SprWritePtr;
	g_BeamFlickerNew[1] = spr->link;
	g_BeamFlickerNew[0] = spr->link - 1;
	level = beam->level;
	count = level->count1;
	piece = BEAM_PIECES(level, list1);
	second = 0;
	for (;;) {
		lastGroup = 0xFF;
		for (n = 0; n < count; n++, piece++) {
			if (piece->tile < 0)
				break;
			size = piece->size & 0xF;
			w = ((size & 0xC) + 4) * 2;
			h = ((size & 3) + 1) << 3;
			x = piece->x;
			y = piece->y;
			x0 = x;
			if (flip)
				x = -x - w;
			if ((piece->flags & 7) != lastGroup) {
				lastGroup = piece->flags & 7;
				h = (unsigned short)h >> 1;
				h += y;
				if (h < 0)
					h = -h;
				if ((unsigned short)x0 > (unsigned short)cutX)
					break;
				if ((unsigned short)h > (unsigned short)cutY)
					break;
			}
			y += oy;
			x += ox;
			if (x > -0x20 && x < 0x140 && y > -0x20 && y < 0xE0) {
				if (spr->link == 0x50)
					break;
				g_SprIndex = spr->link;
				spr++;
				g_SprWritePtr = spr;
				spr->y = y + 0x80;
				spr->size = size;
				spr->attr = (short)(attr + piece->tile) ^ (piece->flags & 0x18) << 8;
				spr->x = x + 0x80;
			}
		}
		if (second)
			break;
		second = 1;
		beam->count1 = (u8)n;
		g_BeamFlickerNew[2] = spr->link;
		level = beam->level;
		count = level->count2;
		piece = BEAM_PIECES(level, list2);
	}
	beam->count2 = (u8)n;
	g_BeamFlickerSpr[3] = spr->link;
	if (spr->link != 0x50) {
		g_SprIndex = spr->link;
		spr++;
		g_SprWritePtr = spr;
		spr->y = -1;
		spr->size = 0;
		spr->attr = 0;
		spr->x = -1;
	}
	g_BeamFlickerNew[3] = 0xFF;
	g_BeamFlickerPending = 0xFF;
}
