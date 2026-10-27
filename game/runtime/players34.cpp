/* players34.cpp - players 3 and 4's context (phase 7 R5/R7, XMen2Native only; xmen2_players34.h).
 *
 * While a player 3 or 4 runs (its timer, its draw, its Init: the game's native branches call
 * P34_Enter / P34_Leave around them) player 2's globals hold that player's state: the pads, the
 * character, the meter, the VRAM area, the carried health and lives, the start record, the water
 * splash. Its object has pad 1 and isn't g_Player1, so the game's player code takes player 2's
 * side everywhere and finds that player's own. A swap both ways, so Leave is Enter again; the two
 * always pair up inside one call of the game's, never across a sync point.
 */
#include "xmen2.h"
#include "xmen2_players34.h"

extern "C" {
uint8_t xm_region[0x10000];
}

template <class T> static inline void swap_mem(T &a, T &b)
{
	T t = a;
	a = b;
	b = t;
}

/* players 3-4's objects, and the objects one of them made (its shots, its cards: P34_TagNew), which
   draw and run in its context too (their tiles are in its VRAM area) */
extern "C" int P34_Index(const void *obj)
{
	if (obj == (const void *)&g_Player3) return 3;
	if (obj == (const void *)&g_Player4) return 4;
	if (g_opt.players > 2 && g_ObjStorage) {
		uintptr_t u = (uintptr_t)obj, s = (uintptr_t)(void *)g_ObjStorage;
		uintptr_t n = (uintptr_t)(g_ObjCount - 2) * 0xC2u;
		if (u - s < n && (u - s) % 0xC2u == 0) return g_P34Owner[(u - s) / 0xC2u + 2];
	}
	return 0;
}

extern "C" void P34_TagNew(Actor *obj)
{
	g_P34Owner[obj->index] = (u8)g_P34Active;
}

static void swap_in(PlayerX *x)
{
	swap_mem(g_Pad2Held, x->padHeld);
	swap_mem(g_Pad2Accum, x->padAccum);
	swap_mem(g_Pad2Latch, x->padLatch);
	swap_mem(g_Pad2Released, x->padReleased);
	swap_mem(g_Pad2Pressed, x->padPressed);
	swap_mem(g_Pad2Raw, x->padRaw);
	swap_mem(g_P2CharIndex, x->charIndex);
	swap_mem(g_P2HasMeter, x->hasMeter);
	swap_mem(g_P2VramTile, x->vramTile);
	swap_mem(g_P2Health, x->health);
	swap_mem(g_P2Lives, x->lives);
	swap_mem(g_P2StartRec, x->startRec);
	swap_mem(g_P2SplashTimer, x->splashTimer);
	swap_mem(g_P2SplashY, x->splashY);
	swap_mem(g_P2SplashX, x->splashX);
	swap_mem(g_SprIndex, x->sprIndex);
	swap_mem(g_SprWritePtr, x->sprWritePtr);
	swap_mem(g_SprLast, x->sprLast);
}

/* Nested Enters of the same player (a hit while its timer runs) only count; another player's is a
   bug. Player k's VRAM area has player 2's tile numbers (the platform shows its sprites from its own
   tiles: plat_p34_enter), so its vramTile is player 2's whenever it enters. */
extern "C" void P34_Enter(int k)
{
	if (g_P34Active == k) { g_P34Depth = g_P34Depth + 1; return; }
	if (g_P34Active) xm::fatal("P34_Enter(%d) inside player %d's", k, (int)g_P34Active);
	g_PX[k - 3].vramTile = g_P2VramTile;
	swap_in(&g_PX[k - 3]);
	g_P34Active = (short)k;
	g_P34Depth = 1;
	plat_p34_enter(k);
}

extern "C" void P34_Leave(int k)
{
	if (g_P34Active != k) xm::fatal("P34_Leave(%d) without its Enter (%d)", k, (int)g_P34Active);
	g_P34Depth = g_P34Depth - 1;
	if (g_P34Depth) return;
	plat_p34_enter(0);
	g_P34Active = 0;
	swap_in(&g_PX[k - 3]);
}

/* ---- the player select ------------------------------------------------------------------------- */

extern "C" int P34_CharFree(short c)
{
	if (!g_P34SelectOn) return 1;
	for (int k = 0; k < P34_COUNT; k++)
		if (g_P34Slot[k].charIndex == c) return 0;
	return 1;
}

extern "C" PlayerX *P34_SlotX(const BigSprite *slot)
{
	return slot->player >= 3 && slot->player <= 4 ? &g_PX[slot->player - 3] : nullptr;
}

/* As the portrait's flight ends (PortraitActor_StateSwapChar / _StateFirstChar): the character asked
   for is the slot's, the cursor on it, input free again */
