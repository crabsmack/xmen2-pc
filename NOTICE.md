# Notice

## The game

*X-Men 2: Clone Wars* and its characters are the property of their respective rights holders.
This project is not affiliated with or endorsed by Marvel, Sega or anyone else connected with
the original game.

This repository and the release packages contain **no game data**. The port reads the
graphics, sound data and levels from a ROM file you supply yourself, and checks it by SHA-1.
Each package is scanned before release: no 256-byte window of any packaged file, other than
runs of one byte value, occurs in the ROM. Before this repository was made public, its full
git history was scanned for ROM content as well (every blob, binary and hex text, 32-byte
windows), and none was found.

The MIT license in [LICENSE](LICENSE) covers the original work here. It does not, and cannot,
grant any rights in the original game.

## Third-party components

| Component | Role | License |
|---|---|---|
| [segagenesisrecomp](https://github.com/mstan/segagenesisrecomp) (Matthew Stan), submodule `segagenesisrecomp/` (fork `crabsmack/segagenesisrecomp`) | The recompiler and engine the port grew from | PolyForm Noncommercial 1.0.0, so **builds of this port may not be used commercially** |
| [m68k-recomp-core](https://github.com/mstan/m68k-recomp-core), [z80-recomp-core](https://github.com/mstan/z80-recomp-core) | Recompiler cores (development; the native port has no 68000 left) | their own license files, in the submodules |
| [ymfm](https://github.com/aaronsgiles/ymfm) (Aaron Giles) | YM2612 FM synthesis | BSD-3-Clause |
| [superzazu/z80](https://github.com/superzazu/z80) | Z80 sound CPU | MIT |
| [SDL2](https://libsdl.org) | Window, input, audio | zlib |
| [libjuice](https://github.com/paullouisageneau/libjuice) | ICE / NAT traversal for netplay | MPL-2.0, v1.7.2 unmodified (fetched by recomp-net's CMake from the link) |
| [recomp-net](https://github.com/RetroPortingToolKit/recomp-net) | Netplay transport | MIT |
| [rbengine](https://github.com/RetroPortingToolKit/rbengine) | Rollback snapshot ring for netplay | MIT |
| [minicoro](https://github.com/edubart/minicoro) | The game fiber's context switch | Unlicense or MIT-0 (LuaCoco parts: MIT) |
| [clowncommon](https://github.com/Clownacy/clowncommon), [tomlc99](https://github.com/cktan/tomlc99) | Engine helpers, TOML parsing | ISC, MIT |
| [xmen2disasm](https://github.com/crabsmack/xmen2disasm) | The disassembly (submodule) | MIT, see its NOTICE.md |

The engine's own list is `segagenesisrecomp/THIRD-PARTY-LICENSES.md`; the release packages
carry the license texts in `LICENSES/`.
