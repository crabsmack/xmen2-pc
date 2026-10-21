/* xmen2_options.h - the native runner's enhancement options (xmen2disasm docs/phases/P6-enhance.md
 * R1): set by platform/native/options.cpp from the command line and xmen2.ini, read by the platform
 * and by the game's native-only branches (`#if defined(XMEN2_NATIVE)`). All 0 = the original. */
#ifndef XMEN2_OPTIONS_H
#define XMEN2_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XmOptions {
    int full_rate_2p;   /* R3: two players on the one-player stepping, no frame-tick wait */
    int high_fps;       /* R4: display rate in Hz (0 = off) */
    int widescreen;     /* R5-R7: 1 = 16:9 */
} XmOptions;

extern XmOptions g_opt;

#ifdef __cplusplus
}
#endif

#endif /* XMEN2_OPTIONS_H */
