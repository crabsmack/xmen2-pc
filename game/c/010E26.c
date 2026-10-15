/* BigAcolyte_ActGrow - BIGACOLYTE intro script action (step 4) and state: grow out of the wall,
 * the picture 2 px wider and 3 px taller per frame (moving 1 px left and 1.5 px up to stay in
 * place), re-rendered each frame; the first frame plays sound $DC. Past 56 px wide it bursts
 * the wall (BigAcolyte_StateBurstWall).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActGrow(register BigAcolyte *self)
{
	self->x -= 0x100;
	self->y -= 0x180;
	if (g_ScaleParamsPtr->outW == g_ScaleParamsPtr->rectW) {
		Snd_Stop(0xDC);
		Snd_Play(0xDC);
	}
	g_ScaleParamsPtr->outW += 2;
	g_ScaleParamsPtr->outH += 3;
	if (g_ScaleParamsPtr->outW <= 0x38) {
		BigAcolyte_RenderScaled((ScalePicture *)g_ScaleSrcBitmap, g_ScaleParamsPtr,
			self->spr.tileBase << 5, 0);
		SET_STATE(self, BigAcolyte_ActGrow);
		return self->flags & 8;
	}
	BigAcolyte_RenderScaled((ScalePicture *)g_ScaleSrcBitmap, g_ScaleParamsPtr,
		self->spr.tileBase << 5, 0);
	SET_STATE(self, BigAcolyte_StateBurstWall);
	return self->flags & 8;
}
