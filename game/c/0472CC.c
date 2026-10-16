/* BaniMazaA2_InitDebrisBuf - make buf (four debris groups in BaniMazaA2_RunScene's frame) the
 * scene's debris (g_A2Debris), its first group not moving.
 *
 * buf: the groups. */
#include "xmen2.h"

#define g_A2Debris	(*(PTR(DebrisGroup) *)g_SceneVar3B14)	/* scene variable: the four debris groups */

void BaniMazaA2_InitDebrisBuf(DebrisGroup *buf)
{
	(g_A2Debris = buf)->timer = 0;
}
