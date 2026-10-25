/* Tusk_StateStartFight - TUSK: the fight starts. Pin the camera targets around its home
 * (x: home - $A0 +/- 50, y: home - $70), unlock the arena (Tusk_UnlockArena), stop the sound
 * and start the boss music ($23), give both players low priority (Tusk_PlayerToLayer0), pick
 * the first attack after 100 frames (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0.
 * The empty if (0) is a block the original compiled out: its end is a label, which keeps MPW C
 * from treating `left` in the two x stores as a common subexpression (without it MPW copies
 * left to a third register). */
#include "xmen2.h"

short Tusk_StateStartFight(Tusk *self)
{
	long left, top;

	left = self->px - 0xA0;
	top = self->py - 0x70;
	g_CamMinXTarget = (short)(left - 50);
	if (0) {				/* compiled out (see the header) */
	}
	g_CamMaxXTarget = (short)(left + 50);
	g_CamMinYTarget = (short)top;
	g_CamMaxYTarget = (short)top;
	Tusk_UnlockArena(self);
	Snd_StopAll();
	Snd_Stop(0x23);
	Snd_Play(0x23);
	Tusk_PlayerToLayer0(&g_Player1);
	Tusk_PlayerToLayer0(&g_Player2);
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		Tusk_PlayerToLayer0(q);
	});
#endif
	self->countdown = 100;
	self->state = Tusk_StateChooseAttack;
	return 0;
}
