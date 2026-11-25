/* controls.cpp - the players' controls (controls.h): bindings, xmen2.ini's [player1]..[player4], and the
 * controllers.
 *
 * Every Genesis button takes two keys and two controller inputs. A controller input is one of SDL's
 * standard game controller layout (south face button = `a`, `dpup`, `-lefty`, `lefttrigger`, ...), so
 * one binding works on every controller SDL knows: Xbox, PlayStation 3/4/5, Switch Pro and Joy-Cons,
 * Stadia, 8BitDo and the many others in SDL's own list, plus any in a gamecontrollerdb.txt beside the
 * program (SDL_GAMECONTROLLERCONFIG works too). Face buttons are by position on every controller
 * (SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS=0), so the default layout is the same shape on a Switch
 * pad as on an Xbox one, and the launcher names each input as printed on the player's controller.
 * A joystick SDL doesn't know (a generic USB pad, an arcade stick) gets a mapping made here: its
 * buttons in order, its first hat as the D-pad, its first axes as the sticks; the launcher's
 * press-a-button binds whatever it has.
 *
 * The defaults are the original fixed mapping: keyboard (player 1) arrows, Z X C = A B C, A S D = X Y Z,
 * Enter = Start, Q = Mode; every controller D-pad or left stick, X A B = A B C, LB Y RB = X Y Z,
 * Start, Back = Mode. Besides the 12 buttons, each player has Rewind (hold; rewind.cpp): no pad bit,
 * any player's rewinds the game; by default Backspace (player 1's keys) and every controller's left
 * trigger. Controllers go to players 1-4 in plug-in order (an unplugged one frees its
 * player, the next one plugged in takes the first free player). A player can instead have one
 * particular controller (by its GUID: two of the same model take the pinned players in plug-in
 * order) or none.
 *
 * xmen2.ini, a section per player not at the defaults:
 *   [player2]
 *   controller = auto | none | GUID      (controller_name = its name, for when it's unplugged)
 *   deadzone = 37                        (% of a stick's travel)
 *   a = key:J, pad:x                     (each button: up to 2 key: and 2 pad: inputs; empty = none)
 * A button missing from the section keeps its default.
 */
#include "controls.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

extern "C" {
#include "video/genesis_bus.h"
}
#include "native.h"

PlayerControls g_controls[CTL_PLAYERS];

namespace {

const char *const k_names[CB_COUNT] = {"Up", "Down", "Left", "Right", "A", "B", "C", "Start", "X", "Y", "Z", "Mode",
                                       "Rewind (hold)"};
const char *const k_keys[CB_COUNT] = {"up", "down", "left", "right", "a", "b", "c", "start", "x", "y", "z", "mode", "rewind"};
const uint16_t k_bits[CB_COUNT] = {GPAD_UP, GPAD_DOWN, GPAD_LEFT, GPAD_RIGHT, GPAD_A, GPAD_B, GPAD_C, GPAD_START,
                                   GPAD_X, GPAD_Y, GPAD_Z, GPAD_MODE, 0 /* CB_REWIND: the host's, no pad bit */};

PadIn btn(SDL_GameControllerButton b) { PadIn i; i.kind = PadIn::BUTTON; i.code = (int8_t)b; return i; }
PadIn axis(SDL_GameControllerAxis a, int dir) { PadIn i; i.kind = PadIn::AXIS; i.code = (int8_t)a; i.dir = (int8_t)dir; return i; }

/* the keyboard layouts (KEYS_ARROWS is player 1's default) */
const SDL_Scancode k_arrows[CB_COUNT] = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
                                         SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_RETURN,
                                         SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Q,
                                         SDL_SCANCODE_BACKSPACE};
const SDL_Scancode k_wasd[CB_COUNT] = {SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,
                                       SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_SPACE,
                                       SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_O, SDL_SCANCODE_TAB,
                                       SDL_SCANCODE_UNKNOWN /* rewind: the arrows layout's Backspace is enough */};

std::string trim(const std::string &s)
{
    size_t a = s.find_first_not_of(" \t"), b = s.find_last_not_of(" \t");
    return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
}

std::string lower(std::string s)
{
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

/* ---- the controllers ---------------------------------------------------------------------------- */

std::vector<SDL_GameController *> s_open;   /* every controller, in plug-in order */
SDL_GameController *s_slot[CTL_PLAYERS];      /* each player's */
std::vector<std::string> s_generic;           /* GUIDs of the mappings made here */
Sint16 s_rest[16][SDL_CONTROLLER_AXIS_MAX];   /* controls_capture_begin: s_open's axes at rest */
SDL_JoystickID s_rest_id[16];

SDL_JoystickID pad_id(SDL_GameController *c) { return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(c)); }

std::string pad_guid(SDL_GameController *c)
{
    char g[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(c)), g, sizeof g);
    return g;
}

std::string pad_name(SDL_GameController *c)
{
    const char *n = SDL_GameControllerName(c);
    return n && *n ? n : "Controller";
}

bool is_generic(const std::string &guid) { return std::find(s_generic.begin(), s_generic.end(), guid) != s_generic.end(); }

/* A joystick SDL has no mapping for: one made from what it has (buttons in order, the first hat, the
   first axes). Wheels, flight sticks and throttles are left alone, and anything with under 4 buttons or
   nothing to move with. true: it is a game controller now. */
