/* AvalonB2_AnimSprite - the Inner Shell's sprite animation, a V-blank hook (AvalonB2_RunScene's
 * timer installs it after 60 frames): the scene sprite g_SceneSprites[0] shows frame 0 or 1 by
 * the V-blank count's low bit, uploaded at once. */
#include "xmen2.h"

void AvalonB2_AnimSprite(void)
{
	Spr_SetFrame(&g_SceneSprites[0], g_VBlankCount & 1);
	Spr_UploadFrame(&g_SceneSprites[0], 1);
}
