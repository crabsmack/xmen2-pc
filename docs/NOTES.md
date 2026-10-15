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

## Exact timing (the approach that makes K5-K8 reachable)

The game's frame sync polls the VDP vblank status bit and then runs its logic straight
through the frame boundary, so the RAM at every frame end is a mid-logic snapshot: matching
it needs the 68000, VDP, Z80 and interrupts to be timed like the reference to the master
clock. The default engine scheduler is heuristic (per-line chunks of 488 cycles, atomic
interrupt handlers on a private stack with 256 bytes of RAM saved/restored, instant DMA,
WaitForVBlank yields), so the engine fork gained an opt-in exact mode:

- recompiler (`game.toml exact_timing = true`, core fork): `XI(pc, cost)` before every
  instruction (slice end / interrupt boundary + exact cost in master clocks), data-dependent
  costs inline, `XSR()` after SR writes, hardware call/return (RTS pops `g_xrts`; call sites
  unwind until the RTS came back to them).
- runtime (`GameSpec.exact_timing = 1`, `runner/exact_sched.c`): gpgx's frame layout, slices,
  interrupt entry, VDP port timing (status, FIFO, DMA), pads, Z80 sync.
- verification: an instruction-level trace on both sides. Reference:
  `xmen2disasm/emu/cycle_trace.py` (and `reg_probe.py` for registers at a PC); runner:
  `GENESIS_XTRACE=from,to,path`. `uv run tools/xloop.py --to N` builds, traces both, diffs
  (`tools/ctrace_diff.py`: first row where pc, master clock, SR, A7 or the register hash
  differ) and compares the frame CRCs. Each mismatch names one instruction.

