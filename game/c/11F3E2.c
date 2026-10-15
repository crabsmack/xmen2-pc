/* BioBalls_StartPath - send a BIOBALLS object's balls along a path (BioBalls_Move eases them to
 * it), waiting in BioBalls_WaitPathDone.
 *
 * self:   the object.
 * path:   its points (PathPt; x >= 1000 restarts it).
 * flags:  ballFlags bits 1-3 (mirror x, mirror y, absolute points).
 * dx, dy: the anchor's offset from the object (pixels).
 * Not done. */
#include "xmen2.h"

void BioBalls_StartPath(BioBalls *self, void *path, char flags, short dx, short dy)
{
	self->done = 0;
	self->path = (PathPt *)path;
	self->ballFlags = self->ballFlags & ~0xE | flags & 0xE;
	self->pathDx = dx;
	self->pathDy = dy;
	self->state = BioBalls_WaitPathDone;
	self->timer = 0;
}
