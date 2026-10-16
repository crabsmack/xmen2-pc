/* EvilWolverine_Init - EVILWOLVERINE constructor (class slot +$12): an XCLONE that looks and fights
 * like Wolverine.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Its own sprite definition (Art_EvilWolverineSpr), Wolverine's character record, its clone record
 * (Tbl_EvilWolverineCloneRecord) and that record's main script; background palette 1. */
#include "xmen2.h"

void EvilWolverine_Init(EvilWolverine *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilWolverine_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_EvilWolverineSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilWolverineCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_WolverineCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(1);
}
