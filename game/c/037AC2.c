/* ThronePhone_ClassInit - per-level class setup of THRONEPHONE (class slot +$5E; the parent's
 * isn't called): its sprite definition (Art_ThronePhoneSpr, palette 0, g_ThronePhoneSprDef).
 *
 * self: the class's first argument (unused; the class's index is the second).
 * MPW: the label at the end keeps the call's argument pop after the store, as in the ROM. */
#include "xmen2.h"

void ThronePhone_ClassInit(ThronePhone *self)
{
	g_ThronePhoneSprDef = SprDef_Add(CODEPTR(Art_ThronePhoneSpr), 0);
end:	;
}