bool generic_map(int index)
{
    SDL_JoystickType t = SDL_JoystickGetDeviceType(index);
    if (t == SDL_JOYSTICK_TYPE_WHEEL || t == SDL_JOYSTICK_TYPE_FLIGHT_STICK || t == SDL_JOYSTICK_TYPE_THROTTLE) return false;
    SDL_Joystick *j = SDL_JoystickOpen(index);
    if (!j) return false;
    int nb = SDL_JoystickNumButtons(j), na = SDL_JoystickNumAxes(j), nh = SDL_JoystickNumHats(j);
    char guid[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(j), guid, sizeof guid);
    std::string name = SDL_JoystickName(j) ? SDL_JoystickName(j) : "";
    SDL_JoystickClose(j);
    if (nb < 4 || (nh < 1 && na < 2)) return false;
    for (char &c : name)
        if (c == ',' || c < 0x20) c = ' ';
    if (trim(name).empty()) name = "Generic controller";
    /* the common DirectInput pad's order: 4 face buttons, L1 R1 L2 R2, Select Start, L3 R3 */
    static const char *const bn[] = {"a", "b", "x", "y", "leftshoulder", "rightshoulder", "lefttrigger", "righttrigger",
                                     "back", "start", "leftstick", "rightstick", "guide", "misc1", "paddle1", "paddle2",
                                     "paddle3", "paddle4", "touchpad"};
    static const char *const an[] = {"leftx", "lefty", "rightx", "righty"};
    std::string m = std::string(guid) + "," + name + ",";
    for (int i = 0; i < nb && i < (int)(sizeof bn / sizeof bn[0]); i++) m += std::string(bn[i]) + ":b" + std::to_string(i) + ",";
    if (nh > 0) m += "dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,";
    for (int i = 0; i < na && i < 4; i++) m += std::string(an[i]) + ":a" + std::to_string(i) + ",";
    if (SDL_GameControllerAddMapping(m.c_str()) < 0 || !SDL_IsGameController(index)) {
        fprintf(stderr, "[native] controller %s: no mapping (%s)\n", name.c_str(), SDL_GetError());
        return false;
    }
    if (!is_generic(guid)) s_generic.push_back(guid);
    fprintf(stderr, "[native] %s: unknown to SDL, mapped as a generic controller (%d buttons, %d axes, %d hats)\n",
            name.c_str(), nb, na, nh);
    return true;
}

/* the player a controller goes to: one that asked for it by GUID, else the first free `auto` one */
int assign(SDL_GameController *c)
{
    std::string g = pad_guid(c);
    for (int p = 0; p < CTL_PLAYERS; p++)
        if (!s_slot[p] && g_controls[p].device == g) { s_slot[p] = c; return p; }
    for (int p = 0; p < CTL_PLAYERS; p++)
        if (!s_slot[p] && g_controls[p].device == "auto") { s_slot[p] = c; return p; }
    return -1;
}

int slot_of(SDL_GameController *c)
{
    for (int p = 0; p < CTL_PLAYERS; p++)
        if (s_slot[p] == c) return p;
    return -1;
}

/* SDL2 also sends SDL_CONTROLLERDEVICEADDED for the controllers present at SDL_Init, which the scan has
   opened already: opening one again gives the same handle, so a controller is taken only once (v1.0.0
   on Linux: one pad was players 1 and 2). */
std::string pad_add(int index)
{
    if (!SDL_IsGameController(index) && !generic_map(index)) return "";
    SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(index);
    for (SDL_GameController *c : s_open)
        if (pad_id(c) == id) return "";
    SDL_GameController *c = SDL_GameControllerOpen(index);
    if (!c) return "";
    s_open.push_back(c);
    int p = assign(c);
    if (p >= 0) fprintf(stderr, "[native] controller %d: %s (instance %d)\n", p + 1, pad_name(c).c_str(), (int)id);
    else fprintf(stderr, "[native] controller %s (instance %d): no player wants it\n", pad_name(c).c_str(), (int)id);
    return p >= 0 ? "PLAYER " + std::to_string(p + 1) + ": " + pad_name(c) : "";
}

std::string pad_remove(SDL_JoystickID id)
{
    std::string msg;
    for (size_t i = 0; i < s_open.size(); i++) {
        SDL_GameController *c = s_open[i];
        if (pad_id(c) != id) continue;
        int p = slot_of(c);
        if (p >= 0) {
            s_slot[p] = nullptr;
            fprintf(stderr, "[native] controller %d removed (instance %d)\n", p + 1, (int)id);
            msg = "PLAYER " + std::to_string(p + 1) + ": CONTROLLER UNPLUGGED";
        }
        SDL_GameControllerClose(c);
        s_open.erase(s_open.begin() + (long)i);
        /* a fifth controller, waiting, takes the freed player (if it may) */
        if (p >= 0)
            for (SDL_GameController *o : s_open)
                if (slot_of(o) < 0 && assign(o) >= 0) {
                    msg = "PLAYER " + std::to_string(p + 1) + ": " + pad_name(o);
                    break;
                }
        break;
    }
    return msg;
}

enum Family { FAM_XBOX, FAM_PS, FAM_NINTENDO, FAM_GENERIC };

Family family(SDL_GameController *c)
{
    if (!c) return FAM_XBOX;
    if (is_generic(pad_guid(c))) return FAM_GENERIC;
    switch (SDL_GameControllerGetType(c)) {
    case SDL_CONTROLLER_TYPE_PS3: case SDL_CONTROLLER_TYPE_PS4: case SDL_CONTROLLER_TYPE_PS5: return FAM_PS;
    case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO: case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
    case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT: case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR: return FAM_NINTENDO;
    default: return FAM_XBOX;
    }
}

