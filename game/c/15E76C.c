/* Obj_CarryChild - Obj_ForEachChild callback of Actor_Update: move an object carried by its
 * parent along, and the objects it carries in turn.
 *
 * parent: the carrier.  child: an object attached to it.
 * A carried child (flags $1800 both set) moves by the parent's movement this frame (g_ShiftDX/
 * g_ShiftDY pixels), takes the parent's speed and acceleration and gets its boxes recomputed. */
#include "xmen2.h"

void Obj_CarryChild(Actor *parent, Actor *child)
{
	if ((child->flags & 0x1800) == 0x1800) {
		child->x += g_ShiftDX << 8;
		child->y += g_ShiftDY << 8;
		child->vx = parent->vx;
		child->vy = parent->vy;
		child->ax = parent->ax;
		child->ay = parent->ay;
		Obj_UpdateBoxes(child);
	}
	if (child->spr.firstChild != 0xFF)
		Obj_ForEachChild(child, Obj_CarryChild);
}
