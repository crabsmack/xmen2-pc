/* XClone_Init - XCLONE constructor (class slot +$12): the parent's Init, then a clone that waits
 * for the players (XClone_StateWaitPlayers) in its arena.
 *
 * self:  the new object.
 * x, y:  position (8 fraction bits).
 * place: its placement entry: x, y = the arena's centre bottom; param[1] = the arena's half width
 *        (0: $A0).
 * Default clone record and character record (the EVIL* Inits set their own), the record's main
 * script, stats Tbl_XCloneStats but 32 health, flags6 |= $82, palette 2, sprite priority like
 * the first live player's (player 1's when none is), walking speed $300, keeping $60 px away.
 * The arena: x limits half width - $18 either side, the floor at the placement y, the ceiling
 * at the lowest point of the segment above (else $C0 up), and the arena walls (scene variables,
 * the near sides of the type $19 segments left and right, else $A0 px away). Hidden (frame $FF). */
#include "xmen2.h"

#define g_CloneWallLeft		(*(s16 *)g_SceneVar3B2C)	/* scene variable: x of the arena's left wall */
#define g_CloneWallRight	(*(s16 *)g_SceneVar3B2A)	/* scene variable: x of the arena's right wall */

void XClone_Init(XClone *self, long x, long y, Placement *place)
{
	short half;
	short px, py;
	Seg *seg;

	(*CLASS(Tbl_XClone_vtable)->parent->method[3])(self, x, y, place);	/* BIONPC Init */
	STOP_MOVING(self);
	self->state = XClone_StateWaitPlayers;
	self->defaultState = XClone_StateWaitPlayers;
	self->record = (CloneRecord *)CODEPTR(Tbl_XCloneDefaultRecord);
	self->charRec = (CharRecord *)CODEPTR(Tbl_XCloneAiScript);
	AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	self->stats = (NpcStats *)CODEPTR(Tbl_XCloneStats);
	self->health = self->stats->health;
	self->health = 0x20;
	self->flags6 |= 0x82;
	self->spr.flags = self->spr.flags & ~3 | 2;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	self->child = 0;
	self->walkSpeed = 0x300;
	self->keepDistance = 0x60;
	self->arenaLeft = place->x - ((half = place->param[1]) ? half : 0xA0);
	self->arenaLeft += 0x18;
	self->arenaRight = place->x + ((half = place->param[1]) ? half : 0xA0);
	self->arenaRight -= 0x18;
	self->arenaFloor = place->y;
	self->shotsLeft = 0;
	px = place->x;
	py = place->y;
	if (seg = Seg_FindAbove(px, py + 32, 0))
		self->arenaTop = MAX(seg->y1, seg->y2);
	else
		self->arenaTop = py - 0xC0;
	if (seg = Seg_FindLeft(px, py + 32, 0x19))
		g_CloneWallLeft = MAX(seg->x1, seg->x2);
	else
		g_CloneWallLeft = px - 0xA0;
	if (seg = Seg_FindRight(px, py + 32, 0x19))
		g_CloneWallRight = MIN(seg->x1, seg->x2);
	else
		g_CloneWallRight = px + 0xA0;
	self->attackCooldown = 0;
	self->recentHits = 0;
	self->lockedArena = 0;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
}