/* the controller whose labels player p's inputs are shown with: theirs, else the first plugged in */
SDL_GameController *label_pad(int p)
{
    if (p >= 0 && p < CTL_PLAYERS && s_slot[p]) return s_slot[p];
    return s_open.empty() ? nullptr : s_open[0];
}

}   // namespace

const char *ctl_button_name(int b) { return k_names[b]; }
const char *ctl_button_key(int b) { return k_keys[b]; }
uint16_t ctl_button_bit(int b) { return k_bits[b]; }

/* ---- settings ----------------------------------------------------------------------------------- */

void controls_pad_defaults(int p)
{
    PlayerControls &c = g_controls[p];
    for (auto &row : c.pad)
        for (PadIn &i : row) i = PadIn();
    c.pad[CB_UP][0] = btn(SDL_CONTROLLER_BUTTON_DPAD_UP);       c.pad[CB_UP][1] = axis(SDL_CONTROLLER_AXIS_LEFTY, -1);
    c.pad[CB_DOWN][0] = btn(SDL_CONTROLLER_BUTTON_DPAD_DOWN);   c.pad[CB_DOWN][1] = axis(SDL_CONTROLLER_AXIS_LEFTY, 1);
    c.pad[CB_LEFT][0] = btn(SDL_CONTROLLER_BUTTON_DPAD_LEFT);   c.pad[CB_LEFT][1] = axis(SDL_CONTROLLER_AXIS_LEFTX, -1);
    c.pad[CB_RIGHT][0] = btn(SDL_CONTROLLER_BUTTON_DPAD_RIGHT); c.pad[CB_RIGHT][1] = axis(SDL_CONTROLLER_AXIS_LEFTX, 1);
    c.pad[CB_A][0] = btn(SDL_CONTROLLER_BUTTON_X);
    c.pad[CB_B][0] = btn(SDL_CONTROLLER_BUTTON_A);
    c.pad[CB_C][0] = btn(SDL_CONTROLLER_BUTTON_B);
    c.pad[CB_START][0] = btn(SDL_CONTROLLER_BUTTON_START);
    c.pad[CB_X][0] = btn(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    c.pad[CB_Y][0] = btn(SDL_CONTROLLER_BUTTON_Y);
    c.pad[CB_Z][0] = btn(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    c.pad[CB_MODE][0] = btn(SDL_CONTROLLER_BUTTON_BACK);
    c.pad[CB_REWIND][0] = axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, 1);   /* the game uses neither trigger */
    c.deadzone = 37;
}

void controls_defaults(int p)
{
    PlayerControls &c = g_controls[p];
    c = PlayerControls();
    controls_pad_defaults(p);
    if (p == 0)
        for (int b = 0; b < CB_COUNT; b++) c.key[b][0] = k_arrows[b];
}

void controls_clear(int p)
{
    PlayerControls &c = g_controls[p];
    for (int b = 0; b < CB_COUNT; b++)
        for (int s = 0; s < CTL_SLOTS; s++) { c.key[b][s] = 0; c.pad[b][s] = PadIn(); }
}

bool controls_is_default(int p)
{
    PlayerControls now = g_controls[p];
    controls_defaults(p);
    PlayerControls def = g_controls[p];
    g_controls[p] = now;
    if (now.device != def.device || now.deadzone != def.deadzone) return false;
    for (int b = 0; b < CB_COUNT; b++)
        for (int s = 0; s < CTL_SLOTS; s++)
            if (now.key[b][s] != def.key[b][s] || now.pad[b][s] != def.pad[b][s]) return false;
    return true;
}

bool controls_reserved_key(int sc)
{
    return sc == SDL_SCANCODE_ESCAPE || sc == SDL_SCANCODE_F4 || sc == SDL_SCANCODE_F5 || sc == SDL_SCANCODE_F6 ||
           sc == SDL_SCANCODE_F7 ||
           sc == SDL_SCANCODE_F8 || sc == SDL_SCANCODE_F9 || sc == SDL_SCANCODE_F10 || sc == SDL_SCANCODE_F11;
}

std::string controls_key_label(int sc)
{
    if (sc <= 0) return "";
    if (sc == SDL_SCANCODE_RETURN) return "Enter";
    if (sc == SDL_SCANCODE_KP_ENTER) return "Keypad Enter";
    const char *n = SDL_GetScancodeName((SDL_Scancode)sc);
    if (!n || !*n) return "Key " + std::to_string(sc);
    return n;
}

/* xmen2.ini's names: key:NAME (SDL's scancode name; #N where it has a comma or none), pad:NAME (SDL's
   mapping names; an axis signed: pad:-lefty, pad:+lefttrigger or pad:lefttrigger) */
static std::string key_text(int sc)
{
    const char *n = SDL_GetScancodeName((SDL_Scancode)sc);
    if (!n || !*n || strchr(n, ',')) return "key:#" + std::to_string(sc);
    return std::string("key:") + n;
}

static std::string pad_text(PadIn in)
{
    if (in.kind == PadIn::BUTTON) {
        const char *n = SDL_GameControllerGetStringForButton((SDL_GameControllerButton)in.code);
        return std::string("pad:") + (n ? n : "?");
    }
    const char *n = SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)in.code);
    bool trig = in.code == SDL_CONTROLLER_AXIS_TRIGGERLEFT || in.code == SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
    return std::string("pad:") + (trig ? "" : in.dir < 0 ? "-" : "+") + (n ? n : "?");
}

