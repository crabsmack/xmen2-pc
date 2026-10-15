/* BigBio_TakeDamage - BIGBIO hit (class slot +$26; the parent's isn't called). Ignored while the
 * orbs are out (orb mode bits 0 or 3).
 *
 * self:     the boss.
 * attacker: unused; type, x, y unused too.
 * damage:   taken from health; hit cooldown 20.
 * The body palette (line 3) shows the damage: Pal_BigBioBody row 0 above 50 health, row 1 at
 * 31-50 (row 2, flashing, comes from BigBio_Draw below 16). At 0 or less it stops, blinks and is
 * defeated in 20 frames (BigBio_StateDefeated); else it saves the state it was in (unless one is
 * saved already), blinks and resumes it in 20 frames (BigBio_ResumeAfterHit). */
#include "xmen2.h"

void BigBio_TakeDamage(register BigBio *self, Actor *attacker, char type, char damage, short x, short y)
{
	s16 *src;
	s16 *pal;
	register s16 *dst;
	long i;

	pal = (s16 *)CODEPTR(Pal_BigBioBody);
	if (self->orbMode & 9)
		return;
	self->health -= damage;
	self->hitCooldown = 20;
	if (self->health > 50) {
		src = pal;
		dst = &g_PalBase[0x30];
		for (i = 0; i < 16; i++)
			*dst++ = *src++;
		g_PalBaseDirty = 1;
	} else if (self->health > 30) {
		src = &pal[16];
		dst = &g_PalBase[0x30];
		for (i = 0; i < 16; i++)
			*dst++ = *src++;
		g_PalBaseDirty = 1;
	}
	if (self->health <= 0) {
		STOP_MOVING(self);
		self->state = BigBio_StateDefeated;
		self->flags6 |= 0x40;
		self->timer = 20;
	} else {
		if (!self->hurtState)
			self->hurtState = self->state;
		self->flags6 |= 0x40;
		self->state = BigBio_ResumeAfterHit;
		self->timer = 20;
	}
}
