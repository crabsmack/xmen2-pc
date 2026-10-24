/* SavageBio_StateDeathExplode - SAVAGEBIGBIO death: every 16 frames blow up one ball of the
 * BIOBALLS companion, last ball first; when the countdown ends, end the scene in $F0 frames,
 * let the camera go and remove itself.
 *
 * self: the boss (after SavageBio_StateDeathWait set the countdown +$6E to $F0).
 * Each explosion is ExplodeFx_BurstMedium on a plain ACTOR made at the ball, and the ball is
 * marked gone (-1, -1).
 * Returns self->flags & 8 while exploding; nothing in particular at the end. */
#include "xmen2.h"

short SavageBio_StateDeathExplode(SavageBigBio *self)
{
	BioBalls *balls;
	Actor *fx;
	BioBall *b;
	short count, i;

	balls = self->balls;
	if (self->countdown <= 0) {
		Scene_EndAfter(0xF0);
		*(PTR(Actor) *)&g_SceneVar3B20 = 0;	/* the object the SavageA4 camera follows */
		(*self->cls->method[19])(self);		/* Kill */
	} else {
		if (!(self->countdown & 0xF) && balls) {
			b = (BioBall *)(balls->ballList + 1);
			count = *balls->ballList;
			if ((i = count - (self->countdown >> 4)) >= 0) {
				b += i;
				fx = (Actor *)(*CLASS(Tbl_Actor_vtable)->method[2])(0,	/* New */
					CLASS(Tbl_Actor_vtable), b->x << 8, b->y << 8, 0);
				if (fx)
					ExplodeFx_BurstMedium(fx);
				b->x = -1;
				b->y = -1;
			}
		}
		SET_STATE(self, SavageBio_StateDeathExplode);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
