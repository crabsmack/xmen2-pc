/* RThetaObject_Move - RTHETAOBJECT Move (class slot +$4E; the parent's isn't called): polar motion.
 * Speed += acceleration (kept within +-maxSpeed), turn rate += turn acceleration, heading += turn
 * rate (both kept in 0..360 degrees), then the velocity is speed along the heading and the
 * position moves by it.
 *
 * self: the object.  step: frames this move stands for (2 in two-player stepping: the
 * accelerations are applied twice; the velocity is not doubled).
 * The velocity is (cos, -sin) of the heading (8 fraction bits) times the speed, computed with no
 * multiply: the speed's whole part 0-15 picks a sum of shifts (cases 0-15, the switch
 * RThetaObject_Move_Case0/2/4 and unnamed blocks), 16 and above (or negative) gives x16
 * (RThetaObject_Move_Case16); then bits $80 / $40 of the speed add a half / a quarter. The
 * velocity before the move is kept in prevVx/Vy. */
#include "xmen2.h"

void RThetaObject_Move(register RThetaObject *self, register short step)
{
	register long cx, cy;

	self->prevVx = self->vx;
	self->prevVy = self->vy;
	self->speed += self->accel;
	if (step > 1)
		self->speed += self->accel;
	if (self->speed > self->maxSpeed)
		self->speed = self->maxSpeed;
	else if (self->speed < -self->maxSpeed)
		self->speed = -self->maxSpeed;
	self->turnRate += self->turnAccel;
	if (step > 1)
		self->turnRate += self->turnAccel;
	while (self->turnRate >= 0x16800)
		self->turnRate -= 0x16800;
	while (self->turnRate < 0)
		self->turnRate += 0x16800;
	self->heading += self->turnRate;
	if (step > 1)
		self->heading += self->turnRate;
	while (self->heading >= 0x16800)
		self->heading -= 0x16800;
	while (self->heading < 0)
		self->heading += 0x16800;
	cx = Math_CosDeg(self->heading >> 8) >> 8;
	cy = -(Math_SinDeg(self->heading >> 8) >> 8);
	switch (self->speed >> 8) {
	case 0:			/* RThetaObject_Move_Case0 */
		self->vx = 0;
		self->vy = 0;
		break;
	case 1:
		self->vx = cx;
		self->vy = cy;
		break;
	case 2:			/* RThetaObject_Move_Case2 */
		self->vx = cx << 1;
		self->vy = cy << 1;
		break;
	case 3:
		self->vx = (cx << 1) + cx;
		self->vy = (cy << 1) + cy;
		break;
	case 4:			/* RThetaObject_Move_Case4 */
		self->vx = cx << 2;
		self->vy = cy << 2;
		break;
	case 5:
		self->vx = (cx << 2) + cx;
		self->vy = (cy << 2) + cy;
		break;
	case 6:
		self->vx = (cx << 2) + (cx << 1);
		self->vy = (cy << 2) + (cy << 1);
		break;
	case 7:
		self->vx = (cx << 2) + (cx << 1) + cx;
		self->vy = (cy << 2) + (cy << 1) + cy;
		break;
	case 8:
		self->vx = cx << 3;
		self->vy = cy << 3;
		break;
	case 9:
		self->vx = (cx << 3) + cx;
		self->vy = (cy << 3) + cy;
		break;
	case 10:
		self->vx = (cx << 3) + (cx << 1);
		self->vy = (cy << 3) + (cy << 1);
		break;
	case 11:
		self->vx = (cx << 3) + (cx << 1) + cx;
		self->vy = (cy << 3) + (cy << 1) + cy;
		break;
	case 12:
		self->vx = (cx << 3) + (cx << 2);
		self->vy = (cy << 3) + (cy << 2);
		break;
	case 13:
		self->vx = (cx << 3) + (cx << 2) + cx;
		self->vy = (cy << 3) + (cy << 2) + cy;
		break;
	case 14:
		self->vx = (cx << 3) + (cx << 2) + (cx << 1);
		self->vy = (cy << 3) + (cy << 2) + (cy << 1);
		break;
	case 15:
		self->vx = (cx << 3) + (cx << 2) + (cx << 1) + cx;
		self->vy = (cy << 3) + (cy << 2) + (cy << 1) + cy;
		break;
	case 16:
	default:		/* RThetaObject_Move_Case16 */
		self->vx = cx << 4;
		self->vy = cy << 4;
		break;
	}
	if (self->speed & 0x80) {
		self->vx += cx >> 1;
		self->vy += cy >> 1;
	}
	if (self->speed & 0x40) {
		self->vx += cx >> 2;
		self->vy += cy >> 2;
	}
	self->x += self->vx;
	self->y += self->vy;
}
