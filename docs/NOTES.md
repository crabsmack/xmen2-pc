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

`xmen2disasm tools/cc.py --match-recomp ../XMen2Recomp --json` compiles each game/c file
with the headers. MPW C 3.3's global symbol table can't hold every declaration of the game
(Fatal Error 429), so cc.py gives each compile the lines of `xmen2_funcs.h`, `xmen2_data.h`
and `xmen2_globals.h` that declare names the file uses (unused declarations emit no code).

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
  from 4,529, RAM CRC only. (First choice `g_P1Lives` differed at sync 1: the lives are 8/9 from
  the first sync point, not only "at new game"; `g_LevelFrame` at sync 2.)
- **Result:** `trace --mode native` equals the stepped-gen reference (every row: sync + four
  CRCs, same count; 0 misses, 0 interpreter instructions) on the TAS (51,209 sync points, 24 s,
  2,200 fps) and on **all 76 corpus runs** (2,042,653 sync points; `tools/synccorpus.py --jobs 24`,
  92.5 s wall, longest fuzz-55800-s2 51 s).

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
- Link (`link.categories`): cpu_core 172 (exact_sched.obj, g_cpu and m68k_read/write in glue.obj,
  CPU_* in handc_rt.obj), interpreter 48 (m68k_interp.obj, m68k_decoder.obj), recomp_runtime 6,968
  (6,781 of them the bridge's func_/hcslot_/conv_ slot entries; glue.obj 145, dispatch 12),
  generated 1,951, emu_ram 3 (ram_views.obj). Compiler labels ($unwind$, string literals) and the
  CRT's inline printf/scanf COMDATs are left out; C tentative definitions (g_cpu, g_ram, g_x*), which
  the map lists as `<common>`, are resolved to their declaring objects from the COFF tables.
- **Generated code still linked**: 0 game functions (D6 holds), but 1,951 func_ definitions: 1,606
  A5 jump-table thunks (`jmp abs.l` entries; call_slots follows such a jump itself) and **345
  interior labels of C functions** (`game/xmen2.labels.toml`) in 37 generated bodies, e.g.
  `func_body_005CF8` inside Game_SceneLoop with 17 entries. D6's definition counts only function starts, so it never saw
  them. Nothing in the corpus showed a difference, but a dispatch to one would run generated code
  silently instead of stopping: the stepped build's skip list should take them (open, P3).

### Builds (this machine, own build tree)

Clean build of the four phase-4 runners 3m16s (3 recompiler runs, 32 threads); adding XMen2Native
plus a header edit that recompiles the hand C of three targets 6m15s; one source changed 2m02s;
nothing changed (`build --native`, MSBuild up-to-date checks of 5 targets + the scan) 23 s. The
build tree is ~0.7 GB without traces. Note: any edit of `game/include/*.h` (even a comment)
changes the key of xmen2disasm's `cc.py --match-recomp` cache, so the next `phase.py 4`
re-matches every file once (a 6-file sample re-matched after this change).

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
