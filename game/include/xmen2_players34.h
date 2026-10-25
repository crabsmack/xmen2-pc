/* xmen2_players34.h - players 3 and 4 (phase 7, NATIVE only: xmen2disasm docs/phases/P7-multiplayer.md
 * R5, R7): their objects and state in the region of their own (xmen2_mem.h xm_region, $00FExxxx),
 * used only with three or more players (g_opt.players).
 *
 * Players 3 and 4 run the game's own player code as player 2 does: their objects have pad 1 and
 * aren't g_Player1, so every `self == &g_Player1 ? <player 1's> : <player 2's>` choice takes player
 * 2's side, and while their code runs (P34_Enter/P34_Leave, game/runtime/players34.cpp: their timer,
 * their draw, their Init) player 2's globals hold theirs (PlayerX below, field for field the globals
 * it swaps with), so that side is theirs. */
#ifndef XMEN2_PLAYERS34_H
#define XMEN2_PLAYERS34_H
#if defined(XMEN2_NATIVE)

/* What player 2 has in work RAM, for one more player (the order P34_Enter swaps them in) */
typedef struct PlayerX {
	s16 padHeld, padAccum, padLatch, padReleased, padPressed, padRaw;	/* g_Pad2* */
	s8 charIndex;			/* g_P2CharIndex */
	u8 hasMeter;			/* g_P2HasMeter */
	s16 vramTile;			/* g_P2VramTile */
	s16 health;			/* g_P2Health */
	s16 lives;			/* g_P2Lives */
	PTR(Placement) startRec;	/* g_P2StartRec */
	s16 splashTimer, splashY, splashX;	/* g_P2Splash* */
	s16 sprIndex;			/* g_SprIndex: its own sprite table's (g_SpriteTable below) */
	PTR(HwSprite) sprWritePtr;	/* g_SprWritePtr */
	s16 sprLast;			/* g_SprLast */
	s16 pad0;
} PlayerX;

#define XM_RG(type, off)	(*(type *)(xm_region + (off)))
#define XM_RGA(type, off)	((type *)(xm_region + (off)))

#define g_Player3		XM_RG(XPlayer, 0x0000)	/* the objects, $F0 bytes as players 1-2 */
#define g_Player4		XM_RG(XPlayer, 0x0100)
#define g_PX			XM_RGA(PlayerX, 0x0200)	/* [0] player 3, [1] player 4 */
#define g_P34Active		XM_RG(s16, 0x0280)	/* the player whose code runs now (3, 4), else 0 */
#define g_P34InPlay		XM_RG(s16, 0x0282)	/* 1 once the game has players 3-4 (g_opt.players > 2) */
#define g_P34SelectOn		XM_RG(s16, 0x0284)	/* PlayerSelect_Run is running with players 3-4's slots */
#define g_P34Depth		XM_RG(s16, 0x0286)	/* nested Enters of g_P34Active */
#define g_P34ObjIndex		XM_RGA(s16, 0x0288)	/* [0], [1]: players 3 and 4's g_ObjTable slots (Obj_AllocPool) */
#define g_P34BeamTable		XM_RG(s16, 0x028C)	/* Cyclops' beam went into player 3-4's table (3, 4), else 0 */
#define g_P34A4Hit		XM_RGA(s32, 0x0290)	/* [0], [1]: SavageBigBio's tentacle hit point on players 3-4 (g_A4Hit1/2's) */
#define g_P34Slot		XM_RGA(BigSprite, 0x0300)	/* [0] player 3's select slot, [1] player 4's ($42 bytes each) */
#define P34_SPR_TABLE(k)	(0x0400 + 0x300 * ((k) - 3))	/* player k's sprite table: 82 hardware sprites */
#define P34_SPR_MAIN		0x1000	/* the main sprite table with three or more players: 162 (SPR_STOP) */
#define g_P34Owner		XM_RGA(u8, 0x0A00)	/* by g_ObjTable slot: the player 3-4 that made the object, else 0 */

