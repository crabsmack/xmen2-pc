# Review 001 (2026-10-16, reviewer: D7 reviewer agent 1, sample seed 41604, entries 1-95)
reviewed: 95
agree: 88
open disputes: 0

For each of the 95 entries I read `game/c/<ADDR>.c` beside `src/f/<ADDR>.asm` (and, for the switch functions, the split-off fragment files: TechWallGun_UpdateBullet's cases and Draw, Cerebro_PrintText_part1/2, Magneto_BombOrigin's cases, Player2_StartLife's cases). For the 92 MPW-matched functions I took the code as proven and checked the types, the PC-only branches (Failure_RunScene's 68000-stack buffer), that the scene-variable aliases are taken over array globals (`g_SceneVar3Bxx[]`), how pointer parameters cross the bridge (Magneto_BombOrigin: its only caller is C with host locals), and every claim in the doc comment. For the 3 shadow-tested functions (Scaler_Start, FluxBomb_StateHeld, Scroll_RequestVsram) I also traced every path against the asm by hand. I checked claims against the C and asm of the callees and callers (Actor_Update, Level_Run, ClassList_CallSlot62, Hologram_SetPose, SprDef_Add, Math_SinDeg, Timer_Create, ApocBomb_Update, ChildShip_OnHurt, Scaler_RowBlocks, TrapDoor states, FlyBrood_StateWaitPlayerPassed, Beast_StateCrouch...), the field and global comments in xmen2_types.h and xmen2_globals.h, xmen2disasm docs/NOTES.md, the data labels in xmen2.asm, and one TAS screenshot (f024600: the Apocalypse arena). I ran no builds, traces or game runners. Nits are noted against agreed functions. Several of them are wording that recurs across the code base, so it is worth sweeping for.

## Disputes

## Resolved
- 0B7394 ChildShip_TakeDamage: the comment now says "flags6 |= 1 and 2, bits 0 and 1: no attack-list and no body-list entry, +$80/+$84". Checked.
- 0E9DEA Hologram_WaitLowerLiftBoarded: the $1B store is now described as a dead store kept from the original, overwritten at once by Hologram_SetPose(0), which arms the flicker only for pose $1B. Checked.
- 15D980 Actor_Nop62: now "ACTOR LevelEnd (class slot +$62)", run when the level's frame loop has returned (ClassList_CallSlot62: Level_Run; PlayerSelect_Run at its scene's end). BabyBio/Platform1/XPlayer_LevelStart are renamed *_LevelEnd (xmen2_funcs.h; no "LevelStart" left in game/c or the headers), and the disassembly NOTES slot map reads "+$62 `LevelEnd` ... after the level's frame loop". Checked.
- 04E0AC ApocBomb_Move: angle 180 is now "the far side of the orbit, where ApocBomb_Init drops it: ground y 162, against 226 nearest the viewer at angle 0". ApocBomb_Init (far side) and ApocBomb_Update are also rewritten from the code: frame 0 near to 4 far, priority low over the far side and high from about 275 through 0 to 60, hittable only near the near side. Checked.
- 09B224 BroodClaw_Swing: "rising on a sine curve"; dy is explained as 1/256 px (going out, sin(offset px / 2) x 32 px + 4; 0 at the ends; going back, 0 and 4 at the ends). The direction byte is SWING_BACK(o), a commented file-local alias for ACTOR +$8B, used the same way in 09ACDC.c and 09B1AC.c. Checked.
- 1FC5FE Scaler_Start: now "five $280-byte tables g_ScalerLine0-4 and the last $140 bytes, g_ScalerLine5, the expanded half-width source row". The g_ScalerLine0 and g_ScalerLine5 comments in xmen2_globals.h agree. Checked.
- 0BEDB0 SporeMine_ClassInit: "palette 3". Debris_CreatePool's parameter is now `pal` in both the C (13F4D4.c) and the prototype. Checked.

