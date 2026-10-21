/* plat_engine.cpp - the platform layer (platform/xmen2_platform.h) over the recomp engine's
 * emulated machine: what the hand-C runners (stepped, verify, and the native one until it has a
 * machine of its own, phase 5 P6) link. The C asks the platform; this file asks the engine, with
 * the access the original made: the same address and width on the 68000 bus (so the stepped
 * runner's logical positions, verify's hardware supplies and the device effects are unchanged).
 */
#include <stdint.h>

extern "C" {
#include "genesis_runtime.h"
}

#define VDP_DATA_A   0xC00000u
#define VDP_CTRL_A   0xC00004u
#define VDP_HV_A     0xC00008u
#define PSG_A        0xC00011u
#define IO_A(reg)    (0xA10001u + 2u * (uint32_t)(reg))
#define Z80_BUSREQ_A 0xA11100u
#define Z80_RESET_A  0xA11200u
#define Z80_RAM_A    0xA00000u
#define TMSS_A       0xA14000u

extern "C" {

/* video */
uint8_t  plat_vdp_data_read8(void) { return m68k_read8(VDP_DATA_A); }
uint16_t plat_vdp_data_read16(void) { return m68k_read16(VDP_DATA_A); }
uint32_t plat_vdp_data_read32(void) { return m68k_read32(VDP_DATA_A); }
void     plat_vdp_data_write8(uint8_t v) { m68k_write8(VDP_DATA_A, v); }
void     plat_vdp_data_write16(uint16_t v) { m68k_write16(VDP_DATA_A, v); }
void     plat_vdp_data_write32(uint32_t v) { m68k_write32(VDP_DATA_A, v); }
uint16_t plat_vdp_status16(void) { return m68k_read16(VDP_CTRL_A); }
uint32_t plat_vdp_status32(void) { return m68k_read32(VDP_CTRL_A); }
void     plat_vdp_ctrl_write16(uint16_t v) { m68k_write16(VDP_CTRL_A, v); }
void     plat_vdp_ctrl_write32(uint32_t v) { m68k_write32(VDP_CTRL_A, v); }
uint8_t  plat_vdp_hv8(void) { return m68k_read8(VDP_HV_A); }
uint16_t plat_vdp_hv16(void) { return m68k_read16(VDP_HV_A); }

/* sound */
void     plat_psg_write8(uint8_t v) { m68k_write8(PSG_A, v); }
uint8_t  plat_z80_busreq_read8(void) { return m68k_read8(Z80_BUSREQ_A); }
uint16_t plat_z80_busreq_read16(void) { return m68k_read16(Z80_BUSREQ_A); }
void     plat_z80_busreq_write8(uint8_t v) { m68k_write8(Z80_BUSREQ_A, v); }
void     plat_z80_busreq_write16(uint16_t v) { m68k_write16(Z80_BUSREQ_A, v); }
uint16_t plat_z80_reset_read16(void) { return m68k_read16(Z80_RESET_A); }
void     plat_z80_reset_write16(uint16_t v) { m68k_write16(Z80_RESET_A, v); }
uint8_t  plat_z80_ram_read8(uint32_t off) { return m68k_read8(Z80_RAM_A + off); }
void     plat_z80_ram_write8(uint32_t off, uint8_t v) { m68k_write8(Z80_RAM_A + off, v); }
void     plat_tmss_write32(uint32_t v) { m68k_write32(TMSS_A, v); }

/* input */
uint8_t  plat_io_read8(int reg) { return m68k_read8(IO_A(reg)); }
void     plat_io_write8(int reg, uint8_t v) { m68k_write8(IO_A(reg), v); }
uint16_t plat_io_ctrl12_read16(void) { return m68k_read16(0xA10008u); }
uint32_t plat_io_ctrl12_read32(void) { return m68k_read32(0xA10008u); }
uint16_t plat_io_ctrl3_read16(void) { return m68k_read16(0xA1000Cu); }

/* interrupts: the state is the 68000's SR. A write is what the generated code does for
   `move to SR`: the stepped runner first takes a logged interrupt still due under the old mask,
   the timed one checks for interrupts after it. */
unsigned short plat_sr(void) { return g_cpu.SR; }

void plat_set_sr(unsigned short sr)
{
    x68k_sr_pre();
    g_cpu.SR = sr;
    x68k_sr_written();
}

/* IRQ_WAIT(): the program waits for the V-int with no sync point; the frame-synchronous runner
   takes it now, any other runner nothing. */
void plat_wait_vint(void) { x68k_wait_vint(0); }

}  /* extern "C" */
