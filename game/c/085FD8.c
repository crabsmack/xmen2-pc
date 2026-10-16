/* BroodBossWall_Init - BROODBOSSWALL Init (ICANDY slot +$0E): the parent's (ICANDY) Init, then the
 * wall's sprite definition (g_WallSprDef) and animation (Tbl_BroodBossWallAnim), 6 blocks high,
 * not smashed, no solid block yet (BroodBossWall_Update makes it).
 *
 * self: the wall.  x, y: position (8 fraction bits). */
#include "xmen2.h"

#define g_WallSprDef	(*(s16 *)g_SceneVar3B08)	/* scene variable (CloneA5): sprite-definition slot of Art_BroodBossWallSpr, the wall and the clone debris */

void BroodBossWall_Init(BroodBossWall *self, long x, long y)
{
	(*CLASS(Tbl_BroodBossWall_vtable)->parent->method[2])(self, x, y);	/* ICANDY Init */
	ICandy_SetSpriteDef(self, g_WallSprDef);
	ICandy_SetAnim(self, CODEPTR(Tbl_BroodBossWallAnim));
	self->height = 6;
	self->smashed = 0;
	self->seg = 0;
}
