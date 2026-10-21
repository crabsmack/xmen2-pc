/* Launcher_Draw - LAUNCHER Draw (class slot +$1A): the parent's (ACTOR) Draw, then each shot in
 * flight moves and is drawn.
 *
 * self: the launcher.
 * A shot moves vx per step, 2 steps when g_TwoPlayerStep or g_FrameTick is set, else 1. It
 * bursts (an Explosion at it, slot freed) when it hits a player (Launcher_ShotHitsPlayer, player
 * 1 first; hit type 9 flying right, else 10) or when the FG map block under it is solid (bit
 * $4000) or it is outside the map. A shot more than $80 px left of or $C0 px right of the
 * screen is freed. Otherwise it is drawn when on screen as one 8x8 hardware sprite of the
 * shots' tiles (g_LauncherShotSprDef), palette 1, high priority. */
#include "xmen2.h"

void Launcher_Draw(register Launcher *self)
{
	short attr;
	register LauncherShot *shot;

	(*CLASS(Tbl_Launcher_vtable)->parent->method[5])(self);	/* ACTOR Draw */
	attr = g_SpriteDefs[g_LauncherShotSprDef].tileBase | 0x2000 | 0x8000;
	for (shot = self->shots; shot < &self->shots[4]; shot++) {
		if (!shot->x)
			continue;
		{
			register short steps = g_TwoPlayerStep || g_FrameTick ? 2 : 1;
			register short type = shot->vx > 0 ? 9 : 10;
			register short sx, sy;

			do
				shot->x += shot->vx;
			while (--steps);
			sx = shot->x >> 8;
			sy = shot->y >> 8;
			if (Launcher_ShotHitsPlayer(&g_Player1, sx, sy, type)
			    || Launcher_ShotHitsPlayer(&g_Player2, sx, sy, type)) {
				Explosion_Spawn(shot->x, shot->y, 0);
				shot->x = 0;
				continue;
			}
			if ((sx >> 5 >= 0 && sx >> 5 < g_FgMapW && sy >> 5 >= 0 && sy >> 5 < g_FgMapH)
			    ? (g_FgBlockMap[g_FgRowTable[sy >> 5] + (sx >> 5)] & 0x4000 ? 1 : 0) : 1) {
				Explosion_Spawn(shot->x, shot->y, 0);
				shot->x = 0;
				continue;
			}
			sx -= g_CameraX;
			sy -= g_CameraY;
			if (sx < -0x80 || sx > 0x1C0) {
				shot->x = 0;
				continue;
			}
			if (sx > VIEW_L(-0x20) && sx < VIEW_R(0x140) && sy > -0x20 && sy < 0xE0) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			}
		}
	}
}
