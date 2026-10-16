/* SporeMine_RecArm3 - spore handler: third arming step: animation Tbl_SporeMineArmAnim3 (its
 * first step's frame shown), sound $B9 (restarted), then SporeMine_RecArm4 in 13 frames.
 *
 * s: the spore. */
#include "xmen2.h"

void SporeMine_RecArm3(register Spore *s)
{
	s->anim = (u8 *)CODEPTR(Tbl_SporeMineArmAnim3);
	s->frame = *(s->anim + 7);
	s->animStep = 0;
	s->animTime = 0;
	Snd_Stop(0xB9);
	Snd_Play(0xB9);
	s->handler = SporeMine_RecArm4;
	s->delay = 0xD;
}
