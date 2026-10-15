/* XPlayer_StateControl - XPLAYER class slot +$A6, the hub every player state returns to: read the
 * pad and dispatch (class slots in brackets).
 *
 * self: the player (its segment's type decides where it may jump and walk).
 * Leaving the ground: LeaveGround (+$8E). A wall or ceiling to grab: the climbing state (+$AA).
 * A slow fall in the scene (g_FloatFallSpeed) in the air: XPlayer_StartFloatFall.
 * In the air: B attacks with strength[0] (Up +$BA, Down +$BE, else +$C2); A (released for a
 * character whose special fires on release: playerFlags $10) the specials with strength[3]
 * above $700 health, else [1] (Up +$CA, Down +$CE, else +$C6); C pressed when it can double
 * jump (+$92) the double jump (+$B6); else steer (AirSteer +$8A).
 * Landing (on the ground, playerFlags $200 not yet set): not jumped any more, unlatch C, Land
 * (+$9A); stopping: a hard landing (+$EA) from a fall speed of $44C on, else stopped, here
 * again next frame.
 * On the ground: B attacks with strength[4] facing the direction held (Up +$D6, Down +$DA,
 * else +$D2), A the specials with strength[7] / [5] (Up +$E6, Down +$E2, else +$DE). On a
 * segment of type 3, 5-$D, $13-$17 or $1C, C (without Down, and no playerFlags $10000) jumps
 * (+$82, then the checks go on), Left/Right walk (+$AE) unless held back by the tether or flags
 * $100 (then unlatch the side it faces). Down+C alone on a segment of a layer above 0 drops
 * through it (+$86). Down crouches (+$B2); nothing pressed stands (+$EE).
 * The hit type set with an attack: 12 Up, 11 Down (in the air), else 9.
 * Returns self->flags & 8 when it switches for the next frame; nothing when it runs a state
 * now. */
#include "xmen2.h"

