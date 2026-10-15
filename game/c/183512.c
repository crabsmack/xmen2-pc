/* Beast_UpdateShockwaves - BEAST class slot +$72, the hook XPlayer_Update runs after the state:
 * XCLIMBER's, then each live stomp shockwave moves on (Beast_ShockwaveStep) until it ends.
 *
 * self: the player.
 * playerFlags $80000: the left wave runs, $100000: the right one (Beast_StateStompEnd sets
 * them); a wave that ends clears its bit. Both hit enemies, with Beast's sprite priority. */
#include "xmen2.h"

void Beast_UpdateShockwaves(register Beast *self)
{
	(*CLASS(Tbl_Beast_vtable)->parent->method[27])(self);	/* XCLIMBER slot +$72 */
	if (self->playerFlags & 0x80000) {
		if (Beast_ShockwaveStep(&self->waveLeftX, &self->waveY, &self->waveSpr, 0,
			self->spr.flags & 4 ? 1 : 0, 1))
			self->playerFlags &= ~0x80000;
	}
	if (self->playerFlags & 0x100000) {
		if (Beast_ShockwaveStep(&self->waveRightX, &self->waveY, &self->waveSpr, 1,
			self->spr.flags & 4 ? 1 : 0, 1))
			self->playerFlags &= ~0x100000;
	}
}
