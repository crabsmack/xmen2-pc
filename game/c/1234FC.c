/* BigBio_ClassInit - load BIGBIO's level resources (class slot +$5E, static; the parent's isn't
 * called).
 *
 * Allocates the 16 orbs and the spit pool (8 records, none in use), registers the orb/spit
 * sprite definition (Art_BigBioSpr) and starts the timer that runs BigBio_SpitUpdateAll every
 * frame. */
#include "xmen2.h"

void BigBio_ClassInit(void)
{
	register Timer *t;

	BigBio_EmptyStub();
	g_BigBioOrbs = (BigBioOrb *)Mem_StackAlloc(16 * sizeof(BigBioOrb), STR(Str_Empty_123560, ""));
	g_BigBioSpitPool = (BigBioSpit *)Mem_StackAlloc(8 * sizeof(BigBioSpit), STR(Str_Empty_12355E, ""));
	g_BigBioSpitCount = 0;
	g_BigBioSprDef = SprDef_Add(CODEPTR(Art_BigBioSpr), 3);
	t = Timer_Create(BigBio_SpitUpdateAll, 0, 0, 1);
	Timer_Start(t);
}
