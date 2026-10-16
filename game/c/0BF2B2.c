/* SporeMine_RecArm1 - spore handler: first arming step: animation Tbl_SporeMineArmAnim1 (its
 * first step's frame shown), sound $B9 (restarted), then SporeMine_RecArm2 in 20 frames.
 *
 * s: the spore. */
#include "xmen2.h"

void SporeMine_RecArm1(register Spore *s)
{
	s->anim = (u8 *)CODEPTR(Tbl_SporeMineArmAnim1);
	s->frame = *(s->anim + 7);
	s->animStep = 0;
	s->animTime = 0;
	Snd_Stop(0xB9);
	Snd_Play(0xB9);
	s->handler = SporeMine_RecArm2;
	s->delay = 0x14;
}
