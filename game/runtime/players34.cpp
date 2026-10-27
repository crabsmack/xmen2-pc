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

/* The player select with three or more players (the user, 2026-10-27): four fixed columns, players
   1-4 from the left as the HUD is, each with its character's portrait flying in and out as players 1-2's
   do (players 3-4's slots use players 1-2's character areas, unused on a Cerebro screen: the
   portrait's tiles are g_PlayerTiles at most), "PRESS"/"START" under it, "READY" once confirmed, the
   cursor boxes with the player's digit on its side of the screen, players 3-4 in light blue and
   yellow (palette line 2, black and unused on a Cerebro screen), and the lives in the frame's two
   boxes: "L n" for players 1-2 in the left one, 3-4 in the right. */
#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))
#define TEXT_CELL(row, col)	(((row) << 7) + 0xE000 + ((col) << 1))	/* Cerebro_PrintText's plane */
/* players 2-4's cursor boxes, 16 tiles each, at VRAM $F800-$FDFF: past the sprite table ($F000) and the
   line scroll table ($F400, 224 lines), the HUD's in a level, unused on a Cerebro screen */
#define BOX_TILE	0x7C0

static const BigSprite *select_slot(int p)
{
	return p == 1 ? (const BigSprite *)g_P1Slot : p == 2 ? (const BigSprite *)g_P2Slot : &g_P34Slot[p - 3];
}

/* PlayerSelect_PlayIdleAnim with the select_idles option: the draw XPlayer_StateStand makes when an
   animation ends (Rand_Next above the character's idleChance plays its idle animation), here on the
   select screen; Gambit's with GAMBIT_IDLE_CHANCE (xmen2_players34.h). 1: the idle is playing; 0: the
   caller plays the standing pose. */
extern "C" int P34_SelectIdle(ScalingActor *self)
{
	static void *const rec[7] = { CODEPTR(Tbl_BeastCharRecord), CODEPTR(Tbl_CyclopsCharRecord),
		CODEPTR(Tbl_GambitCharRecord), CODEPTR(Tbl_NightcrawlerCharRecord), CODEPTR(Tbl_PsylockeCharRecord),
		CODEPTR(Tbl_WolverineCharRecord), CODEPTR(Tbl_MagnetoCharRecord) };
	int c = self->slot->charIndex;
	if (c < 0 || c > 6)
		return 0;
	CharRecord *r = (CharRecord *)rec[c];
	unsigned short chance = c == 2 ? GAMBIT_IDLE_CHANCE : (unsigned short)r->idleChance;
	if ((unsigned short)Rand_Next() <= chance)
		return 0;
	Obj_SetAnim(self, CHAR_PART(r, idleAnim));
	return 1;
}

/* PortraitActor_StateInit: where player p's portrait stands, the middle of its column */
extern "C" short P34_SelectHomeX(short p)
{
	static const unsigned char x[4] = { 0x44, 0x82, 0xBF, 0xFD };
	return x[p - 1];
}

/* CursorInput_StatePoll / PortraitActor_StateWaitTurn: a player 3-4's input or flight is being carried
   out (players 1-2's slots are tested beside it) */
extern "C" char P34_AnyBusy(void)
{
	for (int k = 0; k < P34_COUNT; k++)
		if (g_P34Slot[k].busy) return 1;
	return 0;
}

/* CursorInput_StatePoll: a Start of player p (this frame's press, or one kept from a frame where a slot
   was busy) to act on now. With four players a portrait is flying much of the time, and the original
   drops a Start pressed meanwhile: here it waits instead, so a join or a confirm is never lost. */
extern "C" char P34_TakeStart(const BigSprite *slot, int pressed, char busy)
{
	int i = slot->player - 1;
	if (slot->confirmed) {
		g_P34StartWait[i] = 0;
		return 0;
	}
	if (pressed)
		g_P34StartWait[i] = 1;
	if (busy || !g_P34StartWait[i])
		return 0;
	g_P34StartWait[i] = 0;
	return 1;
}

/* Line 2 from line 3's (Cerebro_AnimatePalettes' glow on colours 8-11, players 1-2's box borders and
   "PRESS START"; 12-15 fixed): player 3's in light blue at the same brightness as player 1's green,
   player 4's in yellow as player 2's red. Colour 1 black (the outlines). */
static unsigned short blue_of(unsigned short green)
{
	int g = (green >> 5) & 7;
	return (unsigned short)(7 << 9 | (g > 1 ? g - 1 : 0) << 5 | (g > 3 ? g - 3 : 0) << 1);
}

static unsigned short yellow_of(unsigned short red)
{
	int r = (red >> 1) & 7;
	return (unsigned short)((r < 6 ? r + 1 : 7) << 5 | 7 << 1);
}