std::string controls_ini_value(int p, int b)
{
    const PlayerControls &c = g_controls[p];
    std::string o;
    for (int s = 0; s < CTL_SLOTS; s++)
        if (c.key[b][s]) o += (o.empty() ? "" : ", ") + key_text(c.key[b][s]);
    for (int s = 0; s < CTL_SLOTS; s++)
        if (c.pad[b][s].kind) o += (o.empty() ? "" : ", ") + pad_text(c.pad[b][s]);
    return o;
}

static bool parse_one(const std::string &t, int *key, PadIn *pad)
{
    *key = 0;
    *pad = PadIn();
    std::string l = lower(t);
    if (l.compare(0, 4, "key:") == 0) {
        std::string n = trim(t.substr(4));
        if (!n.empty() && n[0] == '#') *key = atoi(n.c_str() + 1);
        else if (lower(n) == "enter") *key = SDL_SCANCODE_RETURN;
        else *key = SDL_GetScancodeFromName(n.c_str());
        return *key > 0 && *key < SDL_NUM_SCANCODES && !controls_reserved_key(*key);
    }
    if (l.compare(0, 4, "pad:") == 0) {
        std::string n = trim(l.substr(4));
        int dir = 1;
        if (!n.empty() && (n[0] == '-' || n[0] == '+')) { dir = n[0] == '-' ? -1 : 1; n.erase(0, 1); }
        SDL_GameControllerButton b = SDL_GameControllerGetButtonFromString(n.c_str());
        if (b != SDL_CONTROLLER_BUTTON_INVALID) { *pad = btn(b); return true; }
        SDL_GameControllerAxis a = SDL_GameControllerGetAxisFromString(n.c_str());
        if (a != SDL_CONTROLLER_AXIS_INVALID) {
            bool trig = a == SDL_CONTROLLER_AXIS_TRIGGERLEFT || a == SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
            *pad = axis(a, trig ? 1 : dir);
            return true;
        }
    }
    return false;
}

/* "key:Up, pad:dpup, pad:-lefty": up to two of each kind (more are dropped); false if any part isn't one */
bool controls_parse(const std::string &text, int keys[CTL_SLOTS], PadIn pads[CTL_SLOTS])
{
    int nk = 0, np = 0;
    bool ok = true;
    for (int s = 0; s < CTL_SLOTS; s++) { keys[s] = 0; pads[s] = PadIn(); }
    for (size_t p = 0; p <= text.size(); ) {
        size_t c = text.find(',', p);
        if (c == std::string::npos) c = text.size();
        std::string part = trim(text.substr(p, c - p));
        p = c + 1;
        if (part.empty()) continue;
        int k;
        PadIn in;
        if (!parse_one(part, &k, &in)) { ok = false; continue; }
        if (k && nk < CTL_SLOTS) keys[nk++] = k;
        if (in.kind && np < CTL_SLOTS) pads[np++] = in;
    }
    return ok;
}

static bool s_loaded;
static void rewind_keys_yield(void);

void controls_load(void)
{
    if (s_loaded) return;
    s_loaded = true;
    for (int p = 0; p < CTL_PLAYERS; p++) {
        controls_defaults(p);
        std::string sec = "player" + std::to_string(p + 1);
        const std::string none = "\x01";
        PlayerControls &c = g_controls[p];
        std::string d = options_ini_get(sec.c_str(), "controller", "auto");
        c.device = lower(d) == "none" ? "none" : lower(d) == "auto" || d.empty() ? "auto" : d;
        c.device_name = options_ini_get(sec.c_str(), "controller_name", "");
        int dz = atoi(options_ini_get(sec.c_str(), "deadzone", "37").c_str());
        c.deadzone = dz < 5 ? 5 : dz > 90 ? 90 : dz;
        for (int b = 0; b < CB_COUNT; b++) {
            std::string v = options_ini_get(sec.c_str(), k_keys[b], none);
            if (v == none) continue;
            if (!controls_parse(v, c.key[b], c.pad[b]))
                fprintf(stderr, "[native] xmen2.ini [%s] %s = %s: some of it isn't a key or controller input\n", sec.c_str(),
                        k_keys[b], v.c_str());
        }
    }
    rewind_keys_yield();
}

/* an xmen2.ini from before Rewind had a key may bind its default (Backspace) to a button: the key
   stays the button's, Rewind has none there */
static void rewind_keys_yield(void)
{
    for (int p = 0; p < CTL_PLAYERS; p++)
        for (int s = 0; s < CTL_SLOTS; s++) {
            int &k = g_controls[p].key[CB_REWIND][s];
            for (int q = 0; q < CTL_PLAYERS && k; q++)
                for (int b = 0; b < CB_COUNT && k; b++)
                    for (int t = 0; t < CTL_SLOTS && k; t++)
                        if (b != CB_REWIND && g_controls[q].key[b][t] == k) k = 0;
        }
}

