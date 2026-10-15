/* Exodus_ClassInit - per-level class setup of EXODUS (class slot +$5E; the parent's isn't
 * called): its empty hook (Exodus_NopStatic, slot +$6A) and its sprite definition
 * (Art_ExodusSpr, palette 1, g_ExodusSprDef).
 *
 * self: passed on to Exodus_NopStatic (the class's index is the second argument, ignored). */
#include "xmen2.h"

void Exodus_ClassInit(Exodus *self)
{
	Exodus_NopStatic(self);
	g_ExodusSprDef = SprDef_Add(CODEPTR(Art_ExodusSpr), 1);
}
