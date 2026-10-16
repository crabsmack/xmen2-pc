/* Spr_Draw - draw a sprite instance at a position.
 *
 * spr: the instance (object+$2E).  x, y: where (pixels; Actor_Draw passes screen coordinates).
 * Its frame is uploaded when stale (queued). Nothing when no frame is uploaded. By definition
 * type: preloaded ($20) or raw-tile ($18/$19) pieces go into the hardware sprite list at x/y +$80,
 * with its tileBase: Spr_EmitPre / Spr_EmitStream when the frame's bounding box lies within
 * (g_CameraX, g_CameraY) .. +(320, 224) of x/y (note: x/y are compared with the camera position
 * itself), else the clipping Spr_EmitPreClip / Spr_EmitStreamClip; a sprite carrying objects uses
 * Spr_EmitPreLinked (at +$80) / Spr_EmitStreamLinked (at x, y: it adds the $80 itself). */
#include "xmen2.h"

void Spr_Draw(SprInst *spr, short x, short y)
{
	short frame;
	short tile = spr->tileBase;
	register short type;
	SprFrame *f;

	if ((spr->flags & 0x20) == 0)
		Spr_UploadFrame(spr, 0);
	frame = spr->uploaded;
	if (frame == 0xFF)
		return;
	type = ((SprDef *)spr->def)->type;
	f = SPR_FRAME(spr->def, frame);
	if (type == 0x20) {
		if (spr->firstChild == 0xFF) {
			if (f->left + x >= g_CameraX && f->right + x <= g_CameraX + 0x140
			    && f->top + y > g_CameraY && f->bottom + y <= g_CameraY + 0xE0)
				Spr_EmitPre(spr, x + 0x80, y + 0x80, tile);
			else
				Spr_EmitPreClip(spr, x + 0x80, y + 0x80, tile);
		} else
			Spr_EmitPreLinked(spr, x + 0x80, y + 0x80, tile);
	} else if (type == 0x18 || type == 0x19) {
		if (spr->firstChild == 0xFF) {
			if (f->left + x >= g_CameraX && f->right + x <= g_CameraX + 0x140
			    && f->top + y > g_CameraY && f->bottom + y <= g_CameraY + 0xE0)
				Spr_EmitStream(spr, x + 0x80, y + 0x80, tile);
			else
				Spr_EmitStreamClip(spr, x + 0x80, y + 0x80, tile);
		} else
			Spr_EmitStreamLinked(spr, x, y, tile);
	}
}
