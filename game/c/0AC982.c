/* PerimeterDoor_StateCrumble - PERIMETERDOOR state once broken: every 10 frames the door's 3x3 map
 * blocks change to the next of four crumbling stages (Map_SetBlockFG), by the stage its top-left
 * block shows; after the last, PerimeterDoor_StateWaitExit from the next update.
 *
 * self: the door.
 * The stages' tiles count up from $63 in SavageA2, $66 elsewhere (the intact top-left block: $38,
 * $3A), with the top middle block of stages 2-4 the same tile and the centre and bottom middle
 * of stages 2-4 empty (0). A top-left block of no stage leaves the door as it is.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

/* The tiles of the four stages (sNx: stage N, blocks a-c top row, d-f middle, g-i bottom; s0 the
   intact top-left one), as the original computed them: counting up from first, with the shared
   top middle tile (mid). MPW keeps s3a, and s2a / s2c on their way, in registers. */
#define DOOR_TILES(intact, first) \
	s0 = intact; \
	s1a = first; \
	s1b = s1a + 1; \
	s1c = s1b + 1; \
	s2a = s1c + 1; \
	mid = s2a + 1; \
	s2b = mid; \
	s2c = mid + 1; \
	s3a = s2c + 1; \
	s3b = mid; \
	s3c = s3a + 1; \
	s4a = s3c + 1; \
	s4b = mid; \
	s4c = s4a + 1; \
	s1d = s4c + 1; \
	s1e = s1d + 1; \
	s1f = s1e + 1; \
	s2d = s1f + 1; \
	s2e = 0; \
	s2f = s2d + 1; \
	s3d = s2f + 1; \
	s3e = 0; \
	s3f = s3d + 1; \
	s4d = s3f + 1; \
	s4e = 0; \
	s4f = s4d + 1; \
	s1g = s4f + 1; \
	s1h = s1g + 1; \
	s1i = s1h + 1; \
	s2g = s1i + 1; \
	s2h = 0; \
	s2i = s2g + 1; \
	s3g = s2i + 1; \
	s3h = 0; \
	s3i = s3g + 1; \
	s4g = s3i + 1; \
	s4h = 0; \
	s4i = s4g + 1

/* the door's blocks become stage s's: column by column */
#define SET_STAGE(a, b, c, d, e, f, g, h, i) \
	Map_SetBlockFG(col, row, a, 0); \
	Map_SetBlockFG(col, row + 1, d, 0); \
	Map_SetBlockFG(col, row + 2, g, 0); \
	Map_SetBlockFG(col + 1, row, b, 0); \
	Map_SetBlockFG(col + 1, row + 1, e, 0); \
	Map_SetBlockFG(col + 1, row + 2, h, 0); \
	Map_SetBlockFG(col + 2, row, c, 0); \
	Map_SetBlockFG(col + 2, row + 1, f, 0); \
	Map_SetBlockFG(col + 2, row + 2, i, 0)

short PerimeterDoor_StateCrumble(PerimeterDoor *self)
{
	short mid;
	short cur;
	short s0;
	short s1a, s1b, s1c, s2a, s2b, s2c, s3b, s3c, s4a, s4b, s4c;
	short s1d, s1e, s1f, s2d, s2e, s2f, s3d, s3e, s3f, s4d, s4e, s4f;
	short s1g, s1h, s1i, s2g, s2h, s2i, s3g, s3h, s3i, s4g, s4h, s4i;
	register short col = self->tileCol;
	register short row = self->tileRow;
	short s3a;

	cur = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (g_ScnSavageA2 == g_SceneIndex) {
		DOOR_TILES(0x38, 0x63);
	} else {
		DOOR_TILES(0x3A, 0x66);
	}
	if (cur == s0) {
		SET_STAGE(s1a, s1b, s1c, s1d, s1e, s1f, s1g, s1h, s1i);
	} else if (cur == s1a) {
		SET_STAGE(s2a, s2b, s2c, s2d, s2e, s2f, s2g, s2h, s2i);
	} else if (cur == s2a) {
		SET_STAGE(s3a, s3b, s3c, s3d, s3e, s3f, s3g, s3h, s3i);
	} else if (s3a == cur) {
		SET_STAGE(s4a, s4b, s4c, s4d, s4e, s4f, s4g, s4h, s4i);
		SET_STATE(self, PerimeterDoor_StateWaitExit);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, PerimeterDoor_StateCrumble, 10);
	return self->flags & 8;
}
