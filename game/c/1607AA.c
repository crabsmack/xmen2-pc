/* Cam_New - CAMERA slot +$0A: allocate a camera object on the heap and initialise it.
 *
 * cls: the CAMERA descriptor; hook: the post-update hook (postHook).
 * Its size from the class's GetSize (slot +$06), Mem_StackAlloc, then the class's Init (slot
 * +$0E). Returns the camera (scenes keep it in g_Camera). */
#include "xmen2.h"

Camera *Cam_New(Class *cls, CamFn hook)
{
	register Class *c = cls;
	register Camera *cam;

	cam = (Camera *)Mem_StackAlloc((*c->method[0])(), STR(Str_Empty_1607E6, ""));
	cam->cls = c;
	cam->postHook = hook;
	(*cam->cls->method[2])(cam);	/* Init */
	return cam;
}
