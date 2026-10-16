/* SavageBigBio_Init - SAVAGEBIGBIO Init (class slot +$12): the parent's (SAVAGEBIOBASE) Init,
 * then the SavageA4 boss.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * No tentacle hits or hit points yet; the arena 48 px narrower on each side; the Chamber's
 * shared stats record (g_A4Stats) gets the boss's health (the low byte of g_A4BossHealth) and
 * is its stats (its health from it); its AI script (Tbl_SavageBio_Script2) from the first
 * step on its first update (no script: the default state); sprite Art_SavageBioSpr with palette 3;
 * no tentacle; top x speed $580 (g_A4BossMaxSpeed); no frame shown ($FF, slot +$56) and its boxes
 * (Obj_UpdateBoxes). Then its companion: a BIOBALLS at its position (balls), palette 3, no frame
 * shown.
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): SavageBio_Script2.
 */
#include "xmen2.h"

#define g_A4Stats		(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4BossHealth		(*(s16 *)g_SceneVar3B02)	/* scene variable: SAVAGEBIGBIO's health (SavageBigBio_TakeDamage) */
#define g_A4TentacleHits	(*(s16 *)g_SceneVar3B06)	/* scene variable: players the tentacle touched this frame (bits 1, 2) */
#define g_A4Hit1		(*(s32 *)g_SceneVar3B18)	/* scene variable: where the tentacle touched player 1: x | y << 16 */
#define g_A4Hit2		(*(s32 *)g_SceneVar3B1C)	/* scene variable: the same for player 2 */
#define g_A4BossMaxSpeed	(*(s32 *)g_SceneVar3B24)	/* scene variable: SAVAGEBIGBIO's top x speed (SavageBigBio_Move) */

void SavageBigBio_Init(register SavageBigBio *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SavageBigBio_vtable)->parent->method[3])(self, x, y, pl);	/* SAVAGEBIOBASE Init */
	g_A4TentacleHits = 0;
	g_A4Hit1 = 0;
	g_A4Hit2 = 0;
	self->arenaLeft += 0x30;
	self->arenaRight -= 0x30;
	g_A4Stats->health = g_A4BossHealth;
	self->stats = g_A4Stats;
	self->health = g_A4Stats->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_SavageBio_Script2));
	AI_FIRST_STEP(self, self->ai, 0);
	self->spr.def = CODEPTR(Art_SavageBioSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->tentacle = 0;
	self->tentacleLen = 0;
	g_A4BossMaxSpeed = 0x580;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	Obj_UpdateBoxes(self);
	if (self->balls = (BioBalls *)(*CLASS(Tbl_BioBalls_vtable)->method[2])(0, CLASS(Tbl_BioBalls_vtable),
	    self->x, self->y, 0)) {	/* New */
		self->balls->spr.flags = self->balls->spr.flags & ~3 | 3;
		(*self->balls->cls->method[20])(self->balls, 0xFF);	/* +$56 SetFrame: none */
	}
}
