/* MagnetoShots_Reset - free Magneto's two shot objects, the magnetic globe (g_MagneticGlobObj)
 * and the flux bomb (g_FluxBombObj): flags 0 (not present). */
#include "xmen2.h"

void MagnetoShots_Reset(void)
{
	g_MagneticGlobObj.flags = 0;
	g_FluxBombObj.flags = 0;
}
