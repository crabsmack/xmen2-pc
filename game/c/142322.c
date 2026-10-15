/* TechWallGun_StateHurt - TECHWALLGUN hurt state: the hurt animation; no health left: blow up;
 * else back to aiming after the animation (or 255 frames): level for a wall gun, down for a
 * ceiling gun (its own hurt animation).
 *
 * self: the gun.
 * Returns flags bit 3 when it goes on aiming; nothing in particular otherwise. */
#include "xmen2.h"

short TechWallGun_StateHurt(TechWallGun *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1432C0));
	if (self->health <= 0) {
		TechWallGun_Explode(self);
		RETURN_NO_VALUE;
	}
	switch (self->mount) {
	case 0:
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1432C0));
		SET_STATE_WAIT(self, TechWallGun_StateAimLevel, 0xFF);
		return self->flags & 8;
	case 1:
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1432EE));
		SET_STATE_WAIT(self, TechWallGun_StateCeilAimDown, 0xFF);
		return self->flags & 8;
	case 2:
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1432C0));
		SET_STATE_WAIT(self, TechWallGun_StateAimLevel, 0xFF);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
