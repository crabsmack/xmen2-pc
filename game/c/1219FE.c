/* BigBio_Draw - draw BIGBIO (class slot +$1A).
 *
 * self: the boss.
 * Below 16 health its body palette (line 3) flashes between Pal_BigBioBody rows 2 and 3 every 8
 * frames. With no live player left the scene ends (g_SceneExitTimer = 1). Orb modes:
 *   none:   the body, by the parent's Draw;
 *   bit 3:  each orb as one hardware sprite of the Art_BigBioSpr tiles (frame orbFrame,
 *           palette 3);
 *   bits 0/1 (without 3): the body's own frame $64 (bit 0) or $65 uploaded, and drawn as one
 *           hardware sprite at each orb that is on screen.
 * Orbs at x <= 0 and y <= 0 are not drawn. */
#include "xmen2.h"

void BigBio_Draw(register BigBio *self)
{
	s16 *src;
	SprInst *def;
	s16 *pal;
	register s16 *dst;
	register BigBioOrb *orb;
	register short j;
	register short attr;
	long i;

	if (self->health < 16) {
		pal = (s16 *)CODEPTR(Pal_BigBioBody);
		if (g_FrameCounter & 8) {
			src = &pal[0x20];
			dst = &g_PalBase[0x30];
			for (i = 0; i < 16; i++)
				*dst++ = *src++;
			g_PalBaseDirty = 1;
		} else {
			src = &pal[0x30];
			dst = &g_PalBase[0x30];
			for (i = 0; i < 16; i++)
				*dst++ = *src++;
			g_PalBaseDirty = 1;
		}
	}
	if (!IS_ALIVE(&g_Player1) && !IS_ALIVE(&g_Player2))
		g_SceneExitTimer = 1;
	if (self->orbMode & 0xB) {
		orb = g_BigBioOrbs;
		if (self->orbMode & 8) {
			def = &g_SpriteDefs[g_BigBioSprDef];
			for (j = 0; j < 16; j++, orb++) {
				if (orb->x > 0 || orb->y > 0) {
					attr = self->orbFrame << 2;
					attr = (def->tileBase + attr) | 0x6000;
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = orb->y - g_CameraY + 0x80;
					g_SprWritePtr->size = 5;
					g_SprWritePtr->attr = attr | 0x8000;
					g_SprWritePtr->x = orb->x - g_CameraX + 0x80;
				}
			}
		} else {
			if (self->orbMode & 1)
				(*self->cls->method[20])(self, 0x64);	/* SetFrame */
			else
				(*self->cls->method[20])(self, 0x65);
			Spr_UploadFrame(&self->spr, 0);
			attr = self->spr.tileBase | 0x8000;
			attr |= (self->spr.flags & 3) << 13;
			for (j = 0; j < 16; j++, orb++) {
				if ((orb->x > 0 || orb->y > 0)
				    && orb->x - g_CameraX > -32 && orb->x - g_CameraX < 320
				    && orb->y - g_CameraY > -32 && orb->y - g_CameraY < 224) {
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = orb->y - g_CameraY + 0x80;
					g_SprWritePtr->size = 5;
					g_SprWritePtr->attr = attr;
					g_SprWritePtr->x = orb->x - g_CameraX + 0x80;
				}
			}
		}
	} else
		(*CLASS(Tbl_BigBio_vtable)->parent->method[5])(self);	/* BASICBIOSENTINEL Draw */
}
