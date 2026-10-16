/* Ninja_ClassInit - per-level class setup of NINJA (class slot +$5E): only calls the empty
 * Ninja_NopStatic (the sprite set, g_NinjaSprDef2, is loaded by the scenes). The parent's isn't
 * called. */
#include "xmen2.h"

void Ninja_ClassInit(void)
{
	Ninja_NopStatic();
}