#define PLAYER34(k)		((k) == 3 ? &g_Player3 : &g_Player4)
#define P34_COUNT		(g_opt.players - 2)	/* players 3-4 in this game: 0, 1 or 2 */

/* While a player 3-4's code runs, the sprite table is its own (as g_SprIndex, g_SprWritePtr and
   g_SprLast: P34_Enter): its sprites never take the 80 of players 1-2 and the rest, and the platform
   draws them from its own tiles (platform/native/players.cpp). The game's own sprite code, unchanged,
   fills it. */
#undef g_SpriteTable
#define g_SpriteTable		(g_P34Active ? XM_RGA(HwSprite, P34_SPR_TABLE(g_P34Active)) \
				 : g_opt.players > 2 ? XM_RGA(HwSprite, P34_SPR_MAIN) : XM_GA(HwSprite, 0xFFE226))

#ifdef __cplusplus
extern "C" {
#endif
/* game/runtime/players34.cpp */
int  P34_Index(const void *obj);	/* 3 or 4 for players 3-4's objects, else 0 */
void P34_Enter(int k);			/* player k's (3, 4) state into player 2's globals */
void P34_Leave(int k);			/* and back */
/* the player select (PlayerSelect_Run and its cursors) */
int  P34_CharFree(short c);		/* no player 3-4 slot has character c */
PlayerX *P34_SlotX(const BigSprite *slot);	/* the PlayerX of a player 3-4 slot, else null */
void P34_SlotChanged(BigSprite *slot);	/* a 3-4 slot's change applies at once (they have no portrait) */
struct Class *P34_ClassOf(short c);	/* the character's class (as PlayerSelect_Run's cases), 0 for -1 */
void P34_SelectLives(void);		/* PlayerSelect_Run's frame: their "LIVES n" */
/* the game's loop (Game_ResetPlayers, Cerebro_Run, Game_SceneLoop) */
void P34_ResetPlayers(void);
void P34_BeforeSelect(void);		/* each one's character from its class, or none without health */
void P34_AfterCerebro(void);		/* the carried health back into the objects */
char P34_AfterScene(char *withHealth);	/* health carried, a life spent or out of the game; 1: one is up */
/* in a level */
short P34_AliveCount(void);		/* players 3-4 present and alive */
int  P34_AnyHealth(void);		/* a player 3-4 has health */
void P34_KillAll(void);			/* SpaceA1's lift crash: theirs too */
int  P34_MusicVoice(int k);		/* R9: Player_MusicVoice of player k (3, 4) */
short P34_MusicCount(short n);		/* Music_SetPlayerParams's $A: n with players 3-4, at most 2 */
void P34_ScrollPush(short step, short limit);	/* CloneA5's arena scroll pushes them too */
int  P34_StartPressed(void);		/* the first player 3-4 whose Start was pressed this frame, else 0 */
char P34_StartLife(int k);		/* as Player2_StartLife (the SiberiaB1 opening only) */
short P34_Player2Pick(short me, short other);	/* Player2_StartLife's draw: redrawn while other's or a 3-4's */
/* the group (g_opt.players of them) */
int  P34_OthersAlive(const void *self, XPlayer **list);	/* the players in play but self (up to 3) */
XPlayer *P34_FirstAlive(void);
XPlayer *P34_Player(int n);		/* player n (1-4) */
XPlayer *P34_Nearest(long x, long y);	/* in play, Manhattan; on ties the later; 0: none */
int  P34_Extents(short *x1, short *x2, short *y1, short *y2);	/* pixels; how many in play */
/* video: their sprite tables, their HUD (Level_FrameLoop), the objects they make (Obj_FindFreeSlot) */
void P34_SprBeginFrame(void);
void P34_SprEndFrame(void);
void P34_HudSprites(void);		/* players 3-4's HUD panels, in their own tables */
void P34_TagNew(Actor *obj);		/* an object made now belongs to the player 3-4 whose code runs */
/* R11: what the game takes from players 1-2 by name, for players 3-4 too */
XPlayer *P34_LayerPlayer(void);		/* PLAYER_LIVE */
int  P34_CopyLiveLayer(SprInst *spr, int clear);	/* players 1-2 out: a live 3-4's priority bit; 0: none */
void P34_SetLayer(int behind, int drawList);	/* a stage's layer change for players 3-4 in play */
#ifdef __cplusplus
}
#endif

