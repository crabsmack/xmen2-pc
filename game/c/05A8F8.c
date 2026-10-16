/* Seq_NextWrap - the next word of a -1-terminated word sequence (Boulder_SpawnerTick's delays,
 * Tbl_BoulderSpawnDelays); at the end it starts over at entry 0.
 *
 * seq: the sequence.  idx: the entry to read next (advanced).  out: gets the word.
 * Returns 1 when it wrapped (out = entry 0, idx = 1), else 0. */
#include "xmen2.h"

short Seq_NextWrap(s16 *seq, s16 *idx, s16 *out)
{
	*out = *(seq + *idx);
	if (*out == -1) {
		*idx = 1;
		*out = *seq;
		return 1;
	}
	(*idx)++;
	return 0;
}
