/* SporeMine_RecArm4 - spore handler: last arming step: animation Tbl_SporeMineArmAnim4 (its
 * first step's frame shown), sound $B9 (restarted), then it explodes in 13 frames
 * (SporeMine_RecExplode).
 *
 * s: the spore. */
#include "xmen2.h"

void SporeMine_RecArm4(register Spore *s)
{
	s->anim = (u8 *)CODEPTR(Tbl_SporeMineArmAnim4);
	s->frame = *(s->anim + 7);
	s->animStep = 0;
	s->animTime = 0;
	Snd_Stop(0xB9);
	Snd_Play(0xB9);
	s->handler = SporeMine_RecExplode;
	s->delay = 0xD;
}
