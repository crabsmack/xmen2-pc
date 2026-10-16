/* Synchronizer_Init - SYNCHRONIZER Init (class slot +$0E): its timer (running its Run, slot +$12,
 * every frame with itself as argument) started, no members, gathering them from the first spawn
 * record (Synchronizer_GatherMembers), slot in use, and its board entry flagged (SetFlag, slot
 * +$16). Without a free timer nothing else is set (the slot stays free).
 *
 * self: the synchronizer.  memberClass: the members' class.  board: their g_BBoard entry. */
#include "xmen2.h"

void Synchronizer_Init(register Synchronizer *self, Class *memberClass, unsigned char board)
{
	register Timer *t;
	register short i;

	if (t = Timer_Create((TimerFn)self->cls->method[3], 0, self, 0)) {	/* +$12 Run */
		Timer_Start(t);
		do {
			self->memberCount = 0;
			for (i = 0; i < 10; i++)
				self->member[i] = 0;
		} while (0);
		self->state = Synchronizer_GatherMembers;
		self->scan = 0;
		self->memberClass = memberClass;
		self->board = board;
		self->inUse = 1;
		(*self->cls->method[4])(self);		/* +$16 SetFlag */
	}
}
