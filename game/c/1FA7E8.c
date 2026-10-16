/* ClassList_InitAll - set up the classes of the level's class list (g_ClassList): one pointer per
 * class in g_ClassData (cleared; the class keeps its shared data there), then each class's
 * ClassInit (slot +$5E) as (0, i).
 *
 * A list of no classes is dropped (g_ClassList and g_ClassData 0). The clearing loop steps the
 * descriptor pointer too, before it is set (the original's two loops share a header). */
#include "xmen2.h"

void ClassList_InitAll(void)
{
	register long i;
	register long n;
	register PTR(Class) *p;
	register AnyFn fn;

	if (g_ClassList) {
		n = (((ClassList *)g_ClassList)->size - 2) >> 2;
		if (n < 1) {
			g_ClassList = 0;
			g_ClassData = 0;
			return;
		}
		g_ClassData = (PTR(void) *)Mem_StackAlloc(n * 4, STR(Str_Empty_1FA876, ""));
#ifndef XMEN2_MPW
		p = ((ClassList *)g_ClassList)->cls;	/* stepped before it is set; nothing reads it */
#endif
		for (i = 0; i < n; i++, p++)
			g_ClassData[i] = 0;
		p = ((ClassList *)g_ClassList)->cls;
		for (i = 0; i < n; i++, p++) {
			fn = (*p)->method[22];			/* ClassInit */
			(*fn)(0, i);
		}
	}
}
