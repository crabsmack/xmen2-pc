/* Platform1_LevelEnd - PLATFORM1's class slot +$62 (static; the level's end): the parent's (SYNCHRONIZEDACTOR;
 * given 0), then g_Platform1SprDef = 0 (Platform1_ClassInit registers the sprite set again). Its
 * own arguments (0, the class's index) are unused. */
#include "xmen2.h"

void Platform1_LevelEnd(void)
{
	(*CLASS(Tbl_Platform1_vtable)->parent->method[23])(0);	/* SYNCHRONIZEDACTOR +$62 */
	g_Platform1SprDef = 0;
}
