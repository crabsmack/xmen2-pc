/* EvilCyclops_Init - EVILCYCLOPS constructor (class slot +$12): an XCLONE that looks and fights like
 * Cyclops.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Cyclops's sprite definition and character record, its own clone record
 * (Tbl_EvilCyclopsCloneRecord) and that record's main script; background palette 1. */
#include "xmen2.h"

void EvilCyclops_Init(EvilCyclops *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilCyclops_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_CyclopsSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilCyclopsCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_CyclopsCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(1);
}
