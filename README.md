# XMen2Recomp

X-Men 2: Clone Wars (Sega Genesis) statically recompiled to native C with
[segagenesisrecomp](https://github.com/mstan/segagenesisrecomp) (fork
`crabsmack/segagenesisrecomp`), from the byte-matching disassembly
[xmen2disasm](https://github.com/crabsmack/xmen2disasm). The recompiled game runs in the
engine's cycle-exact mode and is frame-identical to BizHawk 2.4's Genesis Plus GX on the
TASVideos movie and the disassembly's test corpus (work RAM, VRAM, CRAM, VSRAM and Z80 RAM
compared every frame).

No ROM is included. You need your own dump of the USA/Europe cartridge
(SHA-1 `61409e6cf6065ab67d8952b891d8edcf47777193`).

## Build (Windows, Visual Studio 2022)

```
git clone --recursive https://github.com/crabsmack/XMen2Recomp
cd XMen2Recomp
set XMEN2_ROM=C:\path\to\xmen2.bin        (or keep ../xmen2disasm/xmen2.bin)
uv sync
uv run tools/xmen2check.py build          # -> build/Release/XMen2Recomp.exe
```

## Play

```
build\Release\XMen2Recomp.exe game\xmen2.bin
```

Controls and video/audio settings live in `settings.ini` next to the exe (created on first
run). The game expects a 6-button pad (`pad_type = 1`) for the same behaviour as the
reference; a 3-button pad works too.

## Development

See [CLAUDE.md](CLAUDE.md) (layout, rules, tools) and [docs/NOTES.md](docs/NOTES.md)
(findings, the divergence log). The phase-2 criteria are checked from the disassembly repo:
`uv run tools/phase.py 2`.
