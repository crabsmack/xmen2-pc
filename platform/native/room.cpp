/* room.cpp - the launcher's waiting room for a join code: see room.h */
#include "room.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <random>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#pragma comment(lib, "ws2_32.lib")
#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif
typedef SOCKET sock_t;
#define SOCK_BAD INVALID_SOCKET
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
typedef int sock_t;
#define SOCK_BAD (-1)
#endif

extern "C" {
#include "recomp_net/host_relay.h"
}

namespace {

#define MAGIC "XM2ROOM1 "

double now_s(void)
{
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}

void sock_close(sock_t *s)
{
    if (*s == SOCK_BAD) return;
#ifdef _WIN32
    closesocket(*s);
#else
    close(*s);
#endif
    *s = SOCK_BAD;
}

/* XMEN2_ROOM_BIND=127.0.0.1 (the tests): the room's sockets on that address only, and no router or STUN
   server asked (the code's public address is RNET_HOST_RELAY_ENDPOINT's): on Windows a socket on every
   address of a program copied to a new folder asks the user's desktop about the firewall */
uint32_t bind_ip(void)
{
    const char *b = getenv("XMEN2_ROOM_BIND");
    std::vector<JoinAddr> v;
    std::string why;
    return b && *b && joincode_decode(b, 1, &v, &why) ? v[0].ip : INADDR_ANY;
}

/* the netplay version the room compares; XMEN2_ROOM_VERSION stands in for another build's (the test of
   the refusal) */
std::string room_version(void)
{
    const char *v = getenv("XMEN2_ROOM_VERSION");
    return v && *v ? v : netplay_version();
}

/* a non-blocking UDP socket on port (0: any) */
sock_t sock_open(uint16_t port)
{
#ifdef _WIN32
    static bool up = false;
    if (!up) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); up = true; }
#endif
    sock_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == SOCK_BAD) return s;
    sockaddr_in a = {};
    a.sin_family = AF_INET;
    a.sin_port = htons(port);
    a.sin_addr.s_addr = htonl(bind_ip());
    if (bind(s, (sockaddr *)&a, sizeof a) != 0) { sock_close(&s); return s; }
#ifdef _WIN32
    u_long nb = 1;
    ioctlsocket(s, FIONBIO, &nb);
    BOOL no = FALSE;   /* a "port unreachable" from a dead address in the code isn't an error on the next read */
    DWORD got = 0;
    WSAIoctl(s, SIO_UDP_CONNRESET, &no, sizeof no, nullptr, 0, &got, nullptr, nullptr);
#else
    fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
#endif
    return s;
}

void send_to(sock_t s, const JoinAddr &to, const std::string &msg)
{
    sockaddr_in a = {};
    a.sin_family = AF_INET;
    a.sin_port = htons(to.port);
    a.sin_addr.s_addr = htonl(to.ip);
    sendto(s, msg.data(), (int)msg.size(), 0, (sockaddr *)&a, sizeof a);
}

/* the next datagram of the room's wire, its words after the magic; false when none is waiting */
bool recv_from(sock_t s, JoinAddr *from, std::vector<std::string> *words)
{
    for (int tries = 0; tries < 64; tries++) {
        char buf[512];
        sockaddr_in a = {};
        socklen_t al = sizeof a;
        int n = (int)recvfrom(s, buf, sizeof buf - 1, 0, (sockaddr *)&a, &al);
        if (n < 0) {
#ifdef _WIN32
            if (WSAGetLastError() == WSAECONNRESET) continue;
#endif
            return false;
        }
        buf[n] = 0;
        if (n <= (int)strlen(MAGIC) || memcmp(buf, MAGIC, strlen(MAGIC))) continue;   /* not ours (a game's HELLO) */
        words->clear();
        for (char *p = buf + strlen(MAGIC); *p;) {
            while (*p == ' ') p++;
            char *e = p;
            while (*e && *e != ' ') e++;
            if (e > p) words->emplace_back(p, e - p);
            p = e;
        }
        from->ip = ntohl(a.sin_addr.s_addr);
        from->port = ntohs(a.sin_port);
        if (!words->empty()) return true;
    }
    return false;
}

bool parse_ep(const std::string &t, JoinAddr *out)
{
    std::vector<JoinAddr> v;
    std::string why;
    if (t.empty() || !joincode_decode(t, 0, &v, &why) || v.size() != 1) return false;
    *out = v[0];
    return true;
}

}   // namespace

