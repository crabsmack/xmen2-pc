/* controls.h - the players' controls (controls.cpp): which keys and controller inputs make each of
 * the Genesis pad's 12 buttons, which controller each player has, and the controllers themselves
 * (opened, given to players, unknown ones mapped). The launcher's Controls page edits it, xmen2.ini's
 * [player1]..[player4] keep it, host_sdl.cpp reads the pads through it. Only the inputs change: the
 * game sees the same pad bits it always did. */
#ifndef XMEN2_CONTROLS_H
#define XMEN2_CONTROLS_H

#include <SDL.h>
#include <stdint.h>
#include <string>
#include <vector>

/* CB_REWIND is not a pad button: the host's rewind (rewind.cpp), held; its bit is 0 */
enum CtlButton { CB_UP, CB_DOWN, CB_LEFT, CB_RIGHT, CB_A, CB_B, CB_C, CB_START, CB_X, CB_Y, CB_Z, CB_MODE, CB_REWIND, CB_COUNT };
enum { CTL_PLAYERS = 4, CTL_SLOTS = 2 };

/* a standard controller's input (SDL's game controller layout: positional, south = A) */
struct PadIn {
    enum { NONE, BUTTON, AXIS };
    int8_t kind = NONE;
    int8_t code = 0;   /* SDL_GameControllerButton or SDL_GameControllerAxis */
    int8_t dir = 0;    /* an axis: -1 or +1 */
    bool operator==(const PadIn &o) const { return kind == o.kind && (kind == NONE || (code == o.code && dir == o.dir)); }
    bool operator!=(const PadIn &o) const { return !(*this == o); }
};

struct PlayerControls {
    int key[CB_COUNT][CTL_SLOTS] = {};   /* SDL_Scancode, 0 = none */
    PadIn pad[CB_COUNT][CTL_SLOTS];
    std::string device = "auto";   /* auto (plug-in order) | none | a controller's GUID */
    std::string device_name;       /* that controller's name, for when it isn't plugged in */
    int deadzone = 37;             /* % of a stick's or trigger's travel before it counts */
};

extern PlayerControls g_controls[CTL_PLAYERS];

const char *ctl_button_name(int b);   /* "Up", "A", "Start", ... */
const char *ctl_button_key(int b);    /* xmen2.ini's key: "up", "a", "start", ... */
uint16_t    ctl_button_bit(int b);    /* its GPAD_* bit */

/* settings */
void controls_defaults(int p);                 /* player p's defaults (player 1: the keyboard too) */
void controls_clear(int p);                    /* nothing bound */
bool controls_is_default(int p);
enum { KEYS_NONE, KEYS_ARROWS, KEYS_WASD, KEYS_CUSTOM };
std::string controls_keyboard_preset(int p, int preset);   /* the keys of a layout; returns what it moved */
int         controls_keyboard_layout(int p);  /* which layout p's keys are (KEYS_CUSTOM: none of them) */
void        controls_pad_defaults(int p);
void        controls_load(void);              /* xmen2.ini's [playerN] (once; the defaults where there are none) */
bool        controls_save(void);              /* [playerN] for each player not at the defaults */
std::string controls_ini_value(int p, int b);   /* "key:Up, pad:dpup, pad:-lefty" */
bool        controls_parse(const std::string &text, int keys[CTL_SLOTS], PadIn pads[CTL_SLOTS]);
std::string controls_key_label(int scancode);     /* "Up", "Enter", "Left Shift" */
std::string controls_pad_label(PadIn in, int p);  /* as on player p's controller: "A", "Cross", "Button 3" */
std::string controls_summary(int p, const int *buttons, int n, bool keyboard);   /* "Z  X  C" */
bool        controls_reserved_key(int scancode);  /* Esc, F4-F11: the game's own keys */

/* binding (the launcher): the input moves here from wherever else it was; the note says from where */
std::string controls_bind_key(int p, int b, int slot, int scancode);
std::string controls_bind_pad(int p, int b, int slot, PadIn in);

/* the controllers (both the launcher's SDL and the game's) */
void        controls_sdl_hints(void);          /* before SDL_Init */
void        controls_devices_init(void);       /* after it: gamecontrollerdb.txt, every controller opened */
void        controls_devices_close(void);      /* before SDL quits */
std::string controls_event(const SDL_Event &e);   /* plugged in / out: what changed for the players ("" none) */
void        controls_reassign(void);           /* the players' controllers again, from the settings */
uint16_t    controls_mask(int p);              /* player p's GPAD_* bits now */
SDL_GameController *controls_player_pad(int p);
bool        controls_rewind_held(void);        /* any player's Rewind input is held */
bool        controls_rewind_held_by(int p);    /* ... player p's */
bool        controls_any_pad_held(void);       /* a button or a stick of any controller */
struct CtlDevice {
    std::string guid, name;
    int instance;
    int player;    /* -1: none */
    bool generic;  /* SDL didn't know it: controls.cpp mapped its buttons in order */
};
std::vector<CtlDevice> controls_devices(void);   /* in the order they were plugged in */
std::string controls_device_label(int p);      /* "Xbox Series X Controller" / "none connected" */

/* catching the next controller input (the launcher's press-a-button) */
void controls_capture_begin(void);             /* the sticks' and triggers' resting places */
bool controls_capture_pad(const SDL_Event &e, PadIn *in);

int  controls_selftest(void);   /* host_pad_selftest's part: settings, parsing, binding; failures */

#endif
