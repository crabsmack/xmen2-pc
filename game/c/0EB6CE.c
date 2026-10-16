/* CoreShields_StateAllDown - CORESHIELDS state once all 8 panels are broken: the endless camera
 * rumble (Tbl_CoreShieldsQuake), the hologram starts dying (Hologram_BeginDying), and a chain of
 * explosions along Tbl_CoreShieldsDeathChain (harmless, 16 px left of and above it, 4 times:
 * ExplodeFx_ChainRepeat).
 *
 * self: the shields.
 * The state stays set: it runs again whenever the timer is out. */
#include "xmen2.h"

void CoreShields_StateAllDown(CoreShields *self)
{
	g_CamShake = CODEPTR(Tbl_CoreShieldsQuake);
	Hologram_BeginDying();
	ExplodeFx_ChainRepeat(self, CODEPTR(Tbl_CoreShieldsDeathChain), 0, -16, -16, 4);
}
