/* EvilGambit_Init - EVILGAMBIT constructor (class slot +$12): an XCLONE that looks and fights like
 * Gambit.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Its own sprite definition (Art_EvilGambitSpr), Gambit's character record, its clone record
 * (Tbl_EvilGambitCloneRecord) and that record's main script; background palette 3. */
#include "xmen2.h"

void EvilGambit_Init(EvilGambit *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilGambit_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_EvilGambitSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilGambitCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_GambitCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(3);
}
