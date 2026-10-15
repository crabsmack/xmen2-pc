/* XPlayer_TakeDamage - XPLAYER class slot +$26: the player is hit: lose health and get knocked back,
 * or die; nothing while the hit cooldown runs.
 *
 * self:     the player.
 * attacker: the object that hit it (or 0).
 * type:     hit type: 5 knocked left, 6 knocked right, 7 damage only (raw, no knockback),
 *           12 a bounce (in the air in the control state: a jump again, sound $9E; no damage).
 * damage:   whole health units ($100 each; raw for type 7).
 * Health 0 starts the death (XPlayer_StartDeath). A knockback, unless the player is climbing
 * (Player_ClimbState) or on the ground while the camera looks up or down: stopped, out of its
 * animation, detached, jumped (playerFlags 8), the hurt sound, pushed 6 px away (type 5/6:
 * speed $200) or 3 px away from the attacker's facing (speed $100), vy -$100, gravity 60,
 * then its hurt state (+$60). Blinks (flags6 $40) and is unhittable for 40 frames. */
#include "xmen2.h"

void XPlayer_TakeDamage(XPlayer *self, Actor *attacker, char type, char damage)
{
	register short dmg;
	char climbing;
	char looking;

	dmg = damage;
	climbing = (char)Player_ClimbState(self);
	if (self->hitCooldown > 0)
		return;
	looking = !(self->flags & 0x1000) && (g_Camera->camFlags & 8 || g_Camera->camFlags & 4);
	if (type == 12) {
		if (self->flags & 0x1000 && self->state == (StateFn)self->cls->method[40]) {
			self->ay = self->charRec->gravity;
			self->vy = self->charRec->jumpVy;
			self->playerFlags &= ~6;
			Snd_Stop(0x9E);
			Snd_Play(0x9E);
		}
		return;
	}
	if (dmg) {
		if (type != 7)
			dmg = dmg << 8;
		self->health -= dmg;
		if (self->health < 0)
			self->health = 0;
		if (self->health <= 0) {
			self->state = (StateFn)XPlayer_StartDeath;
			self->timer = 0;
			self->hitCooldown = 0x7FFF;
			STOP_MOVING(self);
		}
		g_HudDirty = 1;
		if (type == 7)
			return;
	}
	self->flags6 |= 0x40;
	if (!climbing && !looking) {
		STOP_MOVING(self);
		self->anim = 0;
		self->spr.flags &= ~8;
		self->flags6 &= ~0x4000;
		Obj_Detach(self);
		self->playerFlags |= 8;
		Snd_Stop(self->charRec->hurtSound);
		Snd_Play(self->charRec->hurtSound);
		switch (type) {
		case 5:
			self->vx = -0x200;
			self->x -= 0x600;
			break;
		case 6:
			self->vx = 0x200;
			self->x += 0x600;
			break;
		default:
			if (attacker && attacker->spr.flags & 0x10) {
				self->vx = -0x100;
				self->x -= 0x300;
			} else {
				self->vx = 0x100;
				self->x += 0x300;
			}
			break;
		}
		self->vy = -0x100;
		self->ay = 60;
	}
	if (self->health <= 0) {
		self->state = (StateFn)XPlayer_StartDeath;
		self->timer = 0;
		self->hitCooldown = (short)40000;	/* $9C40: negative as a word */
		STOP_MOVING(self);
	} else {
		if (!climbing && !looking) {
			self->state = self->hurtState;
			self->timer = 0;
		}
		self->hitCooldown = 40;
	}
}
