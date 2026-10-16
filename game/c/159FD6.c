/* Pal_CycleStep - one frame of a palette cycle: count its timer down (twice in two-player step
 * mode) and when it runs out write the next colour of the current sequence into g_PalBase.
 *
 * timer:  frames to the next colour.
 * colour: index of the colour in the current sequence.
 * seq:    index of the sequence in the table.
 * table:  the cycle table (PalCycleSeq records, ended by an offset of -1); a sequence's colour
 *         words are ended by -1.
 * keep:   0: put the three counters back afterwards (the caller shares one state between
 *         several tables and only the last call keeps it).
 * The counters are saved as bytes: a value above 127 comes back sign-extended (the original's). */
#include "xmen2.h"

void Pal_CycleStep(s16 *timer, s16 *colour, s16 *seq, void *table, char keep)
{
	char oldTimer = (char)*timer, oldSeq = (char)*seq, oldColour = (char)*colour;
	register PalCycleSeq *p;
	register s16 *c;

	(*timer)--;
	if (g_TwoPlayerStep)
		(*timer)--;
	if (*timer <= 0) {
		p = (PalCycleSeq *)table + *seq;
		c = (s16 *)((char *)table + p->colours) + *colour;
		g_PalBase[p->entry] = *c;
		g_PalBaseDirty = 1;
		*timer = p->delay;
		(*colour)++;
		c++;
		if (*c == -1) {
			*colour = 0;
			(*seq)++;
			p++;
			if (p->colours == -1)
				*seq = 0;
		}
	}
	if (!keep) {
		*timer = oldTimer;
		*seq = oldSeq;
		*colour = oldColour;
	}
}
