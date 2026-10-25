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
- `runner/exact_sched.c` (phase 4, 2026-10-17): the frame-synchronous runner renders a frame and
  its Z80 takes its V-int ("The D8 playtest build" below).

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
- Phase 4, whole game in C (stepped vs stepped-gen), TAS sync 265 (TAS frame 437, SiberiaB1):
  16 ice-shard records mirrored (each x, y offset the negative of the reference's) ->
  SiberiaB1_SpawnDebris drew `Rand_Next() & m - Rand_Next() & m` in one expression; MSVC drew
  the right one first, MPW the left -> the PC branch draws the left into a variable first (as
  Debris_Spawn already did). Scan for the pattern: only one other statement draws twice, a sum
  of equally masked draws (order-free).

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
- `sync.csv` RAM CRC zeroes the stack area `[stack_low, $FFC518)` (decision 6 below; it was
  `[stack_low, A7)`). stack_low = $FFAF1A, the TAS's lowest A7 (`XI` tracks `g_xa7min`; the
  trace JSON reports `a7_min` per run; the other 53 runs measured stay above it).
- **Replay by position cannot reproduce the timed run** (2026-10-15). TAS 1-600 equal, then:
  sync 661 RAM differs only in the live stack (stale interrupt-frame bytes in uninitialised
  locals: the interrupt landed at another A7); sync 1189: the V-int was taken at WaitVBlank's
  first instruction, before it clears `g_FrameUpdated`, the stepped run took it later
  (fix: record `since`, take "immediate" interrupts right after their event); sync 1209: an
  interrupt due before `Irq_Disable` was masked by the time of the next event (fix: check before
  SR writes, `XSRP`); sync 1243: the V-int lands ~7 instructions after WaitVBlank returns and
  the caller then sets a palette-dirty flag (take late = wrong); sync 2042: the same window, the
  flag set 5 instructions BEFORE the V-int (take early = wrong; `until` and the nearer side
  don't help). A TAS measurement (`GENESIS_SHARED_OUT`) finds 843 non-stack RAM bytes shared
  between the handlers and the main program (palettes, scroll, camera, sprite table, DMA
  queue, `g_VBlankCount`...), much of it reached through pointers, so no event C can reproduce
  pins the V-int inside those windows. Asked the user (P4 doc: "if D2 can't be made equal,
  stop and ask").
- **Decisions (user, 2026-10-15; P4-decomp.md 5-8):** D2/D6 compare the stepped runner with C
  against the same runner with all generated code (`XMen2SteppedGen`); the stepped runner is
  frame-synchronous; the sync CRC masks the whole stack area; fragments are done with their
  parent; the 7 never-executed asm functions may use `verify` with synthetic cases.
- **Frame-synchronous stepped mode** (engine `exact_sched.c`, no replay log): one sync
  interval per runner frame; at each sync point, after its row, the V-int (when enabled and
  unmasked); pads from a per-sync input script (`xmen2check.py` builds it from the run's
  `.input` and the reference frame of each sync point in the D1 `sync.csv`); DMA at once; VDP
  status alternates VBLANK per read (clear first after a sync point, so every wait for the bit
  ends: WaitVBlank sees clear, then set), FIFO empty, never DMA busy; H/V counter 0; the Z80
  runs a frame at each sync point (its V-int line first) and 8 lines per bus release by the
  68000. A run polling hardware 20M times without a sync point stops and names the function
  (the first model, VBLANK set after one clear read, hung in `VDP_UpdateScroll`'s caller).
  First result: TAS, 2,500 sync points, stepped (160 functions in C) == stepped-gen.
- The replay mode (`GENESIS_REPLAY_IN`) stays for diagnostics, with `GENESIS_REPLAY_TRACE`
  (events on both sides), `tools/replaydump.py`, `XMEN2_SYNC_DUMP` + `tools/syncdiff.py`.
- **A wait for the V-int with no sync point** (2026-10-15): with two players the game runs at
  half rate, and Level_FrameLoop ($1FA6C0) and PlayerSelect_Run ($065BD8) spin on
  `g_FrameTick`, which only the V-int handler sets, before reaching VDP_WaitVBlank. The
  frame-synchronous runner takes the V-int only at sync points, so it hung there
  (scenario-options-menu at sync 2,880, the 2h phase run killed). Engine `x68k_wait_vint(pc)`:
  takes the V-int in frame-synchronous mode, does nothing otherwise. Generated code calls it from
  the spins (game.toml `game_hook` sites -> `xmen2_instruction_hook`, while g_FrameTick is 0),
  hand C from `while (!g_FrameTick) IRQ_WAIT();` (`IRQ_WAIT()` is empty under MPW). The
  options-menu run now completes its 6,070 sync points (stepped-gen, 2,200 fps).

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
- **Whole game in C (D6, 2026-10-16)**: with every function in C, the stepped build's skip list
  also takes the fragments (switch cases, blocks, AI-script `jmp` tables) whose containing
  function is C, so it links no generated game code (decision 7). The C reaches a `jmp` table
  only by calling into it (an AI script's step), so `xm::call_slots` follows `jmp d16(a5)` /
  `jmp abs.l` to the target before calling (a jump changes nothing but the PC). The function
  table also maps `hc_<Name>`: inside its own file a function's name is `hc_<Name>`
  (`-D<Name>=hc_<Name>`), and a function storing its own address (a state that re-arms itself,
  e.g. CoreElevator_WaitAtBottom) takes that one; `xm::fn_value` stopped the stepped TAS run at
  sync point 21 on such an address (not in the table).
- **The boot code in C**: EntryPoint (the reset vector) and its two blocks, Boot_RegionCheck and
  Boot_StartApp, were tagged fragments with no C parent, so a hand-C-only build had no code to
  start from. `game/c/1F43AE.c` is EntryPoint (tag `asm noreturn`) with both blocks inline:
  the ICD_BLK4 set-up from Boot_SetupValues, the region check and lockout screen, then
  unk_Boot_005458 and Game_Main. A `noreturn` function gets no shadow test (verify runs a call to
  its return; bridge.py leaves it out of the verify table): the whole-game traces check it. The
  two interrupt vectors (Irq_VBlankVector $1F4860, Irq_HBlankVector $1F4870: call the hook at
  A5Params +$10/+$14) were fragments too: now C (tag `asm conv vector`), their register saves
  and `rte` in an adapter (asm_conv.cpp) that pops SR and PC exactly as the generated rte does;
  `vector` also means no shadow test.
- **Game-memory locals whose address is kept (2026-10-16)**: scenario-game-over and
  scenario-continue (sync 5729) and fuzz-25200-s2 (24145) differed from stepped-gen in RAM only,
  6 bytes: g_SceneVar3B28 / 3B2C and four objects held `$00FFC48C` / `$00FFC4CE` where the
  original has `$00FFC43C` / `$00FFC47E`, the player select's two slots (BigSprites on the
  stack; cursors and portraits keep pointers to them). The C placed them relative to its own A7,
  but this C keeps no frames on the 68000 stack, so its A7 there was 0x50 higher than the
  original's. The original's is fixed (only Game_Main -> Game_SceneLoop -> the scene's run ->
  Cerebro_Run lead there; the TAS emulator's RAM has `$FFC43C` at every player select), so
  PlayerSelect_Run's PC branch now puts its frame at a6 = $FFC4C0. The TAS never showed it: no
  sync point falls inside its player select, and by the next one the variables hold other
  scene values. A
  function placing stack locals relative to A7 (Cerebro_PlayIntro and the seven others using
  `xm::sp_reserve`) has the same gap if an address it hands out stays in RAM to a sync point;
  none does in the corpus.

### verify (D4, shadow differential tests)

`XMen2Verify` = the timed runner (all generated code, same object library as `XMen2Recomp`)
+ the C + `game/runtime/verify.cpp`. At each call of a function with C (`GameSpec.on_entry`
from XENTRY, which may now tell the generated wrapper to return): snapshot, run the generated
body with hardware accesses recorded and F's instructions traced (`g_xinsn_hook`), discard
the call if an interrupt was taken, else roll back and run the C untimed (`g_xend` frozen)
with the recorded reads replayed and writes captured; compare RAM outside [stack_low, returned
A7 + the function's own argument slots) (the slots since 2026-10-16, P4-decomp.md decision 9:
4 bytes per parameter of a stack-convention function, `arg_bytes` in the verify table), A7, return target, d0 (as wide as the C return type), d3-d7/a2-a6, and the hardware
access sequence; put the generated result back. Nested calls run generated code in both.
Checks every call until 2,000 passed, then 1 in 64; since 2026-10-17 a call the sampling skips
is still a probe while the function's checked calls haven't covered all its instructions
(`XMEN2_VERIFY_INSNS`, counts from xmen2check.py): its generated run is traced, and it is compared
only if it reached an instruction not covered yet (Obj_CollideSegs' stuck player and two upward
pushes ran in 9 and 33 corpus runs, in no sampled call). Negative test: a planted bug in
`AiScript_Set` gives 45/45 mismatches ("RAM FF5107: generated 00, C 01").

Additions (2026-10-15, the asm functions):
- **Tail calls.** A `jmp` to another function is a pending tail call in the caller's dispatch
  frame (`recomp_tail_call`); the generated run now goes through `recomp_call_func`, which has
  its own frame and drains it, so a function ending in a jmp (Math_LDivS) is compared after
  the jump ran. Before, such calls compared a half-run function (A7 off by 4).
- **Nesting.** A function with C called inside another's generated run is tested there too
  (a stack of up to 4 test contexts; hardware accesses of a nested generated run are part of
  every enclosing one). Before, a callee only ever called inside a tested caller was never
  tested (Math_DivModS inside Math_LDivS).
- **Synthetic cases** (`game/verify/<ADDR>.syn`, `xmen2check.py verify` passes the directory):
  mutate the entry state of the function's first top-level corpus call (or, `at any`, of the
  first call of any function) and compare both versions, untimed, hardware reads supplied by
  the case (default 0), writes captured. For paths the corpus never takes, calls the corpus
  can only make with an interrupt in the middle (VDP_ClearVram's DMA wait), and functions it
  never runs. They count as checked calls and add coverage.
- **Register conventions** of the 93 hand-written 68000 functions (xmen2_funcs.h tags, read by
  bridge.py): `regs(d0,d1)->d0` (parameters in registers, result register), `&d1` (a pointer
  parameter in and out of a register), `a0=$A00036` (a constant input the C doesn't take),
  `@8(a6)` (an argument of the caller's frame: Intro_WaitFrame), `clobbers(...)` (callee-saved
  registers left changed), `conv` (frame/stack effects: hand-written adapters in
  `game/runtime/asm_conv.cpp`; Z80_BusAcquire builds a frame in its caller that
  Z80_BusRelease, reached by jmp, unwinds). verify compares an asm function's result
  registers and SR's system byte; a register convention's other scratch registers are free.
  CPU-only operations have names in xmen2_mem.h: `CPU_GetSR/SetSR` (SR writes go through the
  engine's `x68k_sr_pre/written` like generated `move to SR`), `CPU_GetSP`, `ADDR68(p)` /
  `AT68(T, a)` (pointer <-> 68000 address number, for DMA sources), byte ports.
- **Z80_BusRelease** ($1FE4EC) is its own function (xmen2disasm split it out of
  Z80_BusAcquire; 3,621 functions): ten Snd_* functions jmp to it, and inside Z80_BusAcquire's
  range its instructions could never be covered by Z80_BusAcquire's own calls.
- **Parallel compiles (2026-10-16)**: each C file is compiled through a generated wrapper
  (`build/bridge/handc/<ADDR>.cpp`: `#define Name hc_Name`, `#define XMEN2_FN`, `#include` the
  file). Per-file `COMPILE_DEFINITIONS` (the old way) gave every file its own cl command line,
  which MSBuild runs one at a time, so `/MP` did nothing: a full rebuild took ~90 min at 1-3%
  CPU, now ~1.5 min. Don't add per-source compile flags to the runner targets.
- **No identical-code folding (2026-10-16)**: a Release link folds identical functions
  (`/OPT:ICF` is the default without /DEBUG), and BrainRocket_StateFly / DropRocket_StateFly
  (each re-arms itself: `SET_STATE(self, itself)`) became one host function: the function table
  then gave DropRocket's state the address $133B42 ("xm::fn_value: $133CD0 takes the address of
  $133B42, which the original never did", 27 verify runs from fuzz-34200 on). CMakeLists links
  with `/OPT:REF /OPT:NOICF`, and the bridge's function table checks once that no two game
  functions share a host address.
- **Builds while C is being written**: `xmen2check.py build` compiles a snapshot of game/c and
  game/include (`build/handc_src`), and bridge.py leaves out a file whose definition doesn't fit
  its prototype (caught mid-edit), so concurrent edits can't mix two states into one link.
- **The whole game in C showed four harness gaps (2026-10-16)**, all in verify, none in the C:
  (1) Game_Main never returns, and a test opened on it (then the scene, level and frame loops,
  all spanning frames) used up the 4 nesting levels, so the per-frame code (Rand_Next, every
  Update...) was never tested in a full run. Game_Main is `noreturn` (no test); a test whose
  generated run takes an interrupt is "doomed" (it will be skipped) and stops collecting
  hardware accesses and coverage; nesting goes to 16 levels (cost is linear: a nested test adds
  one generated run per enclosing C run, and a doomed test has none). (2) The C run and a
  synthetic case's generated run are untimed, but an SR write still forced an interrupt check
  that took a V-int fallen due during the generated run (g_VBlankCount +1 in Music_SetPlayerParams
  and Cam_New): engine `g_xirq_hold` makes `x68k_sr_written` request no check while verify holds
  it. (3) The same in PlayerSelect_Run's synthetic case: it took a V-int, and the run never left
  that case's context, so no test ran after the player-select screen. (4) A C function passing a
  host local through a `void *` (Mem_Set(&placement), the Bitmap_* and Nem tables) to a callee
  that verify runs as generated code: the bridge can't copy it (size unknown); the verify
  wrapper now runs the callee's C for a host pointer (generated code couldn't reach it anyway),
  and Nem_BuildCodeTable's table is typed (NemCodeTable).
- Two PC traps the shadow tests found: `(void)IO_DATA(1);` performs no read on PC (a void cast
  never calls the accessor's conversion; assign it), and an object without a sprite definition
  makes the 68000 read the vector table where the host would dereference null
  (`(SprDef *)Vectors` under `#ifndef XMEN2_MPW`). The same for an object made with no
  placement: Boulder_Spawn's boulder (TUSK carries it) has `pl` 0, and Boulder_Init's
  `pl->param[0]` is the ROM word at 6 on the 68000 (verify crashed in fuzz-21600-s1 at frame
  ~23700; bisected to $05BF4A). No other Init that gets a null placement reads it. And an
  object with no animation script: a WALLCRAWLBIO's first Move comes before its first
  Obj_SetAnim, and WallCrawlBio_Move reads `anim->delay` anyway (verify crashed in the TAS at
  frame ~32650, $11E176); `ANIM_SCRIPT(o)` (xmen2_macros.h) reads the vector table for anim 0
  on PC, in it, BabyBio_Move and PowerUp_Update (after a one-shot clears anim). At address 0 an
  AnimScript is flags 0, count $FF, delay $C518 (the initial SSP $00FFC518). And Player_IsClimber
  given no object (SpaceB1_Seg28Handler's segment message, TAS frame ~35170): its class is the
  long at 0, $00FFC518, no player's. And CloneA2_StartCamera after Players_PlaceAtStart cleared
  g_P1StartRec (fuzz-48600 on): the start is the vector table's words at 2 and 4. These states
  come from the timed runner (verify), where an
  interrupt can land mid-set-up; the stepped runner (D2/D6) never made them.
- **Writing synthetic cases (2026-10-17)**: `poke N b|w|l ADDR = VALUE` writes RAM once the run
  has made N hardware reads (both runs read in the same order, so both see it at the same point):
  input that arrives a frame later, e.g. a pad press (the V-int copies the pads into
  g_Pad1Raw/g_Pad2Raw, which a synthetic run never changes, so a value held from the start is a
  press only on the first frame: PlayerSelect_Run's joining player). `XMEN2_VERIFY_TRACE_CASE=<name>`
  prints the instructions a case's generated run executed (event_second_pass gathered no contact:
  g_SegHighWater is $FFCD8C and g_SegLevelCount $FFCD8E, not the other way round).
- **A crash names itself (2026-10-16)**: XMen2Verify installs a vectored exception handler
  (`game/runtime/crash_win.cpp`) that prints, before the runner dies, the tests open (outermost
  first, with their mode: 1 generated run, 2 C run) and the C function running
  (`xm::fn_current`): `[verify] host exception C0000005 ...; C function running $1F41C2`. It
  replaces bisecting a crash by frames and function sets (an hour per crash).
- **Two more bridge gaps (2026-10-16)**, both in calls between generated code and C:
  (5) a plain `short *` / `long *` parameter (the C's host value, a caller's local) that arrives
  from generated code points at a big-endian 68000 local, and the C wrote it in host order
  (Magneto_ShotOrigin and its four siblings: "RAM FFC406: generated 00, C 63"). The slot entry
  now gives the C a host copy and writes it back after the call (`host_scalar`, the reverse of
  call68's Scratch). (6) A host array through a typed pointer: Obj_CollideSegs passes its
  `SegContact list[30]` to Seg_GatherNearObj; call68 copied one element (8 bytes) to the 68000
  stack, and the generated callee wrote the list's end marker past it, over Obj_CollideSegs' own
  return address (synthetic case event_wall: "return to: generated 001F15B0, C 00010000", the
  marker's flags word 1). The verify wrapper now runs the callee's C for any pointer to host
  memory (gap 4 widened from `void *`). Two kinds of pointer still go by register: register
  out-parameters (`&dN`) and a register image (bridge.py `REG_IMAGES`): Scaler_RowBlocks'
  adapter loads the host ScalerRegs into the registers, and Scaler_BeginPass hands back the d7
  the generated row loop leaves (with the C run instead: 293 synthetic-case mismatches in
  Scaler_BeginPass / Scaler_Resume). The Nemesis/Enigma `conv` adapters pass their pointers as
  68000 addresses, so for them a host pointer runs the C too (excluding all `conv` functions
  stopped fuzz-01800-s1 at "xm::addr: host pointer ... is not in game memory"). The C run this
  way is entered like any C function (`fn_enter`): without it the references of the caller
  applied, and Hornet_SpawnForDeathbird's C (given Deathbird_StateSendHornets' Placement)
  stopped at "$1128E4 takes the address of $136642, which the original never did".
- A C bug the shadow test found: ApocBomb_Draw drew the shadow whether or not the bomb was on
  screen; the ROM's off-screen tests of the bomb all branch to the end (fuzz-25200-s1, "RAM
  FFE21D": a sprite slot holding the shadow's attribute where the original left the old one).

### The D8 playtest build (2026-10-17)

First played on Linux (GCC 13 / SDL 2.30), to Avalon: plays fine, no crash, 0 misses.
Getting there:
- **No picture**: the stepped scheduler's frame returned without rendering a line (only the timed
  path rendered), so the playtest build was black on every host; the CRC checks never look.
  Frame-synchronous mode now renders the whole frame right after the V-int at each sync point
  (`fsync_render`): no H-int, and the main program's mid-frame VDP writes show a frame late.
  Status reads there are synthesized (`fsync_read`), so sprite flags from rendering change
  nothing.
- **No sound**: the Z80 driver's main loop ($0047-$0062) waits for its V-int (the handler at
  $38 stores SP to $3E, the loop polls $3F). `fsync_z80_frame` held the IRQ for frame time
  [0, MPL) only, and the 8-line slices of the 68000's bus releases had run the Z80 to ~165k
  master clocks by then: the IRQ was never taken and the driver went quiet after its init. The
  IRQ is now held for a line from where the Z80 is. Over 600 frames XMen2Stepped and
  XMen2SteppedGen write bit-identical audio, and their sync CSVs agree at all 900 sync points.
  **D2/D6 results recorded before this change used the old Z80 model: re-run them.**
- **Level_LoadUsedTiles overflows its bitmap, as the original does**: Bitmap_FromTileWords
  clears count / 16 + 1 words, up to ~1 KB for the block definitions, into `bits[$100]`. In the
  ROM's frame `remap` sits right above `bits` (-$1004 / -$1104(a6)) and takes it; GCC's stack
  protector caught it at the first level load. The PC branch now keeps that layout (one struct).
  Where the bytes went in MSVC's frame is unknown: Windows runs before this may have had silent
  corruption there.
- **LP64**: `long` is 32 bits on MPW and Windows, 64 on Linux. `xmen2_mem.h` maps it to `int`
  on `__LP64__` (so every file includes system headers before it; the bridge now does). Long
  literals (`0x10000L`) keep 64 bits there; the 24 files with one all truncate back to 32.
- **`be(T)` is explicit**: `c ? 5 : field` with a `be<T>` field converted both ways, which GCC
  rejects (155 sites, mostly MAX/MIN/ABS). Now the field converts to its T, as C promotes it.
- `xmen2check.py` passes `CMAKE_BUILD_TYPE=Release` off Windows. `pccheck.py` is still MSVC-only.

### MPW matching from the game repo

`xmen2disasm tools/cc.py --match-recomp PATH/TO/XMen2Recomp --json` compiles each game/c file
with the headers. MPW C 3.3's global symbol table can't hold every declaration of the game
(Fatal Error 429), so cc.py gives each compile the lines of `xmen2_funcs.h`, `xmen2_data.h`
and `xmen2_globals.h` that declare names the file uses (unused declarations emit no code).

### The last C functions matched (2026-10-24)

Of the 130 game/c files that didn't match at the end of phase 4, 117 are hand-written 68000 in
the original: 97 tagged `asm`, and ~20 in an MPW-style frame whose bodies use what MPW C 3.3
never emits (`dbra`, `exg`, `rol`/`ror`, `a2` as a variable, a store into an argument slot).
The other 13 were C and now match (3,047 of 3,164 files; `docs/p4_mpw_matched.txt`). What it
took, as rules for the next match (also in xmen2disasm docs/COMPILER.md, b876007):

- `while ((x & 8) != 0)` / `== 0` on a word gives `moveq #0,d1 / move.w d0,d1 / tst.l d1`; a
  bare `while (x & 8)` doesn't (VDP_WaitVBlank). `(signed char)b & k` on a u8 field gives
  `moveq #k / and.b mem` (SiberiaB1_DebrisUpdate).
- `const & expr` loads the expression first and emits `and.<sz> #imm,d0` (the C03C encodings);
  `expr & const` with a memory operand emits `moveq #c,d0 / and mem,d0` (Spr_SetFromDef).
- A char function ending `if (c) return 0; return 1;` gives `sCC d3 / neg / seq d0 / neg`;
  `return !c` adds `move.b d3,d0` (BigAcolyte_OutsideActiveWindow).
- A global array defined in the compile unit (`#ifdef XMEN2_MPW` definition, as g_Timers) is
  kept in an address register and indexed `.w`; through a pointer local it is `.l`
  (Scene_Register).
- A global read through `*(T *)array_global` stays in a3/a4 across stores; a typed scalar global
  (`*(T *)&long_global`) is reloaded into a0 after each store. g_SceneVar3B20 is now `s32` and its
  18 users take its address (SavageA4_CamUpdate).
- Comma expressions (`t = v, a = y - t, y = t;`) stop copy propagation and the extra copy
  temporary (Cam_ClampLookToPlayers); several reads of a global pointer in one comma expression
  keep it in a0 (SiberiaB1_StepWind).
- Registers: dropping `register` from competing variables and register parameters lets use counts
  decide (SavageA2_SetBlockTiles, ApocBomb_Draw); a register is reused on the plain variables'
  timeline too, and among equal counts the later declaration is placed first. Variables reused
  across a branch (one branch's temporaries in registers, the other's on the stack) point to the
  original reusing them (Cam_SpringAxis).
- A dead `if (0) (void)v;` (the original has 101 compiled-out blocks) adds v's reference weight
  and live span: it reorders registers (PlayerSelect_Run) or keeps one busy through an if-arm so
  a copy goes to the shared stack temp (Tile_RunHooksAtPoints).
- An empty `do { } while (0);` is a label: after a `register`/long assignment it gives `tst.l dN`
  plus the pending argument pop (SiberiaB1_StepWind). Both `?:` constants need `(short)` for
  `move.w #imm`.

## Phase 5: native port (measurement first)

Criteria: `xmen2disasm/docs/phases/P5-port.md` (draft). Before any state moves, the target, the
exporter and the measurement exist and are proven on the phase-4 C (LESSONS.md "Phase 5 specific").

### XMen2Native and the export (2026-10-17)

- `XMen2Native` (CMakeLists.txt) = XMen2Stepped's sources (the D6 hand-C build) compiled with
  `XMEN2_STEPPED=1 XMEN2_NATIVE=1` (target-level only: /MP keeps batching), plus
  `game/runtime/export.cpp`; linked with `/MAP` (`build/Release/XMen2Native.map`). `xmen2check.py
  build` builds it with the other four runners. `xmen2_mem.h` NATIVE mode is PC mode for now:
  the place native types land.
- **Its sync rows are computed from the export image only** (`xmen2_spec.c` `xmen2_on_sync`:
  `xm_export(&image)`, then the same `sync_row` the other runners use; `XMEN2_SYNC_DUMP` dumps
  the image). `game/runtime/xmen2_image.h`: RAM, VRAM, CRAM (& $0EEE), VSRAM (& $07FF) in the dump
  files' layouts. The other runners now build the same CRAM/VSRAM bytes with `xmen2_words_be` and
  call `sync_row`: one CRC path.
- `xm_export`: (1) copy the emulated work RAM, (2) write every global of `xmen2_globals.h` by
  name, address and declared type (`build/native/xmen2_export_globals.inc`, generated at build
  time by `tools/export_table.py` from the snapshot headers: 549 globals, 62 arrays, 37 declared
  without a length, taken to the next global), `put_value` serializing each value in its 68000
  form (be<> big-endian, bptr/fnp raw, a record as its bytes; a record in native storage with no
  writer is fatal), (3) VRAM/CRAM/VSRAM from the engine's VDP. Counts (runner counters,
  `trace --mode native` JSON `exported`): `native_bytes` = work-RAM bytes written from storage
  outside g_ram, `copied_bytes` = the rest. Baseline on every run: **native_bytes 0, copied_bytes
  65,536** (no global lives off g_ram yet), the globals cover 14,851 RAM bytes, video 65,744.
- Negative test, `XMEN2_EXPORT_SELFTEST=1` (or a global's name): the global's last byte is zeroed
  in the image. Default `g_TextWinTop`: its low byte $FFC5A9 is 0 through TAS sync 4,528 and 5 from
  sync 4,529 (image dumps); the TAS trace with the self-test is equal through 4,528 and differs
  from 4,529, RAM CRC only (picture equal throughout). (First choice `g_P1Lives` differed at sync
  1: the lives are 8/9 from the first sync point, not only "at new game"; `g_LevelFrame` at sync 2.)
- **The picture, `trace --fb`** (stepped modes): since engine 22ed7f6 the frame-synchronous runner
  renders the whole frame right after the V-int at each sync point (`fsync_render`), at the start
  of the next runner frame, and runner frame k ends at sync k. `--fb` runs the runner with
  `--hash-frames 1` (main.c `[FBHASH]`: FNV-1a-64 of the framebuffer's active region, the raw VDP
  output, at the end of every runner frame) and adds column `fb_fnv1a64` to sync.csv: row k = the
  frame rendered after sync k-1's V-int (row 1: the boot's empty frame). The tool checks one hash
  per row. It is FNV-1a-64, not a CRC32, because that is what the engine computes; a CRC32 would
  need an engine accessor to `s_framebuf` (or a GameSpec `video` hook, which would also change the
  playtest build's widescreen options).
- **Result (engine 22ed7f6, the D8 Z80/render change):** the stepped-gen references regenerated in
  this tree (`synccorpus.py --mode stepped-gen --fb`, 272 s wall) equal the phase-4 ones on every
  row of all 76 runs: the Z80's V-int moved no sync row. `trace --mode native --fb` equals them on
  **all 76 corpus runs**, every row, the four CRCs and the picture (2,042,653 sync points; 0 misses,
  0 interpreter instructions; `synccorpus.py --mode native --fb --against`, 275 s wall at 24 jobs,
  longest fuzz-55800-s2 142 s). Before the bump the same 76 took 92.5 s (TAS alone 2,200 fps); the
  render costs ~3x (TAS alone 674 fps, 631 with `--fb`: the hash is ~7%). A trace that compares
  no picture skips the render since 2026-10-24 (`--no-render`, "Phase 5 harness" below).

### What the C still needs from the 68000 (`build --native`, baseline 2026-10-17)

`tools/native_scan.py` (pure-Python COFF reader, regular and /bigobj; MSVC names reduced to
`ns::name`): the undefined externals of every object of XMen2Native (from its .vcxproj; hand-C
wrapper `build/bridge/handc/<ADDR>.cpp` = `game/c/<ADDR>.c`), and the 68000-machine symbols the
link map says the exe defines. ~1-2 s warm (30 s on a cold cache right after a build).

| category (3,164 game/c objects) | references | files | symbols (files) |
|---|---|---|---|
| cpu68 | 2,986 | 1,018 | xm::call_slots 808, sp_restore 815, sp_reserve 767, g_frame/g_novalue 281 (NO_RETURN_VALUE), CPU_SetSR 11, CPU_GetSP 11, CPU_GetSR 9, irq_wait 3 |
| emu_ram | 3,716 | 2,057 | g_ram_ffff 2,057 (XM_G globals), g_ram 1,659 |
| bus | 2,526 | 868 | m68k_read8/16/32 ~815 each, write16 46, write32 29, write8 10 |
| rom_image | 1,957 | 1,957 | g_rom |
| fnid | 1,188 | 1,188 | xm::fn_value |
| generated | 0 | 0 | |

- The largest dependency is **call68**: a call through a function value stored in game memory
  (fnp, class methods, state functions) goes `xm::call_slots` -> recomp_call_addr -> dispatch ->
  the bridge's func_ADDR -> hcslot reads the 68000 stack slots, although every target is C. Its
  `Scratch` (host pointers on the 68000 stack, sp_reserve/sp_restore) references all three
  `m68k_read*` in its copy-back, so most bus reads are call68 too; the real hardware accesses are
  the writes (46/29/10 files). Native dispatch of stored function values (fn_host, typed call)
  is the first thing to port.
- g_ram/g_ram_ffff/g_rom also come from `xm::host`/`xm::addr` (every stored pointer, bptr),
  not only from globals and ROM data.
- Runtime objects (bridge, handc_rt, asm_conv, export, spec): cpu68 42, emu_ram 7, bus 12, rom 3.
- Link (`link.categories`): cpu_core 175 (exact_sched.obj, g_cpu and m68k_read/write in glue.obj,
  CPU_* in handc_rt.obj), interpreter 48 (m68k_interp.obj, m68k_decoder.obj), recomp_runtime 6,968
  (6,781 of them the bridge's func_/hcslot_/conv_ slot entries; glue.obj 145, dispatch 12),
  generated 1,951, emu_ram 3 (ram_views.obj). Compiler labels ($unwind$, string literals) and the
  CRT's inline printf/scanf COMDATs are left out; C tentative definitions (g_cpu, g_ram, g_x*), which
  the map lists as `<common>`, are resolved to their declaring objects from the COFF tables.
- **Generated code still linked**: 0 game functions (D6 holds), but 1,951 func_ definitions: 1,606
  A5 jump-table thunks (`jmp abs.l` entries; call_slots follows such a jump itself) and **345
  interior labels of C functions** (`game/xmen2.labels.toml`) in 37 generated bodies, e.g.
  `func_body_005CF8` inside Game_SceneLoop with 17 entries. D6's definition counts only function
  starts, so it never saw them. Nothing in the corpus showed a difference, but a dispatch to one
  would run generated code silently instead of stopping: the stepped build's skip list should
  take them (open, P3).
- `build --native` reads the MSVC build (COFF objects, .vcxproj, /MAP); elsewhere it says so.

### Builds (this machine, own build tree)

Clean build of all five runners (3 recompiler runs, 32 threads, + the scan) 3m42s; of the four
phase-4 runners alone 3m16s; after the rebase onto the D8 commits (engine and header changed:
nearly everything recompiles) 3m25s; one source changed 2m02s; nothing changed (`build --native`,
MSBuild up-to-date checks of 5 targets + the scan) 23 s. The scan is ~1-2 s warm, 15-30 s on the
cold cache right after a build. The build tree is ~0.7 GB without traces. Note: any edit of
`game/include/*.h` (even a comment)
changes the key of xmen2disasm's `cc.py --match-recomp` cache, so the next `phase.py 4`
re-matches every file once (a 6-file sample re-matched after this change).

### Phase 5 harness (P0 gates, 2026-10-17)

The P0 exit gates (xmen2disasm LESSONS.md "Kickoff checklist") measured by `tools/bench.py` ->
`docs/bench.json`: **clean build 213 s (< 5 min), corpus check 860 s (< 15 min), two build dirs
at once OK: all three pass.** Machine: a 16-core / 32-thread desktop; normal
priority; engine 22ed7f6. Numbers before 14:40 had another agent's 33-`cl` build running for part
of the time (noted where it mattered).

- **Builds.** Clean, five runners: 213 s; `cl.exe` 33 at once (mean ~29 while compiling), none
  during the first ~60 s (configure, recompiler build, three generations) and the links. Two
  fresh `XMEN2_BUILD` dirs at once: 354 s each. Nothing changed: 16-21 s (`gate.py`), 39 s the
  first time. After the engine bump: 238 s; a comment in a header: 133 s (the hand C compiles in
  three targets); relinking all runners: 35 s.
- **Reproducible runners** (CMakeLists `XMEN2_REPRODUCIBLE`, default ON): `/Brepro` on compile and
  link (no PE/COFF timestamps; `__DATE__`/`__TIME__` become "1") and
  `game/runtime/reproducible/genesis_build_info.h` ahead of the engine's (whose `git describe` of
  both repos changed every runner at every commit). Same SHA-1 for all five runners after: a
  relink; a comment edited in `xmen2_mem.h`'s PC branch; a comment line added there (lines
  shift); that header rewritten with LF endings; a planted bug, reverted, with a build killed in
  between; the worktree's build vs a clean dir vs two dirs built at once (bench). The exes hold no
  absolute paths. So caches can key on runner SHA-1: a commit that changes no code reruns nothing.
  (New worktrees check out CRLF here, `core.autocrlf=true`, the main checkout's files are LF: no
  effect on the runners; it does change `cc.py`'s match-cache keys.)
- **pccheck --all**: 558 s serial (`cl` max 1; partly beside the other build) -> **78 s** (pool
  of 32, mean 30 `cl`), same verdicts (3,164 OK, 162 with warnings). ~0.75 CPU-s per file, nearly
  all parsing the headers; no PCH is possible (`-D<Name>=hc_<Name>` renames the file's own
  prototype in them). MSVC only.
- **Corpus check, phase 4's way** (engine 22ed7f6): per run stepped-gen then handc trace, 24 jobs:
  414 s; then `verify --jobs 24`: 503 s; **917 s in sequence: over the gate**. Before the engine
  bump the traces took 62 s (1,460 sync points/s per job; the frame-synchronous render now costs
  ~3x: 422-446/s per job at 24 jobs, 515-535/s alone).
- **Corpus check, bench**: one pool of 32 runners holding the 76 verify runs (`verify --runs R`,
  the same per-run test) and 76 trace tasks (stepped-gen reference rows, then `--mode native`),
  longest first: **860 s** with the reference rows made fresh (an engine change); with them cached (a C
  commit) not measured yet (the references are ~6,100 of the 26,920 job-seconds). Job-seconds: verify 14,622, references 6,139, native 6,159 = 26,920
  in 860 s, 31.3 runners busy on average: CPU-bound, no single run is the pole (longest: verify
  fuzz-55800-s2 345 s at 172 fps; trace task fuzz-55800-s1 154 + 159 s at ~340 fps).
- **Where the time goes (for the savestate step, LESSONS item 2).** Per job at 32 at once:
  verify ~172 fps (TAS alone 273 fps; the timed runner without shadow tests 356 fps), stepped-gen
  and native ~345 sync points/s. (1) verify is 54% of the work, and 62 of its 76 runs are fuzz
  runs that replay the TAS up to their branch point: of 2,273,389 verify frames only 62 x 3,600 =
  223k are fuzzed; states every 1,800 TAS frames on both sides would cut verify ~4x and the fuzz
  traces likewise. (2) The render in frame-synchronous mode is ~2/3 of every trace (12,300
  job-s); traces that compare no picture (P1; only P7 does) could skip it (an engine switch:
  done 2026-10-24, `--no-render` below).
  (3) Shadow tests cost ~25% of the TAS's verify speed (273 vs 356 fps).
- **verify branches from the TAS (2026-10-24, (1) above for verify).** A run whose input starts
  with the TAS's first N >= 600 frames (all 62 fuzz runs at their branch frame; 11 of the 12
  scenarios, mostly at 2,345) is played as a branch of a process playing the TAS: the engine's
  `--branch-plan` (main.c "Branch plans") snapshots the machine after frame N, plays the run's
  remaining frames, writes its results and restores. In-process only: the snapshot is the
  engine's rollback state (rb_state.c), whose `exec` section is the game fiber's raw stack (host
  addresses). verify.cpp keeps its testing state across a branch (`GameSpec.branch_save/
  restore`; its per-run JSON at `branch_end`), so a branch reports exactly what the run reports
  from power-on. `xmen2check.py verify` groups the branches into processes (`verify_units`:
  latest first, bounded so that, longest first on `--jobs` workers, they finish soonest) and
  reports them as `processes`; `--no-branch` is the control (every run from power-on).
  - Two gaps, found on the way. (a) The rollback snapshot had none of the exact scheduler's state
    (`exact_sched.c`: the 68000 clock and slice, the VDP FIFO/status/DMA, pads, the Z80 clock,
    the YM timers): the engine's determinism probe (`GENESIS_RB_PROBE=500:64`) forked at the
    first replayed tick (bus, z80) on every pass. Now an `exact` section (digested as `sched`):
    the probe passes 57/57 over the whole TAS (period 997, depth 64), and fails again with the
    section left out (`GENESIS_RB_NEGCTL_SKIP=exact`: asymmetric + diverging every pass). (b) An
    open test's frame on the restored stack holds a reference to its function's `Stat`, so the
    restore assigns into the existing map nodes; replacing the map made the second branch of a
    process differ (interrupt-skipped and checked counts in 2-4 functions).
  - Proof: a TAS carrier's final `state_fnv1a64`/`audio_state_fnv1a64` after branches equal the
    plain run's (timed and verify runners), its own verify JSON byte-identical; the full corpus,
    both ways on the same runner: **all 76 per-run JSONs byte-identical** and the merged
    `functions` equal (3,160, 0 mismatches). Again on the engine pinned with it (three more fork
    commits): the control's 76 JSONs byte-identical to phase.py 5's branched verify, and phase.py
    5, 6 pass (7: M5 passes, M4 at its 40/46 of 2026-10-23).
  - Time (24 jobs, the same runner, back to back): **597 s from power-on, 317 s branched** (-47%).
    Frames played 2,273,389 -> 1,055,149 in 76 -> 20 processes; the longest is 59,400 frames, as
    before (fuzz-55800's own length), so the TAS-length pole stays. `tools/bench.py` still plays
    each run alone (`--runs R`: a fuzz run is then a one-branch process, same frames as before);
    `tools/gate.py`'s 6,000-frame runs branch where they share the TAS's start.
- **Traces without a picture skip the render (2026-10-24, (2) above).** `--no-render` (with
  `--benchmark`): the engine's frame-synchronous runners (XMen2Stepped, XMen2SteppedGen;
  `exact_set_render(0)`, `fsync_render` returns at once) and XMen2Native (`machine_render_off`,
  the rollback replay's path in `plat_sync_point`) render no frame at their sync points. The
  render changes nothing the game reads there (the status read shows no sprite flags; netplay's
  replays already relied on it natively), so only the framebuffer stays empty. `xmen2check.py
  trace` passes it whenever nothing looks at the picture or the platform's per-frame work
  (`picture_free`: not with `--fb`, `--options`, the present log, flips, game rows, sessions,
  scenes, digests, state probes, `XMEN2_SHOTS`/`XMEN2_CAPTURE`); `--render` keeps it (the
  control); the trace JSON says which (`render`). The runners refuse it with `--hash-frames` and
  the other picture options.
  - Proof, all 76 corpus runs both ways on the same runners (24 jobs, back to back): every
    `sync.csv` byte-identical, stepped-gen and native (2,042,653 sync points each; stepped-gen
    equal to the phase-4 references, native to stepped-gen; 0 misses, 0 interpreter
    instructions).
  - Time: stepped-gen 228 -> 92 s wall, 5,269 -> 2,108 job-s (-60%; TAS 392 -> 1,018 sync points/s);
    native 180 -> 60 s wall, 4,147 -> 1,358 job-s (-67%; TAS 498 -> 1,539/s).
  - Engine 6693d84 is `--no-render` on ab07262. The fork's master (9bcb006, its merge with
    232cd0a) crashed a branched XMen2Verify process (0xc0000005, 3 of 3 replays; ab07262 2 of 2
    clean), so this pin leaves 232cd0a out (the cause and the fix: next bullet but one).
    On 6693d84: phase.py 5 10/10, phase.py 7 as before (M1 = phase 6 all pass, M2/M3/M5 pass,
    M4 40/46, M6 the user's).
- **Exact mode's returns nobody waits for and the C callers (2026-10-24).** The fork's 232cd0a runs
  an RTS on from its address when no call site awaits it (generated JSRs await their return
  address: XCALL_BEGIN/END). The verification runner calls one function from C (verify.cpp
  run_gen) and so does the hand-C runtime (handc_rt.cpp call_slots, call_raw, jump_raw); neither
  awaited, so a run that returned anywhere but its caller ran the game on from there. A
  synthetic case of $15F572 at frame 2 did, untimed (no slices, no interrupts) and for ever,
  logging every instruction until the host stack ran out: 0xC0000005 with nothing printed, the
  private memory at 1.7 GB, in every verify process. Engine 00074ae adds `XAWAIT_ANY` (an await
  that takes any return); both C callers push it, so their run ends at its RTS wherever it goes,
  as before. On it: the process that died plays its TAS and 4 branches (peak 218 MB), the full
  verify passes (3,160 functions, 0 mismatches, 306 s), the native TAS's framebuffers equal the
  engine's with 232cd0a's sprite rules, and phase.py's smoke stage passes.
- **Jobs by threads, memory by a gate (2026-10-24).** phase.py 7 had been run at `PHASE_JOBS=10`
  after running out of memory once, which put every corpus stage on 10 of 32 threads (phase.py 7
  took 2 h 1 min that way). Measured at 24 jobs: stepped-gen runners 1.6 GB together, native
  0.7 GB, the WSL VM of the Linux checks ~2.4 GB, a branched verify process ~0.2 GB over a full
  43,200-frame + 4-branch replay (the ~1.06 GB ones of that day ran the crashing 9bcb006 engine
  above). So the job counts follow the threads (phase.py: the cores less two, `PHASE_JOBS` still
  wins; verify `--jobs 24`; synccorpus `--jobs`), and each pool starts a job only while 3 GB is
  free (`PHASE_MIN_FREE_GB` / `XMEN2_MIN_FREE_GB`), or when none of its jobs is running, or after
  2 minutes; the first hold of a stretch is printed. Verify's per-run JSONs are byte-identical
  held (one process at a time) and not.
- **The gate after the speed-ups (2026-10-25).** phase.py 7 with this batch (jobs by threads and
  the memory gate, E4's first-flip search, native traces branching from the TAS, the smoke stage,
  the crash report) and the engine pin 00074ae, every runner-keyed cache cold: ~75 min (phase 7's
  own stages 1,794 s, D1 references included; M1 = phases 6 and 5 2,691 s) against 2 h 1 min the
  day before at `PHASE_JOBS=10` (7,291 s), every verdict as before (M4 40/46, M6 the user's).
  Stages then and now: M2 1,267 -> 718 s, E3 386 -> 132 s, E4 3,325 -> 1,428 s.
- **A crash names itself (2026-10-24).** A verify process that died of an access violation (the
  fork's 9bcb006) left exit 1 and nothing else: no PDB, no debugger, the stack unknown. Every runner
  now installs `xm_install_crash_trace` (game/runtime/crash_win.cpp; the engine runners at reset,
  XMen2Native first thing): for an exception nobody handles it prints `[crash] exception CODE at
  module+RVA (symbol+off)` and the host stack above it, unwound with the x64 unwind tables and named
  from the linker map (`/MAP` for every runner, beside it; xmen2check.py passes `XMEN2_CRASH_MAP`,
  runner copies don't carry it; the exe's bytes don't change and nothing ships it). xmen2check.py's
  trace, trace-group and verify put those lines in their failure message; verify keeps a failed
  process's stdout as `<name>.stdout`. `XMEN2_CRASH_FRAME=N` faults at runner frame N, the test:
  verify reports `[crash] exception C0000005 at XMen2Verify.exe+0x2d903a (xm_crash_test+0x8a) |
  #1 (xmen2_on_frame_post+0xf6) | #2 (genesis_sim_step+0x18a) | ...` after its own tests-open line.
- **Native traces branch from the TAS (2026-10-24, (1) above for the native corpus).** Phases 6 and
  7 trace the native corpus about nine times (`--fb`, full_rate_2p, 120 and 144 Hz, three widescreen
  passes, the state probe), each run from power-on. `xmen2check.py trace-group` plays them as
  verify does (`pack_branches`): a run whose runner-frame pads (`stepped_pads`, from its input and
  D1 reference) start with the TAS's for 600 frames or more is a branch of a process playing the
  TAS, from a save at the last frame they share (73 of the 75 other runs, in 22 processes).
  - The runner: `--branch-plan` (main.cpp `run_branches`). After frame AT it saves, plays the
    branch's script to its last frame with its own files (each a copy of this run's rows so far,
    then its own), its counters and benchmark line, and puts back everything: the savestate with
    its counters, and what no section holds (present.cpp's frame-to-frame state, the widescreen
    counters' strings, the export's and the memory census's counts, the reported snapshot sizes,
    the stack's paint). A branch point inside a state probe's window moves to the probe's start.
    `--hash-out` puts the `[FBHASH]` lines in a file, so a branch can start with its prefix's.
  - phase.py `Traces.prefetch` runs a stage's traces as one trace-group before the stage (P1, E3,
    E4, E5, M2) and hands each `plain()`/`run()` call its result; cache hits are left out.
  - Proof, all 76 runs alone and as a group, every option set phase.py traces the corpus with:
    every file byte-identical (sync.csv with the fb column, present.csv, game.csv, probe.csv) and
    the JSON equal less timings and `fiber_stack_peak`; probe_snap.csv's saves equal their re-saves
    at the same points (its hashes hold host addresses, so they differ between any two processes).
    `fiber_stack_peak` can differ: the game's fiber also runs the host's allocator (high_fps's
    render-time vector), whose depth depends on the heap's history (fuzz-09000-s1 at 144 Hz: 12,408
    alone, 10,384 as a branch, the rows the same).
  - Time (24 jobs, wall, alone -> group): plain 67 -> 34 s, `--fb` 236 -> 115, full_rate_2p
    236 -> 115, 144 Hz 648 -> 310, widescreen 298 -> 144, state probe 290 -> 140: about half, the
    TAS's own process (its 51k frames) now the long pole. The phase 6/7 corpus stages: ~54 -> ~26 min.
- **Disk.** A trace used to copy every runner (70 MB) to `OUT/runner` and keep it: 5.3 GB per
  mode over the corpus (phase4-decomp's `build/phase_traces` holds 27 GB, mostly these); the
  phase-4-way measurement above left 456 such copies, 31.9 GB. Now a trace copies its own runner
  and DLLs and deletes them after a pass (kept on failure, on a nonzero counter, with
  `--keep-runner` / `XMEN2_KEEP_RUNNER=1`; nothing in xmen2disasm's `phase.py` or the
  first-divergence tools reads `runner/` after a pass): 0 bytes per passed trace. verify's 24
  copies (1.7 GB left behind) are XMen2Verify only (~32 MB each) and go after a pass. The trace
  JSON's `exe` is now the build's runner, plus `exe_sha1`.
- **verify fails loudly** (LESSONS item 6: two full verifies ran on after a fatal had stopped
  testing, 498 and 416 functions checked). The per-run JSON has `stopped_early` (null or why),
  `frames`, `functions_tested`, `calls_checked`, `calls_refused`. Testing stopped when the run
  ended on `xm::fatal` (its message is kept), inside a C run or synthetic case (untimed: one open
  at the end never returned), or after >= 60 frames in which every call of a function under test
  was refused (a C run or case open, or MAX_DEPTH). `xmen2check.py verify` exits 1 naming the run
  and reason, also when a run played short of its frames or its runner failed, and kills the
  runs still going (`--keep-going` plays all). `calls_refused` is ~580k in a normal 6,000-frame
  run: callees inside C runs, cleared when the C run returns. Proof: `XMEN2_VERIFY_DEV_STOP=3000`
  on fuzz-01800-s1 + scenario-pause-cheats: exit 1, "fuzz-01800-s1: testing stopped early: no test
  could open from frame 3000 to the end (2400 frames ...) (475 functions tested)", the other run
  killed; without it: exit 0, 560 / 549 functions tested, 0 mismatches.
- **The worker gate**, `tools/gate.py` (own worktree and build dir; files = changed vs the merge
  base with origin/main, or `--files`): build, pccheck, MPW match of changed files that matched at
  the end of phase 4 (`docs/p4_mpw_matched.txt`, 3,034, from the D4 ledger), verify of the changed
  functions (all of them when anything but game/c, game/verify or docs changed) on tas +
  scenario-two-player/pause-cheats/options-menu/char-nightcrawler x 6,000 frames, and the TAS's
  first 6,000 sync points in `--mode native` against stepped-gen rows of this build (made once per
  XMen2SteppedGen hash, cached in `.git/xmen2-cache`). Fail-fast, independent steps at once.
  No change: PASS 58 s (verify: 663/3,160 functions checked, 1.16M calls). Two files: 32 s.
  Planted bugs: Obj_UpdateBox `boxRight + 1` (not matched) -> FAIL at verify, 44 s ("call 1: RAM
  FFEAFB: generated 7A, C 7B"; the trace step alone also fails, sync 247 RAM); Obj_GetLiftCarrier
  without `|| type == '#'` (matched) -> FAIL at mpw, 2 s, build stopped; a missing `;` -> FAIL at
  pccheck, 1 s. Reverted, every runner hashes as before. A header change re-matched all 3,034
  files (~2.4 s each, 8 at a time: ~15 min). Since xmen2disasm cc.py `mpw_view` (2026-10-17) the
  cache keys on what MPW compiles, so edits to the PC or native branches (and line endings)
  re-match nothing; whether MPW matching stays through phase 5 is P5-port.md R4.

### P3 and P4: no 68000 calls, the platform layer (2026-10-21)

`build --native` object scan of the game objects after this batch: **cpu68 0 (was 2,986), bus 0
(was 2,526)**, and no hardware address literal in game/c or game/include. emu_ram 3,702 and the
link-map categories (P5, P6) are next.

- **Calls through stored pointers (P3).** NATIVE mode's `fnp::operator()` builds `narg`s
  (xmen2_mem.h `make_narg`: a value as the slot call68 pushed; a pointer to host memory, a
  caller's local, as itself) and calls `xm::call_native`, generated by bridge.py
  (`native_calls`): one thunk per C function, typed by its prototype (`from_narg` converts like
  the slot entry's `from_slot`; a plain `short *` / `long *` parameter given a game-memory address
  gets the slot entry's host copy), the result into `xm::g_d0` with the slot entry's width (a
  `short` keeps d0's upper word, as `move.w` does), and the caller reads d0, as it read D0 after
  call_slots. The value goes through jt `jmp abs.l` and AI-script `jmp d(a5)` steps first (as
  call_slots), then a binary search; a value where no C function starts (a fragment, an interior
  label) is fatal, so no generated code can be reached silently (P6). The stack region moves by
  what call68 reserved (host-pointer copies, slots, return address), so escaping locals keep the
  original's addresses. Old-style calls with fewer arguments than the prototype get zeros and a
  one-time stderr line (none on the 6,000-sync TAS).
- **NO_RETURN_VALUE** is `return 0` in NATIVE (no flag): the macro's contract is that no caller
  reads the value (state functions), and a stored-pointer call then sets d0 to 0 where the slot
  entry left it. The 6,000-sync native TAS trace is equal; the corpus is the proof.
- **SR and A7 (P3)**: the 11 asm functions' `CPU_GetSR/SetSR` are the platform's interrupt state,
  `plat_sr()` / `plat_set_sr()` (the 68000 SR's layout: bits 8-10 the mask); `IRQ_WAIT()` is
  `plat_wait_vint()`. The escaping-local sites (9 files, PlayerSelect_Run's fixed frame,
  Sys_GetStackPtr, Scaler_BeginPass's never-reached `direct == 0` path that reads its caller's
  slots) use `xm::frame_mark/frame_alloc/frame_release`, the stack region of game memory: in the
  recomp runners A7 (handc_rt.cpp), in the native runner its own mark (P6). All of it is under
  `#ifndef XMEN2_MPW` or in asm functions, so no MPW match changes. `irq_vector` (asm_conv.cpp)
  saves and restores `xm::g_d0` as it does D0.
- **The platform layer (P4)**: `platform/xmen2_platform.h` holds the hardware names; in MPW they
  are the addresses (moved from xmen2_mem.h with their comments: MPW compiles the same text), in
  PC and NATIVE each is a port of a C API (`plat_vdp_data_write16`, `plat_vdp_status16`,
  `plat_io_read8(reg)`, `plat_z80_ram_write8(off, v)`, ...; reads and writes in the original's
  order and width). `platform/plat_engine.cpp` implements it for the recomp runners with
  `m68k_read*/write*` at the same address and width, so their logical positions, verify's
  hardware supplies and device effects are unchanged. The include path gains `platform/` in
  CMake, pccheck and the build snapshot; xmen2disasm `cc.py` copies `platform/*.h` next to the
  source too (`game_headers`) and keys its match cache on them.
- **The MPW toolchain was damaged** (2026-10-21) by gate runs that killed `mps` jobs mid-compile
  (the gate stops its other steps when one fails): `mpw-c-3.3.3/Tools/RESOURCE.FRK/C` was deleted,
  and every later compile failed "Unable to open the resource fork" (not a verdict on the C).
  Restored from `raw/Tools/C.bin` (MacBinary; the same extraction reproduces DumpObj's fork byte
  for byte). xmen2disasm `cc.py` now runs a private copy of the compiler in each job's temp dir.
- **Stale name tables**: the main xmen2disasm checkout's `build/recomp` (the handoff tables cc.py
  resolves references with: `ram_globals.csv`, `jumptable.csv`) was from 2026-10-15 (497 of today's
  549 globals); phase 4 had matched with a newer one in its own, since removed, worktree. With it,
  129 files that match reported NO MATCH (`A5 -$3A40 -> jt None`: g_ActionButtonMasks unknown).
  `uv run tools/handoff.py` refreshed it, and cc.py's match cache now keys on those tables too (it
  had kept the wrong verdicts).
- **Gate**: its MPW step goes through `cc.py --match-recomp --only` (cached by what MPW compiles),
  so a header edit outside the MPW branches re-matches nothing. The D1 references the gate and
  `phase.py 5` use are in xmen2disasm `build/phase_traces` (`*-recomp-sync`, regenerated by
  phase.py's D1 stage: 76/76 in 316 s; `inputs/` holds the exported inputs).

### P5, P6, P7, P8: the native runner (2026-10-21)

XMen2Native is now its own program with no 68000 in it (`platform/native/native.cmake`):
the hand C in NATIVE mode, `bridge.py --mode native` (C-visible wrappers that keep the
function-reference context, the function table, the typed stored-pointer thunks; no slot entries,
no dispatch), the memory layer `game/runtime/native_rt.cpp`, the exporter, and `platform/native/`
(the machine, the host side, the two ways into the game). From the engine only the video model
(`video/genesis_vdp.c`), the Z80 core (superzazu) and the sound chips with their mixer. Object
scan: cpu68, emu_ram and bus all 0 in the game objects, and the link map defines nothing of the
68000 (`link.forbidden` empty: no exact_sched, glue, interpreter, dispatch, generated code,
ram_views, handc_rt or asm_conv).

- **The machine** (`platform/native/machine.cpp`) is the frame-synchronous machine of the stepped
  runners, adapted from the engine's exact_sched.c (fsync mode), genesis_machine.c and
  genesis_bus.c (PolyForm Noncommercial, Matthew Stan; the notice is in the file and the package's
  LICENSES): the VDP ports with DMA done at once (sources read through the memory layer's 68000
  view), the status' alternating VBLANK bit, H/V counter 0, the pads (6-button protocol, as the
  runners' settings), the Z80 bus request/reset lines with the 8-line slice after a release, the
  Z80's frame with its V-int line, the YM2612 timers the driver polls. Accesses the game never makes
  (a YM2612 read or write from the 68000 side, Z80 RAM without the bus, the Z80 writing the 68000
  bus through its bank window) are fatal, not modelled. The sync point is `VDP_WaitVBlank`'s
  wrapper (depth 0): the row, the end of the runner frame (picture hash, present, benchmark), the
  next frame's pads and Z80 frame, the V-int, the render; `IRQ_WAIT` takes the V-int alone. The
  V-int moves the stack region by its 6-byte frame and the vector's 20-byte movem and restores d0
  around the handler, as the recomp runners did. Interrupt state starts at $2704 (the exact reset).
- **The host side** (`platform/native/main.cpp`): the user's ROM, SHA-1 checked (R2; a wrong ROM
  exits 2); the recomp runners' options (`--benchmark`, `--input-script` PADS/WAIT, `--hash-frames`,
  XMEN2_SYNC_CSV/SYNC_DUMP/STACK_LOW) and counters line, so `trace --mode native` drives it
  unchanged; sync rows from the export only. Without `--benchmark` it is played
  (`host_sdl.cpp`: SDL2 window, keyboard and controllers, the sound, paced to 59.92 Hz; F11
  fullscreen; `--quit-after N --screenshot F.bmp` for a smoke test). The frame's sound is mixed
  right after the Z80's frame (so the chip writes of one Z80 frame are drained together).
- **Host byte order (P5, R1 (b))**: in NATIVE every game-memory scalar (s8..u32, stored pointers)
  is `xm::mem<T>` (xmen2_mem.h): in the arena it is stored in host byte order and each byte has a
  store tag (0 a byte, or width << 4 | index for a byte of a word or long stored whole). A read at
  the width it was stored is a host load; any other read (a word of a long, a byte of a word, two
  bytes as a word) is assembled from the bytes as the 68000 sees them, and a store that cuts a unit
  turns its other bytes into bytes. So the arena always holds exactly the 68000's RAM, the export is
  a per-unit byte swap (`xm_ram_image`), and no source had to change. In the ROM image the same
  types read big-endian; in host memory (a C local of a game type) host order. Copying one record
  to another goes field by field through the value (ROM records into RAM convert). On the TAS's
  first 1,500 frames: 59,862 of 65,536 RAM bytes are host-order words/longs, 5,674 bytes; 868k
  reads and 31k writes took the slow path (layout-dependent code: byte loops over records,
  Mem_Copy). Corpus: equal everywhere, so P5's `copied_bytes` is 0 (every byte is the arena's).
- **The export** (`game/runtime/export.cpp`) is the arena's 68000 view, then every global of
  xmen2_globals.h by its declared type (a global not at its declared arena address is fatal), then
  the video model. `native_bytes` 65,536, `copied_bytes` 0.
- **Results**: TAS 51,209 sync points equal, the four CRCs and (`--fb`) every framebuffer (P7),
  with both the big-endian arena and the host-order one; the corpus 76/76 equal (big-endian arena:
  6 min 49 s at 12 jobs). Speed ~600-950 sync points/s per job (the stepped-gen reference ~345).
- **The package** (`xmen2check.py package`, P8): XMen2.exe, SDL2.dll, README.txt and LICENSES/
  (segagenesisrecomp's PolyForm license and third-party list, SDL2, ymfm, superzazu) zipped;
  `tools/romscan.py` finds no 256-byte ROM window in any of them (any shared window holds three
  aligned 64-byte blocks of the file; every 64-byte ROM window is indexed by a rolling hash; a
  300-byte ROM slice in random bytes is found, a 200-byte one is not); the unzipped package plays
  600 frames on the ROM and refuses a ROM with one byte changed.
- **Linux (P9)**: `package --platform linux` runs `xmen2check.py native-build` in this machine's WSL
  Ubuntu (GCC 15, CMake 4.2, Ninja; `libsdl2-dev pkg-config ninja-build` installed 2026-10-21, as
  R7 says), CMake `XMEN2_NATIVE_ONLY` (only XMen2Native: no recompiler, no generated code), build
  dir `~/xmen2-native-build` (WSL's own filesystem). GCC gets `-fwrapv -fno-strict-aliasing` on
  the native target. The package (`xmen2`, README, LICENSES; SDL2 is the system's) has no ROM
  window, plays and refuses as on Windows, and **its TAS trace in WSL equals the stepped-gen
  reference at all 51,209 sync points**. One fix it found: `g_snd_frame` was defined twice
  (chip_trace.c and machine.cpp); MSVC had linked two different variables (C++ name mangling),
  GCC refused. machine.cpp now uses chip_trace.c's.
- **Reads through a null reference** (P5-port.md "Platform layer": in the memory layer, not in
  scattered guards): in NATIVE a game-type read whose host address is within 64 KB of null reads
  the ROM image there, big-endian, which is what the 68000 reads (its vector table); a store there
  is fatal. `XMEN2_NULL_SELFTEST=1` proves it (a word at +4 and a long at +$78 through a null
  pointer equal the ROM's; GCC gets `-fno-delete-null-pointer-checks`). The six phase-4 guards
  (`grep -n Vectors game/c/*.c game/include/xmen2_macros.h`) stay: the MPW and PC modes need them.
- **Frame time (R5's work item: D8 chugged in the Sentinel escape on a slower Linux PC).**
  `XMEN2_FRAME_TIMES=path` logs each runner frame's wall time. The whole TAS headless on the
  16-core desktop (with `phase.py` running alongside): 636 fps, median 1.7 ms, p99 2.0 ms,
  p99.9 2.9 ms, worst frame 17 ms (single frames at scene loads, where the original lags too).
  So sustained play needs ~10% of a 60 Hz frame here; the D8 build ran the engine's stepped
  runner (68000 state, dispatch, ~345 sync points/s per job), the port about twice as fast. A
  played run no longer builds the export image every frame (it compares nothing): that was
  ~0.3 ms a frame.

## Phase 6: enhancements (options of the native runner)

Criteria, rulings and the measurement contract: xmen2disasm `docs/phases/P6-enhance.md`
(approved 2026-10-21, R1-R7). Checked by `uv run tools/phase.py 6` there.

### E2: the options (2026-10-21)

- `platform/native/options.cpp`: the table (`full_rate_2p` 0/1, `high_fps` 0 or a rate in Hz,
  listed 120/144/165/240, any of 61-1000 accepted; `widescreen` 0 / `16:9`), each off by
  default. `--options K=V,...` on any run, `xmen2.ini` beside the runner (`[options]` section,
  `K=V`; the runner's own directory, not the working one), the command line winning; an unknown
  option or value exits 2. `--options --json` prints the table, the effective values and the
  settings file path, and exits before the ROM is read.
- The game's C reads them through `g_opt` (`platform/xmen2_options.h`, included by
  `xmen2_platform.h` under `XMEN2_NATIVE` only), always in native-only branches.
- `xmen2check.py options --json` adds `from_settings`: the effective values a private runner
  copy reports with an `xmen2.ini` setting every option to its first non-default value.
  `trace --mode native --options ...` passes them on; its JSON adds `options` (effective) and
  `enhance` (the counters, from the runner's counters line). `package` adds the packaged
  runner's `--options --json`.
- `enhance` counters: `two_player_frames` / `two_player_step_frames` read `g_PlayersAlive`
  ($FFC920) == 2 / `g_TwoPlayerStep` ($FFC922) at every sync point (`plat_sync_row`);
  `frame_tick_waits` counts the V-ints `plat_wait_vint` (IRQ_WAIT) takes.
- **Which corpus runs have two players** (options off): only `scenario-options-menu`, 3,192 of
  its 6,070 logic frames (3,194 with the step flag; 746 frame-tick waits). The TAS and
  `scenario-two-player` never have two players alive in a level: each has one frame with
  `g_TwoPlayerStep` set and no wait: sync point 1 (see E3).

### E3: two players at the full rate (2026-10-21)

- **Who sets `g_TwoPlayerStep`.** `Level_FrameLoop` (`1FA124`) every pass (`g_PlayersAlive == 2`),
  then waits for the frame tick (`IRQ_WAIT`) in stepped frames. `PlayerSelect_Run` (`0656B0`)
  never sets it: it inherits the last level pass's value (two players: it waits and steps by 2
  too). `Level_Load` (`1F98A8`) sets 1 and `SentinelsA1_LoadLevel` (`0E78F2`) sets 0 then 1: both
  hold only until the frame loop's first pass, in one-player games too (every run has the flag
  at sync point 1: the power-on `Level_Load`, before any frame loop). `BaniMazaA1_`, `SavageC1_`
  and `SavageB1_LoadLevel` (`0383DC`, `0C0E54`, `0C1258`) set 1 then 0 at once (no effect).
  `Cerebro_PlayIntro` paces itself by V-blank counts in every mode, not by the step.
- **The option** is one native branch in `Level_FrameLoop`:
  `g_TwoPlayerStep = g_PlayersAlive == 2 && !g_opt.full_rate_2p`. So the frame-tick wait and
  every `g_TwoPlayerStep ? 2 : 1` go with it, and `PlayerSelect_Run` inherits the 0. The loaders'
  1 stays (it is one-player behaviour too, and E3 requires one-player runs unchanged).
- **Logic frames** for the `enhance` counters are sync points inside the game's frame loops
  (`g_InLevelLoop`, $FFC942: `Level_Run` sets it around `Level_FrameLoop`, `PlayerSelect_Run`
  around its loop), not the loads and fades between them, where the loaders' 1 sits.
- **Result** (corpus, `--fb`): the 75 runs without two players have identical rows and frames
  with the option on; `scenario-options-menu` (3,192 two-player frames) has 0 stepped frames and
  0 frame-tick waits with it on, and runs to its end.
- **Played two-player speed with the option off.** The frame-synchronous machine (P5 R5) shows
  one frame per logic frame, so two players (logic stepping by 2) played at twice the speed. A
  played run (no trace) now gives each frame-tick V-int its console frame: the Z80's frame and
  its sound, and one more display period of the same picture (`plat_frame_repeat`). Two players
  play at the original's 30 logic frames a second with the music at its tempo. Traced runs keep
  the stepped runners' machine (no Z80 frame there), so the corpus is unchanged.

### E5: 16:9 (2026-10-21, first batch)

- **The view**: 400x224, 40 px beside each side of the 320 (square pixels as the host shows them;
  224 x 16 / 9 = 398). The renderer is the engine's VDP model with its margins
  (`gvdp_set_ws_extra(40)`) on a fixed 400-px canvas (`gvdp_set_ws_canvas`): 4:3 screens are
  centred with black bars. Unlimited sprites while the option is on (the native machine's status
  never shows the overflow flag, so it is the picture only). `platform/native/widescreen.cpp`.
- **Which screens** (R5, `game/widescreen_screens.toml`): levels wide, Cerebro screens 4:3. Drawn
  wide (`XMEN2_WS_ALLWIDE=1`), the Cerebro frame's machinery continues at the left but ends in a
  black strip at the right, and sprites placed off its left edge show. **The player select after
  a death runs under the stage's scene index** (`PlayerSelect_RunScene` restores `g_SceneIndex`
  before `Cerebro_Run`), so `Cerebro_Run` tells the platform (`plat_ws_cerebro`, native branch)
  and the screen is reported as `CerebroScreen PlayerSelect` (queued in xmen2disasm
  docs/QUESTIONS.md: the contract's screen id is the scene's name).
- **The game's width sites** (every `0x140` / `320` / `0x1C0` / `0x60` hardware x in game/c,
  classified; the macros are in `xmen2_mem.h`, each the original bound token for token in MPW and
  PC, so all 3,034 byte matches stay):
  - drawn, `VIEW_L`/`VIEW_R` (+-40): the on-screen culls of 017506, 01C282, 026AA0, 037BFE,
    039E94, 03A2F4, 04E3F6, 04E644, 05B65A, 05B98C, 05BD92, 077988, 081D08, 0ABF3A, 0BA8FA,
    0BEA74 (draw), 0BF680, 0BFE6C, 111BAA, 1182D4, 11BE72, 11E9CC, 1219FE, 1375DA, 13DBD8,
    13E14A, 13E6EE, 13EE20, 158AF8, 158C0E, 1CF432, 1E1604, 1F1A2E, and the sprite emitters'
    clipping (1FB592, 1FB8A2, 1FBB5E, 1FBCCE, 1FD520). `Spr_Draw` (1FB346) and `XPlayer_Draw`
    (1F1A2E) always clip in the wide view: Spr_Draw's "wholly on screen" test compares screen x
    with the camera position itself, and the player's pieces reach past its box test.
  - activation, `ACT_L`/`ACT_R` (R6): 0BF9EA, 11D2C6, 1340D0, and the eight horizontal spawn,
    despawn, activate and deactivate windows `Level_Load` (1F98A8) sets (every reader of them
    reads the globals: 15DB26, 15DBF2, 15DE1E, 15E18C, 106E14, 106EA8, 15A6FC, 01738E...).
  - camera: the clamps to `g_CamMinX`/`g_CamMaxX` (00FC12, 0AD674, 0FC78A, 10B7A0, 16136E,
    1F5788, 1F5F78, 1F60E4) become `CAM_LO`/`CAM_HI(g_CamMinX, g_CamMaxX)`: the limits less the
    margin, the middle when the range is under 80 px. The limit setters (`- 0x140`: 0820AE, 082176,
    09907A, 0FC8A4, 1F5F5A, 1F5788's `w - 0x28`) stay: they give the 4:3 camera's range.
  - left alone, behaviour that isn't the view: 06DB86 (a box around the object), 08827A and
    16136E/1F4334 (camera tracking with its wall margin), 0868F0 (boss logic on the players),
    0BEA74's shot removal at -128/448 and 112C9E/112D9E/142CD6 (removal 60 px or more past the
    screen, beyond the 40-px margin), 0E9B2E (a gate 320 px left of an object).
  - not widths: 006584 (a DMA length), 038BAC and 1FDAEC (loop counts), 0AD866 and 0B0484 (scroll
    factors), 0B62F2, 0B674E, 0B6824, 0B6916, 0B6B2A, 0B70B0, 0BA1B8, 0E9BCE, 0EA5FE (positions),
    133A40 (speeds).
  - `cc.py` substitutes the globals' names in the source text before MPW compiles it, so a header
    macro must not expand to a global (the first CAM_ macros did: "identifier not declared"): the
    globals are its arguments.
- **The streamer** (R7): with the wide view the streamer keeps the whole plane (64 columns, rows
  of 64 tiles) also for the levels that stream 44 (`g_MapStreamNarrow`, `g_BgNarrow`), and the
  drawn x from 40 px left of the camera to 184 - 40 behind it (Map_StreamColumns), so the 400 px
  stay drawn; `Map_RedrawBlock` redraws blocks across the whole plane. The initial fill
  (Level_LoadMaps) already starts 96 px left of the camera.
- **The HUD**: player 1's sprites at their x - 40, player 2's at x + 40 (Level_FrameLoop).
- **Bars** (R6): where the wide view would show past the camera's limits or the FG map's edges
  (Siberia's 320-px rooms, the arenas), black, the HUD's sprite rectangles kept on top
  (`ws_mask_line`, after each rendered row).
- **Wrapped sprites**: a sprite whose x word is outside 0-511 is off screen where the game put
  it, but the VDP's 9 bits put it 512 px away, which the wide view can show (in 4:3 it never
  could): ~60 functions write sprite entries directly, some without a cull (Spr_EmitFlicker...).
  For a wide frame those entries of the VRAM sprite table get y 0 while it renders, then their y
  back (`sprites_wrapped` counts them; `sprites_lost`, the contract's definition, counts x words
  in the wide view that the 9 bits can't hold, none at 400 px).
- **The terrain check** reads the block map, block definitions, tile attribute, stream position
  and scroll adjustments through the 68000's view, finds the cells each line shows (its
  horizontal scroll, the vertical scroll, the window plane excluded) and compares the name table
  with the map tile the streamer put there (both write paths: a row adds the tile base to pairs as
  longs, a column word by word). E5 checks the wide columns; `XMEN2_WS_CHECK_ALL=1` checks the
  whole view, and with `XMEN2_WS_MARGIN=0` (the 4:3 view) the TAS has 0 errors in 118M cells: the
  check agrees with the original everywhere.
- **What the corpus reaches in the native machine**: SiberiaB1, LogoAndDemo, the player select
  and the four attract-demo stages (BaniMazaA1, SentinelsB1, AvalonB2, SentinelsD1) only. The
  frame-synchronous machine (P5 R5: no lag frames) is not timed like the TAS was recorded, so the
  TAS's input stops fitting in the first stage (its sync rows differ from the D1 recomp's from sync
  1; P1 compares the native with stepped-gen, the same machine): at sync 3,000 the native TAS is
  still in Siberia where the recording is in the Sentinels stage, and it never leaves Siberia.
- **Results** (corpus, `widescreen=16:9`): 76/76 runs to their end, 0 terrain errors, 0 lost
  sprites, 0 misplaced HUD frames. `phase.py 6` E5 passes (2026-10-21, XMen2Recomp 79a85fa).
- **Every world** (a development test, beyond the corpus): `XMEN2_DEBUG_SCENE=n` starts a new
  game at scene n (Game_ResetPlayers, native branch); from each world's Cerebro briefing (scenes
  1, 8, 18, 24, 34, 38) a "walker" input (right held, B/C/A presses, a Start pair every 300
  frames) plays 30,000 frames with widescreen: SentinelsD1, AvalonB1, BaniMazaB2, SavageC1,
  SpaceB1 and CloneA1 (plus their briefings, player selects and the failure screen) all with 0
  terrain errors, 0 lost sprites, the HUD in place; the same with all three options on (15,000
  frames each). `uv run tools/ws_warp.py OUT [--frames N] [--options ...]` (headless,
  `XMEN2_SHOTS` every 1,500 frames; exit 1 on any terrain error, lost sprite or misplaced HUD).
  `--all` starts from the player select before every later stage too (22 stages; scene 7 for
  SentinelsB2): all wide, 0 lost sprites, the HUD in place; 0 terrain errors in 19 of them.
- **Known exceptions to the terrain check** (the definition's "tiles the game rewrites"): the
  same three stages fail it in the original 4:3 view too (`XMEN2_WS_MARGIN=0 XMEN2_WS_CHECK_ALL=1
  tools/ws_warp.py --all`), so their planes are not their map's to begin with:
  - **AvalonA3 Exodus**: plane B holds tiles the block map doesn't (the arena's own background
    drawing); 1.6M cells over 20k frames in 4:3.
  - **SavageA4 Chamber**: plane B shows rows below the BG map's last (tile row 56 of a shorter map).
  - **CloneA5 BroodBoss**: plane A's arena tiles differ from the map by small tile numbers (the
    boss's walls written at run time).
  Drawn wide they look right (screenshots checked: AvalonA3 with a bar where the arena ends,
  SavageA4 full width, CloneA5 the boss across the wide view); none is in the corpus.
  AvalonA2 Falling_Room is a 320-px shaft: barred on both sides, so nothing to check. 4:3 screens
  are not terrain-checked at all (no wide columns; their planes aren't a level's map).

### R8: the 16:9 view presentation only (2026-10-22)

The user's ruling (xmen2disasm P6-enhance.md R8): `widescreen=16:9` alone changes only the picture,
and is the default; `widescreen_gameplay=1` is R6's mode as above.
- **What the game sees** (`platform/native/widescreen.cpp`, `xmen2_options.h`): `plat_ws_act()`
  (ACT_L/ACT_R, and now CAM_LO/CAM_HI too) is the margin only with `widescreen_gameplay`, else 0,
  so activation windows and camera limits are the original's. `plat_ws_left()`/`plat_ws_right()`
  (VIEW_L/VIEW_R) are the drawn window's sides this frame; `plat_ws_view()` (WS) is how far the
  streamer keeps the planes drawn: the margin, or under R8 two (80 px: Map_StreamColumns keeps the
  drawn x 24 px wide, `184 - 2 * WS`, and the plane holds 512 of the 480 needed). `plat_ws_hud()`
  (new HUD_L/HUD_R in `Level_FrameLoop` and `players34.cpp`) is the view's margin.
- **The render camera**: the window starts at `clamp(camera - 40, L, R - 400)`; `[L, R)` = the
  camera's limits widened by 320 px, within the FG map. Applied to what the VDP renders, not to the
  game: `ws_frame_setup` shifts every horizontal scroll word (A and B, all 256 lines) and every
  sprite x of the VRAM table but the HUD's, `ws_frame_check` puts them back. present.cpp's
  in-between frames lerp the shifted words, so the shift interpolates with the camera. Players 3-4's
  sprites, drawn by the host, follow `ws_render_shift()` (their panels don't).
- **The HUD is written before the camera moves** (`Level_FrameLoop` draws it first, right after
  `Spr_BeginFrame`), so placing it by the frame's shift was one frame late: 170 of the TAS's
  51,179 gameplay frames had it misplaced. It keeps the view's margin and the render shift skips
  its sprites (by tile attribute, the same set `bars_now` keeps above the bars).
- **Siberia's FG map is 16x8 blocks, the 512-px plane, and scrolls apart from the camera**
  (`g_FgScrollX` 49-484 while `g_CameraX` runs 1,000-1,900; limits 0..8000). The bars compared the
  camera with the map's width, so R6's mode barred Siberia's right 40 px on every frame (the
  terrain check skips barred columns, so nothing counted it). The map's edges are now taken in
  its own x (`camera - g_FgScrollX` is where its x 0 is), and only for a map that isn't exactly the
  plane's width (that one wraps, as in 4:3). Both modes show Siberia full width now.
- **Sprite x under the shift**: the game's 16-bit x word, shifted, can come inside 0-511 from
  outside it (attract demos: 2,292 `sprites_lost` before), so the shift takes the signed word and
  hides only what is outside 0-511 after it; `sprites_lost` reads x the same way (the contract's
  definition, noted in the doc).
- **The game's state** (`--game-rows`, `main.cpp` `k_view_ram`): work RAM less the stack area and
  the ranges below, CRAM, VSRAM, VRAM less planes A, B and the sprite table. Each range was found
  by dumping the first differing sync point (`XMEN2_SYNC_DUMP`) of a presentation run against the
  4:3 run:
  - `$FFE21A-$FFE225`: `g_SprWritePtr`, `g_SprIndex`, `g_SprLast` (TAS sync 13: 13 vs 32 sprites).
  - `$FFE226-$FFE4AD`: `g_SpriteTable` and its stop slot 80 (walker CloneA1, sync 1045: the
    wide view filled the table to the stop).
  - `$FFC732`/`$FFC736`: `g_BgStreamX`/`g_FgStreamX`, the streamer's drawn x (80 px further left).
  - `$FFE98E-$FFE99B`: `g_BeamFlicker*` (idle sync 7735: `g_BeamFlickerSpr[3]`, the slot of
    Cyclops's beam splice).
  - `$FFC620-$FFC627`: `g_HoloFlicker*`, the hologram's splice of the same kind (walker,
    SentinelsA1, sync 295: `g_HoloFlickerLinks`).
  With those, the game's state equals the 4:3 run's at every sync point of all 76 corpus runs
  (TAS: 51,209), and differs from sync 1 with `widescreen_gameplay=1` (the control).
- **Screen tests that decide the game** (the "drawn" VIEW_ sites above, read again for what else
  their branch does; the walker runs found the first two): under R8 they take the game's bound,
  `ACT_L`/`ACT_R` (in R6's mode the same value as VIEW_, so nothing changes there):
  - `Gambit_UpdateCard` (1CF432): a card more than 50 px off screen is freed; the native branch
    frees by the game's bound before the drawn test (walker BaniMaza/Space, sync 457:
    `g_GambitCards[0].kind` -1 in 4:3, still flying wide).
  - `Boulder_StateBounce` (05B65A): the camera shake picked by "on screen".
  - `Boulder_StatePiecesFall` (05B98C): pieces on screen keep the boulder alive (and draw
    `Rand_Next`).
  - `CloneA2_FrameHook` (081D08): the quake starts when its point is on screen (it moves the camera).
  - `Boulder_Draw` (05BD92) and `XPlayer_Draw` (1F1A2E): their box test (40 px around the body)
    guards `Spr_Draw` / `Spr_UploadFrame18`, which upload the frame's tiles and set the instance's
    upload cache (`uploaded`, flags bit 5), and XPlayer's `sparkTimer`. With the drawn window a
    boulder in a margin left `uploaded` set where 4:3 didn't (walker BaniMaza, syncs 621-4185,
    down to stale bytes in the last level's object pool), so the whole test takes the game's
    bound; the 40 px still reach into the margins. Every other upload runs whatever the position
    (the emitters clip the pieces).
  The others only choose sprites (sprite-table writes, `Spr_*` emitters) or, as RetractFloor_Draw's
  `return`, stop emitting.
- **Map_StreamColumns past a narrow map** (15F1F8): SpaceA1's FG map is 480 px (15 blocks), under
  the plane; with the original camera range (up to 160) and the streamer's 80 px the right branch
  got a negative limit (`g_FgMapExtraW` = (60 - 64) * 8) and its `while (n-- != 0)` ran ~65,000
  columns: an access violation at its first frames. Native: `n` at least 0. R6's narrowed camera
  never reached it.
- **Results** (2026-10-22): the corpus, 76 runs in each mode: presentation's game state equals the
  4:3 run's at every sync point, 0 terrain errors, 0 lost sprites, 0 misplaced HUD frames, 0
  unneeded bars; gameplay mode the same counters. `tools/ws_warp.py --all --game-state` (every
  stage, 30,000 walker frames each): the game state equal in all 22, and the counters 0 but the
  three stages known to fail the terrain check in 4:3 too (AvalonA3, SavageA4, CloneA5), in both
  modes. Diagnosing: `--game-rows` with `XMEN2_GAME_DUMP=n,...` (+ `XMEN2_DUMP_DIR`) writes the
  state as compared at those sync points; `XMEN2_WS_CAM=n` prints the camera, its limits, the FG
  map and the shift every n frames.

### E4: high frame rate (2026-10-21)

- **Timing** (R4: no added latency): `platform/native/present.cpp`. Runner frame k's picture R_k
  is rendered at sync point k-1 right after the V-int and shown at k x T (T = 16,688.9 us), as at
  60 Hz. With `high_fps` = H, the frames of an H Hz clock between (k-1) x T and k x T are
  interpolated from R_(k-1) toward R_k right after R_k is rendered, before the logic of runner
  frame k: R_k's state is already there (its logic and its pads are those the 60 Hz build has at
  that moment), so the whole frames keep their times and the in-between ones only show earlier
  what R_k will show. A played run waits for each frame's time on the present clock
  (`host_present_at`); a frame more than 4 NTSC frames late moves the clock.
- **An interpolated frame** is R_k's VDP state rendered with the horizontal scroll of every line
  (the camera, the parallax), the VSRAM and the matched sprites moved back toward R_(k-1); then
  VRAM and VSRAM are put back (presentation only: the corpus at 144 Hz has the reference's rows).
  A scroll step over 64 px or a sprite step over 48 px snaps. Not moved: the HUD's sprites, the
  palettes, the planes' tiles.
- **Matching sprites to their object**: the game announces each owner before it draws
  (`plat_spr_owner`, native branches): every draw-list node's object (DrawList_DrawAll), every
  timer (Timer_RunAll), and in Level_FrameLoop the scene's frame hook, the hits, the camera, the
  scroll and palette hooks and the draw hook; in PlayerSelect_Run its hooks, the portrait frames
  and the lives. The platform reads `g_SprIndex` at each mark; slots fill in order from 0, so
  each slot has its owner. The six sprite emitters (Spr_EmitPre/Stream, their Clip and Linked
  forms: 1FB592, 1FB75A, 1FB8A2, 1FBA0C, 1FBB5E, 1FBCCE) also announce the sprite instance whose
  pieces they write and its anchor (`plat_spr_anchor(spr, x, y)`, hardware coordinates; the linked
  raw-tile emitter gets x, y without the $80 and adds it). A sprite's step is its instance's anchor
  step, so a frame of animation that rearranges the pieces changes in place, as at 60 Hz; a shot
  that travels by animation frames at a fixed anchor (the Sentinels drones' laser) advances at
  60 Hz too. Pieces written by no emitter move with their owner, by the one step that lines up most
  of its pieces with R_(k-1)'s (the same size and tile word, else the same size; at least a third
  of them, of equal steps the shortest). A sprite written outside every owner (the pause screen's
  text) is matched with the same slot, tile and size in R_(k-1).
- **The playtest (2026-10-21) found the first matching wrong**: it paired each piece with the
  owner's piece of the same index and size, so when an animation frame changed, a piece slid from
  a different body part up to 48 px away: ghost limbs to the left and right of an idling Cyclops,
  and part of him gone in his charged blast (its dashes, which the game swaps between two places
  28 px apart every frame, also slid back and forth). Rigid steps per owner fixed the ghosts but
  left a whole-body animation change (the logo's tumbling figure) without a step: 91.8% on the TAS;
  the anchors give 99.3%. Repro: `XMEN2_DEBUG_SCENE=1`, Cyclops (Right at the select), idle, then A
  held 90 frames; `XMEN2_PRESENT_SHOTS=FROM-TO` + `XMEN2_DUMP_DIR` write the in-between frames.
- **Measured** on the TAS at 144 Hz: whole frames equal P7's (`--fb`) and at the 60 Hz times,
  99.3% of the non-HUD sprites on gameplay frames interpolated (96.2% with the first matching),
  render p99 0.76 ms in that trace (1.2 ms in phase.py's run, alongside other work). `--present-log` writes `present.csv`; `--flip N` inverts player 1's C in
  runner frames N to N+7. The `present` counters add `unmatched` (no owner, new piece, size,
  snap) and the fallback matches, for diagnosing.
- **The latency probes** don't see the flip on most of the native TAS (queued in xmen2disasm
  docs/QUESTIONS.md): the TAS doesn't play its recorded game in the frame-synchronous machine and
  keeps pausing (the pause screen is a gameplay frame by the definition); C does nothing there. Of
  the 10 probes, the 3 whose flip changes the picture show it one 144 Hz tick earlier than 60 Hz.
- **The probe search** (phase.py E4, since 2026-10-22: each window's first gameplay sync point whose
  flip changes the 60 Hz picture within 40 frames) used `--flip-scan` runs of 512 frames, each
  replaying the TAS from frame 0, and a window in a long pause needed them one after another
  (2026-10-24: 24 scans over ~13 min, 10 of them in series for one window). `--flip-scan-first`
  (2026-10-24) compares each flipped pass with the run's own unflipped rows as the run reaches them
  and stops flipping at the first gameplay N that shows; phase.py scans each window's whole rest in
  one run, all windows at once, and still picks the point from flip_scan.csv against the unflipped
  present log. Same 10 points (31, 6652, 10266, 16481, 21308, 25635, 30742, 40766, 40980, 46092),
  ~5 min (window 7, the pause: 4,951 flips in one run).
- **Keys** (R1, played runs): F9 turns `high_fps` off and back (144 if none was given), F10 asks
  for the 16:9 view on or off from the next screen (the camera, the streamer and what activates
  follow it, so it starts with a scene). The window opens 16:9 when widescreen is on at start.

### Cheat options (2026-10-21, the user's request; not phase-6 criteria)
- `lives`: `0` (the original: 9 each at a new game, Game_ResetPlayers), `1`-`99` (the start count;
  the select screen shows two digits), `inf` (never spent). The game has no continues: a player
  spends a life at each select of a character (PlayerSelect_* 067FCA/068078/068126, the join
  006242/0062E4) and Game_SceneLoop (005CC6) spends one for a player out of a stage with a
  character, else the game is over. Native-only macros in xmen2_mem.h: `START_LIVES` at the two
  starts, `LOSE_LIFE(g_P1Lives)` at the 12 decrements (MPW/PC: `9`, `--(v)`; 3034/3034 matches
  kept). The game's own cheat code (15C42A) still sets 99.
- `magneto=1`: PlayerSelect_Run uses Tbl_PlayerSelectSlots7 (seven portraits) from the start;
  the original from the Bani Maza briefing (`g_ScnCerebroBaniMaza <= g_SceneIndex`). Checked: the
  select shows Magneto, LIVES 9 with `inf` and 2 with `lives=3` (8 originally), and Magneto plays
  Sentinels. A death with `inf` was not played through.
- `start_char` (`beast` .. `magneto`, g_P1CharIndex's order): player 1's character when
  Player1_StartLife (006242) would pick one at random: at power-on and at each new game the game
  starts in SiberiaB1 with no player 1 (the power-on demo, 1200 frames, until Start), and
  SiberiaB1_InstallHooks seeds Rand_Next from the V counter + g_AttractDemoNum before it. A native
  `case 6` gives Magneto, whom the original never picks there; without `magneto=1` the selects
  before Bani Maza still have six portraits (a player who was Magneto gets another at random).
- **The V counter in played runs** (machine.cpp `vcounter_now`): the native machine returned 0 (no
  beam without time), so the seed was only g_AttractDemoNum and the power-on character the same
  every time. Played runs now read the line the beam would be on from the wall clock (traced runs
  keep 0, so every trace is unchanged): four played starts gave Cyclops, Wolverine, Wolverine,
  Nightcrawler. It is the only V-counter read in the game.

### Gameplay tweaks (2026-10-22, the user's balance prototype; not phase-6 criteria)
- `magneto_flight=1`: hovering (air A, Tbl_MagnetoAnim) with Left/Right held, Magneto_AirSteer
  (1CDB32) runs Magneto_HoverDamp(0, 1) (the bob and the facing, no x stop) and then
  XPlayer_SteerFacing with his walking speed and acceleration (walkMaxVx 706 = 2.76 px/frame,
  walkAccel 80: 9 frames to full speed; the opposite direction zeroes vx at once, as air
  steering does). Let go, the original damping brakes him (-vx/8 a frame, stop under $40). The
  hover stays unlimited, vertical stays the bob, and air shots still damp him (StateAirShot /
  StateAirShotEnd, untouched). Native-only branch: 1CDB32 still byte-matches.
- Checked (start_char=magneto, Siberia, headless): option off, x stays put while Right is held in
  the hover (the original); on, 2.76 px/frame right and left with the same bob (y equal frame for
  frame); an air shot fired at full speed slows him to 0.44 px/frame over its 15 frames, then he
  accelerates again; the bolt flies. Gate passes (TAS 6000 sync points equal, verify 0 mismatches).
  The user played it ("magneto fly feels good"); landing, walls and the two-player tether while
  flying were not tested on purpose.
- **Native window across monitors** (e443f28): moving the game to another monitor froze and garbled
  the picture while the game and music ran on (SDL 2.28's default Direct3D 9 renderer loses its
  device). The renderer is now direct3d11 on Windows, rebuilt on SDL_RENDER_DEVICE_RESET or 30
  failed draws in a row. Checked only that it opens and draws on direct3d11: the monitor change
  itself has not been re-tried. If it freezes again, stderr's `[native] renderer:` line says which
  renderer it had.

## Phase 7: online play and four players

Criteria, rulings (R1-R10) and the measurement contract: xmen2disasm `docs/phases/P7-multiplayer.md`
(approved 2026-10-21). Checked by `uv run tools/phase.py 7` there.

### M2: the game on a fiber, savestates (2026-10-21)

- **The host owns the loop** (`platform/native/machine.cpp`). The game runs on a fiber of its own
  (the engine's `runner/fiber_compat.c`: minicoro in assembly mode, a fixed 1 MB stack under a 64 KB
  guard, the Windows TIB stack fields switched; `/CETCOMPAT:NO` on MSVC, `-fcf-protection=none
  -fstack-clash-protection` on GCC). `plat_sync_point` yields to the host; the host does the row, the
  end of the runner frame (present, benchmark), the next frame's pads and Z80 frame (`machine_tick`),
  then resumes the game, which takes the V-int and renders. A session machine's frame-tick V-int
  (`plat_wait_vint`) yields too, so the host shows the repeated display period. No trace changed:
  TAS 51,209 rows and framebuffers, `scenario-options-menu` 6,070, `scenario-two-player` 4,914, equal
  to the runner before the change.
- **Fiber stack**: the deepest the game went (painted stack) is 7-8 KB (TAS 8,328 bytes, options menu
  7,160): the game's C has no deep recursion; 1 MB is ample.
- **Sections** (`platform/native/state.cpp`, registered by each module): `fiber` (its context and live
  stack, `fiber_snapshot_save`), `arena` (work RAM in native storage, the store tags, the stack
  region's mark, the bridge's d0), `machine` (`g_machine`: VDP memories and registers, the bus with
  Z80 RAM and pads, the Z80; host pointers zeroed in the saved bytes, kept on load), `platform` (the
  machine's statics: VDP port latches and DMA, the Z80's clock and bus lines, the YM2612 timers and
  the chips' register shadows, I/O and pads' protocol state, the interrupt state, the frame numbers,
  the sound frame; widescreen's Cerebro flag, the 16:9 option and an F10 request), `sound` (ymfm,
  the PSG and the event queue, `*_rb_save`: only in the session machine, where they run). Derived
  state is rebuilt after a load (the ARGB palette from CRAM, widescreen's screen cache). The
  options are configuration, not state (constant within a run except F10, whose request is in
  `platform`). About 225 KB a snapshot (TAS: 225,454 bytes; 225,643 with the sound).
- **Counters** (the trace's measurements: sync rows, two-player frames, frame-tick waits, the `ws`
  counters) are not state: a snapshot carries them besides the sections, outside its hash, and a
  restore puts them back, so a probe's first pass (or a rollback's replay) is counted once.
- **The probe** (`--state-probe P:L`, main.cpp `plat_at_sync`): every P-th sync point with L more in
  the run: save, L ticks (pass 1: no row, no frame end, no presentation), restore, save again (the
  SHA-1 of both in `probe_snap.csv`), L ticks again (pass 2: the run goes on). Both passes' rows with
  the picture's hash in `probe.csv`. Results: `scenario-options-menu` 20 save points and
  `scenario-two-player` 16, both passes equal and the resaved snapshot equal at every one, and the
  live `sync.csv` equal to the run without the probe; TAS in the session machine with 16:9 (played
  by runner frame, 57,465 frames): 191 save points, all exact.
- **Negative control** (`--state-skip NAME`, same TAS run): every section is caught. `fiber`: 75
  resaved snapshots differ, 2 save points with different passes (sync 20,101: RAM); `arena`: all 191
  (every CRC and the picture); `machine`: all 191 snapshots, 112 passes (VRAM, the picture);
  `platform`: the frame numbers stay at pass 1's end, so the run ends 11,460 frames early;
  `sound`: all 191 snapshots (it never changes the game: 0 passes differ).
- **The session machine** (`--session SEED`; played runs take a seed from the clock): each
  frame-tick V-int gets its Z80 frame and sound, the sound chips run (headless too: the mixer drains
  each frame, nothing is heard), and the V counter is a pure function of the seed and the runner
  frame (`vcounter_now`, a splitmix of both). Before, a played run read the line from the wall
  clock. Traced runs without `--session` are the stepped runners' machine as before (V counter 0).
- **The canonical digest** (`--digest`, `machine_digest`): the 68000 view of work RAM (all 64 KB),
  VRAM, CRAM and VSRAM masked as the rows take them, Z80 RAM, and a fixed field list written at fixed
  widths, little-endian (VDP registers, address and code, the port latches and DMA; Z80 registers
  with the flags packed, its cycle count's low 32 bits (MSVC's `unsigned long` is 4 bytes, GCC's 8),
  the Z80 clock and bus lines; the YM2612 timers and register shadows (512 bytes), the PSG's; I/O and
  pads; SR and the runner frame; widescreen's three). Not in it: host pointers, padding, the sprite
  flags the renderer sets. A word-wise FNV-1a-style 64-bit hash.
- Input scripts take up to four pad columns (`PADS p1 p2 p3 p4`); pads 3 and 4 are `g_pad34`.
- `--scene N` (as `XMEN2_DEBUG_SCENE=N`): a new game starts at scene N.
- `phase.py 7` (snapshot at c29d681): **M2 passes**: 76/76 corpus runs restore exactly (rows, frames,
  snapshots), the negative control catches 5/5 sections.

### M3: two players online (2026-10-22)

- **The glue** (`platform/native/netplay.cpp`): `recomp-net` (session, LAN / LAN hub, the rollback
  episode driver) and `rbengine` (the snapshot ring) linked into XMen2Native (native.cmake adds both
  subdirectories; ICE's libjuice comes from recomp-net's pinned FetchContent, on Windows and in
  WSL). XMen2Native's own `RNetRbHost`: snapshots are state.cpp's (a blob per tick in the ring,
  depth 64, every tick), a tick is `machine_tick`, INLINE replays (see the costs below), digests are the canonical digest folded to 32 bits (partitions: RAM, VDP, the
  rest), pads 12-bit active high. Tick t = runner frame t+1; the snapshot keyed t = the state at sync
  point t (key 0: right after power-on, the fiber made but not run: `machine_start`, and runner frame
  1 now begins in the first tick rather than in `machine_power_on`; frame 1's Z80 frame skips the
  pads' timeout refresh as before, when the 6-button type was set after it: no trace changed).
- **Rows of the final timeline**: a seat keeps each sync point's row and digest, overwritten when a
  replay runs it again, and writes them at the end (`main_np_write_rows`). Replays are quiet (no
  present, no frame end, the chips run but nothing is queued to the device).
- **The input delay** is the session's (`rnet_session_committed_delay`), which the session changes
  at run time (DELAY_SYNC): a seat samples its script at tick t + D + 1 (the session plays a sample
  taken at sim tick t at t + D; ticks 0..D-1 are neutral). First runs sampled at the configured D and
  desynced at sync 2,951 on `mid` (both seats equal, both off the offline run): the scheduler had
  raised the session's D. D starts at 5 (the driver's rollback floor: it raises any lower D to 5)
  and the scheduler's auto-delay is off (`RBE_RB_AUTO_DELAY=0` unless set): on `mid` it ratcheted D
  to 9 (150 ms of input lag) and no prediction was ever corrected. With D fixed, a link slower than
  83 ms is predicted and rolled back.
- **Pacing and late frames** (headless harness seats; a played seat is paced by its window): live
  frames on the NTSC clock, a stall over 4 periods moves the clock; a frame is late when it comes
  more than one period after the one before (two periods between them: a visible hitch). The first
  version counted lateness against an absolute schedule and slept with Windows' default 15.6 ms timer:
  the host seat carried 11-16 s of the scheduler's time-sync debt over the session (paid as short
  admission stalls), and every 1 ms wait became a hitch (986 late of 8,346 on `mid`).
  `timeBeginPeriod(1)` and the absolute clock: 10 and 2.
- **R10, the seal**: before tick 0 the host sends its configuration as a pre-match state transfer
  (`RNET_STATE_OP_SRAM`, the engine's SRAM barrier): the game options but high_fps, the session seed
  and the start scene (`xmen2-native-netplay-1;full_rate_2p=...;seed=1;scene=-1`); a guest adopts it
  (options.cpp `options_adopt_game`; an option or value it lacks refuses the match) and the driver's
  mod-set handshake then checks every seat runs that text (a different one refuses), and it is the
  identity's content fingerprint. F10 is refused in a session; F9 stays local.
- **The end** (the harness): a seat runs until it is 60 ticks past the input's end with every
  remote seat's input tip 30 past it and no correction open, then asks for the coordinated stop
  (`rnet_rb_driver_request_quiesce`: the match drains), writes its rows through the input's end and
  leaves. (Ending on the digest watermark left a seat 10 ticks short: the peer leaves first.)
- **`xmen2check.py netplay`** (the contract): N private runner copies (or the WSL build for
  `--linux-seats`), UDP ports picked free, seat 0 hosting (`--options`, `--session`, `--scene` only
  on its command line: the guests get them from the seal), `RNET_SIM_*` per seat (the seed + seat),
  each seat's `seat.json` (effective options, replays, the longest, late frames, frames, episodes).
  A Windows/WSL pair: WSL's address and its gateway (NAT mode); the 2-seat host dials its guest too,
  so its first packet opens the firewall's path back.
- **Results** (`scenario-options-menu`, 8,346 runner frames by runner frame, session 1; against the
  offline `trace --session 1 --digest`): `clean` 0 desyncs, host rows equal, 0 replays; `mid` 0
  desyncs, rows equal, late 10/2; `bad` 0 desyncs, rows equal, 6 replays (1 tick each); sealed (the
  host's `full_rate_2p=1,widescreen=16:9,lives=inf`, seat 1's own `high_fps=144,widescreen=0`): 0
  desyncs, seat 1 ran `widescreen=16:9` with `high_fps=144`; **Windows (MSVC) host against a Linux
  (WSL, GCC 15) seat under `mid`: 0 desyncs, host rows equal**: the canonical digest is the same on
  both compilers.
- **Costs** (`bad`, with `phase.py` loading the machine): a snapshot save (state.cpp's copy, its blob,
  the ring's store) 114 us; a replayed tick 2.9 ms with the frame rendered, 1.5 ms (max 2.4) without:
  a replay renders nothing (`machine_set_render(0)` between resim_begin and resim_end; the renderer
  changes nothing the game reads, the status never shows sprite flags), the probe's first pass still
  renders (it compares pictures). Under `bad` the corrections were 14-15 one-tick replays; late frames
  13-14% there (no criterion: R7's `mid` is the bound).

### M5's transports (2026-10-22)

- **ICE** (R8: libjuice, the default public STUN server `stun.l.google.com`, no lobby): the two seats
  exchange their signals through files (`--ice-signal-dir`: seat i appends `sig<i>.txt`, "type flag
  text" a line, as the engine's `ice_manual_signaling` example; an SDP's lines travel as `\n`). The
  peer's LOCAL_SDP / LOCAL_CANDIDATE are pushed as REMOTE_SDP / REMOTE_CANDIDATE (the agent takes only
  those: the first try pushed them unchanged and seat 1 never answered). On loopback ICE picked a host
  candidate (an IPv6 address of this machine). `scenario-options-menu` under `clean`: 0 desyncs, the
  offline rows.
- **The LAN hub**, four seats (seat 0 relays, `rnet_session_start_lan_hub`), `scenario-options-menu`
  under `mid` (pads 3-4 unused until `players`): 0 desyncs on all four seats, the host's rows, late
  45/8/1/2 of 8,346.

### M4: four players (2026-10-22, first batches)

- **`players`** (options.cpp: 2, 3, 4; default 2; sealed online, R10). With 2 no four-player code runs
  (every branch is `g_opt.players > 2`).
- **R5, the region** (xmen2_mem.h `xm_region`, 64 KB at the addresses $00FExxxx the game never forms):
  big-endian storage (the `mem<T>` path the ROM image takes, writable), so byte-wise code reads it as
  the 68000 would; `xm::host`/`addr`/`in_game_memory` know it, a DMA reads it (machine.cpp). Layout in
  `game/include/xmen2_players34.h`: players 3-4's objects ($F0 bytes), their `PlayerX` (player 2's
  per-player globals, field for field), the select slots, their sprite tables, the owner tags. In the
  snapshot (section `players34`, the 8 KB used) and the digest with three or more players.
- **Players 3-4 run the game's own player code as player 2** (`game/runtime/players34.cpp`): their
  objects have pad 1 and aren't `g_Player1`, so every `self == &g_Player1 ? <1's> : <2's>` choice
  takes player 2's side, and while their code runs (`P34_Enter`/`P34_Leave` around their timer
  (Timer_RunAll), their draw (DrawList_DrawAll), a hit by or on them (Collide_ObjVsList), their Init
  and Activate) player 2's globals hold theirs: the pads, the character, the meter, the VRAM area,
  carried health and lives, the start record, the splash, the sprite table's index, write pointer and
  last. Swapped both ways, nested Enters of the same player only count. That spares the 107 files
  that read `!self->pad ? g_Pad1X : g_Pad2X` and every `g_P2VramTile`/`HasMeter` choice.
  Objects made while their code runs (shots, cards: Obj_FindFreeSlot) are tagged theirs and run and
  draw in their context too.
- **Lifecycle**: pads 3-4 read with 1-2 in the V-int in the game's format (6-button: plat_pad34) and
  edged in Pad_Update; Game_ResetPlayers, Cerebro_Run (character from class, health back),
  Game_SceneLoop (health carried, a life spent or out; one of them standing counts), the player
  select (their slots and cursors: Start to join (a life), Left/Right among free characters, Start to
  confirm; the change applies at once (no portrait to fly: P34_SlotChanged); "PRESS START"/"SELECTED"
  two rows above players 1-2's; LIVES text; the unique-character rule over four (FREE_CHAR); the end
  when every player with a character confirmed), Players_PlaceAtStart (60 and 90 px right of player
  1), Pause_Check (their Start joins in SiberiaB1's opening, else pauses: a dead player waits for the
  next select everywhere else, as players 1-2 do), the development switch's start at a stage with no
  select before it (SentinelsB2, after SentinelsA1): Game_ResetPlayers runs the select first and resumes
  the stage, as after a death (names: neither the stage nor the scene before it a "CerebroScreen"), the
  object table (two slots past the pool's: no level loop sees them; Obj_FindFreeSlot's
  kept one-past-the-end read would have taken player 3's object: stopped at the pool with players 3-4),
  XPlayer_Init's index, Level_FrameLoop's count (all four, clamped to the original's 0-2: R9) and Start.
- **The group** (R7): the camera's two-player track takes the extents of everyone in play
  (Cam_TrackBoth), its one-player target falls back to a player 3-4 (Cam_FollowPlayer), Cam_Init counts
  them; the tether holds each player within $11A px of the farthest other in x and $E0 of the lowest /
  highest in y (XPlayer_TetherToPartner), PushingTetherEdge / DeathDone (the scene ends when nobody else
  is in play) / StateRejoin (at another player in play); enemies' Player_Nearest / NearestLive (71
  callers) take the nearest of the four. Enemies hurt them through the generic hit lists (no change:
  XPlayer_Activate lists any player). The enemy files that tested players 1-2 by name take players
  3-4 too since 2026-10-25 (R11 (c) below).
  The stage cameras with their own tracking do the same: XClone_CamArenaUpdate (the clone arenas) and
  SavageA4_CamUpdate take the group's extents (P34_Extents), SpaceA1_CamUpdate stays active while a
  player 3-4 is (and its lift's crash takes their health too), CloneA5_CamUpdate pushes them with
  Brood's arena scroll (P34_ScrollPush). CloneA2's teleporter (Teleporter_StateWait/Teleport/Hold/
  Arrive) sends and holds all of them; the loops over the players go through GROUP_N / GROUP_P
  (xmen2_players34.h: the original's `i < 2` loop and its `i == 0 ? (p = &g_Player1) : (p =
  &g_Player2)` token for token in MPW/PC, players 1-4 natively).
- **R6, video** (platform/native/players.cpp): in a player 3-4 context `g_SpriteTable` is that
  player's own 82-entry table in the region (a macro switch in xmen2_players34.h; with g_SprIndex,
  g_SprWritePtr, g_SprLast swapped), so their sprites never take players 1-2's 80, and nothing is
  dropped for them; the game's own sprite code fills it (Spr_InitLinks/BeginFrame/EndFrame run in
  their context each frame). A DMA (VDP_QueueDma: copied at once) or VRAM port write in their context
  into player 2's character area or HUD area ($FA00-$FCFF) goes to their own tiles (`s_chr`, `s_hud`:
  snapshot section `p34video`). The VDP model's host-sprite hook draws their tables after the hardware
  sprites (so behind them), each tile from their own tiles or VRAM, palette and priority as the SAT's.
- **R6a, the HUD**: players 3-4's panels beside players 1 and 2's (64 px in): player 3's made from
  player 1's art (Art_HUDTiles), player 4's from player 2's, the old digit's columns cleared and "3" /
  "4" drawn from the game's 1-bit font (Art_RegionFont) at twice its size in the panel's light colour
  with a dark outline; health and meter strips as HUD_UpdateHealthBars draws them (Art_HUDTiles2/3),
  every frame (plat_p34_hud).
- **R6, the raised stop**: with three or more players the main sprite table moves to the region
  (P34_SPR_MAIN, 160 entries) and the game's stop is SPR_STOP = 160 (xmen2_mem.h; 80 in players 3-4's
  own tables and with one or two players, the original); every emitter compares against it and counts
  what it leaves out (plat_spr_dropped: `sprites_dropped`). The VDP gets the main table's first 80 as
  the hardware's (links past 79 cut to 0 around each render: players_frame_setup / restore) and the host
  draws entries 80 and on with players 3-4's tables.
- **R9, the second sound machine** (platform/native/sound2.cpp): its own Z80 with the GEMS driver
  (copied from the ROM as Z80_LoadDriver does), YM2612 timers as machine.cpp's, and its own YM2612 and
  PSG (the engine's chip wrappers compiled a second time under snd2_ names: snd2_chips.cpp,
  snd2_psg.c). It hears the first machine's commands at the ring: machine.cpp hands every Z80 RAM byte
  the game writes to snd2_ring_write, which parses the bytes the write index ($36) moves past ($FF, the
  command, its arguments). It takes Snd_Init's addresses, play / stop of the songs with dispatchers
  ($27, $25, $48, and $5E/$5F, which start $64), stop all, pause, resume and the variables, with $B/$C
  = players 3's and 4's voices (P34_MusicVoice) and $A = 2; no sound effects. Every track slot whose song
  isn't a part song is muted (+6 bit 1) whenever the driver writes the slot, so only the parts sound.
  It runs while a player 3 or 4 is in the game (their character set; g_P34InPlay once the region is
  set up), started then with the driver, the init, the variables and the song playing (late, if the
  song began before), stopped when none is. On the first machine $A counts players 3-4 too, at most 2
  (Music_SetPlayerParams: P34_MusicCount). Key-ons (YM $28 with operators; a PSG channel's volume from
  silent) are counted by the song of the track owning the voice (the pools at $1772 / $179D / $17B3:
  +3 the track). Measured on the stage inputs (players 4): $27 685 part key-ons, $25 1,457, $48 911,
  $64 643 (CloneA1) and 138 (CloneA2), 0 base key-ons on all; with pads 3-4 emptied `frames` is 0 on
  every input. The host adds its samples to the first machine's (snd2_mix, saturating). Snapshot
  section `snd2` (the machine as one struct, its queued chip writes, the chips), and with three or more
  players in the canonical digest (snd2_digest_fields).
- **The mp counters** read the game's state as the 68000 sees it (objects' flags and positions, the
  camera, the sprite table, players 3-4's tables in the region); `drawn` from the draw list's owner
  marks (players 1-2) or a non-HUD entry in their table (3-4); `hud_missing` only while the HUD is on
  (g_HudOn); `XMEN2_MP_LOG=1` logs scene / level-loop / in-play changes by frame.
- **Stage inputs** (`tools/mp_inputs.py`, game/mp_inputs/): a probe presses each pad's Start in turn
  (boot.input: pads 3-4 before 2, as SiberiaB1's opening ends when player 2 joins) and reads the log;
  the input keeps the presses up to the select's end, then plays (three patterns, each with the pads
  on their own phases or all four in step; the one meeting M4's shares kept, else the one whose
  worst-off player does best). A dead player stays out until the stage ends, so where enemies kill
  players early the full 3,000 frames of play can't keep everyone in: there the longest of 2,000 / 1,400
  / 900 frames that does (avalonA3, banimazaA2, savageA4, space, spaceA1: 1,400-2,000; cloneA2: 900).
  A candidate is judged by both runs M4 makes of it, players 4 and players 3 (the enemies play
  differently with three: avalonA4, spaceA1 and clone made for four left one player under 50% with
  three; remade, 2026-10-22: 62-83% in play with both). 20 of 23 meet every bound with both. Not met:
  boot (SiberiaB1: player 1 in play 44%, every
  player 3-4% off view: feet below the view's bottom in the snow; hud_missing 106: 102 are the
  original's own, player 1's panel absent in frames 300-401 of the opening as with `players` 2, and 4
  are join frames, the HUD being drawn before the pads are read), avalon (AvalonB1's opening pan:
  191 frames off view for players 1-2, the same 191 with `players` 2, the original), cloneA5 (Brood
  kills players 1-2 in under 500 frames: 35% in play; its attacks aim at players 1-2 by name).
  xmen2disasm docs/QUESTIONS.md asks about all three. Diagnostics: XMEN2_MP_HUD=1 prints each frame a
  player's panel is missing.
- **Late frames online** (M3: at most 1% of a seat's frames under `mid`): XMEN2_NP_LATE=1 prints each
  late frame's gap and what the loop did before it. On w-clone they are one-period holds by the
  scheduler's stall-before-invent grace (polls that admit nothing for ~31 ms, no replays, a 2 ms tick):
  1-24 of 3,512 per seat with the session alone; RBE_RB_INVENT_GRACE_MS=0 (invent at once) is worse
  (11-122, rollbacks up to 11 ticks). phase.py's 11 sessions at once made it 38/3,510; it runs 4 at a time.
  The cause was the loop's pacing: it asked for a frame right after showing the one before and slept
  after the tick, so the scheduler's holds (timesync's <=6 ms shaves on the seat that runs ahead)
  vanished into the sleep, the ahead seat never slowed, and kept so little cushion that a lost packet
  meant a grace hold (w-clone 71 late on seat 0, 2 on seat 1). Now the seat waits for the frame's
  time, then asks, and a hold moves the clock (the played window's too: host_slip): alone, w-clone
  0-1 of 2,508 per seat (was 3-28) and w-sentinels 0-1 of 4,433 (was 17-41), no rollbacks left
  under `mid` (the seats line up inside the 83 ms delay; om-bad's 120 ms still rolls back).
- **Recording a capture** (video with sound, for showing the port): a headless run of a stage input
  (`--benchmark N --input-script game/mp_inputs/sceneS.input --scene S --session 1 --options ...`)
  with `XMEN2_SHOTS=first,...,last` + `XMEN2_DUMP_DIR` (fNNNNNN.bmp per runner frame) and
  `XMEN2_WAV=out.wav` (223,721 Hz stereo from runner frame 1, so frame F starts at
  (F-1)/59.9227 s), `SDL_AUDIODRIVER=dummy`. Then ffmpeg: `-framerate 59.9227 -start_number F -i
  f%06d.bmp -ss (F-1)/59.9227 -i out.wav`, `scale=iw*3:ih*3:flags=neighbor`, audio
  `aresample=48000:resampler=soxr`. With `full_rate_2p=1` the picture is 60 Hz (two or more players
  are 30 Hz in the original). scene20.input frames 1300-2500 with `players=4,widescreen=16:9` shows
  all four apart in the wide view (2026-10-22).
- **Capture (2026-10-24, platform/native/capture.cpp, tools/capture.py):** recording the port to
  look at it frame by frame or to show it. Nothing the game sees:
  - `--record-inputs F`: each runner frame's four pads, kept per frame number (a frame run again,
    by a rollback or a savestate load, drops what followed), written at exit as an input script
    whose header replays it (`# session SEED`, `# scene N`, `# options` = options_game_text). Netplay
    cuts it to the final timeline: `frames` in harness sessions, and in a played one (it ends at the
    window) the lowest remote input tip less 60 frames. Checked: a scripted run vs its recording's
    render, digests equal over 2,400 frames; 2 seats on loopback at 150 ms/50 ms jitter/10% loss
    (25 replays, up to 8 ticks): both recordings byte-identical, the render's digest = the
    session's; played (windowed) solo with a clock seed, equal over 1,200 frames; played 2-seat,
    both recordings identical (1,139 of 1,200 frames kept).
  - `XMEN2_CAPTURE=F.mkv`: every shown frame piped to ffmpeg (FFV1, 53693175/896040 fps) on a fixed
    canvas (`XMEN2_CAPTURE_W`, 400 with widescreen, else 320, x 224; narrower screens centred).
  - `XMEN2_STEM=music|sfx`: the other kind's FM key-ons (sent as key-offs), DAC samples (as $80)
    and PSG volumes (as silent) are changed on the way to the synthesis (plat_audio_write); the
    driver, Z80 RAM and the YM timers (machine.cpp ym_write) are untouched, so the game is the
    same: full/music/sfx renders of a Savage take have equal digests over 3,000 frames, and
    music + sfx = the full mix to -30 dB (rms 22 of 669, without the chips' DC bias of ~380, which
    a silent stem keeps). A voice is its owner's song's (voice pools +3 -> slot +$0E, as sound2.cpp);
    a channel written after its voice was freed (PSG releases) is its last owner's.
  - Music or effect: GEMS plays both as songs. Music = a song started in the same frame as a
    stop-all (`$16`: Music_Play, Music_PlayStage, Cerebro_Run, the stage scripts), or one of the 25
    a survey of every stage + the title and attract found that way (some, the Brood's $30 and
    Avalon's $00, also start without one); a song the game never started (a part the driver starts)
    is music too; everything else is an effect. Stop-then-play is no signal: the game does it for
    almost every sound. `XMEN2_SND_LOG=F` lists each play with its class.
  - The TAS can't be captured on the port: the frame-synchronous machine has no lag frames, so its
    input stops fitting in Siberia (xmen2disasm LESSONS.md, phase 6); takes are played from stage
    starts.
  - Two players at the original 30 Hz show each frame for two display periods
    (plat_frame_repeat): the capture writes it twice, so the video keeps the sound's time (a 2P
    take: 3,524 video frames = 58.81 s, its WAV 58.79 s; before the fix the video ran at twice
    the speed).
  - Music beds (before `music_voices`, which removes the contention: Phase 7 "music_voices";
    with `music_voices=0` this still holds): the music stem still has the original's voice contention: GEMS steals voices by
    priority, and a stage song with two player parts wants more FM voices than the chip has
    (P7-kickoff.md section 4), so parts lose notes (the user heard Wolverine's part cut in the
    Siberia bed). `XMEN2_SOLO=27` (hex song ids) keeps every other song's track slots muted in the
    driver (+6 bit 1, forced and released per slot as sound2.cpp does), so a layer plays with the
    chip to itself: render the base song and each part alone from the same input and mix them.
    The slots are not game state: work RAM CRCs equal in all passes (9,500 frames). `XMEN2_BED=1`
    turns effects' play commands into stops in the ring before the driver reads them (one
    publish per byte: followed byte by byte), for a bed that no effect touches (3 more base notes
    in Siberia); the game may then differ, so it is for beds only. The Siberia bed (P1 Gambit
    via start_char, P2 joining as Wolverine: session 3 with game/mp_inputs/boot.input; $1B 0B/0C =
    4/5 at frame 477) is the base $27, $2B and $2D mixed.
- **R4, the cost of keeping the byte matches** (35 game files so far, the first batches): 444 lines
  added, 8 changed (`git diff --stat`), all in `#if defined(XMEN2_NATIVE)` blocks, most
  `if (g_opt.players > 2) { ... }` next to the original line they extend, the logic in
  game/runtime/players34.cpp (~500 lines of C++: the context swap, the select's and the game loop's
  helpers, the group, the sprite frame and HUD) so most branches are 2-6 lines. Macros: FREE_CHAR in the
  three free-character files (the original condition token for token in MPW/PC), the g_SpriteTable
  switch (a header). Readable: each branch says what players 3-4 add, the original stays untouched
  above or below it. The MPW gate checks every one still byte-matches.

### R11 (c): enemies choose among all four (2026-10-25)

With three or more players nothing in game/c picks players 1-2 by name any more where an enemy, a
hazard or a stage object chooses, hits or waits for the players. Natively only (`#if
defined(XMEN2_NATIVE)`, R4's pattern); with `players` 2 every addition is a loop over no players or
sits under `g_opt.players > 2`, so the TAS stays the original's. Helpers (xmen2_players34.h,
players34.cpp): `P34_Each(fn)` runs fn for each player 3-4 in play **in that player's context** (as a
hit list's hit, Collide_ObjVsList: a TakeDamage, a pad read, a climb state take player 2's globals);
`P34_Any(fn)` the same as a test (stops at the first true); `P34_EveryLive(test)` every live player
1-4 (no context); `P34_HIT(p, call)` a call on a GROUP_P loop's player in its context; `PLAYER_LIVE`,
`P34_CopyLiveLayer`, `P34_SetLayer` for the sprite layer; `P34_IS_PLAYER(obj)`. Code already in a
player 3-4's context (its own objects) calls the other one's as is. By how each now chooses:

- **The group loops** (the original's `for (i = 0; i < 2; ++i)`, GROUP_N / GROUP_P, now with the hit
  in the player's context, P34_HIT): BigBio_SpitUpdateAll, BigBio_UpdateOrbRing,
  Brood_StateWalkToPlayer, DropBomb_StateExplode, FlyBrood_StateWaitPlayerInRange,
  HornetBullet_Update, Mine_Explode, PowerUp_Update, GaussBullet_Update, SmallBrood_StateExplode,
  SmallBrood_StateWaitForPlayer, SporeMine_RecExplode, TechWallGun_UpdateBullet,
  Deathbird_UpdateBurst.
- **Player 2's test repeated for each player 3-4** (P34_Each, in context): BroodBoss_Update,
  BroodBossDamage_Update (the Brood's body and attacks: NOTES M4's "kills players 1-2 in under 500
  frames"), BioFlame_Update, EvilBeam_Update, CoreZap_Update, Launcher_Draw (P34_Any: a shot stops at
  the first player it hits), SavageBigBio_Draw/Update (the tentacle: bits 4 and 8 of its hit mask,
  their hit points in the region, `g_P34A4Hit`), YoyoAcolyte_Draw, RollingDiscs_UpdateScroll,
  Water_DrawHook (the hurt; their splashes aren't drawn), Exodus_Update / CheckFloorHit,
  AvalonA3_PsiFlashStep, RetractFloor_StateDrop, TrapDoor_StateSag / Spring / Open,
  XClone_LockArena / UnlockArena (the arena flag), XClone_TileHookOpenWall (a Magneto among them),
  Tusk_StateStartFight, the AvalonB1 intro's five camera hooks (held still, their pads cleared in
  their context as player 2's), BaniMazaA2_DrawHook (their shadows, drawn outside their context: the
  main sprite table), Tusk_Update and BigAcolyte_ActWaitPlayers (their layer), SentinelsB2_FrameHook
  (the escape's end takes their health too).
- **Hooks the players' own code runs** (a floor segment or map tile under them, already in their
  context): `obj == &g_Player1 || obj == &g_Player2` gains P34_IS_PLAYER: CloneA5_SegHook17,
  SentinelsB_HazardSeg, SegHook_HurtPlayerStop, SegHook_HurtPlayer, PerimeterDoor_TileHit (its hit
  cooldown).
- **Waits for "every live player" / "any player"**: BroodClaw_StateDropClone (and with players 1-2
  out it clones a live player 3-4), BigBio_StateWaitPlayers, BigBio_InActivateWindow, BigBio_Draw
  (the scene ends when nobody is alive), BigAcolyte_StateCheckPlayersLow, Barrier_StateWatch,
  XClone_StateWaitPlayers, PerimeterDoor_StateWaitExit (alive or not, as the original),
  BanimazaDoor_StateWaitRespawn, TrapDoor_StateWaitPlayer, Hologram_WaitPlayersInArena,
  CoreElevator_WaitAtTop / CarriesAllPlayers, BeamScooter_StatePickUp (and freezes them),
  Tusk_StateBoulderRain / WaitP1Platform / WaitP2Platform (the fight starts once every live one is
  on the floor; either wait state then waits for all).
- **Picks**: Barbarian_StateWatchP2 (then players 3-4), EvilNightcrawler_ActTeleport (with two or
  more alive the farthest, which with two is the original's "the other one"), XClone_ActKeepDistance
  (every live player near: turn away when all are on one side), Spark_StateStart (the nearest each
  lap, Player_Nearest, instead of always player 1), Boulder_SpawnerTick (the highest live one limits;
  the sequence index picks among the live ones, with two as its bit 0 did), Platform1_StateRide (a
  player 3-4 riding steers with its own pad).
- **The sprite layer** (a new object takes the live player's: players 1-2 out, a live player 3-4's;
  the stages that move players 1-2 in front of / behind the foreground move them too): the 22
  three-way inits (BabyBio, Barrier, Brood, CapedAcolyte, CoreElevator, EvilDrop, FireWall, FluxBomb,
  FlyBrood, HoloBolt, Hornet, EvilMaker, BioFlame, BioBalls, HornetBullet, MagneticGlob_Activate,
  Ninja, SmallBrood, Tank, ToxicCan, BigAcolyte, XClone `_Init`), BioSentinel_Init (set only),
  PLAYER_LIVE in MaintRobot_Init, Platform1_Init, PowerUp_Init, TechWallGun_UpdateBullet;
  P34_SetLayer in BaniMazaA1/A2/B2_PlacePlayers, SavageJungle_PlacePlayers, CloneA1 / SentinelsA1 /
  SpaceA1 / SpaceB1_InstallHooks, CloneA5_RunScene.
- **Left as they are**: SmallBrood_ActWaitCooldown (its player 1 read is compiled out),
  SpaceA1_SwitchTileHit (keeps the player in a register it never reads), SiberiaB1_FrameHook (the
  intro's music: player 2 is in play whenever 3-4 are), the per-player helpers that take the player
  as an argument (Exodus_HurtPlayer, Tusk_PlayerOnPlatform, Barbarian_TargetPlayer, ...), player
  code (Beast_StateUpAttack's partner, Cyclops_ResetBeams, Player2_*), the cameras' look clamps and
  SpaceB1 / CloneA5_InitCamera (R7's camera, not a choice of target), the HUD and the pause (R6).
- **Checked**: tools/gate.py in handc and native mode (pccheck 3164/3164, MPW 3047/3047 still
  byte-match, the TAS's 6,000 sync points equal, verify 0 mismatches); four-seat hub sessions
  (`players=4`, session 1, the `mid` link) on boot, scene42 (cloneA5), scene38 and scene24: every
  seat's digests equal the offline run's (3,479, 1,408, 2,508, 3,533 sync points; no rollbacks
  happened). cloneA5 with four players now takes all four alike: in play 456, 471, 419, 434 of 1,261
  gameplay frames (before, the Brood went for players 1-2 only). The stage inputs change who dies:
  part 2 regenerates them.

### R11 (a), (b): M4 against the original; the stage inputs remade (2026-10-25)

- **The counters** (players.cpp, P7-multiplayer.md's counter table): next to `hud_missing`,
  `hud_missing_at` lists `[frame, player, join]` for each player in play with no panel (runner
  frames, the first 256) and `hud_panels` the stretches `[player, first, last]` where a player in play
  has its panel (both on gameplay frames with the HUD on; `hud_lists_full` when either list was cut,
  which fails the check). In the snapshot's counters, not in the state or the digest.
- **`join`** is 1 on the first *game* frame of the player's stretch in play (`g_LevelFrame` at the
  stretch's first sync point), not its first sync point: a join that makes two players in play turns
  the game to 30 Hz, so the join frame, whose HUD was drawn before Pause_Check read the Start, is
  shown on two sync points. That was boot's player 3 (the first to join: missing at 425 and 426,
  the panel from 427); players 4 and 2, joining at 30 Hz already, miss one.
- **The check** (phase.py M4, and mp_inputs.py's `problems`, the same rules): each input also runs
  with `players=2`; off view passes at up to the larger of players 1-2's shares there + 1%; a
  missing panel counts unless `join`, and for players 1-2 only where the players 2 run has that
  player's panel at that frame. Players 3-4 have no panel in the original, so theirs count wherever
  they miss (join frames aside): the strict reading, as R11 (b) names no reference for them.
  Measured: boot's 106 missing (105 with three) count 0 (102 are player 1's, frames 300-401 of the
  opening, which the players 2 run misses too; 4 are join frames); boot's players 2 run has player 1
  off view 15.5% of its in-play frames (the snow drifts), so up to 16.5% passes (players 4: at most
  8.6%); avalon's opening pan 6.4% in both runs; spaceA1's player 2 1.6% against 1.2% + 1%.
- **The stage inputs, remade** (`tools/mp_inputs.py`, the R11 (c) enemies): 23 of 23 meet every
  bound with players 4 and 3 (`--check`). boot: player 1 in play 100% (was 44%), the others 87-89%.
  cloneA5: the Brood aiming at all four killed all four within ~450 frames whatever the walker,
  fight or careful patterns did (36% in play at 900 frames, 44% at 600: the select's frames count as
  gameplay too); a fourth pattern, `retreat` (left, an attack now and then, away from the boss), tried
  only where none of the three meets at any length (FALLBACK), keeps them in: 1,400 frames of play,
  54-63% in play with four, 52-70% with three. Every other input comes from the three patterns (the
  scoring prefers survival, so retreat among them won sentinelsB1 and avalon while exploring nothing).
  14 of the 23 files are unchanged; boot, cloneA5 and seven others changed with R11 (c). Regenerating takes ~13 min (23 inputs, 12 at a time).

### R12: each seat's own 16:9 view (2026-10-23)

The user's ruling R12 (P7-multiplayer.md) follows phase 6's R8 and R10's note: online, a
presentation-only `widescreen` is each seat's own, like `high_fps`, and F10 works; with the host's
`widescreen_gameplay=1` it is sealed as before and F10 is refused.
- **The seal** (options.cpp): `options_game_text` leaves `widescreen` out unless
  `widescreen_gameplay=1`; `options_adopt_game` reads the text's `widescreen_gameplay` before it
  classes the rest, and refuses a text with a presentation option without changing anything. A
  guest's own view survives adopting the seal. Netplay version 2 (`NP_VERSION`): the digest changed.
- **The digest** (machine.cpp `machine_mask_picture`, `mask_picture_p34`): R8's `--game-rows`
  masks, now shared, plus what the first measurement found the view moves:
  - the VDP data port's address, last word and FIFO, which hold where the last write went (the
    widened streamer writes more; at frame 2 of `scenario-options-menu`);
  - with three or more players, the players 3-4 region's sprite tables (`P34_SPR_TABLE(3/4)`,
    `P34_SPR_MAIN`) and each `g_PX` fill state (sprite x 40 px apart: the margin);
  - the view and its F10 request (`ws_digest_fields`: only `s_cerebro` now).
  Measured with the session machine (`trace --session 1 --digest`), the view 0 against 16:9:
  digests equal at every sync point on `scenario-options-menu` (8,346) and all 23 stage inputs
  with `players=4`. Control: with `widescreen_gameplay=1` 8,346 of 8,346 differ (from sync 1).
- **F10 online** (widescreen.cpp): the request (`s_pending`) is in the snapshot, so a rollback's
  load would forget a press after the save point. Online and seat-owned, the seat's latest request
  is kept outside the snapshot (`s_seat_*`) and put back after every load; it stays until the
  view is what it asks (a replay crossing the screen change makes the change again). Offline the
  old behaviour, unchanged (cleared when applied). `XMEN2_NP_F10=SEAT:TICK,...`
  (`netplay --seat-f10`) presses it headless; `XMEN2_WS_DBG=1` logs each load's view and request.
- Sessions (`scenario-options-menu`, two seats, against the offline digests of the host's options):
  host `full_rate_2p=1,widescreen=16:9,lives=inf`, seat 1 `high_fps=144,widescreen=0` under
  `mid`: 0 desyncs, seat 1 plays 4:3. The same with `widescreen_gameplay=1`: seat 1 takes the
  host's 16:9, 0 desyncs. Host `widescreen=16:9` under `bad` with seat 1 (own 4:3) pressing F10
  before tick 600: its view turns 16:9 at the next screen (frame 1203), 7 rollbacks after it keep
  the request, 0 desyncs.

### music_voices: the music on sound machines of its own (2026-10-24)

The user (2026-10-24): "fix these sound issues in the game itself, not just our renderer. as an
option (default on)". The first option on by default. The issues: GEMS takes a voice per note
and, with none free, steals the lowest-priority one or drops the note (P7-kickoff.md section 4),
so a stage song with two players' parts (up to 7 FM voices of 6) loses notes, and effects take
voices from the music. The capture's `XMEN2_SOLO`/`XMEN2_BED` only worked around it in renders.
- **How** (platform/native/sound2.cpp, now three instances of one audio-only machine): with the
  option on, in the session machine (where the chips run), a *music* machine and a *parts*
  machine (each its own Z80 + GEMS driver + YM2612 + PSG, chips compiled again as mus_/prt_)
  get every command the game writes into the first machine's ring, as it is, effects included.
  The music machine keeps the effects' and the part songs' tracks muted in its slots (+6 bit 1,
  as R9's second machine); the parts machine everything but the part songs. A muted track takes
  no voice but still does what it does besides notes, which matters: some "effects" are music
  changes (`$45` = stop SavageA4's `$42`, start `$43`; `$46` = stop `$43`, start `$44`; `$73`
  stops the Brood's `$30` and starts `$72`; `$D1`, `$21` stop songs), and the variables and the
  tempo stay the first machine's. So the three drivers run the same songs and each sounds its
  own. The first machine runs as always (RAM, registers, timers: the game and the digest), and
  `capture_audio_write` drops its music voices (key-ons sent as key-offs, DAC samples as $80,
  PSG volumes silent: the `XMEN2_STEM` filter), so it sounds the effects. Music vs effect is the
  capture's class (capture.cpp `is_music`); `$42` (SavageA4's music, started the frame after its
  stop-all) was added to its list. A first version sent the music machines only the music
  commands: the stage sweep below found `$43` lost in SavageA4 with four players (started by the
  "effect" `$45`), and `$42` would have kept playing beside it.
- **The digest and the seal** (R12's precedent for presentation-only state): the two machines are
  not in the canonical digest and `music_voices` is not in the sealed options
  (`presentation_only`): each seat's own. They are in the snapshot (`snd_music`, `snd_parts`), so
  rollbacks and savestates keep them in step. The traced runs (not the session machine) never
  start them. R9's second machine is unchanged (its snapshot bytes, digest fields, counters).
- **Counters** (the counters line's `"sound"`; every machine and the first): per song, FM key-ons
  and PSG notes the driver wrote, its voices stolen by another song's note (a pool entry's +3
  owner rewritten while busy, with no free in between) and stolen from others, and the time its
  FM channels were keyed / PSG channels loud (ms).
- **Measured** (session 3, 2026-10-24; references: the option off with `XMEN2_SOLO` of the
  base or of the parts, so each layer has the chip to itself):
  - Siberia, P1 Gambit + P2 Wolverine (`boot.input`, `start_char=gambit,lives=inf`, 3,400
    frames, song `$27`, parts `$2B` `$2D`): off, the base loses 3 of 481 FM notes, 15 voices
    stolen, 135.1 s of 156.3 s FM time; the parts lose ~0.3 s. On: base 481 notes / 156.3 s,
    `$2B` 72 / 60.5 s, `$2D` 114 / 11.8 s, 0 stolen: the solo references (key-ons exact, times
    within 0.1%).
  - BaniMazaB2 (`scene18`, `$25` + `$28`/`$2F`): off, the base 959 of 1,053 FM notes and 392 of
    435 PSG; on 1,053 / 435, parts exact, 0 stolen. SavageB1 (`scene26`, `$47`/`$48` +
    `$49`/`$4E`) and CloneA1 (`scene38`, `$64`/`$6C` + `$65`/`$6A`: off, `$6C` 18 voices stolen,
    51.3 of 62.0 s): on, the solo references, 0 stolen (times within 1.2%).
  - Effects: with the option on, the first machine's heard FM key-ons per effect equal the
    original's `XMEN2_STEM=sfx` stem, and the effects-only WAV is sample-identical on and off.
    The full mix on = its music stem + its effects stem (AC residual 0.3; the -380 is one chip's
    DC bias counted twice). Loudness: AC rms 843 on, 834 off. Each extra chip adds its DC bias
    (~360 a chip in the mean).
  - Every stage input + boot, `players` 2 and 4 (46 runs): every song the first machine keys on
    is an effect (heard there) or plays on a music machine, but one `$51` blip (one 2 ms note in
    scene26 with four players: the game stops and restarts it within a frame, which the first
    machine, a frame ahead, catches). That checked only that each song plays somewhere, not how
    much of it: a part with none of its tracks started was missed ("Track slots" below has the
    note-for-note check).
  - The game: canonical digests equal with the option on and off (3,400 sync points). Two seats
    on loopback, one with `music_voices=0`: 0 desyncs against the offline digests under `mid`
    and under `bad` (8 rollbacks).
  - Savestates: `--state-probe 300:60` passes; leaving `snd_music` or `snd_parts` out of the
    restore is caught at every save point (11 of 11).
  - Cost: two more machines ~0.5 ms a Z80 frame (a headless 3,400-frame two-player session
    4.5 s -> 7.6 s).
- **The capture** with it on: `XMEN2_STEM=music` = the music machines (the first machine silent),
  `sfx` = the first machine's effects (the others muted); `XMEN2_SOLO` mutes the music machines'
  other songs too. A music bed no longer needs `XMEN2_SOLO` layers or `XMEN2_BED`.
- **Track slots** (2026-10-25; the user heard player 2's part missing in a four-player Siberia,
  P1 Gambit, P2 Wolverine, P3 Nightcrawler, P4 Psylocke). The driver has 16 track slots, and a song's
  tracks beyond the free ones are not started (P7-kickoff.md section 1). A muted track takes no voice
  but holds its slot, and with four players' effects the parts machine's slots were full of the
  effects' muted tracks when Siberia's dispatchers started the parts at tick 480: boot.input, session
  40, `players=4,start_char=gambit,lives=inf,full_rate_2p=1`, `$2B` got three slots and Wolverine's
  `$2D` none of its four. The first machine loses it too in that run (the original's limit: `$2D`
  got one slot there, a track without notes), and so did the old reference (`music_voices=0`,
  `XMEN2_SOLO=2B,2D`: `$2B` 80, no `$2D`), so it is not a copy's error but the slots. Now the music
  machines don't start an effect that only plays notes: `song_controls` (sound2.cpp) scans the
  sequence bank (GEMS 2.8, `$1641BE`) for any command that acts beyond its own track, decoded from
  the driver's dispatch at Z80 `$0537`: `$68` tempo, `$6B` start a song, `$6E` the DAC rate, `$70`
  store a variable, `$72` 0-4 (stop a song, `$0CAA`/`$0B50` on a song, the master volume at `$15AE`
  that every note adds; `$72 5` is the track's own volume). 78 of the 256 songs have one, among the
  effects `$20`, `$21`, `$45`, `$46`, `$73`, `$D1`, `$F6` and `$F7`; the other effects start no
  tracks on the music machines. All 256 songs' tracks parse to their `$60` with the driver's argument counts, and no
  song has a goto. `XMEN2_BED` lets those effects through too (muted under `XMEN2_SOLO`), so a bed
  with a solo is the first machine's reference for the music machines.
  - The failing runs: session 40 with `full_rate_2p=1`, parts `$2B` 38 + `$2D` 60 (were `$2B` 75,
    no `$2D`), and session 78 at 30 Hz, `$2B` 74 + `$2D` 121: both the references' (the bed and
    `XMEN2_SOLO=2B,2D`; session 78's plain solo too). The lineups of the backlog's seeds have moved
    with boot.input's remake (a0fd6f9): session 40 at 30 Hz is now P2 Beast, and its parts play.
  - The check, `uv run tools/soundcheck.py` (every stage input; `--players 4,3`, `--seeds 1,40,78`,
    30 and 60 Hz: 276 cases, 6 min at 30 jobs): each part song's notes on the parts machine and the
    second against the first machine and the second in a bed with a solo of the parts, note for note
    within 8 frames (`XMEN2_KEYON_LOG`, every note counted: machine, frame, song). 273 pass; the
    three others are scene42 (cloneA5) with `players=4` at 30 Hz, where the game itself differs run
    to run from sync 1649 (two bytes at `$FF6B4E` first, then everything), with the option on or
    off and without this change: no reference there. The same sweep before the change fails on the
    parts (`$2D` 0 vs 60, `$2B` 75 vs 38, `$28` 350 vs 351). `--players 2` on the four part stages:
    24 of 24. The music machine's other songs are compared too, and printed, not counted: a driver
    polls its command ring between tempo ticks, so a command lands up to a few frames before or after
    the first machine's (a stop-all three frames late: five more notes of `$08` before scene10's
    briefing ends), and PSG notes (a volume on a silent channel) merge or split with the timing.
  - Canonical digests on and off equal (boot sessions 3, 40, 78; scene18, 26, 38 four players,
    scene24 three); `--state-probe 300:60` on session 40 at 60 Hz: 11 save points exact, and with
    `--state-skip snd_parts` all 11 resaved snapshots differ; `tools/gate.py --mode native` passes.

## Open questions

- (Resolved 2026-10-17 by the user: keep AvalonB1_RunScene's CLEAR_COUNTER.) It byte-matches only
  through `#define CLEAR_COUNTER(x) if (1) (x) = 0` (game/c/00FDAA.c), a code-free basic-block
  boundary that stops MPW C merging the six adjacent counter clears into clr.l; the original
  probably cleared six separate globals, which ram_globals doesn't name. The alternatives were
  naming six globals (only if no code indexes them as an array) or dropping the match (then
  nothing verifies the function: every call spans frames).
- (Resolved 2026-10-17 by the user, P4-decomp.md decision 13: dead features and interrupt-only
  paths listed as unreachable; the reachable rest got synthetic cases.)
- (Resolved 2026-10-16 by the user, P4-decomp.md decisions 9-12: the cases verify couldn't decide
  as defined.) (9) A tested function's own argument slots are dead stack: Spr_EmitPre,
  Spr_EmitPreClip and Spr_EmitPreLinked store into theirs (`move.w d7,$16(a6)`), VDP_QueueDma
  rewrites the caller's `$4/$A/$E(a7)` when it splits a request at a 64 KB boundary (7 of 117k
  calls), Tile_RunHooksAtPoints as MPW C does with a parameter it changes (4 bytes short of a
  match); five more do it and byte-match (AvalonB1_PalFadeStep, BanimazaDoor_DrawDebris,
  Apocalypse_Move, Cerebro_PrintText, Collide_ObjVsList); 26 ROM functions write a slot of their
  MPW frame. (10) Statically unreachable instructions listed in the phase doc count as covered:
  Scroll_ComputeParallax's `beq.s` at $16008E, after a `bne.w` whose flags (a `cmpi` whose `bls`
  was just not taken) always take it. (Player_NearestLive's `$1F6AB6-$1F6AB8` and
  Exodus_StateSwoopStart's `$02D40A-$02D410` were the same kind and byte-match.) (11) A synthetic
  case for a state the game never creates counts: Actor_Move's paths for g_HalfSpeed ($FFC946)
  and g_SegFlag38C4 ($FFC93C), which the ROM only clears (`game/verify/1F4C56.syn`). (12)
  EntryPoint and the two interrupt vectors (no match, no shadow test) are verified by D2 + D6.
- (Resolved 2026-10-15 by the user: fragments, the live stack at sync points, never-executed asm
  and the D2 reference: P4-decomp.md decisions 5-8, "Phase 4" above.)
- MPW switch tables (`move.w Sw(pc,dN),dN / jmp Sw-2(pc,dN)`): the core's offset-table probe
  reads the table from the JMP base (= the JMP's own extension word, one word early), seeds
  `base` as code and decodes the table as instructions. This is the source of the 26
  `MISALIGNED_FUNC_ENTRY` warnings and the 52 `offset_table` hybrid dispatch sites. Fix in the
  core: recognise the preceding `move.w d8(pc,Xn),Xn`, read targets from its table, bound the
  table with `code_addrs_file`.
- (Resolved: frame equality while the CPU runs across frame boundaries, by exact mode above.
  Phase 2 complete 2026-10-15, K10 user-verified.)
