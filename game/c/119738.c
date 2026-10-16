/* BBoard_New - BBOARD New (class slot +$0A): a switch board of count entries on the level stack
 * (Mem_StackAlloc of the class's CalcSize, slot +$12), its class set and Init (slot +$0E) run.
 *
 * cls:   the class descriptor (Tbl_BBoard_vtable; Switch_ClassInit, 100 entries).
 * count: entries.
 * Returns the board. (No test for a failed allocation.) CalcSize is passed the class as its
 * first argument and its result is taken as a short. */
#include "xmen2.h"

BBoard *BBoard_New(Class *cls, register short count)
{
	register Class *c = cls;
	register BBoard *board;

	board = (BBoard *)Mem_StackAlloc((short)(*c->method[3])(cls, count), STR(Str_Empty_119782, ""));	/* +$12 CalcSize */
	board->cls = c;
	(*board->cls->method[2])(board, count);		/* +$0E Init */
	return board;
}
