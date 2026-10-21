/* xmen2_platform.h - the platform layer: the hardware the game's C reaches (phase 5 P4), as
 * names and plain C functions. Included by xmen2_mem.h; the game's C never names an address.
 *
 *   MPW: every name is the 68000 address the original used (the code must byte-match the ROM).
 *   PC and NATIVE: every name is a port of the platform API below; a read or write of it is one
 *        call, in the original's order and width. The recomp runners link plat_engine.cpp (the
 *        engine's emulated bus); the native runner links the platform's own machine.
 *
 * API (C linkage):
 *   video   the VDP's data port (8/16/32-bit), control port (commands, register writes, DMA from
 *           game memory; reads: the status), H/V counter
 *   sound   the PSG port; the Z80's bus request and reset lines and its RAM (the sound driver's
 *           mailbox: Snd_*); a 32-bit write of the TMSS register
 *   input   the I/O ports: version, the pads' data and control registers
 *   irq     the interrupt state (a status word in the 68000 SR's layout: bits 8-10 the mask
 *           level), and waiting for the V-int where the program spins on a flag only it sets
 */
#ifndef XMEN2_PLATFORM_H
#define XMEN2_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif
unsigned short plat_sr(void);                 /* the interrupt state */
void           plat_set_sr(unsigned short sr);   /* set it (a lower mask lets a pending interrupt in) */
void           plat_wait_vint(void);          /* IRQ_WAIT */
#ifdef __cplusplus
}
#endif

#ifdef XMEN2_MPW
/* ======================================================================== */
/* MPW: the hardware's own addresses                                         */
/* ======================================================================== */
#define HW8(a)                 (*(volatile u8 *)(a))
#define HW16(a)                (*(volatile s16 *)(a))
#define HW32(a)                (*(volatile s32 *)(a))
#define VDP_DATA       HW16(0xC00000)   /* VDP data port */
#define VDP_DATA_B     HW8(0xC00000)    /* its byte (the value of a DMA fill) */
#define VDP_DATA32     HW32(0xC00000)
#define VDP_CTRL       HW16(0xC00004)   /* VDP control port: status (read), commands (write) */
#define VDP_CTRL32     HW32(0xC00004)
#define VDP_HV         HW16(0xC00008)   /* H/V counter */
#define VDP_HV_B       HW8(0xC00008)    /* its high byte: the V counter (SiberiaB1_InstallHooks) */
#define PSG_PORT       HW8(0xC00011)    /* SN76489 */
#define IO_VERSION     HW8(0xA10001)    /* region / version register */
#define IO_DATA1       HW8(0xA10003)    /* pad port 1 data */
#define IO_DATA2       HW8(0xA10005)    /* pad port 2 data */
#define IO_CTRL1       HW8(0xA10009)    /* pad port 1 control */
#define IO_CTRL2       HW8(0xA1000B)    /* pad port 2 control */
#define IO_CTRL12_L    HW32(0xA10008)   /* control ports 1 and 2 as one long (the boot's soft-reset test) */
#define IO_CTRL12_W    HW16(0xA10008)   /* the same as a word (Boot_StartApp's cold-start test) */
#define IO_CTRL3_W     HW16(0xA1000C)   /* expansion port control (the boot's soft-reset test) */
#define IO_DATA(n)     HW8(0xA10003 + 2 * (n))   /* pad port n (0, 1) data */
#define IO_CTRL(n)     HW8(0xA10009 + 2 * (n))   /* pad port n (0, 1) control */
#define Z80_BUSREQ     HW16(0xA11100)   /* Z80 bus request */
#define Z80_BUSREQ_B   HW8(0xA11100)    /* its byte (btst #0: 1 = bus not yet granted) */
#define Z80_RESET      HW16(0xA11200)   /* Z80 reset */
#define TMSS_PORT      HW32(0xA14000)   /* TMSS "SEGA" */
#define Z80_RAM(off)   HW8(0xA00000 + (off))   /* Z80 RAM byte (bus held) */

#else
/* ======================================================================== */
/* PC and NATIVE: the platform API                                           */
/* ======================================================================== */
#include <stdint.h>

