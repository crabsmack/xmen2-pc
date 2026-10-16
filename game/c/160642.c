/* Sys_InitHardware - power-on set-up of the video hardware: no V-int hook, the heap at the start
 * of work RAM, interrupts on, the VDP registers, an empty DMA queue, VRAM, CRAM and VSRAM cleared,
 * then the display and V-int switched on (VDP register 1 |= $20, then $40) and the scroll set up
 * (Scroll_Init).
 *
 * With g_ExtIntMode set, external interrupts too (register 11 $8B08, port 3 control = $80).
 * The original kept an if (0) block storing three candidate V-int hooks (Irq_VBlank, Irq_HBlank
 * and one whose jump-table entry was linked to nothing, Jt_NullTarget), then 0. It waits for any
 * DMA (VDP status bit 1) before clearing. */
#include "xmen2.h"

void Sys_InitHardware(void)
{
	register long debug;

	g_VBlankHook = 0;
	unk_Sys_1F4890();
	debug = 0;
	if (debug) {
		g_VBlankHook = Irq_VBlank;
		g_VBlankHook = Irq_HBlank;
#ifdef XMEN2_MPW
		g_VBlankHook = Vectors;		/* jump-table entry 2612 (Jt_NullTarget): address 0 */
#else
		g_VBlankHook = FN_AT(FrameHookFn, Vectors);
#endif
		g_VBlankHook = 0;
	}
	g_HeapTop = AT68(void, 0xFF0000);
	Irq_Enable();
	if (g_ExtIntMode[0])
		g_VdpReg11 = (short)0x8B08;
	else
		g_VdpReg11 = (short)0x8B00;
	VDP_InitRegs();
	if (g_ExtIntMode[0])
#ifdef XMEN2_MPW
		*(volatile s8 *)&IO_CTRL(2) = -0x80;	/* a signed byte: move.b #$FF80 */
#else
		IO_CTRL(2) = 0x80;
#endif
	VDP_ResetDmaQueue();
	g_VBlankCount = 0;
	unk_VDP_1FE05C();
	while (VDP_CTRL & 2)
		;
	VDP_ClearCram();
	VDP_ClearVram();
	VDP_ClearVsram();
	g_VdpReg1 |= 0x20;
	VDP_CTRL = g_VdpReg1;
	g_VdpReg1 |= 0x40;
	VDP_CTRL = g_VdpReg1;
	Scroll_Init();
}
