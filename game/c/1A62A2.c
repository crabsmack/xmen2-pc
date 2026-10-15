/* Nightcrawler_WallAttackEnd - NIGHTCRAWLER after a slash on a wall: the wall pose
 * (Tbl_Anim_1A5FBE) and back to clinging (Nightcrawler_StateWallCling) from the next update.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_WallAttackEnd(Nightcrawler *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
	SET_STATE(self, Nightcrawler_StateWallCling);
	return self->flags & 8;
}
