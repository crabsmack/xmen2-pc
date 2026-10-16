/* ClassList_CallSlot5A - run class slot +$5A as (0, i) for every class i of the level's class
 * list (g_ClassList; Level_Load). ACTOR's slot is empty.
 *
 * Nothing without a class list. */
#include "xmen2.h"

void ClassList_CallSlot5A(void)
{
	register long i;
	register long n;
	register PTR(Class) *p;

	if (g_ClassList) {
		n = (((ClassList *)g_ClassList)->size - 2) >> 2;
		p = ((ClassList *)g_ClassList)->cls;
		for (i = 0; i < n; i++, p++)
			(*(*p)->method[21])(0, i);
	}
}
