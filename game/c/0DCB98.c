/* Blast_ClassInit - per-level class setup of BLAST (class slot +$5E): register the barrier sprite
 * set, Art_BarrierSpr (palette 0), as g_BarrierSprDef (the blast's own sprite) and the zap
 * flicker's, Art_BlastSpr (palette 1), as g_BlastSprDef. The parent's isn't called. */
#include "xmen2.h"

void Blast_ClassInit(void)
{
	g_BarrierSprDef = SprDef_Add(CODEPTR(Art_BarrierSpr), 0);
	g_BlastSprDef = SprDef_Add(CODEPTR(Art_BlastSpr), 1);
}
