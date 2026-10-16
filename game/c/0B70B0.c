/* ChildShip_Init - CHILDSHIP Init (class slot +$12): the parent's (XNPC) Init, then the
 * Chamber's shared stats record (g_A4Stats; its health from it, then 8), sprite
 * Art_ChildShipSpr (palette 3), the arena x $60-$220 / cruise y $60-$140, its AI script
 * (Tbl_ChildShip_ScriptLen: the steps of ChildShip_Script, drop a rocket, wait 20, drop, wait
 * 60, fly over the nearest player, wait 20), state and default state ChildShip_StateEnter,
 * flags6 |= $1080, not moving, at the camera's left edge (x g_CameraX, px 100 left of it) and
 * cruise y $F0, frame 0 (slot +$56), its boxes (Obj_UpdateBoxes), palette line 3 from the 16
 * words at Art_ChildShipSpr + 4, no explosion trail, sound $42 restarted, the camera keeps it in
 * view (g_A4CamFocus), and its bob from Tbl_BabyBioBobOffsets.
 *
 * self: the object.  x, y: position (8 fraction bits), passed on.  pl: its placement entry
 * (passed on). */
#include "xmen2.h"

#define g_A4Stats	(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4CamFocus	(*(PTR(Actor) *)g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void ChildShip_Init(register ChildShip *self, long x, long y, Placement *pl)
{
	s16 *src;
	register s16 *dst;
	register long i;

	(*CLASS(Tbl_ChildShip_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->stats = g_A4Stats;
	self->health = g_A4Stats->health;
	self->health = 8;
	self->spr.def = CODEPTR(Art_ChildShipSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->xMin = 0x60;
	self->xMax = 0x220;
	self->floorY = 0x140;
	self->ceilY = 0x60;
	AiScript_Set(&self->ai, CODEPTR(Tbl_ChildShip_ScriptLen));
	self->state = self->defaultState = ChildShip_StateEnter;
	self->flags6 |= 0x80;
	self->flags6 |= 0x1000;
	STOP_MOVING(self);
	self->px = g_CameraX - 100;
	self->py = 0xF0;
	self->x = g_CameraX << 8;
	self->homeY = 0xF000;
	self->y = self->homeY;
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	Obj_UpdateBoxes(self);
	src = (s16 *)CODEPTR(Art_ChildShipSpr) + 2;
	dst = &g_PalBase[48];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	self->trail = 0;
	Snd_Stop(0x42);
	Snd_Play(0x42);
	g_A4CamFocus = (Actor *)self;
	self->bob = (HoverStep *)CODEPTR(Tbl_BabyBioBobOffsets);
}
