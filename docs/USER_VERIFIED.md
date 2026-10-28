# User verification

Phase-2 criterion K10: the user played the Release build (`build\Release\XMen2Recomp.exe
game\xmen2.bin`) and confirmed it looks, sounds and plays correctly (quick play).

user-verified: 2026-10-15

Phase-4 criterion D8: the user played the hand-C build (`build/XMen2Stepped game/xmen2.bin`,
frame-synchronous, all 3,164 functions in C, live input; commit e906790, engine 22ed7f6) on
Linux up to Avalon and confirmed it plays fine, with no crash and 0 misses. Some chugging during
the Sentinel escape was not investigated. The D2/D6 traces needed a re-run on the new engine
(docs/NOTES.md "The D8 playtest build"); phase.py 4 re-ran them on 2026-10-25, 76/76 equal.

handc-verified: 2026-10-17

Phase-5 criterion P8: the user played the native Windows package (`xmen2-native-windows.zip`,
`XMen2.exe xmen2.bin`, built 2026-10-21 from XMen2Recomp bb6ae07 or later, enhancement options
all off, no `xmen2.ini`) on Windows, a quick test, and said it looks good.

port-verified: 2026-10-21

Phase-6 criterion E6: the user played the native port with the enhancement options (widescreen
16:9, high_fps, full_rate_2p) since XMen2Recomp d8bd610 (the Cyclops sprite fix) and said, asked in
the session on 2026-10-22, that it all plays well.

enhancements-verified: 2026-10-22
