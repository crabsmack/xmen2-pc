/* List_Unlink - unlink a node from a doubly linked list and clear its owner.
 *
 * node: the node (null is ignored).
 * head: where the list's first node is kept; advanced when node was the first. */
#include "xmen2.h"

void List_Unlink(register ListNode *node, register PTR(ListNode) *head)
{
	if (node) {
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
		if (*head == node)
			*head = node->next;
		node->owner = 0;
	}
}