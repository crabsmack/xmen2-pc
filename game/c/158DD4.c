/* FluxBomb_Draw - FLUXBOMB Draw (class slot +$1A): the particles of its phase (implosion
 * FluxBomb_DrawImplode, burst FluxBomb_DrawBurst), then the shared bomb sprite g_FluxBombSpr at
 * its screen position.
 *
 * self: the bomb. */
#include "xmen2.h"

void FluxBomb_Draw(register FluxBomb *self)
{
	switch (self->phase) {
	case 1:
		FluxBomb_DrawImplode(self);
		break;
	case 3:
		FluxBomb_DrawBurst(self);
		break;
	}
	Spr_Draw(&g_FluxBombSpr, (self->x >> 8) - g_CameraX, (self->y >> 8) - g_CameraY);
}