bool controls_save(void)
{
    bool ok = true;
    for (int p = 0; p < CTL_PLAYERS; p++) {
        std::string sec = "player" + std::to_string(p + 1);
        std::vector<std::pair<std::string, std::string>> kv;
        if (!controls_is_default(p)) {
            const PlayerControls &c = g_controls[p];
            kv.emplace_back("controller", c.device);
            if (c.device != "auto" && c.device != "none") kv.emplace_back("controller_name", c.device_name);
            kv.emplace_back("deadzone", std::to_string(c.deadzone));
            for (int b = 0; b < CB_COUNT; b++) kv.emplace_back(k_keys[b], controls_ini_value(p, b));
        }
        std::string comment = "player " + std::to_string(p + 1) + "'s controls (the launcher's Controls page writes this; README \"Controls\")";
        ok &= options_ini_write(sec.c_str(), kv, comment.c_str());
    }
    return ok;
}

int controls_keyboard_layout(int p)
{
    const PlayerControls &c = g_controls[p];
    bool any = false, arrows = true, wasd = true;
    for (int b = 0; b < CB_COUNT; b++) {
        any |= c.key[b][0] || c.key[b][1];
        arrows &= c.key[b][0] == k_arrows[b] && !c.key[b][1];
        wasd &= c.key[b][0] == k_wasd[b] && !c.key[b][1];
    }
    return !any ? KEYS_NONE : arrows ? KEYS_ARROWS : wasd ? KEYS_WASD : KEYS_CUSTOM;
}

std::string controls_keyboard_preset(int p, int preset)
{
    PlayerControls &c = g_controls[p];
    for (int b = 0; b < CB_COUNT; b++) c.key[b][0] = c.key[b][1] = 0;
    if (preset != KEYS_ARROWS && preset != KEYS_WASD) return "";
    const SDL_Scancode *k = preset == KEYS_ARROWS ? k_arrows : k_wasd;
    std::string notes;
    for (int b = 0; b < CB_COUNT; b++) {
        if (!k[b]) continue;   /* none in this layout */
        std::string n = controls_bind_key(p, b, 0, k[b]);
        if (!n.empty()) notes += (notes.empty() ? "" : " ") + n;
    }
    return notes;
}

/* ---- binding ------------------------------------------------------------------------------------ */

std::string controls_bind_key(int p, int b, int slot, int sc)
{
    if (controls_reserved_key(sc))
        return controls_key_label(sc) + " is the game's own key (Esc quits, F5-F7 save and load, F8-F11 change the picture).";
    std::string note;
    for (int q = 0; q < CTL_PLAYERS; q++)
        for (int bb = 0; bb < CB_COUNT; bb++)
            for (int s = 0; s < CTL_SLOTS; s++) {
                if ((q == p && bb == b && s == slot) || g_controls[q].key[bb][s] != sc) continue;
                g_controls[q].key[bb][s] = 0;
                if (q == p && bb == b) continue;   /* the same button's other box */
                note = controls_key_label(sc) + " was player " + std::to_string(q + 1) + "'s " + k_names[bb] + ": moved here.";
            }
    g_controls[p].key[b][slot] = sc;
    return note;
}

std::string controls_bind_pad(int p, int b, int slot, PadIn in)
{
    std::string note;
    PlayerControls &c = g_controls[p];
    for (int bb = 0; bb < CB_COUNT; bb++)
        for (int s = 0; s < CTL_SLOTS; s++) {
            if ((bb == b && s == slot) || c.pad[bb][s] != in) continue;
            c.pad[bb][s] = PadIn();
            if (bb != b) note = controls_pad_label(in, p) + " was " + k_names[bb] + ": moved here.";
        }
    c.pad[b][slot] = in;
    return note;
}

/* ---- labels ------------------------------------------------------------------------------------- */

static std::string generic_label(SDL_GameController *c, PadIn in)
{
    SDL_GameControllerButtonBind bd = in.kind == PadIn::BUTTON
                                          ? SDL_GameControllerGetBindForButton(c, (SDL_GameControllerButton)in.code)
                                          : SDL_GameControllerGetBindForAxis(c, (SDL_GameControllerAxis)in.code);
    if (bd.bindType == SDL_CONTROLLER_BINDTYPE_BUTTON) return "Button " + std::to_string(bd.value.button + 1);
    if (bd.bindType == SDL_CONTROLLER_BINDTYPE_HAT) {
        int m = bd.value.hat.hat_mask;
        return m == 1 ? "D-pad up" : m == 2 ? "D-pad right" : m == 4 ? "D-pad down" : "D-pad left";
    }
    if (bd.bindType == SDL_CONTROLLER_BINDTYPE_AXIS) return "Axis " + std::to_string(bd.value.axis + 1) + (in.dir < 0 ? "-" : "+");
    return "";
}

