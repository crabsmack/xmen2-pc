/* Collide_RunAll - the frame's hits: every player attacker against the enemy bodies, then every
 * enemy attacker against the player bodies (Collide_ObjVsList); none while the scene is ending
 * (g_SceneExitTimer). A body list that is empty skips its attackers. */
#include "xmen2.h"

void Collide_RunAll(void)
{
	register ListNode *n;

	if (g_SceneExitTimer)
		return;
	if (g_HitListEnemyBody) {
		n = g_HitListPlayerAtk;
		while (n) {
			Collide_ObjVsList((Actor *)n->owner, g_HitListEnemyBody);
			n = n->next;
		}
	}
	if (g_HitListPlayerBody) {
		n = g_HitListEnemyAtk;
		while (n) {
			Collide_ObjVsList((Actor *)n->owner, g_HitListPlayerBody);
			n = n->next;
		}
	}
}