/* the game's `for (i = 0; i < 2; ++i) ... i == 0 ? (p = &g_Player1) : (p = &g_Player2)` loops over
   the players, with players 3-4 (what the original's loop is in the other modes, token for token).
   MPW C 3.3 can't take a parameter named `p` here: inside IS_ALIVE(GROUP_P(i, p)) its g_Player1 /
   g_Player2 come out undeclared (measured), hence n_ / v_. */
#define GROUP_N			g_opt.players
#define GROUP_P(n_, v_)		((v_) = P34_Player((int)(n_) + 1))
/* a hit or a pick-up on a player found by such a loop: on a player 3-4 it runs in that player's
   context, as a hit list's does (Collide_ObjVsList). The original's statement in the other modes. */
#define P34_HIT(v_, call_)	do { int k_ = P34_Index(v_); k_ = k_ && (!g_P34Active || g_P34Active == k_) ? k_ : 0; \
				     if (k_) P34_Enter(k_); call_; if (k_) P34_Leave(k_); } while (0)
/* the original's `IS_ALIVE(&g_Player1) ? &g_Player1 : &g_Player2` (whose layer a new object
   takes): player 1, else 2, else a live player 3-4 (R11), else player 2 */
#define PLAYER_LIVE		P34_LayerPlayer()

#ifdef __cplusplus
/* R11, where the game hits or tests players 1-2 one by one by name: the same for each player 3-4
   in play, in that player's context as a hit list's hit is (Collide_ObjVsList). Code already in a
   player 3-4's context (its own objects') calls the other one's as is (one context at a time). */
template <class F> static inline void P34_Each(F fn)
{
	for (int k = 3; k <= g_opt.players; k++) {
		int in = !g_P34Active || g_P34Active == k;
		if (in) P34_Enter(k);
		fn(P34_Player(k));
		if (in) P34_Leave(k);
	}
}
/* ... and a test: true when it holds for one of them (the rest aren't tested, as `a || b` stops) */
template <class F> static inline int P34_Any(F fn)
{
	for (int k = 3; k <= g_opt.players; k++) {
		int in = !g_P34Active || g_P34Active == k, r;
		if (in) P34_Enter(k);
		r = fn(P34_Player(k)) != 0;
		if (in) P34_Leave(k);
		if (r) return 1;
	}
	return 0;
}
/* every live player in play (1-4) passes test (no context: a test only); 1 with none live */
template <class F> static inline int P34_EveryLive(F test)
{
	for (int n = 1; n <= g_opt.players; n++) {
		XPlayer *q = P34_Player(n);
		if (IS_ALIVE(q) && !test(q))
			return 0;
	}
	return 1;
}
#endif
/* obj is player 3's or 4's object (not one they made) */
#define P34_IS_PLAYER(o_)	((const void *)(o_) == (const void *)&g_Player3 || (const void *)(o_) == (const void *)&g_Player4)

#else
#define GROUP_N			2
#define GROUP_P(n_, v_)		(n_ == 0 ? (v_ = &g_Player1) : (v_ = &g_Player2))
#define P34_HIT(v_, call_)	call_
#define PLAYER_LIVE		(IS_ALIVE(&g_Player1) ? &g_Player1 : &g_Player2)
#endif /* XMEN2_NATIVE */
#endif /* XMEN2_PLAYERS34_H */