std::string controls_pad_label(PadIn in, int p)
{
    if (!in.kind) return "";
    SDL_GameController *c = label_pad(p);
    Family f = family(c);
    if (f == FAM_GENERIC) {
        std::string g = generic_label(c, in);
        if (!g.empty()) return g;
        f = FAM_XBOX;
    }
    if (in.kind == PadIn::AXIS) {
        switch (in.code) {
        case SDL_CONTROLLER_AXIS_LEFTX: return in.dir < 0 ? "Left stick left" : "Left stick right";
        case SDL_CONTROLLER_AXIS_LEFTY: return in.dir < 0 ? "Left stick up" : "Left stick down";
        case SDL_CONTROLLER_AXIS_RIGHTX: return in.dir < 0 ? "Right stick left" : "Right stick right";
        case SDL_CONTROLLER_AXIS_RIGHTY: return in.dir < 0 ? "Right stick up" : "Right stick down";
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return f == FAM_PS ? "L2" : f == FAM_NINTENDO ? "ZL" : "LT";
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return f == FAM_PS ? "R2" : f == FAM_NINTENDO ? "ZR" : "RT";
        }
        return "Axis";
    }
    bool one = c && SDL_GameControllerGetType(c) == SDL_CONTROLLER_TYPE_XBOXONE;
    SDL_GameControllerType t = c ? SDL_GameControllerGetType(c) : SDL_CONTROLLER_TYPE_UNKNOWN;
    switch (in.code) {
    /* by position: south, east, west, north */
    case SDL_CONTROLLER_BUTTON_A: return f == FAM_PS ? "Cross" : f == FAM_NINTENDO ? "B" : "A";
    case SDL_CONTROLLER_BUTTON_B: return f == FAM_PS ? "Circle" : f == FAM_NINTENDO ? "A" : "B";
    case SDL_CONTROLLER_BUTTON_X: return f == FAM_PS ? "Square" : f == FAM_NINTENDO ? "Y" : "X";
    case SDL_CONTROLLER_BUTTON_Y: return f == FAM_PS ? "Triangle" : f == FAM_NINTENDO ? "X" : "Y";
    case SDL_CONTROLLER_BUTTON_BACK:
        return f == FAM_PS ? (t == SDL_CONTROLLER_TYPE_PS5 ? "Create" : t == SDL_CONTROLLER_TYPE_PS4 ? "Share" : "Select")
             : f == FAM_NINTENDO ? "Minus" : one ? "View" : "Back";
    case SDL_CONTROLLER_BUTTON_GUIDE: return f == FAM_PS ? "PS" : f == FAM_NINTENDO ? "Home" : "Guide";
    case SDL_CONTROLLER_BUTTON_START:
        return f == FAM_PS ? (t == SDL_CONTROLLER_TYPE_PS3 ? "Start" : "Options") : f == FAM_NINTENDO ? "Plus" : one ? "Menu" : "Start";
    case SDL_CONTROLLER_BUTTON_LEFTSTICK: return f == FAM_PS ? "L3" : "Left stick press";
    case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return f == FAM_PS ? "R3" : "Right stick press";
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return f == FAM_PS ? "L1" : f == FAM_NINTENDO ? "L" : "LB";
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return f == FAM_PS ? "R1" : f == FAM_NINTENDO ? "R" : "RB";
    case SDL_CONTROLLER_BUTTON_DPAD_UP: return "D-pad up";
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return "D-pad down";
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return "D-pad left";
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return "D-pad right";
    case SDL_CONTROLLER_BUTTON_MISC1: return f == FAM_PS ? "Mute" : f == FAM_NINTENDO ? "Capture" : "Share";
    case SDL_CONTROLLER_BUTTON_PADDLE1: return "Paddle 1";
    case SDL_CONTROLLER_BUTTON_PADDLE2: return "Paddle 2";
    case SDL_CONTROLLER_BUTTON_PADDLE3: return "Paddle 3";
    case SDL_CONTROLLER_BUTTON_PADDLE4: return "Paddle 4";
    case SDL_CONTROLLER_BUTTON_TOUCHPAD: return "Touchpad";
    }
    return "Button";
}

/* the first box of each button, side by side ("Z  X  C"); directions that are the arrow keys, the
   D-pad or one stick in full say so */
