/* Explosion_Update - an explosion puff's timer (every frame): step its animation, draw it, hurt
 * what is in its box.
 *
 * e: the slot (the timer's argument).
 * When the step's frames run out the next step starts (4 frames each, counted 1 a frame, 2 in
 * two-player stepping); after step 4 the puff ends (Explosion_Free). A step shows the 16 tiles
 * at g_FxTileBase + step * 16. The sprite (32x32, size $F) is emitted when its top-left is on
 * screen (-32 < x < 320, -32 < y < 224). A hurting puff runs Explosion_HurtActor on the enemy
 * and the player body lists over its 32x32 box. */
#include "xmen2.h"

void Explosion_Update(register Explosion *e)
{
	if (e->delay <= 0) {
		e->delay = 4;
		e->step++;
		if (e->step > 4) {
			Explosion_Free(e);
			return;
		}
		e->attr &= 0xF800;
		e->attr |= g_FxTileBase + (e->step << 4);
	}
	{
		register short sx = e->x - g_CameraX;
		register short sy = e->y - g_CameraY;

		if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xF;
			g_SprWritePtr->attr = e->attr;
			g_SprWritePtr->x = sx + 0x80;
		}
	}
	if (e->hurts) {
		ObjList_ForEachInBox(e->x, e->y, e->x + 32, e->y + 32, g_HitListEnemyBody,
			FN_DATA(Explosion_HurtActor), 0);
		ObjList_ForEachInBox(e->x, e->y, e->x + 32, e->y + 32, g_HitListPlayerBody,
			FN_DATA(Explosion_HurtActor), 0);
	}
	e->delay -= g_TwoPlayerStep ? 2 : 1;
}
