/* Cyclops_StartBeam - CYCLOPS fire the optic beam: a beam set for the direction, its strength level
 * from his health and meter, 14 frames of beam.
 *
 * obj:   the player (copied to the register self after rec: the ROM loads rec first).
 * rec:   the beam set (BeamSet, Tbl_CyclopsBeam<Ground|Air|Crouch><Dir>).
 * vflip: draw the beam v-flipped (the ground down-diagonal reuses the up-diagonal set).
 * A beam still out ends first (Cyclops_EndBeam). The sprite: Art_CyclopsBeamVertSpr for the
 * up/down sets, Art_CyclopsBeamDiagSpr for the diagonals, else Art_CyclopsBeamLevelSpr. Sound
 * $A2 with a full meter (playerFlags $800), else $A1. Level 0-2: +1 above 7 health units, +1
 * with a full meter; attack strength 1 << level, its BeamLevel, active, actionTimer = 14, the level's
 * hit type (9 becomes 10 facing right). Spr_Init in the VRAM tiles $2B after his own; the beam
 * takes his facing and priority; its pieces' attribute word is its tiles with palette 1 (and
 * priority); origin on screen from the level's offset (mirrored facing left). */
#include "xmen2.h"

void Cyclops_StartBeam(Cyclops *obj, register void *rec, char vflip)
{
	register Cyclops *self = obj;
	register short level;
	register short tile;

	level = 0;
	if (self->beam.def)
		Cyclops_EndBeam(self);
	if (rec == CODEPTR(Tbl_CyclopsBeamGroundUp) || rec == CODEPTR(Tbl_CyclopsBeamAirUp)
	    || rec == CODEPTR(Tbl_CyclopsBeamAirDown) || rec == CODEPTR(Tbl_CyclopsBeamGroundDown)) {
		self->beam.frame = 0;
		self->beam.def = CODEPTR(Art_CyclopsBeamVertSpr);
	} else if (rec == CODEPTR(Tbl_CyclopsBeamGroundUpDiag)
		   || rec == CODEPTR(Tbl_CyclopsBeamGroundDownDiag)
		   || rec == CODEPTR(Tbl_CyclopsBeamAirUpDiag)
		   || rec == CODEPTR(Tbl_CyclopsBeamAirDownDiag)) {
		self->beam.frame = 1;
		self->beam.def = CODEPTR(Art_CyclopsBeamDiagSpr);
	} else {
		self->beam.frame = 2;
		self->beam.def = CODEPTR(Art_CyclopsBeamLevelSpr);
	}
	if (self->playerFlags & 0x800) {
		Snd_Stop(0xA2);
		Snd_Play(0xA2);
	} else {
		Snd_Stop(0xA1);
		Snd_Play(0xA1);
	}
	if (self->health >= 0x701)
		++level;
	if (self->playerFlags & 0x800)
		++level;
	self->attack = 1 << level;
	self->beam.level = (BeamLevel *)((char *)rec + *(((BeamSet *)rec)->level + level));
	self->beam.active = 1;
	self->actionTimer = 14;
	self->hitType = self->beam.level->hitType;
	tile = Spr_Init((SprInst *)&self->beam, 1, self->beam.def, level, 1, self->spr.tileBase + 0x2B, 1);
	if (vflip)
		self->beam.flags |= 8;
	else
		self->beam.flags &= ~8;
	if (self->spr.flags & 0x10)
		self->beam.flags |= 0x10;
	else {
		self->beam.flags &= ~0x10;
		if (self->hitType == 9)
			self->hitType = 10;
	}
	if (self->spr.flags & 4)
		self->beam.flags |= 4;
	else
		self->beam.flags &= ~4;
	self->beam.frameCount = 0;
	self->beam.attr = self->beam.tileBase | 0x2000;
	if (self->spr.flags & 4)
		self->beam.attr |= 0x8000;
	if (!(self->spr.flags & 0x10)) {
		self->beam.originX = (self->x >> 8) + self->beam.level->dx;
		self->beam.originY = (self->y >> 8) + self->beam.level->dy;
	} else {
		self->beam.originX = (self->x >> 8) - self->beam.level->dx;
		self->beam.originY = (self->y >> 8) + self->beam.level->dy;
	}
	self->beam.originX -= g_CameraX;
	self->beam.originY -= g_CameraY;
}