extern "C" void P34_SlotChanged(BigSprite *slot)
{
	if (!slot->changing) return;
	PlayerSelect_RevertCursor(slot);
	slot->busy = 0;
}

/* PlayerSelect_Run's frame: "LIVES n" for each player 3-4 with a character, as players 1 and 2's
   (font banks: player 3 white as player 1, player 4 blue as player 2), 16 px above them */
extern "C" void P34_SelectLives(void)
{
	for (int k = 0; k < P34_COUNT; k++) {
		if (g_P34Slot[k].charIndex == -1) continue;
		short lives = g_PX[k].lives, sx = k ? 0xCA : 0x42, y = 0xB6, bank = k ? 0x6680 : 0x66C0;
		auto put = [&](int ch) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = 0;
			g_SprWritePtr->attr = (short)(ch - 0x20 + bank);
			g_SprWritePtr->x = sx + 0x80;
			sx += 8;
		};
		if (lives > 9) sx -= 4;
		for (const char *s = "LIVES "; *s; s++) put(*s);
		if (lives > 9) put('0' + (lives / 10 > 9 ? 9 : lives / 10));
		put('0' + lives % 10);
	}
}

extern "C" struct Class *P34_ClassOf(short c)
{
	switch (c) {
	case 0: return CLASS(Tbl_Beast_vtable);
	case 1: return CLASS(Tbl_Cyclops_vtable);
	case 2: return CLASS(Tbl_Gambit_vtable);
	case 3: return CLASS(Tbl_Nightcrawler_vtable);
	case 4: return CLASS(Tbl_Psylocke_vtable);
	case 5: return CLASS(Tbl_Wolverine_vtable);
	case 6: return CLASS(Tbl_Magneto_vtable);
	default: return nullptr;
	}
}

/* ---- in a level --------------------------------------------------------------------------------- */

extern "C" short P34_AliveCount(void)
{
	short n = 0;
	for (int k = 3; k <= 2 + P34_COUNT; k++) n += IS_ALIVE(PLAYER34(k)) ? 1 : 0;
	return n;
}

/* the music (R9): player k's (3, 4) voice for the second sound machine's $B/$C (platform/native/
   sound2.cpp), and Music_SetPlayerParams's $A with them: the players in, at most 2 */
extern "C" int P34_MusicVoice(int k)
{
	return g_opt.players >= k ? (int)Player_MusicVoice(PLAYER34(k)) : 0;
}

extern "C" short P34_MusicCount(short n)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++)
		if (P34_MusicVoice(k)) n++;
	return n > 2 ? 2 : n;
}

/* SpaceA1_CamUpdate's lift crashing: players 3-4 in play lose their health too */
extern "C" void P34_KillAll(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++)
		if (IS_ALIVE(PLAYER34(k))) PLAYER34(k)->health = 0;
}

/* CloneA5_CamUpdate's arena scroll: players 3-4 pushed with it, as it pushes players 1-2 */
extern "C" void P34_ScrollPush(short step, short limit)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		XPlayer *p = PLAYER34(k);
		if (((IS_ALIVE(p) && !(p->flags & 0x1000))
		     || (p->cls == CLASS(Tbl_Magneto_vtable) && p->anim == CODEPTR(Tbl_MagnetoAnim)))
		    && p->boxRight < limit) {
			p->segPos -= (long)step << 8;
			p->x -= (long)step << 8;
			Obj_UpdateBoxes(p);
		}
	}
}

extern "C" int P34_AnyHealth(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++)
		if (PLAYER34(k)->health > 0) return 1;
	return 0;
}

extern "C" int P34_StartPressed(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++)
		if (g_PX[k - 3].padPressed & 0x80) return k;
	return 0;
}

/* Player2_StartLife for player k: only in the SiberiaB1 opening; its character kept, or one at random
   that no other player has; health $700, the character set (Player_SetCharacter, in its context), a
   life spent */
extern "C" char P34_StartLife(int k)
{
	XPlayer *self = PLAYER34(k);
	short me = Player_CharIndexOf(self->cls);
	if (!g_SiberiaIntro) return 0;
	if (me == -1) {
		short taken[4];
		int n = 0;
		taken[n++] = Player_CharIndexOf(g_Player1.cls);
		taken[n++] = Player_CharIndexOf(g_Player2.cls);
		for (int j = 3; j <= 2 + P34_COUNT; j++)
			if (j != k) taken[n++] = Player_CharIndexOf(PLAYER34(j)->cls);
		bool clash;
		do {
			me = (unsigned short)Rand_Next() % 6;
			clash = false;
			for (int i = 0; i < n; i++) clash |= taken[i] == me;
		} while (clash);
	}
	g_PX[k - 3].health = 0x700;
	P34_Enter(k);
	Player_SetCharacter(P34_ClassOf(me), self);
	P34_Leave(k);
	g_PX[k - 3].charIndex = (s8)me;
	LOSE_LIFE(g_PX[k - 3].lives);
	return 1;
}

