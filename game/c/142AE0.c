/* TechWallGun_ClassInit - TECHWALLGUN ClassInit (class slot +$5E): the guns' sprite definition
 * (Art_Spr_1445D2, palette 3: g_TechWallGunSprDef), the shared pool of 16 bullets
 * (g_TechWallGunRecs, all free) and the timer that moves them every frame
 * (TechWallGun_UpdateBullets). */
#include "xmen2.h"

void TechWallGun_ClassInit(void)
{
	WallGunBullet *b;

	TechWallGun_NopStatic();
	g_TechWallGunSprDef = SprDef_Add(CODEPTR(Art_Spr_1445D2), 3);
	g_TechWallGunRecs = (WallGunBullet *)Mem_StackAlloc(0x100, STR(Str_Empty_142B52, ""));
	for (b = &g_TechWallGunRecs[0]; b < &g_TechWallGunRecs[16]; b++)
		b->flags = 0;
	Timer_Start(Timer_Create(TechWallGun_UpdateBullets, 0, g_TechWallGunRecs, 0));
}
