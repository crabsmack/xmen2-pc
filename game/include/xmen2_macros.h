/* xmen2_macros.h - the idioms the original C used over and over, as macros (hand-written C,
 * phase 4). xmen2disasm docs/COMPILER.md "How the original C was written" has the evidence:
 * state changes were do { } while (0) macros, and a?b:c conditions are materialized in d3. */
#ifndef XMEN2_MACROS_H
#define XMEN2_MACROS_H

/* An object is present and alive: flags bits $C000 = $8000. */
#define IS_ALIVE(o)		(((o)->flags & 0xC000) == 0x8000)

/* |v| as the original wrote it (tst / sge / neg: a materialized condition). */
#define ABS(v)			((v) >= 0 ? (v) : -(v))

/* Switch an object to state f now: store it and run it (Barbarian, Exodus...). */
#define GOTO_STATE(o, f)	do { (o)->state = (f); (*(f))(o); } while (0)
/* Switch an object to state f from the next update: reset the delay, store it. */
#define SET_STATE(o, f)		do { (o)->timer = 0; (o)->state = (f); } while (0)

#endif /* XMEN2_MACROS_H */
