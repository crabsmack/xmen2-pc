/* BigBio_StateWaitPlayers - BIGBIO: wait until every live player is inside the arena (left of its
 * left edge + 360 px; a dead player counts as inside), then wall the arena in (two floor segments
 * of type $14, 230 px tall, at the left edge - 10..+30 and + 370..+410), pan the camera to the
 * arena (left edge + 40) and lock it, no looking up/down, load the boss palette (line 3, row 0 of
 * Pal_BigBioBody) and walk in (BigBio_StateWalkIn, vx $D0).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateWaitPlayers(register BigBio *self)
{
	s16 *src;
	register Seg *seg;
	register s16 *dst;
	char in1, in2;
	short px;
	long i;

	in1 = 0;
	in2 = 0;
	if (IS_ALIVE(&g_Player1)) {
		px = g_Player1.x >> 8;
		if (px < self->arenaLeft + 360)
			in1 = 1;
	} else
		in1 = 1;
	if (IS_ALIVE(&g_Player2)) {
		px = g_Player2.x >> 8;
		if (px < self->arenaLeft + 360)
			in2 = 1;
	} else
		in2 = 1;
	if (in1 && in2) {
		seg = &g_Segs[self->wallSeg = (u8)Seg_Alloc()];
		Seg_Setup(seg, 0x14, 0, self->arenaLeft + -10, (self->y >> 8) - 235,
			self->arenaLeft + 30, (self->y >> 8) - 5);
		seg = &g_Segs[Seg_Alloc()];
		Seg_Setup(seg, 0x14, 0, self->arenaLeft + 370, (self->y >> 8) - 235,
			self->arenaLeft + 410, (self->y >> 8) - 5);
		self->flags6 |= 0x1000;
		(*g_Camera->cls->method[12])(g_Camera, self->arenaLeft + 40, g_CameraY, 2, 2, 1, 0);	/* PanTo */
		g_Camera->lookStep = 0;
		src = (s16 *)CODEPTR(Pal_BigBioBody);
		dst = &g_PalBase[0x30];
		for (i = 0; i < 16; i++)
			*dst++ = *src++;
		self->vx = 0xD0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_12061C));
		SET_STATE(self, BigBio_StateWalkIn);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateWaitPlayers);
	return self->flags & 8;
}
