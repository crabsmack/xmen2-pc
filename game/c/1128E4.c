/* Deathbird_StateSendHornets - DEATHBIRD while the lift is low (below $10000): when the wave timer
 * runs out and fewer than 2 HORNETs are alive, drop in the next hornet of the schedule
 * (Tbl_DeathbirdHornetSchedule, from the start after its -1 end) and wait its delay; once the lift
 * is up there, the attacks begin in 10 frames (sounds $3E, $3C, $3D stopped, $3D played;
 * Deathbird_StateChooseAttack).
 *
 * self: the boss. Returns self->flags & 8.
 * A hornet starts 20 px above the screen at x 110 or 370 px (HornetWave side); its record is a
 * Placement on the stack with all params 0 (Hornet_Init's defaults).
 *
 * MPW: the original reached HORNET's class in an unusual way, the only `move.l x+2(a5)` in the
 * ROM: it declared the class as an external data record, and MPW Link resolved that A5-relative
 * data reference to the class's jump-table entry, whose long at +2 is the class (HORNET_CLASS;
 * the name is pasted so cc.py's header subset leaves out data.h's code declaration of it). And it
 * called Obj_CountOfClass undeclared: the short count is compared as an int (COUNT_OF_CLASS). The
 * object code equals the ROM's; cc.py refuses the jump-table reference with an addend (+2). */
#include "xmen2.h"

#define PASTE(a, b)		a##b
#ifdef XMEN2_MPW
extern struct { short jmp; Class *cls; } PASTE(Tbl_Hornet, _vtable);
#define HORNET_CLASS		(PASTE(Tbl_Hornet, _vtable).cls)
#define COUNT_OF_CLASS(c)	((long (*)())Obj_CountOfClass)(c)
#else
#define HORNET_CLASS		CLASS(PASTE(Tbl_Hornet, _vtable))
#define COUNT_OF_CLASS(c)	Obj_CountOfClass(c)
#endif
#define g_DeathbirdHornets	(*(PTR(HornetWave) *)g_SceneVar3B1C)	/* scene variable: next entry of the hornet schedule */

short Deathbird_StateSendHornets(register Deathbird *self)
{
	Placement rec;
	Placement *pl;
	char kind;
	HornetWave *next;
	char side;
	char delay;
	char unused;
	long x, y;

	if (g_Elevator->pos < 0x10000) {
		--g_HornetWaveTimer;
		if (g_HornetWaveTimer <= 0 && COUNT_OF_CLASS(HORNET_CLASS) < 2) {
			pl = &rec;
			next = g_DeathbirdHornets;
			kind = next->kind;
			side = next->side;
			delay = next->delay;
			unused = next->unused;
			x = 0xF000;
			y = (g_CameraY - 20) << 8;
			if (!side)
				x -= 0x8200;
			else
				x += 0x8200;
			pl->x = (short)(x >> 8);
			pl->y = (short)(y >> 8);
			pl->param[0] = 0;
			pl->param[1] = 0;
			pl->param[2] = 0;
			pl->param[3] = 0;
			pl->param[4] = 0;
			Hornet_SpawnForDeathbird(x, y, pl, kind);
			g_HornetWaveTimer = delay;
			next++;
			if (next->kind == -1)
				next = (HornetWave *)CODEPTR(Tbl_DeathbirdHornetSchedule);
			g_DeathbirdHornets = next;
		}
		SET_STATE(self, (StateFn)Deathbird_StateSendHornets);
		return self->flags & 8;
	}
	self->countdown = 10;
	Snd_Stop(0x3E);
	Snd_Stop(0x3C);
	Snd_Stop(0x3D);
	Snd_Play(0x3D);
	SET_STATE(self, (StateFn)Deathbird_StateChooseAttack);
	return self->flags & 8;
}
