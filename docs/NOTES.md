# XMen2Recomp notes (phase 2)

Shared findings for whoever works the phase-2 criteria next. Criteria and status:
`xmen2disasm/docs/phases/P2-recomp.md`, `xmen2disasm/docs/progress/P2.md`.

## Setup decisions (2026-10-15)

- Engine fork `crabsmack/segagenesisrecomp` (Actions disabled). Its relative submodule URLs
  for `m68k-recomp-core` / `z80-recomp-core` resolved against the fork owner and failed; the
  fork's `.gitmodules` now uses absolute URLs.
- The recompiler core lives in `m68k-recomp-core`, a submodule of the engine, so recompiler
  fixes need a fork of it too: `crabsmack/m68k-recomp-core` (Actions disabled), branch
  `xmen2` (its `main` mirrors upstream).
- Build: Visual Studio 2022 generator, cmake from the uv environment (no system cmake).
- Trace runs use a private copy of the Release exe per output dir (`<out>/runner/`) with a
  `settings.ini` that sets both pads to 6-button (the TAS's BizHawk SyncSettings use 6-button).

## Engine fork changes

- `runner/input_script.c`: `PADS p1 p2` directive (whole 12-bit masks per player, hex), X/Y/Z/MODE
  button names, 16-bit masks, unlimited directives (the TAS is ~16k). `main.c` feeds scripted
  X/Y/Z/Mode into 6-button ports.
- `runner/sim_step.c`: `g_game_spec.on_frame_post(tick)` is now called at the end of every tick.
  It existed in `GameSpec` but nothing called it (nor `on_post_reset`/`handle_arg`).
- `video/genesis_vdp.c`: VSRAM cells keep 11 bits (hardware, gpgx); they were masked to 10.
- `m68k_interp.c`: `g_m68ki_insn_total`, a never-reset count of interpreted instructions (K7).

## Recompiler (core fork) changes

- Push-then-RTS (`move.l <ea>,-(sp)` / `pea` falling into an RTS no branch targets) is a
  computed jump to the pushed long. Only two sites: the boot hand-off `Boot_StartApp`
  ($1F483E, `move.l ($21C).w,-(a7); rts` -> Game_Main) and the scaler resume ($1FC6F6).
  Before the fix the entry point returned at frame 3.

## Divergence log (frame: symptom -> cause -> fix)

- 3: RAM frozen, "entry point returned unexpectedly" -> push+RTS emitted as a C return ->
  core fork codegen peephole (above).
- 4: game races ahead (Z80 driver upload a frame early) -> `VDP_ClearVram` starts a 64 KB
  VRAM fill DMA and polls status bit 1; the engine VDP completes fill/copy instantly, gpgx runs
  them at 204/17 bytes per line (blank/active, H40) with DMA-busy set -> OPEN: model DMA
  fill/copy duration in the engine VDP.

## Open questions

- MPW switch tables (`move.w Sw(pc,dN),dN / jmp Sw-2(pc,dN)`): the core's offset-table probe
  reads the table from the JMP base (= the JMP's own extension word, one word early), seeds
  `base` as code and decodes the table as instructions. This is the source of the 26
  `MISALIGNED_FUNC_ENTRY` warnings and the 52 `offset_table` hybrid dispatch sites. Fix in the
  core: recognise the preceding `move.w d8(pc,Xn),Xn`, read targets from its table, bound the
  table with `code_addrs_file`.
- Exact-every-frame RAM equality during loading (CPU busy across frame boundaries) needs the
  engine's 68000 cycle accounting and VDP/DMA timing to match gpgx 2.4 closely.
