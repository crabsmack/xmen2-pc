/* PortraitActor_StateInit - SCALINGACTOR first state: the portrait shows its slot's character
 * (select-screen sprite and frame; none: frame $FF), standing at its player's side (x $48 player
 * 1, $F8 player 2 facing left; y $AE on screen), its flight picture prepared
 * (PlayerSelect_CalcPortraitPos) and its sprite set up (Spr_Init, streamed into its tiles). Then
 * PortraitActor_StateIdle from the next update.
 * Native, three or more players: each player's column (P34_SelectHomeX), players 3-4 facing left,
 * 8 px higher (SELECT_FACES_LEFT, SELECT_HOME_Y; the same in the other portrait states).
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateInit(register ScalingActor *self)
{
	if ((self->spr.def = PlayerSelect_CharSpriteDef(self->slot)) != 0)
		self->frame = PlayerSelect_CharFrameNum(self->slot);
	else
		self->frame = 0xFF;
	self->spr.frame = self->frame;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)	/* phase 7: four columns, players 1-4 from the left */
		self->homeX = (unsigned char)P34_SelectHomeX(self->slot->player);
	else
#endif
	self->homeX = (unsigned char)(self->slot->player == 1 ? 0x48 : 0xF8);
	PlayerSelect_CalcPortraitPos(self);
	Spr_Init(&self->spr, 1, self->spr.def, self->frame, self->spr.flags & 3, self->spr.tileBase, 0);
	if (SELECT_FACES_LEFT(self))
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->x = (self->homeX + g_CameraX) << 8;
	self->y = (g_CameraY + SELECT_HOME_Y) << 8;
	SET_STATE(self, PortraitActor_StateIdle);
	return self->flags & 8;
}
