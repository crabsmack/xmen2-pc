/* ExplodeFx_ChainUpdate - a chain's timer, once a frame: count down the path delay (2 a frame in
 * two-player stepping) and place the next explosions when it runs out (ExplodeFx_ChainStep);
 * each live explosion dies outside the deactivation window (g_DeactWin*, relative to the
 * screen) or after its 7th picture (6 frames each), and a damaging chain's hurt what touches
 * their 32x32 box (ExplodeFx_ChainHit, the enemy then the player body lists). With none live,
 * the effect ends (ExplodeFx_Free) once its path is due (delay 0) or its origin is outside the
 * window.
 *
 * fx: the chain (its 27 FxBlast follow the header).
 * MPW: the loop is written with labels: they keep gone and away in registers of their own (a
 * for loop lets them share one and moves frame into the other). */
#include "xmen2.h"

void ExplodeFx_ChainUpdate(register ExplodeFxChain *fx)
{
	short i;
	char live;
	short ticks;
	short frame;
	register FxBlast *b;
	register char gone;
	register char away;
	register short sx, sy;

	i = 0;
	b = (FxBlast *)(fx + 1);
	live = 0;
	if (fx->soundDelay > 0)
		fx->soundDelay--;
	if (fx->delay > 0)
		fx->delay -= g_TwoPlayerStep ? 2 : 1;
	if (fx->step && fx->delay <= 0)
		ExplodeFx_ChainStep(fx);
loop:
	if (i >= 27)
		goto done;
	if (!b->live)
		goto next;
	gone = 0;
	sx = b->x - g_CameraX;
	sy = b->y - g_CameraY;
	if (sx > g_DeactWinRight)
		gone = 1;
	else if (sx < g_DeactWinLeft)
		gone = 1;
	else if (sy > g_DeactWinBottom)
		gone = 1;
	else if (sy < g_DeactWinTop)
		gone = 1;
	if (gone) {
		b->live = 0;
		goto next;
	}
	ticks = b->anim & 0xF;
	frame = b->anim >> 4;
	ticks -= g_TwoPlayerStep ? 2 : 1;
	if (ticks <= 0) {
		frame++;
		if (frame > 6) {
			b->live = 0;
			goto next;
		}
		b->anim = frame << 4 | 6;
	} else {
		b->anim &= 0xF0;
		b->anim |= ticks & 0xF;
	}
	if (fx->chainFlags & 1) {
		ObjList_ForEachInBox(b->x, b->y, b->x + 32, b->y + 32, g_HitListEnemyBody,
			FN_DATA(ExplodeFx_ChainHit), 0);
		ObjList_ForEachInBox(b->x, b->y, b->x + 32, b->y + 32, g_HitListPlayerBody,
			FN_DATA(ExplodeFx_ChainHit), 0);
	}
	live = 1;
next:
	i++;
	b++;
	goto loop;
done:
	if (!live) {
		away = 0;
		sx = fx->x - g_CameraX;
		sy = fx->y - g_CameraY;
		if (sx > g_DeactWinRight)
			away = 1;
		else if (sx < g_DeactWinLeft)
			away = 1;
		else if (sy > g_DeactWinBottom)
			away = 1;
		else if (sy < g_DeactWinTop)
			away = 1;
		if (fx->delay <= 0 || away)
			ExplodeFx_Free((ExplodeFx *)fx);
	}
}
