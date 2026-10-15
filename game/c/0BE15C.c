/* Barbarian_StateLanded - BARBARIAN after a leap: stand on the target segment
 * (Barbarian_StandOn), then idle (Barbarian_StateIdle).
 *
 * self: the object.
 * Returns self->flags & 8.
 * The original reads unk_BD here and does nothing with it: a switch with no cases left (MPW
 * loads the value into d0, the ROM's lone move.b, and tests nothing). */
#include "xmen2.h"

short Barbarian_StateLanded(Barbarian *self)
{
	Barbarian_StandOn(self, self->targetSeg);
	switch (self->unk_BD) {
	}
	SET_STATE(self, Barbarian_StateIdle);
	return self->flags & 8;
}
