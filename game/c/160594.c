/* VDP_InitRegs - program every VDP register with the game's base mode: mode 1 $04, mode 2 $24
 * (also g_VdpReg1: display off, V-int on), plane A $E000, window $D000, plane B $0000, sprites
 * $F000, backdrop 0, H-int counter 0, mode 3 g_VdpReg11, mode 4 $81 (H40), H-scroll $F400, auto
 * increment 2, 64x32 planes, window 0/0, DMA length $0200, DMA source 0. */
#include "xmen2.h"

void VDP_InitRegs(void)
{
	VDP_CTRL = 0x8004;
	g_VdpReg1 = 0x8124;
	VDP_CTRL = g_VdpReg1;
	VDP_CTRL = 0x8238;
	VDP_CTRL = 0x8334;
	VDP_CTRL = 0x8400;
	VDP_CTRL = 0x8578;
	VDP_CTRL = 0x8700;
	VDP_CTRL = 0x8A00;
	VDP_CTRL = g_VdpReg11;
	VDP_CTRL = 0x8C81;
	VDP_CTRL = 0x8D3D;
	VDP_CTRL = 0x8F02;
	VDP_CTRL = 0x9011;
	VDP_CTRL = 0x9100;
	VDP_CTRL = 0x9200;
	VDP_CTRL = 0x9302;
	VDP_CTRL = 0x9400;
	VDP_CTRL = 0x9500;
	VDP_CTRL = 0x9600;
	VDP_CTRL = 0x9700;
}
