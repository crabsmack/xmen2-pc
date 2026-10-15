/* Nightcrawler_DropThrough - NIGHTCRAWLER DropThrough (class slot +$86, Down+C): the parent's
 * (XCLIMBER, i.e. XPlayer_DropThrough), then falling at $14A.
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_DropThrough(register Nightcrawler *self)
{
	(*CLASS(Tbl_Nightcrawler_vtable)->parent->method[32])(self);	/* XCLIMBER DropThrough */
	self->vy = 0x14A;
}
