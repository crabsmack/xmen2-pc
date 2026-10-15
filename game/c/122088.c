/* BigBio_StateMorphToShape - BIGBIO: slide the orbs sideways onto the shape around its position
 * (up to 8 px per frame; y is set at once). When all are in place: orb mode 8 off (the body is
 * drawn again), turn around, morph animation, sound $F8, then BigBio_ActIdleAnim90 once the
 * animation has played.
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus.
 * Unlike BigBio_PlaceOrbs, the h-flip here subtracts dx and nothing moves the 16 px. */
#include "xmen2.h"

short BigBio_StateMorphToShape(BigBio *self)
{
	short n;			/* counted, never used */
	char flipH;
	short px, py;
	register BigBioOrb *orb;
	register BigBioShapePt *pt;
	register void *anim;
	char done;
	short i, tx, d;

	orb = g_BigBioOrbs;
	pt = self->shape;
	done = 1;
	flipH = self->spr.flags & 0x10;
	px = self->x >> 8;
	py = self->y >> 8;
	for (i = 0, n = 0; i < 16; i++, n++) {
		if (flipH)
			tx = px - pt->dx;
		else
			tx = px + pt->dx;
		d = tx - orb->x;
		orb->y = py + pt->dy;
		if (d > 8) {
			orb->x += 8;
			done = 0;
		} else if (d > 0) {
			orb->x += d;
			done = 0;
		} else if (d < -8) {
			orb->x -= 8;
			done = 0;
		} else if (d < 0) {
			orb->x += d;
			done = 0;
		}
		pt++;
		orb++;
	}
	if (done) {
		anim = CODEPTR(Tbl_BigBioAnim2);
		self->orbMode &= ~8;
		self->facingRight = !self->facingRight;
		if (self->facingRight)
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		Obj_SetAnim(self, anim);
		Snd_Stop(0xF8);
		Snd_Play(0xF8);
		SET_STATE_WAIT(self, (StateFn)BigBio_ActIdleAnim90, 0xFF);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateMorphToShape);
	return self->flags & 8;
}
