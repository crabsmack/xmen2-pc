/* EvilNightcrawler_Init - EVILNIGHTCRAWLER constructor (class slot +$12): an XCLONE that looks and
 * fights like Nightcrawler.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry (XClone_Init: the arena).
 * Its own sprite definition (Art_EvilNightcrawlerSpr), Nightcrawler's character record, its clone
 * record (Tbl_EvilNightcrawlerCloneRecord) and that record's main script; background palette 3. */
#include "xmen2.h"

void EvilNightcrawler_Init(EvilNightcrawler *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_EvilNightcrawler_vtable)->parent->method[3])(self, x, y, place);	/* XCLONE Init */
	self->spr.def = CODEPTR(Art_EvilNightcrawlerSpr);
	self->record = (CloneRecord *)CODEPTR(Tbl_EvilNightcrawlerCloneRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_NightcrawlerCharRecord);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	XClone_SetBgPalette(3);
}
