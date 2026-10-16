/* EntryPoint - the reset vector: Sega's standard hardware set-up (ICD_BLK4) unless this is a soft
 * reset, then the region check and the start of the game. Never returns.
 *
 * Cold reset (the I/O control ports still 0), from Boot_SetupValues (BootSetup below): the TMSS
 * "SEGA" write on consoles with a version > 0; a VDP status read; the 24 VDP registers
 * ($80nn | value, n counting up) and a VRAM DMA fill with 0; the Z80 bus taken, the Z80 out of
 * reset, its 38-byte stub program copied to Z80 RAM, reset pulsed, the bus released; the work
 * RAM cleared (16K longs, from the top down); VDP registers 1 and 15 again, CRAM (32 longs) and
 * VSRAM (20 longs) cleared; the PSG's four channels muted; the Z80 held in reset; interrupts
 * masked (SR $2700). Two 68000-only steps have no C: USP = 0 and every register loaded from the
 * cleared RAM (the game never runs in user mode, and no C reads a register).
 *
 * Boot_RegionCheck (fragment $1F44A8): the console's region (version bits 6-7) through
 * RegionCheck_Data to a letter; 0 hangs. The letter among the header's 16 region bytes
 * (Header_Regions) starts the game. Otherwise the lockout screen: VDP set up, colour 1 white, the
 * 1bpp Art_RegionFont expanded to 4bpp tiles at VRAM 0, "DEVELOPED FOR USE ONLY WITH" on row 8,
 * then a line per system the header names (Tbl_RegionCodePairs: {word letter, long offset of its
 * name in RegionCheck_Data}; Str_RegionMsgSystemsPos before the last of several), "SYSTEMS." and
 * a hang. Texts start with their column byte (Boot_DrawText).
 * Boot_StartApp (fragment $1F483E): on a cold start (control ports 1-2 still 0) unk_Boot_005458,
 * then the game through the application entry at $21C, Game_Main.
 * Hand-written 68000: verified by the whole-game traces (it never returns, so no shadow test). */
#include "xmen2.h"

/* Boot_SetupValues ($6C bytes, ROM). */
typedef struct BootSetup {
	u16 vdpReg;			/* +$00 $8000: the first VDP register command */
	u16 ramLongs;			/* +$02 $3FFF: work RAM longs - 1 */
	u16 regStep;			/* +$04 $0100: to the next register; also the Z80 bus/reset "on" word */
	PTR(void) port[5];		/* +$06 Z80 RAM, Z80 bus, Z80 reset, VDP data, VDP control (the C uses their names) */
	u8 vdpRegs[24];			/* +$1A registers 0-23 */
	u32 vramFill;			/* +$32 $40000080: VRAM write at 0 with DMA (the fill) */
	u8 z80[0x26];			/* +$36 the Z80 stub program */
	u32 vdpRegs2;			/* +$5C $81048F02: registers 1 and 15 */
	u32 cramWrite;			/* +$60 $C0000000: CRAM write at 0 */
	u32 vsramWrite;			/* +$64 $40000010: VSRAM write at 0 */
	u8 psg[4];			/* +$68 $9F $BF $DF $FF: channels 0-3 at volume off */
} BootSetup;
CHECK_SIZE(BootSetup, 0x6C);

/* Tbl_RegionCodePairs: an entry (6 bytes); a letter of 0 ends the table. */
typedef struct RegionPair {
	u16 letter;			/* +$00 low byte: the header's region letter */
	s32 name;			/* +$02 offset of the system's name text in RegionCheck_Data */
} RegionPair;
CHECK_SIZE(RegionPair, 6);

