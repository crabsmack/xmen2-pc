/* PlayerSelect_DrawFrames - draw the portrait frame of every selectable character
 * (PlayerSelect_DrawFrame: palette 1, behind), at their screen positions (the scene's table). */
#include "xmen2.h"

#define g_SelectCount	(*(s16 *)g_SceneVar3B0E)	/* scene variable: selectable characters (6, or 7 with Magneto) */
#define g_SelectPos	(*(PTR(SelectPos) *)&g_SceneVar3B30)	/* scene variable: their portrait frames' screen positions */

void PlayerSelect_DrawFrames(void)
{
	register short i;
	register SelectPos *pos = g_SelectPos;

	for (i = 0; i < g_SelectCount; i++, pos++)
		PlayerSelect_DrawFrame((short)i, pos->x, pos->y, 1, 0);
}
