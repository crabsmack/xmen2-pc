/* PerimeterDoor_ClassInit - per-level class setup of PERIMETERDOOR (class slot +$5E): its empty
 * helper (PerimeterDoor_NopStatic), and no hits on the door yet (g_PerimeterDoorHits). */
#include "xmen2.h"

void PerimeterDoor_ClassInit(void)
{
	PerimeterDoor_NopStatic();
	g_PerimeterDoorHits = 0;
}
