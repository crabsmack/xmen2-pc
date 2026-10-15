/* FloatBubble_ClassInit - per-level class setup of FLOATBUBBLE (class slot +$5E): the empty hook
 * FloatBubble_Nop6A, then register the sprite definition Art_FloatBubbleSpr in
 * g_FloatBubbleSprDef, palette 0 in the jungle scenes (SavageC1, SavageB1), else 2.
 *
 * self: 0 (the slot is called on the class, with the class's index in the level's class list
 *       as a second, ignored argument). */
#include "xmen2.h"

void FloatBubble_ClassInit(FloatBubble *self)
{
	FloatBubble_Nop6A(self);
	{
		short pal = g_ScnSavageC1 == g_SceneIndex || g_ScnSavageB1 == g_SceneIndex ? 0 : 2;

		g_FloatBubbleSprDef = SprDef_Add(CODEPTR(Art_FloatBubbleSpr), pal);
	}
}
