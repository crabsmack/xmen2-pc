/* EvilDrop_Move - EVILDROP Move (class slot +$4E): n times, while falling move by its speed and
 * accelerate, with ball 0 centred on it (8 px up and left), then one step of the parent's
 * (BIOBALLS) Move.
 *
 * self: the capsule.  n: steps (at least one is made). Never runs in the corpus.
 * The loop is a goto: the ROM tests n before popping the parent call's arguments, which a
 * do/while (a label before its test) doesn't give. */
#include "xmen2.h"

void EvilDrop_Move(register EvilDrop *self, register short n)
{
	register BioBall *ball;

again:
	ball = (BioBall *)(self->ballList + 1);
	if (self->falling) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
		ball->x = (self->x >> 8) - 8;
		ball->y = (self->y >> 8) - 8;
	}
	(*CLASS(Tbl_EvilDrop_vtable)->parent->method[18])(self, 1);	/* BIOBALLS Move */
	n--;
	if (n > 0)
		goto again;
}
