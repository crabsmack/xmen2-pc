/* LittleLeg_New - LITTLELEG New (class slot +$0A): make the boss's little leg at (x, y): $68 bytes
 * from the stack allocator, cleared; its three sprites from the leg sprite definition
 * (g_LittleLegSprDef, priority on), walking; a Timer running its Update (class slot +$0E) every
 * frame, started; and a draw-list entry for its Draw (class slot +$12, priority, layer $7F).
 *
 * unused: 0 (New's object argument).  x, y: home position, pixels.
 * Returns the leg, or 0 when out of memory. */
#include "xmen2.h"

#define g_LittleLegSprDef	(*(s16 *)g_SceneVar3B04)	/* scene variable (CloneA5): sprite-definition slot of Art_CloneA5Spr2, the leg */

LittleLeg *LittleLeg_New(long unused, short x, short y)
{
	register LittleLeg *self;

	if (self = Mem_StackAlloc(sizeof(LittleLeg), STR(Str_Empty_085D4A, ""))) {
		Mem_Set(self, 0, sizeof(LittleLeg));
		self->cls = CLASS(Tbl_LittleLeg_vtable);
		self->px = self->x = x;
		self->py = self->y = y;
		Spr_SetFromDef(&self->part[0].spr, g_LittleLegSprDef);
		self->part[0].spr.flags |= 4;
		Spr_SetFromDef(&self->part[1].spr, g_LittleLegSprDef);
		self->part[1].spr.flags |= 4;
		Spr_SetFromDef(&self->part[2].spr, g_LittleLegSprDef);
		self->part[2].spr.flags |= 4;
		self->walking = 0;
		LittleLeg_SetWalking(self, 1);
		if (self->timer = Timer_Create((TimerFn)self->cls->method[2], 1, self, 1))	/* +$0E Update */
			Timer_Start(self->timer);
		self->drawNode = DrawList_Add(self->cls->method[3], self, 4, 0x7F, 0, 0);	/* +$12 Draw */
	}
	return self;
}
