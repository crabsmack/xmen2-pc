/* SentinelsA1_LoadResources - the Coolant Core's sprite sets (SprDef_Add): Art_CoreElevatorSpr
 * (palette 0) as g_CoreElevatorSprDef, Art_CoreShieldsSpr (palette 1) as g_CoreShieldsSprDef,
 * Art_HoloBoltSpr (palette 3) as g_HoloBoltSprDef, Art_HologramSpr (palette 3) as
 * g_HologramSprDef. */
#include "xmen2.h"

void SentinelsA1_LoadResources(void)
{
	g_CoreElevatorSprDef = SprDef_Add(CODEPTR(Art_CoreElevatorSpr), 0);
	g_CoreShieldsSprDef = SprDef_Add(CODEPTR(Art_CoreShieldsSpr), 1);
	g_HoloBoltSprDef = SprDef_Add(CODEPTR(Art_HoloBoltSpr), 3);
	g_HologramSprDef = SprDef_Add(CODEPTR(Art_HologramSpr), 3);
}
