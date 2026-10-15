/* Exodus_TakeDamage - EXODUS TakeDamage (class slot +$26; the parent's isn't called).
 *
 * self: the boss.  damage: health lost.  attacker, type, hx, hy: unused.
 * 90 frames of hit cooldown and blink (flags6 $40, blinkTimer), sound $DF stopped and $D8
 * played, health -= damage (not below 0). At 0, unless the scene is already ending: the scene
 * ends in 40 frames (Scene_EndAfter) and it dies by deathFx: 0 (and any other value) its Kill
 * (class slot +$52), 1 ExplodeFx_BurstMedium, 2 ExplodeFx_BurstSmall, 3 ExplodeFx_Bounce,
 * 4 ExplodeFx_TrailArc. Otherwise at health 10 or less it swoops to segment B
 * (Exodus_AtkSwoopB), at 20 or less to segment A, each once (it replaces the state, the timer
 * runs on).
 * The cases are Exodus_TakeDamage_part1 (0 / default), _part2 (1, 2), _part3 (3), _part4 (4)
 * and _part5 (the swoops) in the disassembly. */
#include "xmen2.h"

void Exodus_TakeDamage(register Exodus *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	self->hitCooldown = 90;
	self->blinkTimer = (u8)self->hitCooldown;
	self->flags6 |= 0x40;
	Snd_Stop(0xDF);
	Snd_Stop(0xD8);
	Snd_Play(0xD8);
	self->health -= damage;
	if (self->health < 0)
		self->health = 0;
	if (!self->health && !g_SceneExitTimer) {
		Scene_EndAfter(40);
		switch (self->deathFx) {
		default:
		case 0:		/* Exodus_TakeDamage_part1 */
			(*self->cls->method[19])(self);		/* +$52 Kill */
			break;
		case 1:		/* Exodus_TakeDamage_part2 */
			ExplodeFx_BurstMedium(self);
			break;
		case 2:
			ExplodeFx_BurstSmall(self);
			break;
		case 3:		/* Exodus_TakeDamage_part3 */
			ExplodeFx_Bounce(self);
			break;
		case 4:		/* Exodus_TakeDamage_part4 */
			ExplodeFx_TrailArc(self);
			break;
		}
	} else if (10 >= self->health && !self->swoopBDone)	/* Exodus_TakeDamage_part5 */
		self->state = Exodus_AtkSwoopB;
	else if (20 >= self->health && !self->swoopADone)
		self->state = Exodus_AtkSwoopA;
}
