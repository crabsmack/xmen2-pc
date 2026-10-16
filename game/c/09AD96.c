/* BroodClaw_Spawn - create a BROODCLAW at a placement's position (class slot +$0E New); with pair,
 * first another one there that idles (BroodClaw_StateIdle, no motion) and becomes the new claw's
 * pair (BroodClaw_Move mirrors the swing on it).
 *
 * rec:  the placement (x, y: pixels; BroodBoss_StateSetup builds one).
 * pair: non-zero: make a pair.
 * The claw returned gets sprite priority, layer 0 and its draw-list priority from them.
 * Returns the (second) claw. */
#include "xmen2.h"

BroodClaw *BroodClaw_Spawn(register Placement *rec, char pair)
{
	register BroodClaw *claw;
	BroodClaw *first;

	claw = (BroodClaw *)(*CLASS(Tbl_BroodClaw_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_BroodClaw_vtable), rec->x << 8, rec->y << 8, rec);
	if (pair) {
		first = claw;
		first->motion = 0;
		first->state = first->defaultState = (StateFn)BroodClaw_StateIdle;
		claw = (BroodClaw *)(*CLASS(Tbl_BroodClaw_vtable)->method[2])(0,	/* New */
			CLASS(Tbl_BroodClaw_vtable), rec->x << 8, rec->y << 8, rec);
		claw->pair = first;
	}
	claw->spr.flags |= 4;
	claw->layer = 0;
	DrawList_SetPriority(claw->drawNode, claw->spr.flags & 4, claw->layer, claw->flags6 & 8);
	return claw;
}