static void select_palette(void)
{
	unsigned short want[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
		yellow_of(g_PalBase[0x38]), yellow_of(g_PalBase[0x39]), blue_of(g_PalBase[0x3A]), blue_of(g_PalBase[0x3B]),
		yellow_of(0x0A), yellow_of(0x0C), blue_of(0xA0), blue_of(0xC0) };
	for (int i = 0; i < 16; i++)
		if (g_PalBase[0x20 + i] != (short)want[i]) {
			g_PalBase[0x20 + i] = (short)want[i];
			g_PalBaseDirty = 1;
		}
}

/* The cursor boxes (Tbl_CerebroVdp frames 7 and 8: player 1's green, digit "1" left; player 2's red,
   "2" right) for players 2-4 at BOX_TILE: player 2's with its digit moved left, player 3's from player
   1's border with a "3" right, player 4's from player 2's with a "4" right (the game's font,
   Art_RegionFont, outlined); 3-4's colours are line 2's. */
static void box_art(int frame, unsigned char px[32][32])
{
	const unsigned char *def = (const unsigned char *)CODEPTR(Tbl_CerebroVdp);
	const unsigned char *fr = def + 0x24 + 0x14 * frame;
	const unsigned char *piece = def + (fr[0x12] << 8 | fr[0x13]);
	const unsigned char *art = def + (piece[6] << 8 | piece[7]);
	for (int t = 0; t < 16; t++)
		for (int y = 0; y < 8; y++)
			for (int x = 0; x < 8; x++) {
				unsigned char b = art[t * 32 + y * 4 + x / 2];
				px[(t % 4) * 8 + y][(t / 4) * 8 + x] = (unsigned char)(x % 2 ? b & 15 : b >> 4);
			}
}

struct Box { int x0, y0, x1, y1; };

static Box digit_of(unsigned char px[32][32])	/* the digit: what is inside the 2-pixel border */
{
	Box b = { 32, 32, -1, -1 };
	for (int y = 2; y < 30; y++)
		for (int x = 2; x < 30; x++)
			if (px[y][x]) {
				b.x0 = x < b.x0 ? x : b.x0, b.x1 = x > b.x1 ? x : b.x1;
				b.y0 = y < b.y0 ? y : b.y0, b.y1 = y > b.y1 ? y : b.y1;
			}
	return b;
}

static void clear_digit(unsigned char px[32][32])
{
	for (int y = 2; y < 30; y++)
		for (int x = 2; x < 30; x++) px[y][x] = 0;
}

static void draw_digit(unsigned char px[32][32], char ch, int right, int bottom, unsigned char colour)
{
	const unsigned char *g = (const unsigned char *)CODEPTR(Art_RegionFont) + (ch - 0x20) * 8;
	bool on[10][10] = {};
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			if (g[y] & (0x80 >> x)) on[y + 1][x + 1] = true;
	int gx1 = 0;
	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 10; x++)
			if (on[y][x] && x > gx1) gx1 = x;
	int ox = right - (gx1 + 1), oy = bottom - 9;	/* its outline's right column at `right` */
	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 10; x++) {
			bool edge = false;
			for (int dy = -1; dy <= 1; dy++)
				for (int dx = -1; dx <= 1; dx++) {
					int yy = y + dy, xx = x + dx;
					edge |= yy >= 0 && yy < 10 && xx >= 0 && xx < 10 && on[yy][xx];
				}
			if (on[y][x]) px[oy + y][ox + x] = colour;
			else if (edge) px[oy + y][ox + x] = 1;
		}
}

static void make_boxes(void)
{
	unsigned char p1[32][32], p2[32][32], box[3][32][32];
	box_art(7, p1);
	box_art(8, p2);
	Box d1 = digit_of(p1), d2 = digit_of(p2);
	memcpy(box[0], p2, sizeof p2);	/* player 2: its "2" where player 1's "1" is */
	clear_digit(box[0]);
	for (int y = d2.y0; y <= d2.y1; y++)
		for (int x = d2.x0; x <= d2.x1; x++) box[0][y][d1.x0 + x - d2.x0] = p2[y][x];
	memcpy(box[1], p1, sizeof p1);	/* player 3: "3" right, in player 1's digit colour ($F) */
	clear_digit(box[1]);
	draw_digit(box[1], '3', d2.x1, d2.y1, 0xF);
	memcpy(box[2], p2, sizeof p2);	/* player 4: "4" right, in player 2's ($D) */
	clear_digit(box[2]);
	draw_digit(box[2], '4', d2.x1, d2.y1, 0xD);
	VDP_CTRL32 = VRAM_WRITE(BOX_TILE * 32);
	for (int b = 0; b < 3; b++)
		for (int t = 0; t < 16; t++)
			for (int y = 0; y < 8; y++)
				for (int x = 0; x < 8; x += 4) {
					const unsigned char *r = &box[b][(t % 4) * 8 + y][(t / 4) * 8 + x];
					VDP_DATA = (unsigned short)(r[0] << 12 | r[1] << 8 | r[2] << 4 | r[3]);
				}
}

static void put_text(short row, short col, const char *s, unsigned short base)
{
	VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(row, col));
	for (; *s; s++)
		VDP_DATA = (unsigned short)(*s + base - 0x20);
}