Cost model: the recompiler's PRM model differs from the reference's Musashi tables in a few
places (ADDQ.W #,An 4; ANDI.L #,Dn 14; BSET/BCHG/BCLR Dn no bit<16 discount) and in which
costs are runtime (Bcc/DBcc/Scc/shift-by-register/MUL/DIV). `uv run tools/cycle_census.py`
compares the recompiler's static cost of every X-Men 2 instruction with gpgx's base table.

## Divergence log (frame: symptom -> cause -> fix)

- 3: RAM frozen, "entry point returned unexpectedly" -> push+RTS emitted as a C return ->
  core fork codegen peephole (above).
- 4: game races ahead (Z80 driver upload a frame early) -> `VDP_ClearVram` starts a 64 KB
  VRAM fill DMA and polls status bit 1; the engine VDP completes fill/copy instantly, gpgx runs
  them per line with DMA-busy set -> exact mode's DMA model.
- 3 (instruction trace): reset SR is $2704 on the reference; version register $A10001 reads
  $A0 (overseas, no expansion unit), not $80.
- 4: ADDQ.W #4,A7 costs 4 cycles in the reference (Musashi), the census key is the opcode's
  size field (the decoder reports An ops as long).
- 4: a V-int enabled by a VDP register write is taken after ONE more instruction (gpgx
  `m68k_set_irq_delay`, which executes that instruction inline without its trace hook: the
  runner's trace omits it too).
- 7: pad reads one frame late -> the runner ticked the input script after each frame; exact
  mode ticks it once before frame 1.
- 7: `Z80_BusAcquire` pops its return address, links A6, pushes the address back and returns;
  the recompiler's "callee captures the return" idiom made the caller return early, and
  `movem.l (sp)+` was counted as discarding two return slots -> hardware call/return in exact
  mode.
- Frames 1-10: 116,968 instructions identical, RAM/VRAM/CRAM/VSRAM CRCs equal (Z80 RAM
  differs from frame 7, reported only).
- 173: a 68K->VRAM DMA spanning lines: gpgx re-freezes the CPU at each line start by SETTING
  its clock (the triggering instruction's own cost is dropped) -> same in exact mode.
- 183: `lea Tbl(pc),a0; adda.w (a0,d0.w),a0; jmp (a0)` into another disassembly function's
  interior label (`Tbl_SegCollideCases`, raw in the disassembly) -> exact mode exports every
  interior code label of the disassembly (`game/xmen2.labels.toml`, extra_seeds): computed
  JMPs switch over in-function labels, and every label is a dispatch entry (host emitted as
  `func_body_HOST(entry)`). This also covers AI-script thunks entered at offset k and the MPW
  switch tables (the old table probes decoded them as code: K3's 32 "unsupported" were those).
- 410: the 68000 reads the Z80 driver's status byte ($A01B21) -> the Z80 must match too.
  Z80 instruction traces (`emu/cycle_trace.py --z80-out`, `GENESIS_ZTRACE`,
  `tools/ztrace_diff.py`): gpgx takes Z80 interrupts BEFORE an instruction (superzazu after),
  powers the Z80 up with all registers 0 and Z set; the driver spins on YM2612 timer A, so the
  exact mode models the YM timers on gpgx's sample grid (1008 master clocks per sample; reads
  and data writes catch up whole samples).
- Frames 1-600 of the TAS equal (K5). Z80 RAM differs from frame 379 (reported only; next).
- 377: the Z80's YM status read was timestamped early: superzazu charges indexed (DD/FD)
  instructions after their accesses; gpgx (MAME core) charges an instruction's base cost
  before executing it -> accesses timestamped at start + base cost (`z80_base_cycles`).
- 380: timer A ticked once more in the reference across a frame end: gpgx re-anchors the FM
  sample clock at frame end to the first sample boundary at/after the frame end, counted from
  the frame's first sample, so a sample a late (overshooting) Z80 access already ran is run
  again -> same re-anchoring (YM access logs with timer counters on both sides found it:
  `cycle_trace.py --fm-out`, `GENESIS_FMLOG`).
- 1479: `clr.l -(a7)` never pushed: CLR stored with the read-modify-write flag but no load
  -> plain store (core fork; a bug in every mode).
- 2009: dispatch miss inside the scaler's unrolled row blocks (entered at
  `$1FC8FE + (160 - width/2)*18`) -> `sync_disasm.py COMPUTED_ENTRY_RUNS` exports the 160
  block starts as label entries.
- 10094: the reference pushed an exception frame at a frame end: an `andi #$F8FF,sr` that
  unmasks a pending V-int takes it immediately, before the slice-end test; `set_irq` alone is
  not a check -> interrupt checks exactly at slice starts, SR writes and IRQ-latency slots.
- 33389: a V-int pending during the H-int handler is taken right after the handler's RTE
  (back to back) -> the boundary re-checks after each handler returns.
- Power-on demo (idle run) reads the HV counter: exact H-counter tables (H40: 14 runs of
  consecutive values with fixed master-clock lengths; H32: 2).
- **Whole TAS (57,465 frames) and the idle run (36,000) equal the reference on every column,
  Z80 RAM included; 0 dispatch misses, 0 interpreted instructions, ~745 fps uncapped.**

## Phase 4: decompilation (hand-written C)

Criteria: `xmen2disasm/docs/phases/P4-decomp.md`; check `uv run tools/phase.py 4` there.

### Logical positions, sync points, replay (D1, D2)

- Game code has no timing once it is C, so the whole game is compared at **sync points**
  (main-program entries to `VDP_WaitVBlank` $1FDC5C) and the C runs in an untimed **stepped**
  runner that replays the timed run's hardware reads and interrupts by **logical position**
  (engine `exact_sched.c` "Logical positions"): (interrupt depth, function entries in that
  context, hardware reads since the last entry). Entries are `XENTRY(addr)` calls the
  recompiler emits at each function's own entry (game.toml `entry_hooks`), counted only for
  the compiled C functions (`game/xmen2_functions.h`: the 3,067 functions with MPW's `link a6`
  prologue). The disassembly's other 553 "functions" are switch cases / blocks split off a C
  function (460, code inside their parent in C) and asm helpers (93); counting them would make
  a C parent's position differ from its generated version's.
- Timed runner, `GENESIS_REPLAY_OUT`: `replay.bin` = every hardware read ($A00000-$A1FFFF,
  $C00000-$C0001F; identical reads run-length coded), every interrupt with its position, every
  sync point. Stepped runner (`XMen2Stepped`, game.toml generated with `--untimed`: the exact
  shape without XI/XC), `GENESIS_REPLAY_IN`: reads return the logged values (the VDP still sees
  data/control reads: address increment, command latch), an interrupt logged at P is taken at
  the first event (entry, hardware read, SR write) that finds the program at P and unmasked,
  before it: exactly where the timed runner could have taken it. DMA completes at once. No Z80.
  A divergence (a read the reference didn't make, a passed interrupt) stops the run loudly.
- `sync.csv` RAM CRC zeroes `[stack_low, A7)`. stack_low = lowest A7 over the corpus
  (`XI` tracks `g_xa7min`; the trace JSON reports `a7_min` per run).
- First result (TAS frames 1-600, 428 sync points): stepped == timed at every sync point on
  RAM/VRAM/CRAM/VSRAM, with `AiScript_Set` already in C.

### Hand-written C in the runners

- One source per function, `game/c/<ADDR>.c`, against `game/include/xmen2.h`
  (`xmen2_mem.h` switches modes: MPW C 3.3 for byte matching, C++17 on PC).
- PC memory model: game memory stays the emulated 68000's (generated code shares it).
  `s16/u16/s32/u32` are big-endian wrappers, `PTR(T)` a stored 68000 pointer, `FNTYPE` an
  old-style stored function pointer (`xm::fnp`: any C function in, called with any arguments
  through `xm::call68`). Plain C pointers point into work RAM, which the engine maps **twice**
  (`ram_views.c`: `g_ram` = $00FFxxxx, `g_ram_ffff` = $FFFFxxxx, same pages): the original
  stores both 32-bit forms of RAM pointers (A5-relative arithmetic gives $FFFF.., absolute
  constants $00FF..; a TAS RAM scan finds both, for overlapping targets), so a host pointer
  must remember which one it is. Globals are on the $FFFF view (A5-relative in the original).
- Function address values: `self->state = Foo` stores what the original stored: Foo itself, or
  its jump-table entry (A5+d) when the original's segment reached it through the table.
  `game/xmen2.fnrefs.csv` (sync_disasm.py) lists, per function, every function address it
  takes and the value; the bridge looks it up for the C function running (`xm::fn_value`).
- Bridge (`tools/bridge.py`, build output): every C caller reaches a function by name; for a
  function in C that name is a wrapper that counts the entry and calls `hc_<Name>` (each
  game/c file is compiled with `-D<Name>=hc_<Name>`), for one still generated it is a stub
  that pushes 4-byte slots and calls the generated code (`xm::call_slots`, a JSR in the
  exact-mode call/return model). `hcslot_<ADDR>` takes calls from generated code (slots in,
  d0 out, RTS); in the stepped build it IS `func_<ADDR>` and the recompiler leaves the
  function out (`--skip`).
- `game/runtime/handc_rt.cpp` is the only hand-written code that touches registers.

### verify (D4, shadow differential tests)

`XMen2Verify` = the timed runner (all generated code, same object library as `XMen2Recomp`)
+ the C + `game/runtime/verify.cpp`. At each call of a function with C (`GameSpec.on_entry`
from XENTRY, which may now tell the generated wrapper to return): snapshot, run the generated
body with hardware accesses recorded and F's instructions traced (`g_xinsn_hook`), discard
the call if an interrupt was taken, else roll back and run the C untimed (`g_xend` frozen)
with the recorded reads replayed and writes captured; compare RAM outside [stack_low, returned
A7), A7, return target, d0 (as wide as the C return type), d3-d7/a2-a6, and the hardware
access sequence; put the generated result back. Nested calls run generated code in both.
Checks every call until 2,000 passed, then 1 in 64. Negative test: a planted bug in
`AiScript_Set` gives 45/45 mismatches ("RAM FF5107: generated 00, C 01").

### MPW matching from the game repo

`xmen2disasm tools/cc.py --match-recomp ../XMen2Recomp --json` compiles each game/c file
with the headers. MPW C 3.3's global symbol table can't hold every declaration of the game
(Fatal Error 429), so cc.py gives each compile the lines of `xmen2_funcs.h`, `xmen2_data.h`
and `xmen2_globals.h` that declare names the file uses (unused declarations emit no code).

## Open questions

- **Phase 4, for the user (definitions):**
  1. *Fragments*: 460 of the disassembly's 3,620 functions are blocks split off a C function
     (switch cases, `_partN`: no frame, only branched into). In C they are code inside their
     parent; they can't be compiled or called on their own. Proposed: a fragment is done when
     its parent is (its code named inside the parent's C, verified with the parent).
  2. *Live stack at sync points*: once a function on the call chain at a sync point (Game_Main,
     scene loops, wait loops) is C, its 68000 frame (return address, saved registers, locals)
     no longer exists in emulated RAM, so the RAM CRC can't match. Proposed: mask the whole
     68000 stack area [stack_low, $FFC518) at sync points, not only [stack_low, A7).
  3. *Never-executed asm*: 7 asm functions (no MPW frame) are never executed by the corpus
     (Nem_PutRowRam, Cyclops_ResetBeamFlicker, Boot_DrawText, Scaler_ExpandSource,
     Scaler_ClearRows, Scaler_Resume, Scaler_RowBlocks); MPW C can't produce asm, so they can't
     match. Proposed: allow `verify` with synthetic cases for them.

- MPW switch tables (`move.w Sw(pc,dN),dN / jmp Sw-2(pc,dN)`): the core's offset-table probe
  reads the table from the JMP base (= the JMP's own extension word, one word early), seeds
  `base` as code and decodes the table as instructions. This is the source of the 26
  `MISALIGNED_FUNC_ENTRY` warnings and the 52 `offset_table` hybrid dispatch sites. Fix in the
  core: recognise the preceding `move.w d8(pc,Xn),Xn`, read targets from its table, bound the
  table with `code_addrs_file`.
- (Resolved: frame equality while the CPU runs across frame boundaries, by exact mode above.
  Phase 2 complete 2026-10-15, K10 user-verified.)
