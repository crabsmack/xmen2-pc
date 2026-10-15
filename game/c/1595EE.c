/* Magneto_SpawnShot - create a magnetic globe (MAGNETICGLOB) at Magneto's hand flying in
 * direction dir; cancel it when it would start inside a wall.
 *
 * x, y:     Magneto's position (8 fraction bits).
 * dir:      aim direction 0-9 (Magneto_ShotOriginAir in the air, Magneto_ShotOriginCrouch
 *           crouching, else Magneto_ShotOrigin: the hand offset, angle and hit type).
 * byPlayer: fired by the player (speed $100, acceleration $100, the direction's hit type), else
 *           by the MAGNETOBOSS or the EVILMAGNETO clone (speed $400, acceleration $60, hit type 8).
 * crouch, air: which origin table.  priority: the globe's sprite priority.  tiles: its VRAM tiles.
 * The globe gets placement params (byPlayer, priority, tiles), the heading, no turning, and the
 * animation for its angle (MagneticGlob_SetAnimForAngle). If the map is solid ($7800 tile
 * hooks, Tile_TestAt) one step back along its path (cos/sin of the angle >> 11), it is killed
 * (slot +$52) and 0 returned; else sound $78.
 * Returns the globe (0 when none could be created). */
#include "xmen2.h"

MagneticGlob *Magneto_SpawnShot(long x, long y, register char dir, register unsigned char byPlayer,
	unsigned char crouch, unsigned char priority, unsigned char air, short tiles)
{
	short dx, dy;
	short speed;
	short accel;
	char type;
	long angle;
	Placement place;
	register long turn;
	register long sx, sy;
	register MagneticGlob *glob;

	speed = byPlayer ? 0x100 : 0x400;
	accel = byPlayer ? 0x100 : 0x60;
	angle = 0;
	turn = 0;
	if (air)
		Magneto_ShotOriginAir(dir, &dx, &dy, &angle, &type);
	else if (crouch)
		Magneto_ShotOriginCrouch(dir, &dx, &dy, &angle, &type);
	else
		Magneto_ShotOrigin(dir, &dx, &dy, &angle, &type);
	place.param[0] = byPlayer;
	place.param[1] = priority;
	place.param[2] = tiles;
	sx = x + (dx << 8);
	sy = y - (dy << 8);
	glob = (MagneticGlob *)(*CLASS(Tbl_MagneticGlob_vtable)->method[2])(0,
		CLASS(Tbl_MagneticGlob_vtable), sx, sy, &place);		/* New */
	if (glob) {
		glob->speed = speed;
		glob->accel = accel;
		glob->heading = angle;
		glob->turnRate = turn;
		MagneticGlob_SetAnimForAngle(glob);
		if (byPlayer)
			glob->hitType = type;
		else
			glob->hitType = 8;
		{
			register short px = sx >> 8;
			register short py = sy >> 8;
			register short stepX = Math_CosDeg(angle >> 8) >> 11;
			register short stepY = -Math_SinDeg(angle >> 8) >> 11;

			if ((char)Tile_TestAt(0, px - stepX, py - stepY, !g_SolidMapIsBg, 0x7800, 1)) {
				(*glob->cls->method[19])(glob);		/* Kill */
				return 0;
			}
		}
		Snd_Stop(0x78);
		Snd_Play(0x78);
	}
	return glob;
}
