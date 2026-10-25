/* PerimeterDoor_TileHit - PERIMETERDOOR class slot +$6E (PerimeterDoor_TileHook passes it the hits
 * on its map tiles): crack the door.
 *
 * self:     the door.
 * col, row: the map block hit.
 * obj:      what hit it (a player gets a 10-frame cooldown of its own).
 * Nothing while the door's hit cooldown runs; else 10 frames of it. Below 9 hits, the block hit
 * (one of the door's 3x3) becomes its cracked tile (SavageA2 $38-$3A / $3E-$40 / $45-$47 by row,
 * other scenes $3A-$3C / $41-$43 / $48-$4A; Map_SetBlockFG) and counts a hit, with a puff and
 * two delayed puffs (Explosion_Spawn / _SpawnSetTimerWord); the 9th hit makes the opening a solid
 * floor segment (Seg_Create type 3, blockId) and marks the 3x3 blocks ($1800); later hits only
 * count. Then sound $38, unless obj is a live player whose own hit cooldown still runs (a player
 * that hit resets it to 10). The block hit is read and not used. */
#include "xmen2.h"

void PerimeterDoor_TileHit(register PerimeterDoor *self, register short col, register short row, register Actor *obj)
{
	Seg *seg;
	short block;
	short t00, t01, t02;		/* cracked tiles: row 0 */
	short t10, t11, t12;		/* row 1 */
	short t22;
	register short t20, t21;	/* row 2 */

	block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (self->hitCooldown > 0)
		return;
	if (g_ScnSavageA2 == g_SceneIndex) {
		t00 = 0x38;
		t01 = t00 + 1;
		t02 = t01 + 1;
		t10 = 0x3E;
		t11 = t10 + 1;
		t12 = t11 + 1;
		t20 = 0x45;
		t21 = t20 + 1;
		t22 = t21 + 1;
	} else {
		t00 = 0x3A;
		t01 = t00 + 1;
		t02 = t01 + 1;
		t10 = 0x41;
		t11 = t10 + 1;
		t12 = t11 + 1;
		t20 = 0x48;
		t21 = t20 + 1;
		t22 = t21 + 1;
	}
	self->hitCooldown = 10;
	if (g_PerimeterDoorHits < 9) {
		if (col == self->tileCol) {
			switch (row - self->tileRow) {
			case 0:
				Map_SetBlockFG(col, row, t00, 0);
				g_PerimeterDoorHits++;
				break;
			case 1:
				Map_SetBlockFG(col, row, t10, 0);
				g_PerimeterDoorHits++;
				break;
			case 2:
				Map_SetBlockFG(col, row, t20, 0);
				g_PerimeterDoorHits++;
				break;
			}
		} else if (col == self->tileCol + 1) {
			switch (row - self->tileRow) {
			case 0:
				Map_SetBlockFG(col, row, t01, 0);
				g_PerimeterDoorHits++;
				break;
			case 1:
				Map_SetBlockFG(col, row, t11, 0);
				g_PerimeterDoorHits++;
				break;
			case 2:
				Map_SetBlockFG(col, row, t21, 0);
				g_PerimeterDoorHits++;
				break;
			}
		} else if (col == self->tileCol + 2) {
			switch (row - self->tileRow) {
			case 0:
				Map_SetBlockFG(col, row, t02, 0);
				g_PerimeterDoorHits++;
				break;
			case 1:
				Map_SetBlockFG(col, row, t12, 0);
				g_PerimeterDoorHits++;
				break;
			case 2:
				Map_SetBlockFG(col, row, t22, 0);
				g_PerimeterDoorHits++;
				break;
			}
		}
		Explosion_Spawn(((col << 5) + 8) << 8, ((row << 5) + 8) << 8, 0);
		Explosion_SpawnSetTimerWord(((col << 5) + 0x10) << 8, ((row << 5) + 0x18) << 8, 0, 4);
		Explosion_SpawnSetTimerWord(((col << 5) + 0x18) << 8, ((row << 5) + 8) << 8, 0, 8);
		if (g_PerimeterDoorHits == 9) {
			seg = Seg_Create(3, self->blockId, self->tileCol << 5, (self->tileRow << 5) + 0x60,
				(self->tileCol << 5) + 0x60, (self->tileRow << 5) + 0x60);
			for (col = self->tileCol; col <= self->tileCol + 2; col++)
				for (row = self->tileRow; row <= self->tileRow + 2; row++)
					g_FgBlockMap[g_FgRowTable[row] + col] |= 0x1800;
		}
	} else
		g_PerimeterDoorHits++;
	if (IS_ALIVE(&g_Player1) && obj == (Actor *)&g_Player1) {
		if (g_Player1.hitCooldown > 0)
			return;
		g_Player1.hitCooldown = 10;
	} else if (IS_ALIVE(&g_Player2) && obj == (Actor *)&g_Player2) {
		if (g_Player2.hitCooldown > 0)
			return;
		g_Player2.hitCooldown = 10;
	}
#if defined(XMEN2_NATIVE)
	else if (P34_IS_PLAYER(obj) && IS_ALIVE(obj)) {	/* players 3-4 too (R11) */
		if (obj->hitCooldown > 0)
			return;
		obj->hitCooldown = 10;
	}
#endif
	Snd_Stop(0x38);
	Snd_Play(0x38);
}