/* Player2_StartLife's random pick with players 3-4: its draw (already not other, player 1's) drawn
   again, in the same Rand_Next sequence, while it is also a player 3-4's character. pads 3-4 may join
   the SiberiaB1 opening before pad 2 does. */
extern "C" short P34_Player2Pick(short me, short other)
{
	for (;;) {
		bool clash = me == other;
		for (int j = 3; j <= 2 + P34_COUNT; j++) clash |= Player_CharIndexOf(PLAYER34(j)->cls) == me;
		if (!clash) return me;
		me = (unsigned short)Rand_Next() % 6;
	}
}

/* ---- the group (the camera, the tether: R7 "the camera keeps the group in view, players are held at
   the screen edges as in two-player") ------------------------------------------------------------- */

static XPlayer *player_n(int n) { return n == 1 ? &g_Player1 : n == 2 ? &g_Player2 : PLAYER34(n); }
extern "C" XPlayer *P34_Player(int n) { return player_n(n); }

extern "C" int P34_OthersAlive(const void *self, XPlayer **list)
{
	int c = 0;
	for (int n = 1; n <= g_opt.players; n++) {
		XPlayer *p = player_n(n);
		if ((const void *)p != self && IS_ALIVE(p)) list[c++] = p;
	}
	return c;
}

extern "C" XPlayer *P34_FirstAlive(void)
{
	for (int n = 1; n <= g_opt.players; n++)
		if (IS_ALIVE(player_n(n))) return player_n(n);
	return nullptr;
}

/* the player in play nearest to (x, y) (8 fraction bits) by Manhattan distance, on ties the later
   one (as Player_Nearest's player 2); 0 when none is */
extern "C" XPlayer *P34_Nearest(long x, long y)
{
	XPlayer *best = nullptr;
	long bd = 0;
	for (int n = 1; n <= g_opt.players; n++) {
		XPlayer *p = player_n(n);
		if (!IS_ALIVE(p)) continue;
		long dx = p->x - x, dy = p->y - y, d = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
		if (!best || d <= bd) { best = p; bd = d; }
	}
	return best;
}

/* ---- R11: what the game takes from players 1-2 by name (enemies' targets, layers) ------------------ */

extern "C" XPlayer *P34_LayerPlayer(void)
{
	if (IS_ALIVE(&g_Player1)) return &g_Player1;
	if (IS_ALIVE(&g_Player2)) return &g_Player2;
	for (int n = 3; n <= g_opt.players; n++)
		if (IS_ALIVE(player_n(n))) return player_n(n);
	return &g_Player2;
}

/* A new object takes the live player's sprite priority (bit 2: behind the foreground), player 1's
   when neither 1 nor 2 is in play; with a player 3-4 in play it takes theirs. clear: also clear the
   bit when theirs is clear (the inits that only ever set it pass 0). 1 when it did. */
extern "C" int P34_CopyLiveLayer(SprInst *spr, int clear)
{
	for (int n = 3; n <= g_opt.players; n++) {
		XPlayer *p = player_n(n);
		if (!IS_ALIVE(p)) continue;
		if (p->spr.flags & 4) spr->flags |= 4;
		else if (clear) spr->flags &= ~4;
		return 1;
	}
	return 0;
}

/* A stage's hook that moves the players in front of or behind its foreground: players 3-4 in play
   too (behind: bit 2 set, else cleared; drawList: their draw node's priority as well) */
extern "C" void P34_SetLayer(int behind, int drawList)
{
	for (int n = 3; n <= g_opt.players; n++) {
		XPlayer *p = player_n(n);
		if (!IS_ALIVE(p)) continue;
		if (behind) p->spr.flags |= 4;
		else p->spr.flags &= ~4;
		if (drawList) DrawList_SetPriority(p->drawNode, behind ? 1 : 0, p->layer, p->flags6 & 8);
	}
}

extern "C" int P34_Extents(short *x1, short *x2, short *y1, short *y2)
{
	int c = 0;
	for (int n = 1; n <= g_opt.players; n++) {
		XPlayer *p = player_n(n);
		if (!IS_ALIVE(p)) continue;
		short x = (short)(p->x >> 8), y = (short)(p->y >> 8);
		if (!c++) { *x1 = *x2 = x; *y1 = *y2 = y; continue; }
		if (x < *x1) *x1 = x;
		if (x > *x2) *x2 = x;
		if (y < *y1) *y1 = y;
		if (y > *y2) *y2 = y;
	}
	return c;
}

/* ---- video: players 3-4's sprite tables and HUD ------------------------------------------------------ */

