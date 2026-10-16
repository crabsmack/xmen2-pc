/* GaussBullet_InitPool - a level's gauss bullets: their sprite definition (Art_GaussBulletSpr,
 * palette 3, g_GaussSprDef), the pool of 6 records on the level stack (Mem_StackAlloc, 200
 * bytes; none live, each record's class GAUSSBULLET; 0 when out of memory) and its timer
 * (GaussBullet_UpdateAll every frame, its own Timer as argument).
 *
 * The timer is made even without a pool (GaussBullet_UpdateAll would then read address 0). */
#include "xmen2.h"

void GaussBullet_InitPool(void)
{
	register GaussPool *pool;
	register Timer *t;

	g_GaussSprDef = SprDef_Add(CODEPTR(Art_GaussBulletSpr), 3);
	if (pool = (GaussPool *)Mem_StackAlloc(sizeof(GaussPool), STR(Str_Empty_11918E, ""))) {
		register GaussBullet *b;

		g_GaussPool = pool;
		g_GaussPool->frames = (GaussFrame *)CODEPTR(Tbl_GaussBulletFrames);
		g_GaussPool->count = 0;
		g_GaussPool->max = 6;
		for (b = g_GaussPool->rec; b < &g_GaussPool->rec[g_GaussPool->max]; b++)
			b->cls = CLASS(Tbl_GaussBullet_vtable);
	} else
		g_GaussPool = 0;
	if (t = Timer_Create((TimerFn)GaussBullet_UpdateAll, 1, 0, 0)) {
		t->arg = t;
		Timer_Start(t);
	}
}
