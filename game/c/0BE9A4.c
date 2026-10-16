/* Launcher_ClassInit - per-level class setup of LAUNCHER (class slot +$5E; the parent's isn't
 * called): the empty Launcher_Nop6A, then its two sprite definitions added to the level's set:
 * the launcher (Art_LauncherSpr, frame 0) as g_LauncherSprDef and its shots (Art_LauncherSpr2,
 * frame 1) as g_LauncherShotSprDef.
 *
 * self: 0, passed on to Launcher_Nop6A. The class's index in the level's class list follows;
 * it is ignored.
 * (The do/while (0) puts the loop labels where MPW C pops the pending arguments, as the ROM.) */
#include "xmen2.h"

void Launcher_ClassInit(Launcher *self)
{
	Launcher_Nop6A(self);
	g_LauncherSprDef = SprDef_Add(CODEPTR(Art_LauncherSpr), 0);
	do {
		g_LauncherShotSprDef = SprDef_Add(CODEPTR(Art_LauncherSpr2), 1);
	} while (0);
}
