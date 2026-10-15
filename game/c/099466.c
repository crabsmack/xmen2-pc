/* XClone_TileHookReward - clone-maker map tile hook type 6: once a clone was killed by the same
 * character as the object touching the tile (clone flags bit 8 + Clone_CharTypeOf(obj),
 * XClone_TakeDamage), and the reward timer is out, mark the reward given (clone flags bit 1,
 * which XClone_TileHookOpenWall waits for), restart the timer ($A0 frames) and set off the wall
 * blast chain (Tbl_XCloneWallBlastChain, high priority, harmless, once) at the view's centre.
 *
 * col, row: the tile (unused).
 * obj:      the object touching it (a player).
 * Returns 0 when nothing happens; otherwise nothing in particular. */
#include "xmen2.h"

#define g_CloneFlags		(*(s16 *)g_SceneVar3B08)	/* scene variable: 1 arena locked, 2 reward given, $100 << type killed by its own character */
#define g_CloneRewardTimer	(*(s16 *)g_SceneVar3B06)	/* scene variable: frames before the next reward */

long XClone_TileHookReward(short col, short row, Actor *obj)
{
	char reward;
	short repeats;
	short type;
	Actor *fx;

	reward = 0;
	if (g_CloneRewardTimer > 0)
		return 0;
	type = Clone_CharTypeOf(obj);
	if (g_CloneFlags & 1L << (type + 8))
		reward = 1;
	if (!reward)
		return 0;
	g_CloneFlags |= 2;
	g_CloneRewardTimer = 0xA0;
	repeats = 1;
	fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_Actor_vtable), (g_CameraX + 0xA0) << 8, (g_CameraY + 0x70) << 8, 0);
	if (fx) {
		fx->spr.flags |= 4;
		ExplodeFx_ChainRepeat(fx, CODEPTR(Tbl_XCloneWallBlastChain), 0, -0x10, -0x10, repeats);
	}
	NO_RETURN_VALUE
}
