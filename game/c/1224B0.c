/* BigBio_StateGatherOrbs - BIGBIO: pull the orbs onto the shape around its position, up to 8 px per
 * frame in x and y. When all are in place: orb mode 8 off (the body is drawn again), morph
 * animation, sound $FB stopped and $F8 played, the next step of the script once the animation
 * has played. Until then, with the countdown at 1 the orb tile frame becomes 1.
 *
 * self: the boss. Returns flags & 8 while gathering. Never runs in the corpus.
 * Unlike BigBio_StateMorphToShape, the h-flip here adds dx. */
#include "xmen2.h"

short BigBio_StateGatherOrbs(BigBio *self)
{
	short i;
	short px, py;
	char flipH;
	short ty;
	register BigBioOrb *orb;
	register BigBioShapePt *pt;
	char done;
	short tx, dy, dx;

	orb = g_BigBioOrbs;
	pt = self->shape;
	done = 1;
	flipH = self->spr.flags & 0x10;
	px = self->x >> 8;
	py = self->y >> 8;
	for (i = 0; i < 16; i++) {
		if (flipH)
			tx = px + pt->dx;
		else
			tx = px - pt->dx;
		dx = tx - orb->x;
		if (dx > 8) {
			orb->x += 8;
			done = 0;
		} else if (dx > 0) {
			orb->x += dx;
			done = 0;
		} else if (dx < -8) {
			orb->x -= 8;
			done = 0;
		} else if (dx < 0) {
			orb->x += dx;
			done = 0;
		}
		ty = py + pt->dy;
		dy = ty - orb->y;
		if (dy > 8) {
			orb->y += 8;
			done = 0;
		} else if (dy > 0) {
			orb->y += dy;
			done = 0;
		} else if (dy < -8) {
			orb->y -= 8;
			done = 0;
		} else if (dy < 0) {
			orb->y += dy;
			done = 0;
		}
		pt++;
		orb++;
	}
	if (done) {
		self->orbMode &= ~8;
		Obj_SetAnim(self, CODEPTR(Tbl_BigBioAnim2));
		Snd_Stop(0xFB);
		Snd_Stop(0xF8);
		Snd_Play(0xF8);
		AI_NEXT_STEP(self, self->ai, 0xFF);
	} else {
		if (self->countdown == 1)
			self->orbFrame = 1;
		SET_STATE(self, BigBio_StateGatherOrbs);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
