/* EvilMagneto_Init - EVILMAGNETO constructor (class slot +$12): an XCLONE that looks and fights like
 * Magneto.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Its own sprite definition (Art_EvilMagnetoSpr), Magneto's character record, its clone record
 * (Tbl_EvilMagnetoCloneRecord) and that record's main script; background palette 2; no globe
 * direction yet (-1). */
#include "xmen2.h"

void EvilMagneto_Init(EvilMagneto *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilMagneto_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_EvilMagnetoSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilMagnetoCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_MagnetoCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(2);
	self->shotDir = -1;
}
