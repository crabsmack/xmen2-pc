/* SwampFly_StatePickPath - SWAMPFLY state: face the nearest player (target) and start its flight
 * path again, from the next update (SwampFly_StateFly).
 *
 * self: the fly.
 * The path is Tbl_SwampFlyPath<pathKind> (0-5; any other kind: 0); it runs from pair 1 to
 * count / 2 - 1 (pathStep is cleared, then counted to 1). The cases are the disassembly's
 * fragments SwampFly_StatePickPath_Default (0), _Case1, _Case5, then _Start. Returns
 * self->flags & 8. */
#include "xmen2.h"

short SwampFly_StatePickPath(register SwampFly *self)
{
	self->target = (Actor *)Player_Nearest((Actor *)self);
	switch (self->pathKind) {
	default:
	case 0:		/* SwampFly_StatePickPath_Default */
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath0);
		break;
	case 1:		/* SwampFly_StatePickPath_Case1 */
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath1);
		break;
	case 2:
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath2);
		break;
	case 3:
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath3);
		break;
	case 4:
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath4);
		break;
	case 5:		/* SwampFly_StatePickPath_Case5 */
		self->path = (FlyPath *)CODEPTR(Tbl_SwampFlyPath5);
	}
	/* SwampFly_StatePickPath_Start */
	self->pathStep = 0;
	self->pathEnd = self->path->count / 2;
	self->pathStep++;
	self->pathEnd--;
	SET_STATE(self, SwampFly_StateFly);
	return self->flags & 8;
}
