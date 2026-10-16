/* EvilBeast_Init - EVILBEAST constructor (class slot +$12): an XCLONE that looks and fights like
 * Beast.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Beast's sprite definition and character record, its own clone record (Tbl_EvilBeastCloneRecord)
 * and that record's main script; background palette 1; no stomp shockwave running. */
#include "xmen2.h"

void EvilBeast_Init(EvilBeast *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilBeast_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_BeastSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilBeastCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_BeastCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(1);
	self->waves = 0;
}
