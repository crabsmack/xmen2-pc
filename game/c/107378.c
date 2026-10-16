/* SpaceB1_ResetPalCycles - reset the Maintenance Shaft's palette cycles (SpaceB1_CyclePalettes):
 * each cycle's delay and step. Only the first two run; the third (scene variables $3B38/$3B40) and
 * a fourth (g_SpaceB1Cycle4Delay/Step) are set and never read.
 * Returns nothing (no caller reads d0). */
#include "xmen2.h"

#define g_B1DelayA	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames before cycle A's next step */
#define g_B1DelayB	(*(s16 *)g_SceneVar3B36)	/* scene variable: frames before cycle B's next step */
#define g_B1DelayC	(*(s16 *)g_SceneVar3B38)	/* scene variable: a third cycle's delay (unused) */
#define g_B1StepA	(*(s16 *)g_SceneVar3B3C)	/* scene variable: cycle A's step 0-13 */
#define g_B1StepB	(*(s16 *)g_SceneVar3B3E)	/* scene variable: cycle B's step 0-7 */
#define g_B1StepC	(*(s16 *)g_SceneVar3B40)	/* scene variable: a third cycle's step (unused) */

void SpaceB1_ResetPalCycles(void)
{
	g_B1DelayA = 3;
	g_B1StepA = 0;
	g_B1DelayB = 2;
	g_B1StepB = 0;
	g_B1DelayC = 1;
	g_B1StepC = 0;
	g_SpaceB1Cycle4Delay = 2;
	g_SpaceB1Cycle4Step = 0;
}
