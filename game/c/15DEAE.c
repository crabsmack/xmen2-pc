/* HitList_Init - empty the four hit lists (player attackers and bodies, enemy attackers and
 * bodies) and free the 50 nodes of the pool (owner 0). */
#include "xmen2.h"

/* The original's file (HitList_Init .. HitList_Add) defined g_HitNodes: MPW C keeps the address
   of an array defined in the compile unit in a register (lea g_HitNodes(a5),a3), copied here. */
#ifdef XMEN2_MPW
ListNode g_HitNodes[50];
#endif

void HitList_Init(void)
{
	register ListNode *n = g_HitNodes;
	short i;

	g_HitListPlayerAtk = g_HitListPlayerBody = g_HitListEnemyAtk = g_HitListEnemyBody = 0;
	for (i = 0; i < 50; i++) {
		n->owner = 0;
		n++;
	}
}
