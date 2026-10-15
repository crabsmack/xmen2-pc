/* Beast_ShockwaveInit - start the stomp's two shockwaves (Beast_StateStomp) 20 px either side of
 * x, with the sound and screen shake of their strength.
 *
 * x:      Beast's x (pixels).
 * leftX, rightX: the waves' x (Beast waveLeftX, waveRightX).
 * spr:    the waves' sprite (Beast waveSpr): frame 0.
 * strong: sound $99 and the big shake (Tbl_BeastQuakeBig); else sound $97 and
 *         Tbl_BeastQuakeSmall. */
#include "xmen2.h"

void Beast_ShockwaveInit(register short x, s16 *leftX, s16 *rightX, SprInst *spr, char strong)
{
	if (strong) {
		Snd_Stop(0x99);
		Snd_Play(0x99);
		g_CamShake = CODEPTR(Tbl_BeastQuakeBig);
	} else {
		Snd_Stop(0x97);
		Snd_Play(0x97);
		g_CamShake = CODEPTR(Tbl_BeastQuakeSmall);
	}
	*rightX = x + 0x14;
	*leftX = x - 0x14;
	Spr_SetFrame(spr, 0);
}
