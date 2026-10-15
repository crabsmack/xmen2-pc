/* Deathbird_Reset - DEATHBIRD's Reset (class slot +$6A; Deathbird_Init calls it): does nothing
 * but call the four empty Deathbird_EmptyStub1-4. */
#include "xmen2.h"

void Deathbird_Reset(void)
{
	Deathbird_EmptyStub1();
	Deathbird_EmptyStub2();
	Deathbird_EmptyStub3();
	Deathbird_EmptyStub4();
}
