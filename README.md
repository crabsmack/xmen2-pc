# xmen2-pc

**X-Men 2: Clone Wars** (Sega Genesis, 1995) as a native PC game for Windows and Linux,
built from a complete decompilation, with optional widescreen, high frame rates, two players
at the full 60 fps, and up to four players, on one PC or online.

| | |
|---|---|
| ![Magneto's throne room in 16:9](docs/media/magneto.png) | ![Two players in Siberia, in 16:9](docs/media/siberia.png) |
| ![Four players choosing their characters](docs/media/select.png) | ![Four players in the Savage Land, in 16:9](docs/media/savage-land.png) |

*Screenshots from the port at 16:9 (the original is 4:3; its Cerebro screens, like the character
select, stay 4:3), with one, two and four players.*

**You need your own copy of the game.** Nothing of the cartridge is in this repository or
in the downloads. The port reads the graphics, sound and levels from your ROM file every time
it starts, and refuses any file but the USA/Europe cartridge (SHA-1
`61409e6cf6065ab67d8952b891d8edcf47777193`).

## Play

Download the Windows or Linux zip from [Releases](https://github.com/crabsmack/xmen2-pc/releases)
(not the source code), unzip it, and start it: no emulator is needed (or [build it yourself](#build-from-source)).

Started with a double-click, it opens the **launcher**: choose your ROM (Browse...), change the
options, and play on this PC, or host or join an online game. It remembers everything in
`xmen2.ini` beside the program, and finds the ROM by itself next time (or one in its folder). Untick
"Show this launcher when the game starts" to go straight into the game with those settings;
`XMen2.exe --launcher` brings it back. Keyboard and controllers work in it too (arrows or the
D-pad to move, Enter or A to choose).

You can also skip the launcher and give it the ROM directly, or drag the ROM onto the program:

```
XMen2.exe path\to\xmen2.bin          Windows
./xmen2 path/to/xmen2.bin            Linux (needs SDL2: libsdl2-2.0-0; the picker needs zenity or kdialog)
```

Keyboard (player 1): arrows; Z/X/C = A/B/C, A/S/D = X/Y/Z, Enter = Start, Q = Mode.
Controllers (players 1-4, in the order they are plugged in; an unplugged controller's player
goes to the next one plugged in): D-pad or left stick;
X/A/B = A/B/C, LB/Y/RB = X/Y/Z, Start = Start, Back = Mode. F11: fullscreen. Esc: quit.
All of these can be changed: see [Controls](#controls).

## Quick saves and rewind

F5 saves the game, F7 loads it, and F6 picks the slot (1-4) the two use. Hold Backspace, or a
controller's left trigger, to rewind: the game goes back a frame at a time, as it was, for up to a
minute; let go to play on from there. The Controls page sets which inputs rewind ("Rewind (hold)").

Quick saves last until the game quits: nothing is written to disk. Neither works in online play.
The rewind keeps the last 60 seconds by default (a few MB); set the length in `xmen2.ini` (0 turns
it off, up to 3600). While it rewinds, the game's sound plays backwards by default; `sound = silent`
mutes it instead:

```ini
[rewind]
seconds = 60
sound = reversed
```

## Controls

The launcher's **Controls** page sets each player's controls (players 1-4, a tab each):

- **Controller**: any controller (the default: controllers go to players 1-4 in the order they
  are plugged in), one particular controller (that player always gets it, wherever it is
  plugged in), or none.
- **Keyboard**: a ready-made layout (arrow keys with Z X C, or W A S D with J K L, so two players
  can share the keyboard) or no keys.
- **Every button of the Genesis pad** takes two keys and two controller inputs: click a box (or
  choose it with Enter or the A button) and press the key, the controller button, or move the stick
  or trigger you want. Esc cancels, Backspace clears; right-click or Delete clears a box. A key
  belongs to one button of one player, so taking it moves it from wherever it was (the page says
  so). Esc and F4-F11 stay the game's own.
- **Rewind (hold)**, the last row, is not a Genesis button: held, it rewinds the game (see
  [Quick saves and rewind](#quick-saves-and-rewind)). Any player's rewinds.
- **Stick dead zone**: how far a stick or trigger moves before it counts.
- Press anything on the page to try it: a light shows what the game will get for that player.

Controllers: anything SDL knows works with the same bindings, with its buttons named as printed on
it: Xbox, PlayStation 3/4/5, Switch Pro and Joy-Cons, 8BitDo, Stadia, Steam and many more. Face
buttons go by position (the bottom button is "A" on an Xbox pad, Cross on a PlayStation one and B
on a Switch one). A controller SDL doesn't know (a generic USB pad, an arcade stick) works too:
its buttons are taken in order, its hat or first stick moves, and the Controls page binds whatever
it has. For more known controllers, put a [`gamecontrollerdb.txt`](https://github.com/mdqinc/SDL_GameControllerDB)
beside the program (or set `SDL_GAMECONTROLLERCONFIG`). A controller plugged in while playing says
on screen which player it went to.

Online, you play with player 1's controls.

The settings go in `xmen2.ini`, a section per player that isn't at the defaults:

```ini
[player2]
controller = auto
deadzone = 37
up = key:W, pad:dpup, pad:-lefty
a = key:J, pad:x
start = key:Space, pad:start, pad:righttrigger
```

`controller` is `auto`, `none` or a controller's GUID (the launcher writes it, and its name as
`controller_name`). Each button (`up`, `down`, `left`, `right`, `a`, `b`, `c`, `start`, `x`, `y`,
`z`, `mode`, and `rewind`) lists up to two `key:` and two `pad:` inputs, or nothing; a button left
out keeps its default. Keys are SDL's names (`key:Left Shift`, `key:Keypad 8`); controller inputs are SDL's
(`pad:a` the bottom face button, `b` right, `x` left, `y` top, `leftshoulder`, `rightshoulder`,
`back`, `start`, `guide`, `leftstick`, `rightstick`, `dpup`..., `misc1`, `paddle1`-`paddle4`,
`touchpad`; a stick direction `-leftx` / `+leftx` / `-lefty` / `+lefty`, the same for `rightx`
/ `righty`; triggers `lefttrigger`, `righttrigger`).

## Options

By default the game plays exactly like the cartridge: every option that changes the game is off
unless you turn it on. What is on by default changes only what you see and hear, never the game:
the picture follows your display (`widescreen` and `high_fps` are `auto`), `music_voices`
plays the music on chips of its own, `sample_retime` and `sample_smooth` play the sound
driver's samples at a steady rate and smoothed.
`widescreen=0,high_fps=0,music_voices=0,sample_retime=0,sample_smooth=0` gives
the cartridge's picture and sound. F4-F11 say on screen what they did.

The launcher's Options page sets them, with a description of each. They can also go on the
command line, `XMen2.exe xmen2.bin --options widescreen=16:9,high_fps=144`, or in the settings
file, `xmen2.ini`: a plain text file in the same folder as `XMen2.exe` (Linux: `xmen2`), the
folder you unzipped. The launcher writes it when you press Play (and the game when you choose your
ROM with the file picker). You can also make it yourself with any text editor (Notepad: save as
`xmen2.ini` with "All files", so it doesn't become `xmen2.ini.txt`). Options go under
`[options]`, one per line; the command line wins over the file:

```ini
[options]
widescreen=16:9
lives=inf
start_stage=avalon

[rom]
path = C:\Games\xmen2.bin
```

`XMen2.exe --options --json` prints the options and, as `settings_file`, the file it reads
(`settings_file_found` says whether it exists).

| Option | What it does |
|---|---|
| `widescreen=16:9` | A 16:9 view in the stages; the game plays as in 4:3 (the Cerebro screens stay 4:3). F10 toggles it. The default, `auto`, is 16:9 on a display wider than 3:2 (16:10, 16:9, ultrawide), else 4:3 |
| `widescreen_gameplay=1` | With widescreen: enemies wake and appear across the wide view, and the camera keeps it inside the stage (changes the game) |
| `high_fps=144` | Frames in between the game's own for a 120/144/165/240 Hz display (any rate from 61 to 1000). F9 toggles it. The default, `auto`, is the display's own rate when it is over 60 Hz (and follows the window to another display), else off |
| `shader_preset=PATH` | The picture through a RetroArch shader preset (`.slangp`): see [Shader presets](#shader-presets). F8 turns it off and on. The default, empty, is the plain picture |
| `scaling=integer` | How the picture is enlarged to the window: see [Scaling](#scaling). F4 switches it. The default, `sharp_bilinear`, fills the window with every pixel the same size |
| `pacing=display` | Experimental, not yet measured on a live display: pictures go out on the display's refreshes (vsync), each at the refresh nearest its time, and with `high_fps` each refresh shows the game at its own moment (`high_fps` is then at most that rate). The default, `timer`, shows them by the PC's clock, as before. If the display holds pictures back (a locked, hidden or minimized window), the timer takes over for 5 s at a time |
| `full_rate_2p=1` | Two players at the full 60 frames a second (the original halves it) |
| `players=4` | Three or four players, on one PC (a controller each, in plug-in order) or online: players 3 and 4 join at the player select, with their own panels; the camera keeps everyone in view, and their characters' music parts play on sound chips of their own |
| `lives=inf` | Lives are never spent (or `lives=N`, 1-99) |
| `magneto=1` | Magneto can be chosen from the start |
| `start_char=wolverine` | The character you start Siberia with: `beast`, `cyclops`, `gambit`, `nightcrawler`, `psylocke`, `wolverine` or `magneto` (otherwise random) |
| `start_stage=avalon` | A level select: every new game starts at this stage, and so does power-on. A world's name (`sentinels`, `avalon`, `banimaza`, `savage`, `space`, `clone`) starts at its Cerebro briefing; a later stage starts at the player select before it: `sentinels_b1`, `sentinels_a1`, `sentinels_b2`, `avalon_b2`, `avalon_a2`, `avalon_a3`, `avalon_a4` (Magneto), `banimaza_a1`, `banimaza_a2` (Apocalypse), `savage_b1`, `savage_a2`, `savage_a3`, `savage_a4`, `space_a1`, `clone_a2`, `clone_a5` (the last stage). From there the game carries on as usual. The default, `0`, is Siberia |
| `magneto_flight=1` | Magneto moves left and right while hovering |
| `select_idles=1` | On the player select, characters play their idle animations now and then, as when standing in a stage, and Magneto's pick is his standing shot with the flux bomb's explosion sound. Gambit's idle, which the original plays only as his look-up (hold Up), never while standing, plays on the select and when he stands in the stages |
| `music_voices=full` | The default: the music plays on sound chips of its own and the players' music parts on further sets, so neither the parts nor the sound effects take notes from the music, and each player's part plays in full, as one player hears it. On the cartridge, two players' parts need more voices than the chip has, and the effects take voices from the music; with two players or more the game plays each part thinned, one of its tracks muted. `1` keeps the chips of their own with the game's thinned parts, `0` gives the cartridge's single set of chips |
| `sample_retime=1` | The default: the samples the sound driver plays through the YM2612's DAC, in the music and the effects, reach the chip at a steady rate, each up to 4 ms later than the driver writes it (about 2 ms). The driver gets them to the chip unevenly; `0` plays them as it writes them |
| `sample_smooth=1` | The default: between one of those samples' values and the next, the chip gets a straight line, a value at each of its own samples, where the cartridge holds each value until the next (a stair-step, whose steps add high frequencies). `0` holds them. With `sample_retime=0` too, the lines lead to the values at the times the driver writes them |
| `fm_chip=ym3438` | The FM sound (music, effects and samples) on the YM3438, the later version of the Mega Drive's sound chip, which has no DAC "ladder" distortion. The distortion is clearest in quiet passages. The default, `ym2612`, is the cartridge's YM2612 |

`XMen2.exe --options --json` lists them.

## Shader presets

`shader_preset` puts the picture through a RetroArch shader preset (a `.slangp` file), with
[librashader](https://github.com/SnowflakePowered/librashader). It is off by default.

The game comes with no presets. Choose a `.slangp` file in the launcher (Options, Picture, Shader
preset, Browse...), or set `shader_preset=C:\path\to\preset.slangp` (on Linux,
`/path/to/preset.slangp`) in `xmen2.ini` (a relative path starts at the game's folder). libretro's
[slang-shaders](https://github.com/libretro/slang-shaders) is one collection of them.

F8 turns the preset off and on while playing. A preset that can't be loaded leaves
the plain picture, with the reason on the console. Presets that keep earlier frames see every
picture shown, so with `high_fps` they get the frames in between too. The librashader library
(MPL-2.0) comes with the game: on Windows `librashader.dll`, the upstream release unmodified; on
Linux `librashader.so`, built from the same release's source unmodified (it needs OpenGL 3.3). Without
it the option does nothing.

## Scaling

The game's picture is 320x224 pixels (400x224 in the 16:9 view), enlarged to the window. Unless the
window is an exact multiple of that, such as the 3x window the game opens in, the pixels can't all
be the same whole number of screen pixels: at 1920x1080, each game pixel is 4.8 screen pixels.
`scaling` chooses what happens then, and F4 switches between the three while playing:

- `sharp_bilinear` (the default): fills the window with every pixel the same size. Each one is
  drawn sharp at a whole multiple (4x at 1080p), and the last step to the window's size is smoothed,
  so where two pixels meet there is at most one blended screen pixel.
- `integer`: the largest whole multiple that fits, sharp, with a black border (4x at 1080p, which
  leaves 160 pixels at the sides and 92 above and below).
- `nearest`: fills the window, sharp, as before this option: at most sizes some pixels come out one
  screen pixel wider or taller than others, which can shimmer when the picture scrolls.

At an exact multiple all three are the same. With a shader preset, the preset draws the picture;
`integer` still sets its size.

## Online

Rollback netplay for two to four players, each on their own machine with their own ROM file.
The host (seat 0) chooses the options that change the game, and every seat plays with them.
The picture and sound options (`high_fps`, the 16:9 view, `shader_preset`, `scaling`, `music_voices`,
`sample_retime`, `sample_smooth`, `fm_chip`) are each seat's own,
except that with the host's `widescreen_gameplay=1` the 16:9 view is the host's too (F10 is off).

There is no lobby or server: one player hosts, and the others connect to the host.

**With a join code (the launcher's Online page):**

1. The host picks the number of players and presses **Get a code**. The launcher asks the router
   to open UDP port 7000 for the game (UPnP or NAT-PMP; the router closes it again by itself within
   two hours) and makes a code like `XM2-5AB3-K9QF-M2PX-TV7` from the host's addresses: the
   internet one, the home network's, and a VPN's such as Tailscale's. It says whether the router
   opened the port. Windows may ask to let `XMen2.exe` use the network: allow it.
2. The host sends the code to the others (a chat message). It holds the host's addresses, so send
   it only to the people you play with.
3. Each of the others pastes it under **Join a game** and presses Join Game. The launcher tries
   every address in the code and uses the nearest that answers; the host decides each player's
   number and the number of players.
4. The game starts on every PC once everyone is in, or when the host presses **Start now** with
   the players in so far. Everyone needs the same release: a player with another is told so.

If the host's router doesn't open the port (some don't, or another router or the internet
provider's stands in front of it: the launcher says which), players on the host's home network or
on a VPN with the host can still join, and so can anyone else once the host forwards UDP 7000 on
the router to their PC. `XMen2.exe --router-check` asks the router as a host would, prints what it
answered, and closes the port again.

The launcher remembers what you used. Its Connection menu also has the ways below without a code.
The steps below are the same thing from a command prompt (Windows: in the game's folder, type
`cmd` in Explorer's address bar and press Enter; Linux: a terminal there, with `./xmen2` for
`XMen2.exe`), and what to check when a connection fails.

**Two players**

1. Everyone uses the same release of the port, each with their own ROM file.
2. The host finds an address the other player can reach:
   - same home network: the host's local address (`ipconfig` on Windows, the IPv4 address,
     such as `192.168.1.20`);
   - over the internet, the simplest way: both install a VPN such as
     [Tailscale](https://tailscale.com) and use the host's Tailscale address (`100.x.y.z`);
   - or the host forwards UDP port 7000 on their router to their PC and gives out their public
     address. Windows may ask to let `XMen2.exe` through the firewall: allow it.
3. The host starts first, choosing the options that change the game (here, infinite lives):
   ```
   XMen2.exe xmen2.bin --netplay lan --seat 0 --seats 2 --bind 0.0.0.0:7000 --options lives=inf
   ```
4. The other player starts theirs, with the host's address in place of `HOST_IP`:
   ```
   XMen2.exe xmen2.bin --netplay lan --seat 1 --seats 2 --bind 0.0.0.0:7001 --peer HOST_IP:7000
   ```
5. Until they find each other, each window says what it is waiting for and for how long; there is
   no time limit, and Esc or closing the window stops it. Then the game starts on both screens (a
   command prompt shows `[netplay] seat 0 of 2 ...`). If they never find each other, check the
   address, the port and the firewall.

**Three or four players:** the host relays everyone's traffic. Each player has their own seat
number (1, 2, 3) and they all connect to the host:
```
host:     XMen2.exe xmen2.bin --netplay hub --seat 0 --seats 4 --bind 0.0.0.0:7000 --options players=4
others:   XMen2.exe xmen2.bin --netplay hub --seat 1 --seats 4 --bind 0.0.0.0:7001 --peer HOST_IP:7000
          (--seat 2 and --seat 3 for the next two players)
```
For three players use `--seats 3` and `players=3`.

**Without a VPN or port forwarding (two players):** `--netplay ice` finds a way through both
routers with a public STUN server. The seats swap their connection details as files in a
folder both can reach (a network share or a synced folder): `--ice-signal-dir DIR` in place
of `--bind` and `--peer`.

`--input-delay D` sets the delay in frames (default 5). `XMen2.exe --netplay --json` lists the flags.
A seat whose release plays a different game from the host's is refused when it connects
(v1.1.0 and v1.1.1 play together; v1.2.0 only with v1.2.0).

## Status

The game logic is the original's. Every function the original wrote in C, 3,047 of 3,164,
has C that compiles with the original compiler (Apple MPW C 3.3) to the cartridge's exact
machine code. The other 117 were written in 68000 assembly, which no compiler produces; their
C is tested against the recompiled cartridge code, with every instruction run and no
differences. With every option
off the port produces the same game state and picture as the recompiled cartridge at every
frame of the test runs. One difference is deliberate: the port runs one game frame per
displayed frame, so it never slows down where the cartridge lagged.

Three and four players are new and have been played less than the rest. Enemies choose their
targets among all the players in play. In two-player stages the original's sound driver
runs short of voices, and the music loses notes to the characters' parts and the sound effects.
By default (`music_voices`) the port gives the music sound chips of its own, so no note is lost,
and plays every player's part in full, where the game thins each part with two players or more.

## How it was made

The game was taken apart in [xmen2disasm](https://github.com/crabsmack/xmen2disasm), a
byte-matching disassembly that is also the hub of the project:

1. A disassembly that rebuilds the cartridge byte for byte, cross-checked against an
   instrumented emulator.
2. A static recompilation to C on
   [segagenesisrecomp](https://github.com/mstan/segagenesisrecomp) (this repo pins a fork,
   [crabsmack/segagenesisrecomp](https://github.com/crabsmack/segagenesisrecomp)),
   frame-identical to the emulator.
3. The original compiler identified and run: Apple MPW C 3.3, under an MPW emulator.
4. Hand-written, readable C for the whole game: 3,164 functions (the disassembly's 3,621,
   less the switch cases and blocks that belong to a parent). The 3,047 the original wrote
   in C byte-match with MPW C 3.3; the 117 written in assembly are tested instruction by
   instruction against the recompiled code.
5. The native port: no 68000 emulation left, the game's memory in the PC's own byte order.
6. The enhancements (16:9, high frame rate, two players at full rate).
7. Multiplayer: savestates, rollback netplay, four players.

Almost all of it was written by AI coding agents (Claude Code) in nine days, under measured
definitions of done. See the disassembly's [README](https://github.com/crabsmack/xmen2disasm)
and [docs/ROADMAP.md](https://github.com/crabsmack/xmen2disasm/blob/main/docs/ROADMAP.md).

## Build from source

You need [uv](https://docs.astral.sh/uv/) and the ROM, named by `XMEN2_ROM`
(Windows: `set XMEN2_ROM=C:\path\to\xmen2.bin`; Linux: `export XMEN2_ROM=/path/to/xmen2.bin`).

```
git clone https://github.com/crabsmack/xmen2-pc
cd xmen2-pc
git submodule update --init --recursive segagenesisrecomp
uv sync
uv run tools/xmen2check.py native-build        # -> build/Release/XMen2Native.exe (Linux: build/XMen2Native)
```

The build doesn't need the `xmen2disasm` submodule (the disassembly; only the recompiler and
the phase checkers use it). The program it builds is the one the zips call `XMen2.exe` / `xmen2`.

Windows: Visual Studio 2022 with the C++ workload. Linux: `build-essential cmake ninja-build
pkg-config libsdl2-dev`. The rest of `tools/` (the recompiler, the trace harness and the phase
checkers) is described in [CLAUDE.md](CLAUDE.md) and [docs/NOTES.md](docs/NOTES.md).

## License

MIT for the original work here ([LICENSE](LICENSE)). The port is built on segagenesisrecomp,
which is PolyForm Noncommercial, so builds of the port may not be used commercially. No
rights in the original game are claimed or granted. Third-party components and their
licenses: [NOTICE.md](NOTICE.md).
