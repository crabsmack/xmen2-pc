/* Tank_ClassInit - per-level class setup of TANK (class slot +$5E; the parent's isn't called):
 * the tanks' sprite definition (Art_TankSpr, frame 3) added to the level's set, its slot into
 * g_TankSprDef. */
#include "xmen2.h"

void Tank_ClassInit(void)
{
	g_TankSprDef = SprDef_Add(CODEPTR(Art_TankSpr), 3);
}