std::string controls_summary(int p, const int *buttons, int n, bool keyboard)
{
    const PlayerControls &c = g_controls[p];
    if (n == 4 && buttons[0] == CB_UP) {
        if (keyboard && c.key[CB_UP][0] == SDL_SCANCODE_UP && c.key[CB_DOWN][0] == SDL_SCANCODE_DOWN &&
            c.key[CB_LEFT][0] == SDL_SCANCODE_LEFT && c.key[CB_RIGHT][0] == SDL_SCANCODE_RIGHT) return "Arrow keys";
        if (!keyboard) {
            bool dpad = false, stick = false;
            for (int s = 0; s < CTL_SLOTS; s++) {
                dpad |= c.pad[CB_UP][s] == btn(SDL_CONTROLLER_BUTTON_DPAD_UP) && c.pad[CB_LEFT][s] == btn(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
                stick |= c.pad[CB_UP][s] == axis(SDL_CONTROLLER_AXIS_LEFTY, -1) && c.pad[CB_LEFT][s] == axis(SDL_CONTROLLER_AXIS_LEFTX, -1);
            }
            if (dpad || stick) return dpad && stick ? "D-pad or left stick" : dpad ? "D-pad" : "Left stick";
        }
    }
    std::string o;
    for (int i = 0; i < n; i++) {
        int b = buttons[i];
        std::string l = keyboard ? controls_key_label(c.key[b][0] ? c.key[b][0] : c.key[b][1])
                                 : controls_pad_label(c.pad[b][0].kind ? c.pad[b][0] : c.pad[b][1], p);
        o += (i ? "  " : "") + (l.empty() ? std::string("-") : l);
    }
    return o;
}

/* ---- the controllers ---------------------------------------------------------------------------- */

void controls_sdl_hints(void)
{
    /* face buttons by position on every controller (Nintendo's labels are swapped: the launcher names
       them as printed) */
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
}

void controls_devices_init(void)
{
    controls_load();
    std::string db = options_exe_dir() + "/gamecontrollerdb.txt";
    if (FILE *f = fopen(db.c_str(), "rb")) {
        fclose(f);
        int n = SDL_GameControllerAddMappingsFromFile(db.c_str());
        fprintf(stderr, "[native] gamecontrollerdb.txt: %d mappings%s%s\n", n < 0 ? 0 : n, n < 0 ? ", " : "", n < 0 ? SDL_GetError() : "");
    }
    for (int i = 0; i < SDL_NumJoysticks(); i++) pad_add(i);
}

void controls_devices_close(void)
{
    for (SDL_GameController *c : s_open) SDL_GameControllerClose(c);
    s_open.clear();
    for (auto &s : s_slot) s = nullptr;
}

std::string controls_event(const SDL_Event &e)
{
    /* a joystick SDL doesn't know as a controller comes as a joystick only: mapped, it's one */
    if (e.type == SDL_JOYDEVICEADDED && !SDL_IsGameController(e.jdevice.which)) return pad_add(e.jdevice.which);
    if (e.type == SDL_CONTROLLERDEVICEADDED) return pad_add(e.cdevice.which);
    if (e.type == SDL_CONTROLLERDEVICEREMOVED) return pad_remove(e.cdevice.which);
    if (e.type == SDL_JOYDEVICEREMOVED) return pad_remove(e.jdevice.which);   /* (a no-op when the above did it) */
    return "";
}

void controls_reassign(void)
{
    for (auto &s : s_slot) s = nullptr;
    for (SDL_GameController *c : s_open) assign(c);
}

uint16_t controls_mask(int p)
{
    if (p < 0 || p >= CTL_PLAYERS) return 0;
    const PlayerControls &c = g_controls[p];
    int n = 0;
    const Uint8 *k = SDL_GetKeyboardState(&n);
    SDL_GameController *pad = s_slot[p];
    int dead = c.deadzone * 32767 / 100;
    uint16_t m = 0;
    for (int b = 0; b < CB_COUNT; b++) {
        bool on = false;
        for (int s = 0; s < CTL_SLOTS && !on; s++) {
            int sc = c.key[b][s];
            on = k && sc > 0 && sc < n && k[sc];
            const PadIn &in = c.pad[b][s];
            if (!on && pad && in.kind == PadIn::BUTTON) on = SDL_GameControllerGetButton(pad, (SDL_GameControllerButton)in.code) != 0;
            if (!on && pad && in.kind == PadIn::AXIS) on = SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis)in.code) * in.dir > dead;
        }
        if (on) m |= k_bits[b];
    }
    return m;
}

SDL_GameController *controls_player_pad(int p) { return p >= 0 && p < CTL_PLAYERS ? s_slot[p] : nullptr; }

/* player p's Rewind input is held */
bool controls_rewind_held_by(int p)
{
    if (p < 0 || p >= CTL_PLAYERS) return false;
    int n = 0;
    const Uint8 *k = SDL_GetKeyboardState(&n);
    const PlayerControls &c = g_controls[p];
    SDL_GameController *pad = s_slot[p];
    int dead = c.deadzone * 32767 / 100;
    for (int s = 0; s < CTL_SLOTS; s++) {
        int sc = c.key[CB_REWIND][s];
        if (k && sc > 0 && sc < n && k[sc]) return true;
        const PadIn &in = c.pad[CB_REWIND][s];
        if (pad && in.kind == PadIn::BUTTON && SDL_GameControllerGetButton(pad, (SDL_GameControllerButton)in.code)) return true;
        if (pad && in.kind == PadIn::AXIS && SDL_GameControllerGetAxis(pad, (SDL_GameControllerAxis)in.code) * in.dir > dead) return true;
    }
    return false;
}

/* any player's (rewind.cpp: the whole game goes back, whoever holds it) */
bool controls_rewind_held(void)
{
    for (int p = 0; p < CTL_PLAYERS; p++)
        if (controls_rewind_held_by(p)) return true;
    return false;
}

bool controls_any_pad_held(void)
{
    for (SDL_GameController *c : s_open) {
        for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; b++)
            if (SDL_GameControllerGetButton(c, (SDL_GameControllerButton)b)) return true;
        for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++)
            if (abs(SDL_GameControllerGetAxis(c, (SDL_GameControllerAxis)a)) > 12000) return true;
    }
    return false;
}

std::vector<CtlDevice> controls_devices(void)
{
    std::vector<CtlDevice> v;
    for (SDL_GameController *c : s_open) v.push_back({pad_guid(c), pad_name(c), (int)pad_id(c), slot_of(c), is_generic(pad_guid(c))});
    return v;
}

std::string controls_device_label(int p)
{
    if (s_slot[p]) return pad_name(s_slot[p]);
    return "none connected";
}

void controls_capture_begin(void)
{
    for (size_t i = 0; i < s_open.size() && i < 16; i++) {
        s_rest_id[i] = pad_id(s_open[i]);
        for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++) s_rest[i][a] = SDL_GameControllerGetAxis(s_open[i], (SDL_GameControllerAxis)a);
    }
}

/* the next button pressed, or a stick or trigger moved over half way from where it rested */
bool controls_capture_pad(const SDL_Event &e, PadIn *in)
{
    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        if (e.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE) return false;   /* the system's (Steam, Game Bar) */
        *in = btn((SDL_GameControllerButton)e.cbutton.button);
        return true;
    }
    if (e.type != SDL_CONTROLLERAXISMOTION) return false;
    int v = e.caxis.value, rest = 0;
    for (size_t i = 0; i < s_open.size() && i < 16; i++)
        if (s_rest_id[i] == e.caxis.which) rest = s_rest[i][e.caxis.axis];
    if (abs(v) < 16000 || abs(v - rest) < 16000) return false;
    *in = axis((SDL_GameControllerAxis)e.caxis.axis, v < 0 ? -1 : 1);
    return true;
}

