/* SiberiaB1_StepWind - one step of the Blizzard's wind (when its 8-frame step timer has run
 * out): angle and strength move by their steps toward the keyframe g_WindKey. Once both reach
 * it, the wind takes it exactly and goes on to the next keyframe (Tbl_SiberiaB1Wind, wrapping
 * at a negative hold): the next one's angle 0 starts wind sound $21, angle $37 sound $20; the
 * angle step spreads the angle change over the strength change / $80 steps (at most 5 degrees
 * either way), the strength step is $80 toward it; the keyframe's hold becomes the step timer.
 * Every frame: the wind offset added to plane A's scroll (SiberiaB1_CamApply) is strength *
 * (cos, -sin) of the angle >> 24.
 * The angle variable starts out as the strength; that value is never used. The empty
 * do/while before the offset stores is in the ROM: a label there (the pending argument pop of
 * the Math calls lands before it, after the `tst.l` of oy). */
#include "xmen2.h"

#define g_WindAngle	(*(s16 *)g_SceneVar3B04)	/* scene variable: wind angle, degrees */
#define g_WindStrength	(*(s16 *)g_SceneVar3B06)	/* scene variable: wind strength, 8 fraction bits */
#define g_WindTimer	(*(s16 *)g_SceneVar3B08)	/* scene variable: frames to the next wind step */
#define g_WindKey	(*(PTR(WindKey) *)g_SceneVar3B14)	/* scene variable: the keyframe the wind moves to */
#define g_WindAngleStep	(*(s16 *)g_SceneVar3B40)	/* scene variable: angle change per step (-5..5) */
#define g_WindStrengthStep	(*(s16 *)g_SceneVar3B48)	/* scene variable: strength change per step ($80 / -$80) */

void SiberiaB1_StepWind(void)
{
	register short steps = 0;
	short angle = g_WindStrength;
	register short strength;
	short oy;
	long sy;
	long cx;
	short hold;
	register short ox;

	if (g_WindTimer > 0)
		return;
	g_WindAngle += g_WindAngleStep;
	g_WindStrength += g_WindStrengthStep;
	g_WindTimer = 8;
	if (g_WindStrengthStep >= 0 && g_WindStrength >= g_WindKey->strength
	    || g_WindStrengthStep < 0 && g_WindStrength <= g_WindKey->strength) {
		g_WindStrength = g_WindKey->strength;
		if (g_WindAngleStep >= 0 && g_WindAngle >= g_WindKey->angle
		    || g_WindAngleStep < 0 && g_WindAngle <= g_WindKey->angle) {
			angle = g_WindKey->angle, strength = g_WindKey->strength,
				hold = g_WindKey->hold, g_WindKey++;
			if (g_WindKey->hold < 0)
				g_WindKey = (WindKey *)CODEPTR(Tbl_SiberiaB1Wind);
			if (g_WindKey->angle == 0) {
				Snd_Stop(0x20);
				Snd_Stop(0x21);
				Snd_Play(0x21);
			}
			if (g_WindKey->angle == 0x37) {
				Snd_Stop(0x21);
				Snd_Stop(0x20);
				Snd_Play(0x20);
			}
			steps = ABS((g_WindKey->strength - strength) >> 7);
			if (!steps)
				g_WindAngleStep = g_WindKey->angle - angle;
			else
				g_WindAngleStep = (g_WindKey->angle - angle) / steps;
			if (g_WindAngleStep > 0)
				g_WindAngleStep = MIN(g_WindAngleStep, 5);
			else if (g_WindAngleStep < 0)
				g_WindAngleStep = MAX(g_WindAngleStep, -5);
			g_WindStrengthStep = strength < g_WindKey->strength ? (short)0x80 : (short)-0x80;
			g_WindAngle = angle;
			g_WindStrength = strength;
			g_WindTimer = hold;
		}
	}
	cx = g_WindStrength * Math_CosDeg(g_WindAngle);
	sy = g_WindStrength * Math_SinDeg(g_WindAngle);
	ox = cx >> 24;
	oy = sy >> 24;
	do {
	} while (0);
	g_BgOffsetX = ox;
	g_BgOffsetY = -oy;
}