/* ---- the host -------------------------------------------------------------------------------------- */

struct Guest {
    uint32_t nonce;
    JoinAddr addr;
    JoinAddrKind via;
    double first, last;
};

struct RoomHost {
    uint16_t port;
    int players;
    std::vector<uint32_t> local;
    RNetHostPort *hp = nullptr;
    sock_t s = SOCK_BAD;
    RoomHostStatus st;
    std::vector<Guest> guests;
    double start_at = 0, last_go = 0;
};

static void host_seats(RoomHost *h)
{
    double now = now_s();
    for (size_t i = 0; i < h->guests.size();)
        if (h->st.state == RoomHostStatus::OPEN && now - h->guests[i].last > 5) h->guests.erase(h->guests.begin() + i);
        else i++;
    h->st.guests.clear();
    for (size_t i = 0; i < h->guests.size(); i++) h->st.guests.push_back({h->guests[i].nonce, (int)i + 1, h->guests[i].via});
}

static void host_finish_discovery(RoomHost *h)
{
    if (h->hp) {
        RNetHostPortStatus ps;
        rnet_host_port_status(h->hp, &ps);
        h->st.how = ps.how;
        h->st.public_ep = ps.endpoint;
        rnet_host_port_release(&h->hp);   /* the socket goes; a router's forwarding stays (its lease, 2 h) */
    } else {   /* XMEN2_ROOM_BIND */
        const char *e = getenv("RNET_HOST_RELAY_ENDPOINT");
        h->st.how = "env";
        h->st.public_ep = e ? e : "";
    }
    h->s = sock_open(h->port);
    if (h->s == SOCK_BAD) {
        h->st.state = RoomHostStatus::FAILED;
        h->st.why = "Port " + std::to_string(h->port) + " is in use by another program: choose another port.";
        return;
    }
    std::vector<JoinAddr> addrs;
    JoinAddr pub;
    if (parse_ep(h->st.public_ep, &pub)) addrs.push_back(pub);
    for (uint32_t ip : h->local) {
        bool dup = false;
        for (const JoinAddr &a : addrs) dup |= a.ip == ip;
        if (!dup && addrs.size() < 4) addrs.push_back({ip, h->port});
    }
    h->st.addrs = addrs;
    h->st.code = joincode_encode(addrs);
    h->st.state = RoomHostStatus::OPEN;
}

RoomHost *room_host_open(uint16_t port, int players, bool try_router, const std::vector<uint32_t> &local)
{
    RoomHost *h = new RoomHost;
    h->port = port;
    h->players = players;
    h->local = local;
    h->st.state = RoomHostStatus::OPENING;
    h->st.seats = 0;
    if (bind_ip() != INADDR_ANY) host_finish_discovery(h);
    else if (rnet_host_port_open(&h->hp, port, nullptr, try_router ? 1 : 0) != 0) {
        h->st.state = RoomHostStatus::FAILED;
        h->st.why = "Port " + std::to_string(port) + " is in use by another program: choose another port.";
    }
    return h;
}

void room_host_pump(RoomHost *h)
{
    if (!h) return;
    if (h->st.state == RoomHostStatus::OPENING && h->hp) {
        rnet_host_port_pump(h->hp);
        RNetHostPortStatus ps;
        rnet_host_port_status(h->hp, &ps);
        if (ps.done) host_finish_discovery(h);
        return;
    }
    if (h->s == SOCK_BAD) return;
    JoinAddr from;
    std::vector<std::string> w;
    double now = now_s();
    while (recv_from(h->s, &from, &w)) {
        if (w.size() < 2) continue;
        uint32_t nonce = (uint32_t)strtoul(w[1].c_str(), nullptr, 10);
        std::string n = std::to_string(nonce);
        Guest *g = nullptr;
        for (Guest &x : h->guests)
            if (x.nonce == nonce) g = &x;
        if (w[0] == "LEAVE") {
            if (g && h->st.state == RoomHostStatus::OPEN) h->guests.erase(h->guests.begin() + (g - h->guests.data()));
            continue;
        }
        if (w[0] != "JOIN" || w.size() < 3) continue;
        if (w[2] != room_version()) { send_to(h->s, from, MAGIC "VER " + n + " " + room_version()); continue; }
        if (!g) {
            if (h->st.state != RoomHostStatus::OPEN || (int)h->guests.size() >= h->players - 1) {
                send_to(h->s, from, MAGIC "FULL " + n);
                continue;
            }
            h->guests.push_back({nonce, from, joinaddr_kind(from.ip), now, now});
            g = &h->guests.back();
        }
        g->addr = from;
        g->last = now;
        host_seats(h);   /* may drop others (unheard for 5 s): the seat is this one's place after it */
        int seat = 0;
        for (size_t i = 0; i < h->guests.size(); i++)
            if (h->guests[i].nonce == nonce) seat = (int)i + 1;
        if (h->st.state == RoomHostStatus::OPEN)
            send_to(h->s, from, MAGIC "SEAT " + n + " " + std::to_string(seat) + " " + std::to_string(h->players) + " " +
                                    std::to_string(h->guests.size() + 1));
        else send_to(h->s, from, MAGIC "GO " + n + " " + std::to_string(seat) + " " + std::to_string(h->st.seats));
    }
    host_seats(h);
    if (h->st.state == RoomHostStatus::STARTING) {
        if (now - h->last_go >= 0.1) {
            h->last_go = now;
            for (size_t i = 0; i < h->guests.size(); i++)
                send_to(h->s, h->guests[i].addr, MAGIC "GO " + std::to_string(h->guests[i].nonce) + " " + std::to_string(i + 1) +
                                                      " " + std::to_string(h->st.seats));
        }
        if (now - h->start_at >= 0.8) {
            sock_close(&h->s);
            h->st.state = RoomHostStatus::STARTED;
        }
    }
}

