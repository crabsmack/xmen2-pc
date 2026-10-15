/* HornetBullet_ClassInit - per-level class setup of HORNETBULLET (class slot +$5E; the parent's
 * isn't called): register its sprite set (Art_HornetBulletSpr, frame 1) in g_HornetBulletSprDef.
 *
 * Its arguments (0, the class's index in the level's class list) are ignored. */
#include "xmen2.h"

void HornetBullet_ClassInit(void)
{
	HornetBullet_NopStatic();
	g_HornetBulletSprDef = SprDef_Add(CODEPTR(Art_HornetBulletSpr), 1);
}
