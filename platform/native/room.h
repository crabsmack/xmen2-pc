/* room.h - the launcher's waiting room for a join code (launcher.cpp): the host opens its port, makes a code
 * of its addresses, and hands out seats; a joiner tries every address in the code at once and waits for
 * the host to start. No server (P7-multiplayer.md R8): the two launchers talk to each other on the game's
 * UDP port, then each starts its game with the same --netplay arguments the command line takes.
 *
 * Reaching the host: the engine's RNetHostPort (recomp-net host_relay.h) asks the router to forward the
 * port (UPnP IGD, then NAT-PMP; a 2-hour lease, renewed by nothing, so it lapses by itself), else finds the
 * public address by STUN (reachable only through a forwarded port or an open NAT). The code holds that
 * public address and this PC's own (its network's, and a VPN's such as Tailscale's).
 *
 * The wire (text, one datagram each; nonce = the joiner's random id, the same for all its packets):
 *   joiner -> host   XM2ROOM1 JOIN <nonce> <netplay version>      every 250 ms until seated, then every 500 ms
 *                    XM2ROOM1 LEAVE <nonce>
 *   host -> joiner   XM2ROOM1 SEAT <nonce> <seat> <players> <here>  (here: the players in the room, host too)
 *                    XM2ROOM1 FULL <nonce> | VER <nonce> <host's version> | BYE <nonce>
 *                    XM2ROOM1 GO <nonce> <seat> <seats>             the start: each seat starts its game
 * Seats go in the order the joiners came (1, 2, 3), closing up when one leaves; a joiner unheard for 5 s
 * has left. The host repeats GO for 0.8 s, then closes its socket and its game binds the port.
 */
#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "joincode.h"

struct RoomHost;
struct RoomJoin;

struct RoomGuest {
    uint32_t nonce;
    int seat;            /* 1.. (in arrival order) */
    JoinAddrKind via;    /* the kind of address it came to */
};

struct RoomHostStatus {
    enum { OPENING, OPEN, STARTING, STARTED, FAILED } state;
    std::string how;      /* "upnp" | "nat-pmp" | "stun" | "env" | "" (no public address) */
    std::string public_ep;   /* "a.b.c.d:port" or "" */
    std::string code;     /* the join code (OPEN on) */
    std::vector<JoinAddr> addrs;   /* what the code holds */
    std::vector<RoomGuest> guests;
    int seats;            /* STARTING/STARTED: the seats the game starts with */
    std::string why;      /* FAILED: a sentence */
};

/* port: the game's UDP port; try_router 0 asks no router (STUN only). local: this PC's own addresses to put
   in the code (launcher.cpp's list: its network's and a VPN's). */
RoomHost *room_host_open(uint16_t port, int players, bool try_router, const std::vector<uint32_t> &local);
void      room_host_pump(RoomHost *h);
void      room_host_status(const RoomHost *h, RoomHostStatus *out);
void      room_host_start(RoomHost *h);   /* now, with whoever is in (at least one) */
void      room_host_close(RoomHost **h);  /* tells the joiners, closes the port */

struct RoomJoinStatus {
    enum { SEARCHING, SEATED, GO, FAILED } state;
    JoinAddr host;        /* SEATED/GO: the address that answered */
    int seat, players, here, seats;   /* seats: GO's */
    std::string why;      /* FAILED */
};

RoomJoin *room_join_open(const std::vector<JoinAddr> &addrs);
void      room_join_pump(RoomJoin *j);
void      room_join_status(const RoomJoin *j, RoomJoinStatus *out);
void      room_join_close(RoomJoin **j);   /* LEAVE (unless it went), closes */

/* XMen2Native --router-check [PORT]: ask the router to forward PORT (7000) as a host would, print what it
   answered as one JSON line, remove the forward again. Exit 0 when it forwards to the internet. */
int room_router_check(int port);

const char *netplay_version(void);   /* netplay.cpp: NP_VERSION, which both games must share */
