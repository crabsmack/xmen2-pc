/* Barbarian_Draw - BARBARIAN Draw (class slot +$1A): only the parent's (BIONPC) Draw.
 *
 * self: the object.
 * Whatever else it did was compiled out (an if (0) block, as in every class's Nop66): the frame
 * it left (d7 and a4 saved, self in a3) is reproduced by the unused locals below. The do/while
 * (0) keeps the call's argument pop before returning, as the ROM does. */
#include "xmen2.h"

void Barbarian_Draw(Barbarian *self)
{
	register Barbarian *p;
	short n;

	if (0) {
		p = self;
		n = p->health;
		p->health = n;
	}
	do {
		(*CLASS(Tbl_Barbarian_vtable)->parent->method[5])(self);	/* BIONPC Draw */
	} while (0);
}