short XPlayer_StateControl(register XPlayer *self)
{
	Seg *seg;
#ifdef XMEN2_MPW
	short unused;		/* read and never used */
#endif
	char type;

	seg = self->seg;
#ifdef XMEN2_MPW
	unused = seg->slope;
	type = seg->type;
#else
	/* With no segment (in the air, or a new player) the 68000 reads ROM $000001 ($FF): -1. */
	type = seg ? (char)seg->type : (char)-1;
#endif
	if (self->flags & 0x1000 && self->playerFlags & 0x200)
		(*self->cls->method[34])(self);				/* LeaveGround */
	if ((char)(*self->cls->method[36])(self)) {			/* CheckGrab */
		GOTO_STATE(self, (StateFn)self->cls->method[41]);	/* StateClimb */
		RETURN_NO_VALUE;
	}
	if (g_FloatFallSpeed && self->flags & 0x1000) {
		self->state = (StateFn)XPlayer_StartFloatFall;
		XPlayer_StartFloatFall(self);
		RETURN_NO_VALUE;
	}

	/* in the air */
	if (self->flags & 0x1000 && (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
		self->playerFlags &= ~0x20;
		self->attack = self->charRec->strength[0];
		if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
			self->hitType = 12;
			GOTO_STATE(self, (StateFn)self->cls->method[45]);	/* AirAttackUp */
			RETURN_NO_VALUE;
		}
		if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
			self->hitType = 11;
			GOTO_STATE(self, (StateFn)self->cls->method[46]);	/* AirAttackDown */
			RETURN_NO_VALUE;
		}
		self->hitType = 9;
		GOTO_STATE(self, (StateFn)self->cls->method[47]);		/* AirAttack */
		RETURN_NO_VALUE;
	}
	if (self->flags & 0x1000
	    && (self->playerFlags & 0x10
		? (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA)
		: (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))) {
		self->playerFlags &= ~0x20;
		if (self->health >= 0x701)
			self->attack = self->charRec->strength[3];
		else
			self->attack = self->charRec->strength[1];
		if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
			self->hitType = 12;
			GOTO_STATE(self, (StateFn)self->cls->method[49]);	/* AirSpecialUp */
			RETURN_NO_VALUE;
		}
		if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
			self->hitType = 11;
			GOTO_STATE(self, (StateFn)self->cls->method[50]);	/* AirSpecialDown */
			RETURN_NO_VALUE;
		}
		self->hitType = 9;
		GOTO_STATE(self, (StateFn)self->cls->method[48]);		/* AirSpecial */
		RETURN_NO_VALUE;
	}

	/* landing */
	if (!(self->flags & 0x1000) && !(self->playerFlags & 0x200)) {
		self->playerFlags &= ~8;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		if ((char)(*self->cls->method[37])(self)) {		/* Land */
			if (self->prevVy >= 0x44C) {
				GOTO_STATE(self, (StateFn)self->cls->method[57]);	/* HardLanding */
				RETURN_NO_VALUE;
			}
			STOP_MOVING(self);
			SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
			return self->flags & 8;
		}
	}
	if (self->flags & 0x1000) {
		if ((!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC)
		    && (char)(*self->cls->method[35])(self)) {		/* CanDoubleJump */
			GOTO_STATE(self, (StateFn)self->cls->method[44]);	/* DoubleJump */
			RETURN_NO_VALUE;
		}
		(*self->cls->method[33])(self);				/* AirSteer */
	}

	/* on the ground */
	if (!(self->flags & 0x1000) && (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)) {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
			self->spr.flags &= ~0x10;
		else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
			self->spr.flags |= 0x10;
		self->playerFlags &= ~0x20;
		self->attack = self->charRec->strength[4];
		if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
			self->hitType = 12;
			GOTO_STATE(self, (StateFn)self->cls->method[52]);	/* GroundAttackUp */
			RETURN_NO_VALUE;
		}
		if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
			self->hitType = 9;
			GOTO_STATE(self, (StateFn)self->cls->method[53]);	/* GroundAttackDown */
			RETURN_NO_VALUE;
		}
		self->hitType = 9;
		GOTO_STATE(self, (StateFn)self->cls->method[51]);		/* GroundAttack */
		RETURN_NO_VALUE;
	}
	if (!(self->flags & 0x1000)
	    && (self->playerFlags & 0x10
		? (!self->pad ? g_Pad1Released & g_PadMaskA : g_Pad2Released & g_PadMaskA)
		: (!self->pad ? g_Pad1Latch & g_PadMaskA : g_Pad2Latch & g_PadMaskA))) {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight)
			self->spr.flags &= ~0x10;
		else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft)
			self->spr.flags |= 0x10;
		self->playerFlags &= ~0x20;
		if (self->health >= 0x701)
			self->attack = self->charRec->strength[7];
		else
			self->attack = self->charRec->strength[5];
		if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
			self->hitType = 12;
			GOTO_STATE(self, (StateFn)self->cls->method[56]);	/* GroundSpecialUp */
			RETURN_NO_VALUE;
		}
		if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
			self->hitType = 9;
			GOTO_STATE(self, (StateFn)self->cls->method[55]);	/* GroundSpecialDown */
			RETURN_NO_VALUE;
		}
		self->hitType = 9;
		GOTO_STATE(self, (StateFn)self->cls->method[54]);		/* GroundSpecial */
		RETURN_NO_VALUE;
	}
	if (!(self->flags & 0x1000)
	    && (type == 3 || type == 0x13 || type == 0x14 || type == 0x15 || type == 0x16 || type == 0x17
		|| type == 0x1C || type >= 5 && type <= 0xD)
	    && (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC)
	    && !(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && !(self->playerFlags & 0x10000)) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		(*self->cls->method[31])(self);				/* Jump */
	}
	if (!(self->flags & 0x1000)
	    && (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC)
	    && (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && !(!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight)
#ifdef XMEN2_MPW
	    && (s8)seg->layer > 0) {
#else
	    /* with no segment the 68000 reads ROM $000002 ($C5: negative) */
	    && (seg ? (s8)seg->layer : (s8)-59) > 0) {
#endif
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskC;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskC;
		(*self->cls->method[32])(self);				/* DropThrough */
	}
	if (!(self->flags & 0x1000)
	    && (!self->pad ? g_Pad1Latch & g_PadMaskLeftRight : g_Pad2Latch & g_PadMaskLeftRight)
	    && (type == 3 || type == 0x13 || type == 0x14 || type == 0x15 || type == 0x16 || type == 0x17
		|| type == 0x1C || type >= 5 && type <= 0xD)) {
		if (self->flags & 0x100 || XPlayer_PushingTetherEdge(self)) {
			if (self->spr.flags & 0x10) {
				if (!self->pad)
					g_Pad1Latch = g_Pad1Latch & ~g_PadMaskLeft;
				else
					g_Pad2Latch = g_Pad2Latch & ~g_PadMaskLeft;
			} else {
				if (!self->pad)
					g_Pad1Latch = g_Pad1Latch & ~g_PadMaskRight;
				else
					g_Pad2Latch = g_Pad2Latch & ~g_PadMaskRight;
			}
		} else {
			SET_STATE(self, (StateFn)self->cls->method[42]);	/* Walk */
			return self->flags & 8;
		}
	}
	if (!(self->flags & 0x1000) && (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)) {
		SET_STATE(self, (StateFn)self->cls->method[43]);		/* Crouch */
		return self->flags & 8;
	}
	if (!(self->flags & 0x1000)
	    && !(!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown)
	    && !(!self->pad ? g_Pad1Latch & g_PadMaskLeftRight : g_Pad2Latch & g_PadMaskLeftRight)
	    && !(!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB)
	    && !(!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC)) {
		SET_STATE(self, (StateFn)self->cls->method[58]);		/* Stand */
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)self->cls->method[40]);			/* StateControl */
	return self->flags & 8;
}