/* ---- the self-test (XMEN2_PAD_SELFTEST, with host_sdl.cpp's slots) ------------------------------ */

int controls_selftest(void)
{
    int bad = 0;
    auto check = [&](const char *what, bool ok) {
        bad += !ok;
        fprintf(stderr, "[native] controls selftest: %s %s\n", what, ok ? "ok" : "WRONG");
    };
    PlayerControls keep[CTL_PLAYERS];
    for (int p = 0; p < CTL_PLAYERS; p++) keep[p] = g_controls[p];
    for (int p = 0; p < CTL_PLAYERS; p++) controls_defaults(p);
    check("player 1's defaults are the arrows layout", controls_keyboard_layout(0) == KEYS_ARROWS && controls_is_default(0));
    check("player 2 has no keys by default", controls_keyboard_layout(1) == KEYS_NONE);
    check("defaults' text", controls_ini_value(0, CB_UP) == "key:Up, pad:dpup, pad:-lefty" &&
                                controls_ini_value(0, CB_START) == "key:Return, pad:start" && controls_ini_value(1, CB_A) == "pad:x");
    /* every default reads back as itself */
    bool round = true;
    for (int p = 0; p < 2; p++)
        for (int b = 0; b < CB_COUNT; b++) {
            int k[CTL_SLOTS];
            PadIn pd[CTL_SLOTS];
            round &= controls_parse(controls_ini_value(p, b), k, pd);
            for (int s = 0; s < CTL_SLOTS; s++) round &= k[s] == g_controls[p].key[b][s] && pd[s] == g_controls[p].pad[b][s];
        }
    check("the text reads back", round);
    int k[CTL_SLOTS];
    PadIn pd[CTL_SLOTS];
    check("odd names parse", controls_parse("key:Left Shift, key:#54, pad:+lefttrigger, PAD:-RightX", k, pd) &&
                                 k[0] == SDL_SCANCODE_LSHIFT && k[1] == 54 && pd[0] == axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, 1) &&
                                 pd[1] == axis(SDL_CONTROLLER_AXIS_RIGHTX, -1));
    check("the comma key is written by number", key_text(SDL_SCANCODE_COMMA) == "key:#54");
    check("bad parts are refused", !controls_parse("key:Nope, pad:a", k, pd) && pd[0] == btn(SDL_CONTROLLER_BUTTON_A) &&
                                       !controls_parse("key:Escape", k, pd) && !k[0]);
    /* binding moves a key from wherever it was */
    std::string note = controls_bind_key(1, CB_START, 0, SDL_SCANCODE_Z);
    check("a key bound twice moves", g_controls[1].key[CB_START][0] == SDL_SCANCODE_Z && g_controls[0].key[CB_A][0] == 0 &&
                                         note == "Z was player 1's A: moved here.");
    check("Esc can't be bound", !controls_bind_key(0, CB_A, 0, SDL_SCANCODE_ESCAPE).empty() && g_controls[0].key[CB_A][0] == 0);
    note = controls_bind_pad(0, CB_A, 1, btn(SDL_CONTROLLER_BUTTON_A));
    check("a controller input moves within a player", g_controls[0].pad[CB_A][1] == btn(SDL_CONTROLLER_BUTTON_A) &&
                                                          !g_controls[0].pad[CB_B][0].kind && g_controls[1].pad[CB_B][0].kind);
    controls_keyboard_preset(1, KEYS_WASD);
    check("WASD for player 2 takes A S D from player 1", controls_keyboard_layout(1) == KEYS_WASD && !g_controls[0].key[CB_X][0] &&
                                                             g_controls[0].key[CB_UP][0] == SDL_SCANCODE_UP);
    check("not default after a change", !controls_is_default(0) && !controls_is_default(1) && controls_is_default(2));
    /* Rewind (rewind.cpp): Backspace and the left trigger; F5-F7 are the game's own; an old xmen2.ini's
       Backspace on a button keeps it there */
    for (int p = 0; p < CTL_PLAYERS; p++) controls_defaults(p);
    check("Rewind's defaults", controls_ini_value(0, CB_REWIND) == "key:Backspace, pad:lefttrigger" &&
                                   controls_ini_value(1, CB_REWIND) == "pad:lefttrigger" && !ctl_button_bit(CB_REWIND));
    check("F5-F7 can't be bound", !controls_bind_key(0, CB_A, 0, SDL_SCANCODE_F5).empty() &&
                                      !controls_bind_key(0, CB_A, 0, SDL_SCANCODE_F7).empty() &&
                                      g_controls[0].key[CB_A][0] == SDL_SCANCODE_Z && !controls_parse("key:F6", k, pd));
    g_controls[0].key[CB_MODE][0] = SDL_SCANCODE_BACKSPACE;   /* as controls_load reads an old [player1] */
    rewind_keys_yield();
    check("an old Backspace button keeps the key", g_controls[0].key[CB_MODE][0] == SDL_SCANCODE_BACKSPACE &&
                                                       !g_controls[0].key[CB_REWIND][0] &&
                                                       g_controls[0].pad[CB_REWIND][0] == axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, 1));
    for (int p = 0; p < CTL_PLAYERS; p++) g_controls[p] = keep[p];
    return bad;
}