extern "C" {
/* video */
uint8_t  plat_vdp_data_read8(void);
uint16_t plat_vdp_data_read16(void);
uint32_t plat_vdp_data_read32(void);
void     plat_vdp_data_write8(uint8_t v);
void     plat_vdp_data_write16(uint16_t v);
void     plat_vdp_data_write32(uint32_t v);
uint16_t plat_vdp_status16(void);
uint32_t plat_vdp_status32(void);
void     plat_vdp_ctrl_write16(uint16_t v);
void     plat_vdp_ctrl_write32(uint32_t v);
uint8_t  plat_vdp_hv8(void);
uint16_t plat_vdp_hv16(void);
/* sound */
void     plat_psg_write8(uint8_t v);
uint8_t  plat_z80_busreq_read8(void);
uint16_t plat_z80_busreq_read16(void);
void     plat_z80_busreq_write8(uint8_t v);
void     plat_z80_busreq_write16(uint16_t v);
uint16_t plat_z80_reset_read16(void);
void     plat_z80_reset_write16(uint16_t v);
uint8_t  plat_z80_ram_read8(uint32_t off);
void     plat_z80_ram_write8(uint32_t off, uint8_t v);
void     plat_tmss_write32(uint32_t v);
/* input: the I/O ports by their register: 0 version, 1-3 data of pad port 1, 2 and the
   expansion port, 4-6 their control registers (byte reads and writes); and the boot's wide reads */
uint8_t  plat_io_read8(int reg);
void     plat_io_write8(int reg, uint8_t v);
uint16_t plat_io_ctrl12_read16(void);
uint32_t plat_io_ctrl12_read32(void);
uint16_t plat_io_ctrl3_read16(void);
}

namespace plat {
/* A port: reading it calls R, assigning to it calls W (each once, as the original's access). */
template <class T, T (*R)(void), void (*W)(T)>
struct port {
    operator T() const { return R(); }
    const port &operator=(T v) const { W(v); return *this; }
};
template <class T, T (*R)(void)>
struct rport {
    operator T() const { return R(); }
};
template <class T, void (*W)(T)>
struct wport {
    const wport &operator=(T v) const { W(v); return *this; }
};
/* an I/O register (plat_io_read8 / plat_io_write8) */
struct ioport {
    int reg;
    operator uint8_t() const { return plat_io_read8(reg); }
    const ioport &operator=(uint8_t v) const { plat_io_write8(reg, v); return *this; }
};
/* a Z80 RAM byte (the bus held) */
struct z80ram {
    uint32_t off;
    operator uint8_t() const { return plat_z80_ram_read8(off); }
    const z80ram &operator=(uint8_t v) const { plat_z80_ram_write8(off, v); return *this; }
};
}  /* namespace plat */

#define VDP_DATA       (plat::port<uint16_t, plat_vdp_data_read16, plat_vdp_data_write16>{})
#define VDP_DATA_B     (plat::port<uint8_t, plat_vdp_data_read8, plat_vdp_data_write8>{})
#define VDP_DATA32     (plat::port<uint32_t, plat_vdp_data_read32, plat_vdp_data_write32>{})
#define VDP_CTRL       (plat::port<uint16_t, plat_vdp_status16, plat_vdp_ctrl_write16>{})
#define VDP_CTRL32     (plat::port<uint32_t, plat_vdp_status32, plat_vdp_ctrl_write32>{})
#define VDP_HV         (plat::rport<uint16_t, plat_vdp_hv16>{})
#define VDP_HV_B       (plat::rport<uint8_t, plat_vdp_hv8>{})
#define PSG_PORT       (plat::wport<uint8_t, plat_psg_write8>{})
#define IO_VERSION     (plat::ioport{ 0 })
#define IO_DATA1       (plat::ioport{ 1 })
#define IO_DATA2       (plat::ioport{ 2 })
#define IO_CTRL1       (plat::ioport{ 4 })
#define IO_CTRL2       (plat::ioport{ 5 })
#define IO_CTRL12_L    (plat::rport<uint32_t, plat_io_ctrl12_read32>{})
#define IO_CTRL12_W    (plat::rport<uint16_t, plat_io_ctrl12_read16>{})
#define IO_CTRL3_W     (plat::rport<uint16_t, plat_io_ctrl3_read16>{})
#define IO_DATA(n)     (plat::ioport{ 1 + (n) })   /* pad port n (0, 1; 2 the expansion port) */
#define IO_CTRL(n)     (plat::ioport{ 4 + (n) })
#define Z80_BUSREQ     (plat::port<uint16_t, plat_z80_busreq_read16, plat_z80_busreq_write16>{})
#define Z80_BUSREQ_B   (plat::port<uint8_t, plat_z80_busreq_read8, plat_z80_busreq_write8>{})
#define Z80_RESET      (plat::port<uint16_t, plat_z80_reset_read16, plat_z80_reset_write16>{})
#define TMSS_PORT      (plat::wport<uint32_t, plat_tmss_write32>{})
#define Z80_RAM(off)   (plat::z80ram{ (uint32_t)(off) })

#endif /* PC and NATIVE */

#endif /* XMEN2_PLATFORM_H */
