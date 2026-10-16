/* SavageA4_FrameHook - the Chamber's per-frame hook (g_SceneFrameHook): the camera kept between
 * block columns 1 and $13, then count the frame, the effect sprite (SavageA4_AnimEffectSprite) and
 * the palette cycles (SavageA4_CyclePalettes, not while a palette fade runs), twice in two-player
 * step mode; then colours 8-15 of palette line 3 show the boss's health:
 * - at 8 or less they flash between Pal_SavageA4BossStage1 (odd frames) and _Stage2 (even);
 * - when the health has fallen to or past 16, 24, 32 or 40 since the last frame, they become
 *   Pal_SavageA4BossStage3, 4, 5 or 6 (the first that applies) and the boss's top speed rises
 *   by $80.
 * Changed colours are uploaded at the next V-blank (g_PalBaseDirty). */
#include "xmen2.h"

#define g_A4BossHealth		(*(s16 *)g_SceneVar3B02)	/* scene variable: SAVAGEBIGBIO's health (SavageBigBio_TakeDamage) */
#define g_A4BossHealthSeen	(*(s16 *)g_SceneVar3B04)	/* scene variable: its health at the last frame hook */
#define g_A4BossMaxSpeed	(*(s32 *)g_SceneVar3B24)	/* scene variable: SAVAGEBIGBIO's top x speed (SavageBigBio_Move) */

void SavageA4_FrameHook(void)
{
	register s16 *pal = 0;
	register s16 *dst;

	Cam_SetMinXBlocks(1);
	Cam_SetMaxXBlocks(0x13);
	g_FrameCounter++;
	SavageA4_AnimEffectSprite();
	if (!g_PalFadeActive)
		SavageA4_CyclePalettes();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		SavageA4_AnimEffectSprite();
		if (!g_PalFadeActive)
			SavageA4_CyclePalettes();
	}
	if (g_A4BossHealth <= 8) {
		if (g_FrameCounter & 1)
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage1);
		else
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage2);
	}
	if (g_A4BossHealth != g_A4BossHealthSeen) {
		if (g_A4BossHealth <= 0x10 && g_A4BossHealthSeen > 0x10) {
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage3);
			g_A4BossMaxSpeed += 0x80;
		} else if (g_A4BossHealth <= 0x18 && g_A4BossHealthSeen > 0x18) {
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage4);
			g_A4BossMaxSpeed += 0x80;
		} else if (g_A4BossHealth <= 0x20 && g_A4BossHealthSeen > 0x20) {
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage5);
			g_A4BossMaxSpeed += 0x80;
		} else if (g_A4BossHealth <= 0x28 && g_A4BossHealthSeen > 0x28) {
			pal = (s16 *)CODEPTR(Pal_SavageA4BossStage6);
			g_A4BossMaxSpeed += 0x80;
		}
	}
	if (pal) {
		dst = &g_PalBase[56];
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		*dst++ = *pal++;
		g_PalBaseDirty = 1;
	}
	g_A4BossHealthSeen = g_A4BossHealth;
}