/* Level_FrameLoop's Spr_BeginFrame / Spr_EndFrame, for each player 3-4's table (linked once, as
   Spr_InitLinks does the main one) */
extern "C" void P34_SprBeginFrame(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		P34_Enter(k);
		if (g_SpriteTable[81].link != 0 || g_SpriteTable[80].link != 80)
			Spr_InitLinks();
		Spr_BeginFrame();
		P34_Leave(k);
	}
}

extern "C" void P34_SprEndFrame(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		P34_Enter(k);
		Spr_EndFrame();
		P34_Leave(k);
	}
}

/* the next slot of the (current) sprite table along the link bytes, filled: Level_FrameLoop's HUD_SPRITE */
static void hud_sprite(short y, short size, short attr, short x)
{
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = y;
	g_SprWritePtr->size = (u8)size;
	g_SprWritePtr->attr = attr;
	g_SprWritePtr->x = x;
}

/* With three or more players the HUD is in the players' order, 1-2-3-4 from the left (the user,
   2026-10-27): player 2's panel moves to where player 3's was, 64 px in from player 1's, drawn as
   player 1's is (its strips left of it, mirrored: the strip art is symmetric) */
extern "C" void P34_HudP2(void)
{
	hud_sprite(0x132, 0xF, (short)0xA7D0, HUD_L(0xA5 + 0x40));
	if (g_P2HasMeter)
		hud_sprite(0x133, 7, (short)0xAFE0, HUD_L(0x8F + 0x40));
	else
		hud_sprite(0x133, 3, (short)0xAFE0, HUD_L(0x97 + 0x40));
}

/* Players 3-4's panels as Level_FrameLoop draws player 2's, player 3's 64 px in from where player 2's
   was, player 4's there, in their own tables: tiles $7D0- (player 2's HUD area), which the platform
   shows from each one's HUD tiles (plat_p34_hud). Every player's character goes to the platform first:
   all four panels are their portraits. */
extern "C" void P34_HudSprites(void)
{
	plat_hud_char(1, IS_ALIVE(&g_Player1) ? Player_CharIndexOf(g_Player1.cls) : -1);
	plat_hud_char(2, IS_ALIVE(&g_Player2) ? Player_CharIndexOf(g_Player2.cls) : -1);
	for (int k = 3; k <= 4; k++)
		plat_hud_char(k, k <= 2 + P34_COUNT && IS_ALIVE(PLAYER34(k)) ? Player_CharIndexOf(PLAYER34(k)->cls) : -1);
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		XPlayer *p = PLAYER34(k);
		if (!IS_ALIVE(p)) continue;
		P34_Enter(k);
		plat_p34_hud(k, p->health, p->charge, g_P2HasMeter, g_FrameCounter);
		short in = k == 3 ? 0x40 : 0;
		hud_sprite(0x132, 0xF, (short)0xA7D0, HUD_R(0x17A - in));
		hud_sprite(0x133, g_P2HasMeter ? 7 : 3, (short)0xA7E0, HUD_R(0x1A1 - in));
		P34_Leave(k);
	}
}

/* ---- the game's loop -------------------------------------------------------------------------- */

extern "C" void P34_ResetPlayers(void)
{
	for (int k = 3; k <= 4; k++) {
		PlayerX *x = &g_PX[k - 3];
		XPlayer *p = PLAYER34(k);
		x->health = 0;
		x->charIndex = -1;
		p->cls = 0;
		p->flags = 0;
		x->lives = START_LIVES;
	}
	g_P34InPlay = 1;	/* the region holds players 3-4 now (sound2.cpp: who is in the game) */
}

extern "C" void P34_BeforeSelect(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		PlayerX *x = &g_PX[k - 3];
		XPlayer *p = PLAYER34(k);
		x->charIndex = x->health ? (s8)Player_CharIndexOf(p->cls) : (s8)-1;
		p->flags = 0;
	}
}

extern "C" void P34_AfterCerebro(void)
{
	for (int k = 3; k <= 2 + P34_COUNT; k++) PLAYER34(k)->health = g_PX[k - 3].health;
}

extern "C" char P34_AfterScene(char *withHealth)
{
	char up = 0;
	*withHealth = 0;
	for (int k = 3; k <= 2 + P34_COUNT; k++) {
		PlayerX *x = &g_PX[k - 3];
		XPlayer *p = PLAYER34(k);
		if (x->charIndex != -1)
			x->health = p->health;
		char out = x->health > 0 ? 0 : 1;
		if (out && x->charIndex != -1) {
			if (x->lives) {
				LOSE_LIFE(x->lives);
				x->health = 0x700;
			} else {
				x->health = 0;
				x->charIndex = -1;
				p->cls = 0;
				p->flags = 0;
			}
		}
		if (!out) up = 1;
		if (x->health) *withHealth = 1;
	}
	return up;
}
