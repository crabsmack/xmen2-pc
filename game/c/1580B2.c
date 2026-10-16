/* MagneticGlob_SetAnimForAngle - pick the globe's flight animation and h-flip for its heading,
 * in sectors around the 8 directions (degrees): right Tbl_MagneticGlobAnim19, up-right
 * Tbl_MagneticGlobDiagAnim, up Tbl_MagneticGlobVertAnim, up-left the diagonal flipped, left
 * Anim19 flipped, down-left Tbl_MagneticGlobAnim18 flipped, down Tbl_MagneticGlobAnim20 flipped,
 * down-right Anim18; anything else (including negative headings) right.
 *
 * self: the globe (Magneto_SpawnShot). */
#include "xmen2.h"

void MagneticGlob_SetAnimForAngle(register MagneticGlob *self)
{
	register short a = self->heading >> 8;

	if (a >= 0 && a < 0x16) {
		self->hflip = 0;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim19);
	} else if (a >= 0x16 && a < 0x44) {
		self->hflip = 0;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobDiagAnim);
	} else if (a >= 0x44 && a < 0x70) {
		self->hflip = 0;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobVertAnim);
	} else if (a >= 0x70 && a < 0x9E) {
		self->hflip = 1;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobDiagAnim);
	} else if (a >= 0x9E && a < 0xCA) {
		self->hflip = 1;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim19);
	} else if (a >= 0xCA && a < 0xF8) {
		self->hflip = 1;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim18);
	} else if (a >= 0xF8 && a < 0x124) {
		self->hflip = 1;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim20);
	} else if (a >= 0x124 && a < 0x152) {
		self->hflip = 0;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim18);
	} else {
		self->hflip = 0;
		self->flyAnim = CODEPTR(Tbl_MagneticGlobAnim19);
	}
}
