/* XClone_ActChildAimPlayer - XCLONE script action: give the balls form a target, the nearest
 * player (its x, the centre of its body box; kept in the balls' px/py), and home in on it
 * (XClone_StateChildAim). With no BIOBALLS object go on with the script now.
 *
 * self: the clone (a step of its AI script; never runs in the corpus).
 * Returns flags & 8 when aiming; nothing in particular otherwise. */
#include "xmen2.h"

short XClone_ActChildAimPlayer(XClone *self)
{
	XPlayer *p;
	BioBalls *balls = self->child;

	if (!balls) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		p = Player_Nearest((Actor *)balls);
		balls->px = p->x >> 8;				/* target */
		balls->py = (p->boxTop + p->boxBottom) >> 1;
		SET_STATE(self, XClone_StateChildAim);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
