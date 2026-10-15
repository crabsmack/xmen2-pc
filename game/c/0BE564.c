/* Barbarian_ClassInit - per-level class setup of BARBARIAN (class slot +$5E; the parent's isn't
 * called): the empty Barbarian_Nop6A, then Barbarian_SegHook1A becomes the hook of floor-segment
 * type $1A (BARBARIAN's platforms). Loads no resources.
 *
 * self: 0 (called with 0 and the class's index in the level's class list; the index is
 * ignored).
 * (The label is where MPW C pops Barbarian_Nop6A's argument, as the ROM does; a do/while (0)
 * would do that too but puts self in a register.) */
#include "xmen2.h"

void Barbarian_ClassInit(Barbarian *self)
{
	Barbarian_Nop6A(self);
hooks:
	g_SegTypeHooks[0x1A] = Barbarian_SegHook1A;
}
