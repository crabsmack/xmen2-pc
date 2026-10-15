/* Deathbird_MovePolar - move by speed and heading (DEATHBIRD circling, Deathbird_Move kind 4),
 * for a number of steps.
 *
 * Each step: keep the old velocity in prevVx/prevVy; speed += accel, kept within -/+maxSpeed;
 * turnRate += turnAccel and heading += turnRate, both wrapped into 0-360 degrees (8 fraction
 * bits); velocity = speed x (cos, -sin) of the heading; position += velocity.
 * The multiply is done the way the original did it, by shifts and adds of the unit vector
 * (cos/sin >> 8) for the whole part of speed (speed >> 8: 0-16, anything else as 16), plus a
 * half and a quarter for speed bits 7 and 6 (finer bits are dropped).
 *
 * speed, accel, maxSpeed: words (the object's).  heading, turnRate, turnAccel: longs (globals).
 * x, y: the object's position (8 fraction bits).  vx, vy, prevVx, prevVy: its velocities.
 * steps: at least 1.
 * The cases are Deathbird_MovePolar_Case0 (0-2), _Case3, _Case4 (4-15) and _Case16 (16 and
 * anything else, then the step's end). */
#include "xmen2.h"

void Deathbird_MovePolar(s16 *speed, s16 *accel, s16 *maxSpeed, register s32 *heading,
	register s32 *turnRate, s32 *turnAccel, s32 *x, s32 *y, s16 *vx, s16 *vy,
	s16 *prevVx, s16 *prevVy, register short steps)
{
	register long c;	/* the heading's unit vector, 8 fraction bits */
	register long s;

	do {
		*prevVx = *vx;
		*prevVy = *vy;
		*speed += *accel;
		if (*speed > *maxSpeed)
			*speed = *maxSpeed;
		else if (*speed < -*maxSpeed)
			*speed = -*maxSpeed;
		*turnRate += *turnAccel;
		while (*turnRate >= 360L << 8)
			*turnRate -= 360L << 8;
		while (*turnRate < 0)
			*turnRate += 360L << 8;
		*heading += *turnRate;
		while (*heading >= 360L << 8)
			*heading -= 360L << 8;
		while (*heading < 0)
			*heading += 360L << 8;
		c = Math_CosDeg(*heading >> 8) >> 8;
		s = -(Math_SinDeg(*heading >> 8) >> 8);
		switch (*speed >> 8) {
		case 0:
			*vx = 0;
			*vy = 0;
			break;
		case 1:
			*vx = (short)c;
			*vy = (short)s;
			break;
		case 2:
			*vx = (short)(c << 1);
			*vy = (short)(s << 1);
			break;
		case 3:
			*vx = (short)((c << 1) + c);
			*vy = (short)((s << 1) + s);
			break;
		case 4:
			*vx = (short)(c << 2);
			*vy = (short)(s << 2);
			break;
		case 5:
			*vx = (short)((c << 2) + c);
			*vy = (short)((s << 2) + s);
			break;
		case 6:
			*vx = (short)((c << 2) + (c << 1));
			*vy = (short)((s << 2) + (s << 1));
			break;
		case 7:
			*vx = (short)((c << 2) + (c << 1) + c);
			*vy = (short)((s << 2) + (s << 1) + s);
			break;
		case 8:
			*vx = (short)(c << 3);
			*vy = (short)(s << 3);
			break;
		case 9:
			*vx = (short)((c << 3) + c);
			*vy = (short)((s << 3) + s);
			break;
		case 10:
			*vx = (short)((c << 3) + (c << 1));
			*vy = (short)((s << 3) + (s << 1));
			break;
		case 11:
			*vx = (short)((c << 3) + (c << 1) + c);
			*vy = (short)((s << 3) + (s << 1) + s);
			break;
		case 12:
			*vx = (short)((c << 3) + (c << 2));
			*vy = (short)((s << 3) + (s << 2));
			break;
		case 13:
			*vx = (short)((c << 3) + (c << 2) + c);
			*vy = (short)((s << 3) + (s << 2) + s);
			break;
		case 14:
			*vx = (short)((c << 3) + (c << 2) + (c << 1));
			*vy = (short)((s << 3) + (s << 2) + (s << 1));
			break;
		case 15:
			*vx = (short)((c << 3) + (c << 2) + (c << 1) + c);
			*vy = (short)((s << 3) + (s << 2) + (s << 1) + s);
			break;
		default:
		case 16:
			*vx = (short)(c << 4);
			*vy = (short)(s << 4);
			break;
		}
		if (*speed & 0x80) {
			*vx += c >> 1;
			*vy += s >> 1;
		}
		if (*speed & 0x40) {
			*vx += c >> 2;
			*vy += s >> 2;
		}
		*x += *vx;
		*y += *vy;
	} while (--steps > 0);
}