void room_host_status(const RoomHost *h, RoomHostStatus *out) { *out = h->st; }

void room_host_start(RoomHost *h)
{
    if (!h || h->st.state != RoomHostStatus::OPEN || h->guests.empty()) return;
    h->st.seats = (int)h->guests.size() + 1;
    h->st.state = RoomHostStatus::STARTING;
    h->start_at = now_s();
    h->last_go = 0;
    room_host_pump(h);
}

void room_host_close(RoomHost **ph)
{
    RoomHost *h = *ph;
    if (!h) return;
    if (h->s != SOCK_BAD && h->st.state == RoomHostStatus::OPEN)
        for (const Guest &g : h->guests) send_to(h->s, g.addr, MAGIC "BYE " + std::to_string(g.nonce));
    if (h->hp) rnet_host_port_release(&h->hp);
    sock_close(&h->s);
    delete h;
    *ph = nullptr;
}

/* ---- --router-check -------------------------------------------------------------------------------- */

int room_router_check(int port)
{
    RNetHostPort *hp = nullptr;
    if (rnet_host_port_open(&hp, (unsigned short)port, nullptr, 1) != 0) {
        printf("{\"ok\": false, \"why\": \"port %d is in use\"}\n", port);
        return 1;
    }
    RNetHostPortStatus ps;
    double t0 = now_s();
    std::string stage;
    for (;;) {
        rnet_host_port_pump(hp);
        rnet_host_port_status(hp, &ps);
        if (stage != ps.stage) { stage = ps.stage; fprintf(stderr, "[room] %.1f s: %s\n", now_s() - t0, ps.stage); }
        if (ps.done || now_s() - t0 > 30) break;
#ifdef _WIN32
        Sleep(10);
#else
        usleep(10000);
#endif
    }
    JoinAddr pub = {0, 0};
    bool have = parse_ep(ps.endpoint, &pub);
    bool forwarded = !strcmp(ps.how, "upnp") || !strcmp(ps.how, "nat-pmp");
    /* a router that forwards but whose own outside address is private: another router (or the ISP's NAT)
       stands in front of it, and nobody outside reaches the forward */
    bool behind = have && joinaddr_kind(pub.ip) != JA_INTERNET;
    printf("{\"port\": %d, \"how\": \"%s\", \"public\": \"%s\", \"forwarded\": %s, \"behind_another_nat\": %s, "
           "\"seconds\": %.1f}\n", port, ps.how, ps.endpoint, forwarded ? "true" : "false", behind ? "true" : "false",
           now_s() - t0);
    fflush(stdout);
    rnet_host_port_unmap_release(&hp);   /* the forward removed again (UPnP DeletePortMapping, NAT-PMP lifetime 0) */
    fprintf(stderr, "[room] %s\n", forwarded ? "the forward removed" : "nothing to remove");
    return forwarded && !behind ? 0 : 1;
}

/* ---- the joiner ------------------------------------------------------------------------------------ */

