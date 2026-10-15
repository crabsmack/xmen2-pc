/* Magneto_SpawnFluxBomb - create a flux bomb (FLUXBOMB) at Magneto's hand flying in direction dir.
 *
 * x, y:     Magneto's position (8 fraction bits).
 * dir:      aim direction 1-8 (Magneto_BombOriginCrouch crouching, else Magneto_BombOrigin: the
 *           hand offset and the angle).
 * byPlayer: thrown by the player (speed $200, no acceleration), else speed $400, acceleration
 *           -$40 (no other caller passes 0).
 * crouch:   which origin table.  strong: the bomb's strong flag (health above $700).
 * The bomb gets placement param 0 = byPlayer, the heading and no turning.
 * Returns the bomb (0 when none could be created). */
#include "xmen2.h"

FluxBomb *Magneto_SpawnFluxBomb(long x, long y, register char dir, register unsigned char byPlayer,
	unsigned char crouch, unsigned char strong)
{
	long angle;
	Placement place;
	short dx, dy;
	register long speed, accel;
	register FluxBomb *bomb;

	if (byPlayer) {
		speed = 0x200;
		accel = 0;
	} else {
		speed = 0x400;
		accel = -0x40;
	}
	if (crouch)
		Magneto_BombOriginCrouch(dir, &dx, &dy, &angle);
	else
		Magneto_BombOrigin(dir, &dx, &dy, &angle);
	place.param[0] = byPlayer;
	bomb = (FluxBomb *)(*CLASS(Tbl_FluxBomb_vtable)->method[2])(0, CLASS(Tbl_FluxBomb_vtable),
		x + (dx << 8), y - (dy << 8), &place);			/* New */
	if (bomb) {
		bomb->speed = (short)speed;
		bomb->accel = (short)accel;
		bomb->heading = angle;
		bomb->turnRate = 0;
		bomb->strong = strong;
	}
	return bomb;
}
