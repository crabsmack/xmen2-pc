/* ShootAcolyte_StateFire - SHOOTACOLYTE state: fire one gauss bullet of the burst, with the recoil
 * animation Tbl_ShootAcolyteAnim3 and sound $B0, then ShootAcolyte_StateNextShot when the
 * animation ends (delay $FF).
 *
 * self: the object.
 * The muzzle is 33 px ahead and 49 up (mirrored with bullet flag $800 when it faces left);
 * bullet flags: $8000 sprite priority, $A always. Counts the shot off shotsLeft.
 * Returns flags bit 3. */
#include "xmen2.h"

short ShootAcolyte_StateFire(register ShootAcolyte *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ShootAcolyteAnim3));
	{
		register ShootAcolyte *owner = self;
		long bx = self->x;
		register long by = self->y;
		register short dx = 0x21;
		register short dy = -0x31;
		register long flags = self->spr.flags & 4 ? 0x8000 : 0;

		if (self->spr.flags & 0x10) {
			dx = -dx;
			flags |= 0x800;
		}
		bx += (long)dx << 8;
		by += (long)dy << 8;
		flags |= 0xA;
		GaussBullet_Fire(bx, by, owner, dx, dy, flags);
	}
	Snd_Stop(0xB0);
	Snd_Play(0xB0);
	self->shotsLeft--;
	SET_STATE_WAIT(self, ShootAcolyte_StateNextShot, 0xFF);
	return self->flags & 8;
}
