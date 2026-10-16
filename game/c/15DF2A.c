/* HitList_Add - put an object at the head of a hit list (Actor_Activate: the attacker and body
 * lists Collide_RunAll checks against each other; List_Unlink takes it out).
 *
 * obj:  the object (the node's owner).
 * head: the list's head (g_HitListPlayerAtk...).
 * Returns the new node (the object keeps it to unlink itself), or 0 when the pool is full. */
#include "xmen2.h"

ListNode *HitList_Add(void *obj, register PTR(ListNode) *head)
{
	register ListNode *n;

	n = HitList_AllocNode();
	if (n) {
		if (*head)
			(*head)->prev = n;
		n->next = *head;
		n->prev = 0;
		*head = n;
		n->owner = obj;
	}
	return n;
}