/* Player p's text under its column: "PRESS"/"START" in the font PlayerSelect_Run prints "PRESS START"
   in (players 1-2 line 3, as the original; 3-4 line 2), or "READY" (the original's "SELECTED", which
   four columns have no room for) in the one CursorInput_Confirm prints "SELECTED" in */
extern "C" void P34_SelectText(short p, char ready)
{
	static const short col[4] = { 6, 14, 21, 29 };
	static const unsigned short press[4] = { 0xE640, 0xE600, 0xC640, 0xC600 };
	static const unsigned short sel[4] = { 0xE6C0, 0xE680, 0xC6C0, 0xC680 };
	short c = col[p - 1];
	if (ready) {
		put_text(0x15, c, "READY", sel[p - 1]);
		put_text(0x16, c, "     ", sel[p - 1]);
	} else {
		put_text(0x15, c, "PRESS", press[p - 1]);
		put_text(0x16, c, "START", press[p - 1]);
	}
}

/* PlayerSelect_Run with three or more players, once players 1-2's slots are made: line 2, the boxes,
   players 3-4's slots (a cursor and a portrait each, as players 1-2's: the portrait's tiles in player
   1's and 2's character areas), and "PRESS"/"START" under each player who has lives or a character */
extern "C" void P34_SelectStart(short tiles)
{
	if (tiles > g_PlayerTiles)
		xm::fatal("PlayerSelect_Run: a portrait takes %d tiles, players 1-2's areas have %d", tiles, (int)g_PlayerTiles);
	select_palette();
	make_boxes();
	for (int i = 0; i < 4; i++)
		g_P34StartWait[i] = 0;
	for (int k = 0; k < P34_COUNT; k++) {
		BigSprite *s = &g_P34Slot[k];
		s->player = (u8)(3 + k);
		s->charIndex = g_PX[k].charIndex;
		s->savedChar = -1;
		s->frameSpr = (u8)(7 + k);
		s->busy = 0;
		s->hidden = 0;
		s->confirmed = 0;
		s->big = 0;
		s->changing = 0;
		s->moveDelay = 0;
		s->params = g_ScaleParamsPtr;
		s->tiles = tiles;
		s->tileBase = k ? g_P2VramTile : g_P1VramTile;
		s->pal = 1;
		PlayerSelect_PlaceCursor(s);
		(*CLASS(Tbl_BoxInputActor_vtable)->method[2])(0, CLASS(Tbl_BoxInputActor_vtable), 0, 0, s);
		(*CLASS(Tbl_ScalingActor_vtable)->method[2])(0, CLASS(Tbl_ScalingActor_vtable), 0, 0, s);
	}
	for (int p = 1; p <= 2 + P34_COUNT; p++) {
		short lives = p == 1 ? g_P1Lives : p == 2 ? g_P2Lives : g_PX[p - 3].lives;
		if (lives || select_slot(p)->charIndex != -1)
			P34_SelectText((short)p, 0);
	}
	g_P34SelectOn = 1;
}

/* PlayerSelect_Run's frame, after the frame hook: line 2 follows line 3's glow (not while a fade runs) */
extern "C" void P34_SelectFrame(void)
{
	if (!g_PalFadeActive)
		select_palette();
}

/* BoxInputActor_Draw: player 2-4's cursor box (as PlayerSelect_DrawFrame draws player 1's: 2 px up, 4
   left of the portrait frame, in front) */
extern "C" void P34_DrawBox(const BigSprite *slot)
{
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = slot->cursorY + 0x7E;
	g_SprWritePtr->size = 0xF;
	g_SprWritePtr->attr = (short)(0x8000 | (slot->player == 2 ? 3 : 2) << 13 | (BOX_TILE + 16 * (slot->player - 2)));
	g_SprWritePtr->x = slot->cursorX + 0x7C;
}

/* PlayerSelect_Run's frame: the lives of each player with a character, "L n" (at most 99, as the
   original's "LIVES n") in sprites in the frame's two boxes, each player in its half (players 1-2 the
   left box, 3-4 the right) and in its font: players 1-2's as the original's, 3-4's the same banks in
   line 2 */
extern "C" void P34_SelectLives(void)
{
	static const short mid[4] = { 74, 114, 209, 249 };
	static const unsigned short bank[4] = { 0x66C0, 0x6680, 0x46C0, 0x4680 };
	for (int p = 1; p <= 2 + P34_COUNT; p++) {
		if (select_slot(p)->charIndex == -1) continue;
		short lives = p == 1 ? g_P1Lives : p == 2 ? g_P2Lives : g_PX[p - 3].lives;
		int digits = lives > 9 ? 2 : 1;
		short sx = (short)(mid[p - 1] - 4 * (2 + digits));
		auto put = [&](int ch) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = 0xC6 + 0x80;
			g_SprWritePtr->size = 0;
			g_SprWritePtr->attr = (short)(ch - 0x20 + bank[p - 1]);
			g_SprWritePtr->x = sx + 0x80;
			sx += 8;
		};
		put('L');
		sx += 8;
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