## Agreed
- 142CD6 TechWallGun_UpdateBullet (checked with its Case and Draw fragments)
- 133F3C FireWall_GetSize
- 0FD50A SiberiaB1_InstallTileHooks (the scene names, 48 x $14-byte shards and Timer_Create's `user` are all confirmed)
- 0BCE0E SavageMediumBio_StateAppear
- 114D0A BioNpc_TakeDamage (the C is right that spr.flags bits 0-1 are the palette; the asm header's "top bits = flip" is wrong, on the disasm side)
- 05C1B2 Tusk_PlayerOnPlatform
- 04BF52 Apocalypse_StateApproach (nit: the vertical half of the overlap test always passes, because the range starts at the target's own box2Top. Worth saying, since it looks like an original slip.)
- 0841B4 SmallBrood_Nop6A
- 064F70 Cerebro_PrintText (checked with part1/part2)
- 11D758 BabySentinelB_Nop66
- 1368F4 Hornet_MoveWaitThenRiseLeave
- 0060E0 Player1_SetPsylocke
- 02E128 ExodusBlast_LaunchDownLeft
- 05BB0E Boulder_ClassInit ("dust" is confirmed by Boulder_Init/Boulder_Update)
- 133F4A FireWall_Reset
- 08718A BroodBoss_ActStop (nit: "next step at once": with a delay of 0 the step runs on the next update, since Actor_Update calls one state per frame)
- 0BE456 Barbarian_StateAttack4
- 0ADA1E TrapDoor_Init (the tile names are confirmed by the TrapDoor states)
- 11DF44 WallCrawlBio_Nop66 ("every class has this body" is confirmed by NOTES)
- 10B76C SpaceA1_InstallTileHooks
- 137382 HornetBullet_Init
- 0BE7E8 Launcher_ClearShots
- 138284 LaserCannon_OnDeath
- 013D26 CapedAcolyte_ActHover
- 037180 MagnetoBoss_StateZone2Approach (the dead -1 test on a u8 is kept and explained)
- 095ECA XClone_StatePodFall
- 112C9E Deathbird_StateFlyOff (mode 6 matches Deathbird_SetMoveMode)
- 0B7B14 SavageA4_NopStatic
- 13DE4A ExplodeFx_BurstSmall
- 00FC12 AvalonB1_ScrollCameraTo
- 02D75E Exodus_InitSpawnRec (spawn-record bit 3 is confirmed by Actor_SpawnRecOutOfView)
- 087DB0 BroodBoss_StateDeathSink (nit: $A0 is in 1/256 px, while $1A8 is compared in pixels; say so)
- 0061D0 Player_CharIndexOf
- 04C306 Apocalypse_StateDefeatedRise
- 02D96C Exodus_HurtOnHotTiles
- 13557A FlyBrood_StateWaitPlayerPassedSine (nit: its comment only refers to its sibling; one self-contained line would help)
- 0AD060 PerimeterDoor_StateWaitExit
- 01180E BigAcolyte_ActWait30
- 02D7B2 Exodus_Nop66
- 094FB2 EvilDrop_StateBounce1
- 197348 Wolverine_StateDiveHold
- 11CE9A BabyBio_ActMoveHorizontal
- 13F282 Pop_NopStatic
- 0BEF12 SporeMine_UpdatePool (nit: `+ 7` in the frame read is the step's frame byte, 2-byte steps after a 6-byte header; worth a word)
- 142A22 TechWallGun_StateCeilAimRight (the angle is in degrees per TechWallGun_AimCeiling)
- 11C3C2 BabyBio_Move (nit: $6400 is the bob table's end marker; comment or name it)
- 13CC90 Ninja_Init
- 1F0A48 XPlayer_StartLookUp
- 0EA24C Hologram_SeqB_Pose3
- 0AB9A8 FloatBubble_StateWobble1
- 118074 Explosion_SpawnEx
- 0E79A0 SentinelsA1_InstallHooks
- 10B584 Elevator_GetSize
- 0DE7B2 SentinelDoor_StateOpen2
- 10778C SpaceB1_RunScene
- 15980C Magneto_BombOrigin (the host `short *`/`long *` parameters are fine: its only caller, Magneto_SpawnFluxBomb, is C)
- 1CEADA Magneto_GroundSpecialDown (nit: the players keep their crouch flag in ACTOR's `px` field; this is code-base-wide, and a player-side alias name would read better than relying on the comment)
- 121408 BioSentinel_StateHurt
- 097FDA EvilBeam_Construct (nit: "priority" -> "priority 4")
- 13FF2C Spark_StateStop
- 067020 Failure_RunScene (nit: Art_FailureNem is a raw 14x2 tilemap of $38 bytes copied with Mem_Copy, not Nemesis art; the data label should be renamed, e.g. Map_GameOver. The comment itself describes it correctly.)
- 04C4C6 Apocalypse_StateResumeScript
- 04EAFC BeamScooter_StateEnter
- 0062E4 Player2_StartLife
- 04B3CE Apocalypse_MoveTo
- 0DD61A ControlBox_Nop6A
- 1E39B8 Cyclops_CrouchAim
- 11434A XNpc_ShakeStep
- 05C6AA Tusk_StateThrow
- 02723E Zapper_Nop66
- 11D6CE BabySentinelA_Nop66
- 037772 MagnetoBoss_StateFlyToFinal
- 0AC440 PerimeterDoor_NopStatic
- 0EC7F8 CoreElevator_Reset
- 160996 Cam_Nop12
- 159030 FluxBomb_StateHeld (shadow-tested; every path checked by hand; the 45 frames come from 158FE4.c, and "frozen" flags6 $4000 is confirmed by Actor_Update)
- 183630 Beast_StateStomp
- 1F3D36 XClimber_SaveSafePos
- 095ABC XClone_ActSoundFA (nit: "go on with the script now": the step runs on the next update, as for BroodBoss_ActStop)
- 11C6CE BabyBio_StateStart (nit: the same "now" for a delay of 0)
- 0945C6 EvilMaker_GetSize
- 13A736 Mine_GetSize
- 1425A0 TechWallGun_BurstCeilRight
- 1F5228 Scroll_RequestVsram (shadow-tested; checked by hand)
- 183AB6 Beast_StartRoll
- 119E3E SyncActor_OutsideActiveWindow
- 0DF230 SentinelElevator_Init
- 09ACCE BroodClaw_GetSize
