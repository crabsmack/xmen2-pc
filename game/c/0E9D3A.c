/* Hologram_OpenExit - after the hologram's explosion: open the SentinelsA1 exit (the scene's
 * palette cycles switch on g_SentinelsA1ExitOpen, sound $D1, the exit tiles change), release
 * the camera, open the arena gate again, shake the screen (Tbl_HologramQuake) and blacken
 * three base palette colours (palette 0 colour 1, palette 2 colours 1 and 2).
 *
 * self: the hologram (Hologram_StateOpenExit). */
#include "xmen2.h"

void Hologram_OpenExit(Hologram *self)
{
	g_SentinelsA1ExitOpen = 1;
	Snd_Stop(0xD1);
	Snd_Play(0xD1);
	SentinelsA1_OpenExitTiles();
	(*g_Camera->cls->method[13])(g_Camera);		/* +$3A UnlockAxes */
	self->gateSeg->flags |= 0x40;
	g_CamShake = CODEPTR(Tbl_HologramQuake);
	g_PalBase[1] = 0;
	g_PalBase[0x21] = 0;
	g_PalBase[0x22] = 0;
}
