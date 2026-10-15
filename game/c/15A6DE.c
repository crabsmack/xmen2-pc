/* AiScript_Set - point an object's AI script cursor at a script table and rewind it.
 *
 * cursor: the cursor inside the object (most class Init methods pass &obj+$8C or +$96);
 *         a null cursor is ignored.
 * table:  the script table: a word count, then 4-byte `jmp` thunks to the actions. */
#include "xmen2.h"

void AiScript_Set(AiCursor *cursor, void *table)
{
	if (cursor) {
		cursor->index = 0;
		cursor->table = table;
	}
}
