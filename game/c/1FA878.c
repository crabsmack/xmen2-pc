/* ClassList_CallSlot62 - run class slot +$62 as (0, i) for every class i of the level's class
 * list (g_ClassList; PlayerSelect_Run when its scene ends). ACTOR's slot is empty.
 *
 * Nothing without a class list. */
#include "xmen2.h"

void ClassList_CallSlot62(void)
{
	register long i;
	register long n;
	register PTR(Class) *p;
	register AnyFn fn;

	if (g_ClassList) {
		n = (((ClassList *)g_ClassList)->size - 2) >> 2;
		p = ((ClassList *)g_ClassList)->cls;
		for (i = 0; i < n; i++, p++) {
			fn = (*p)->method[23];
			(*fn)(0, i);
		}
	}
}
