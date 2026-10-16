/* EvilPsylocke_Init - EVILPSYLOCKE constructor (class slot +$12): an XCLONE that looks and fights
 * like Psylocke.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Its own sprite definition (Art_EvilPsylockeSpr), Psylocke's character record, its clone record
 * (Tbl_EvilPsylockeCloneRecord) and that record's main script; background palette 1. */
#include "xmen2.h"

void EvilPsylocke_Init(EvilPsylocke *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilPsylocke_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_EvilPsylockeSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilPsylockeCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_PsylockeCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(1);
}
