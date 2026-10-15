/* Apocalypse_CallNopStatics - call APOCALYPSE's three empty helpers (from its ClassInit and
 * Nop6A). */
#include "xmen2.h"

void Apocalypse_CallNopStatics(void)
{
	Apocalypse_NopStatic1();
	Apocalypse_NopStatic2();
	Apocalypse_NopStatic3();
}
