/* SpikeBall_ClassInit - per-level class setup of SPIKEBALL (class slot +$5E; the parent's isn't
 * called): the empty SpikeBall_Nop6A, then its sprite definition (Art_SpikeBallSpr, frame 3)
 * added to the level's set: the slot goes to g_SpikeBallSprDef.
 *
 * self: 0, passed on to SpikeBall_Nop6A. The class's index in the level's class list follows;
 * it is ignored. */
#include "xmen2.h"

void SpikeBall_ClassInit(SpikeBall *self)
{
	SpikeBall_Nop6A(self);
	g_SpikeBallSprDef = SprDef_Add(CODEPTR(Art_SpikeBallSpr), 3);
}
