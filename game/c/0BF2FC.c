/* SporeMine_RecArm2 - spore handler: second arming step: animation Tbl_SporeMineArmAnim2 (its
 * first step's frame shown), sound $B9 (restarted), then SporeMine_RecArm3 in 20 frames.
 *
 * s: the spore. */
#include "xmen2.h"

void SporeMine_RecArm2(register Spore *s)
{
	s->anim = (u8 *)CODEPTR(Tbl_SporeMineArmAnim2);
	s->frame = *(s->anim + 7);
	s->animStep = 0;
	s->animTime = 0;
	Snd_Stop(0xB9);
	Snd_Play(0xB9);
	s->handler = SporeMine_RecArm3;
	s->delay = 0x14;
}