void EntryPoint(void)
{
	register BootSetup *t = (BootSetup *)CODEPTR(Boot_SetupValues);
	register short i;
	unsigned short cmd;
	unsigned short status;
	u8 *hdr;
	u8 code;

	if (!IO_CTRL12_L && !IO_CTRL3_W) {
		if (IO_VERSION & 0xF)
			TMSS_PORT = 0x53454741;		/* 'SEGA' */
		status = VDP_CTRL;
		cmd = t->vdpReg;
		for (i = 0; i < 24; i++) {
			cmd = (cmd & 0xFF00) | t->vdpRegs[i];
			VDP_CTRL = cmd;
			cmd += t->regStep;
		}
		VDP_CTRL32 = t->vramFill;
		VDP_DATA = 0;				/* starts the fill */
		Z80_BUSREQ = t->regStep;
		Z80_RESET = t->regStep;
		while (Z80_BUSREQ_B & 1)		/* until the bus is granted */
			;
		for (i = 0; i < 0x26; i++)
			Z80_RAM(i) = t->z80[i];
		Z80_RESET = 0;
		Z80_BUSREQ = 0;
		Z80_RESET = t->regStep;
		{
			register u32 *ram = AT68(u32, WORK_RAM_START);

			for (i = t->ramLongs; i >= 0; i--)
				ram[i] = 0;
		}
		VDP_CTRL32 = t->vdpRegs2;
		VDP_CTRL32 = t->cramWrite;
		for (i = 0; i < 32; i++)
			VDP_DATA32 = 0;
		VDP_CTRL32 = t->vsramWrite;
		for (i = 0; i < 20; i++)
			VDP_DATA32 = 0;
		for (i = 0; i < 4; i++)
			PSG_PORT = t->psg[i];
		Z80_RESET = 0;
		CPU_SetSR(0x2700);
	}

	/* Boot_RegionCheck */
	status = VDP_CTRL;
	code = ((u8 *)CODEPTR(RegionCheck_Data))[(IO_VERSION >> 6) & 3];
	if (!code)
		for (;;)
			IRQ_WAIT();
	hdr = (u8 *)CODEPTR(Header_Regions);
	for (i = 0; i < 16; i++)
		if (hdr[i] == code)
			goto start;
	{
		register u8 *font = (u8 *)CODEPTR(Art_RegionFont);
		register u8 *p;
		register RegionPair *pair;
		char *s;
		char row;
		short glyph, line, bit;
		unsigned long px;

		VDP_CTRL = 0x8164;
		VDP_CTRL = 0x8230;
		VDP_CTRL = 0x8C81;
		VDP_CTRL = 0x8F02;
		VDP_CTRL = 0x9001;
		VDP_CTRL32 = 0xC0020000;		/* CRAM colour 1 */
		VDP_DATA = 0xEEE;
		VDP_CTRL32 = 0x40000000;		/* VRAM 0 */
		for (glyph = 0; glyph < 0x3B; glyph++)
			for (line = 0; line < 8; line++) {
				px = 0;
				for (bit = 0; bit < 8; bit++)	/* bit n of the row = pixel 7 - n, colour 1 */
					if (*font >> bit & 1)
						px |= 1UL << (bit << 2);
				font++;
				VDP_DATA32 = px;
			}
		row = 8;
		s = (char *)CODEPTR(Str_RegionMsgDeveloped);
		Boot_DrawText(s + 1, s[0], row);
		for (p = hdr; *p != ' '; p++)
			for (pair = (RegionPair *)CODEPTR(Tbl_RegionCodePairs); (u8)pair->letter; pair++)
				if ((u8)pair->letter == *p) {
					if (p[1] == ' ' && p != hdr) {
						s = (char *)CODEPTR(Str_RegionMsgSystemsPos);
						Boot_DrawText(s + 1, s[0], ++row);
					}
					s = (char *)CODEPTR(RegionCheck_Data) + pair->name;
					Boot_DrawText(s + 1, s[0], ++row);
					break;
				}
		s = (char *)CODEPTR(Str_RegionMsgSystems);
		Boot_DrawText(s + 1, s[0], ++row);
		for (;;)
			IRQ_WAIT();
	}

start:
	/* Boot_StartApp */
	if (!IO_CTRL12_W)
		unk_Boot_005458();
	Game_Main();
	if (0)
		(void)status;
}