struct RoomJoin {
    std::vector<JoinAddr> addrs;
    sock_t s = SOCK_BAD;
    uint32_t nonce;
    RoomJoinStatus st;
    double opened, last_send = 0, last_heard = 0, first_seat = 0;
    bool chosen = false;
};

RoomJoin *room_join_open(const std::vector<JoinAddr> &addrs)
{
    RoomJoin *j = new RoomJoin;
    j->addrs = addrs;
    std::random_device rd;
    j->nonce = (uint32_t)rd() ^ (uint32_t)(now_s() * 1e6);
    j->st.state = RoomJoinStatus::SEARCHING;
    j->st.seat = j->st.players = j->st.here = j->st.seats = 0;
    j->st.host = {0, 0};
    j->opened = now_s();
    j->s = sock_open(0);
    if (j->s == SOCK_BAD) { j->st.state = RoomJoinStatus::FAILED; j->st.why = "Can't open a network socket."; }
    return j;
}

static void join_fail(RoomJoin *j, const std::string &why)
{
    j->st.state = RoomJoinStatus::FAILED;
    j->st.why = why;
    sock_close(&j->s);
}

void room_join_pump(RoomJoin *j)
{
    if (!j || j->s == SOCK_BAD || j->st.state == RoomJoinStatus::GO || j->st.state == RoomJoinStatus::FAILED) return;
    double now = now_s();
    std::string join = MAGIC "JOIN " + std::to_string(j->nonce) + " " + room_version();
    bool seated = j->st.state == RoomJoinStatus::SEATED;
    bool settling = seated && now - j->first_seat < 0.3;
    if (now - j->last_send >= (seated && !settling ? 0.5 : 0.25)) {
        j->last_send = now;
        if (seated && !settling) send_to(j->s, j->st.host, join);
        else for (const JoinAddr &a : j->addrs) send_to(j->s, a, join);
    }
    JoinAddr from;
    std::vector<std::string> w;
    while (recv_from(j->s, &from, &w)) {
        if (w.size() < 2 || (uint32_t)strtoul(w[1].c_str(), nullptr, 10) != j->nonce) continue;
        j->last_heard = now;
        if (w[0] == "VER")
            return join_fail(j, "The host's game is another version than yours. Everyone needs the same version of "
                                "X-Men 2 for PC (the newest is on its GitHub releases page).");
        if (w[0] == "FULL") return join_fail(j, "That game is full, or it has started already.");
        if (w[0] == "BYE") return join_fail(j, "The host stopped the game.");
        if ((w[0] == "SEAT" && w.size() >= 5) || (w[0] == "GO" && w.size() >= 4)) {
            /* The host is where its answer came from (the address the game will dial). The first answer
               wins, unless a nearer kind (this network, then a VPN, then the internet) answers within 0.3 s. */
            if (!j->chosen || (now - j->first_seat < 0.3 && joinaddr_kind(from.ip) < joinaddr_kind(j->st.host.ip))) {
                if (!j->chosen) j->first_seat = now;
                j->chosen = true;
                j->st.host = from;
            }
            if (!(from.ip == j->st.host.ip && from.port == j->st.host.port)) continue;
            j->st.seat = atoi(w[2].c_str());
            if (w[0] == "SEAT") {
                j->st.players = atoi(w[3].c_str());
                j->st.here = atoi(w[4].c_str());
                j->st.state = RoomJoinStatus::SEATED;
            } else {
                j->st.seats = atoi(w[3].c_str());
                j->st.state = RoomJoinStatus::GO;
                sock_close(&j->s);
                return;
            }
        }
    }
    if (j->st.state == RoomJoinStatus::SEARCHING && now - j->opened > 8)
        return join_fail(j, "The host didn't answer. Check that the host's launcher still shows this code. Over the "
                            "internet, the host's router has to let the game in (the host's launcher says whether it "
                            "did); otherwise both players can join the same VPN, such as Tailscale.");
    if (j->st.state == RoomJoinStatus::SEATED && now - j->last_heard > 6)
        return join_fail(j, "Lost the host: its launcher closed or the connection dropped.");
}

void room_join_status(const RoomJoin *j, RoomJoinStatus *out) { *out = j->st; }

void room_join_close(RoomJoin **pj)
{
    RoomJoin *j = *pj;
    if (!j) return;
    if (j->s != SOCK_BAD && j->st.state == RoomJoinStatus::SEATED) send_to(j->s, j->st.host, MAGIC "LEAVE " + std::to_string(j->nonce));
    sock_close(&j->s);
    delete j;
    *pj = nullptr;
}
