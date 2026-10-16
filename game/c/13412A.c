/* FireWall_StateSpawnFlame - FIREWALL state: a new ACTOR at its position (its flame, class slot
 * +$0E New) runs a damaging chain explosion (ExplodeFx_ChainRepeat) along the path its size and
 * way pick (Tbl_FireWallChain2/4/6Rise or ..Drop), from 16 px left and 32 px up, repeated $FF
 * times every delay frames; then FireWall_StateWaitFlame from the next update.
 *
 * self: the object. Returns flags & 8. The flame is kept even when New made none (0). */
#include "xmen2.h"

short FireWall_StateSpawnFlame(FireWall *self)
{
	void *path;
	Actor *flame;

	switch (self->size) {
	case 2:
		if (self->rise)
			path = CODEPTR(Tbl_FireWallChain6Rise);
		else
			path = CODEPTR(Tbl_FireWallChain6Drop);
		break;
	case 1:
		if (self->rise)
			path = CODEPTR(Tbl_FireWallChain4Rise);
		else
			path = CODEPTR(Tbl_FireWallChain4Drop);
		break;
	default:
	case 0:
		if (self->rise)
			path = CODEPTR(Tbl_FireWallChain2Rise);
		else
			path = CODEPTR(Tbl_FireWallChain2Drop);
		break;
	}
	if ((flame = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0, CLASS(Tbl_Actor_vtable),
	    self->x, self->y, 0)))	/* New */
		ExplodeFx_ChainRepeat(flame, path, 1, -0x10, -0x20, self->delay << 8 | 0xFF);
	self->flame = flame;
	SET_STATE(self, (StateFn)FireWall_StateWaitFlame);
	return self->flags & 8;
}
