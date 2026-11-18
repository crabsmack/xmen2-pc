/* joincode.h - the launcher's join codes (launcher.cpp, room.cpp): the host's addresses in one code a player
 * can paste, with no server anywhere (P7-multiplayer.md R8: "No lobby server").
 *
 *   XM2-5AB3-K9QF-M2PX-TV7     (a public address and a home network's, port 7000)
 *
 * Crockford base32 (no I, L, O, U; read case-blind, O as 0 and I/L as 1, dashes and spaces ignored) of:
 *   byte 0      0x20 (format 1) | 0x10 port 7000 for all | 0x08 a port per address | n-1 (n = 1..4 addresses)
 *   byte 1      each address's form, 2 bits from the top: 0 all 4 bytes, 1 192.168.a.b (2 bytes),
 *               2 10.a.b.c (3), 3 100.64.0.0/10 (a VPN such as Tailscale: its 22 bits in 3 bytes)
 *   [port]      2 bytes, big-endian: one for all (neither flag), or after each address (0x08)
 *   n addresses in their forms
 *   CRC-8       (polynomial 0x07) of everything before it; the spare bits of the last character are 0:
 *               a mistyped code is refused, not tried
 * The "XM2" in front is a label: the payload's first character is 4-7, never X.
 */
#pragma once
#include <stdint.h>
#include <string>
#include <vector>

struct JoinAddr {
    uint32_t ip;     /* host byte order: 192.168.1.20 = 0xC0A80114 */
    uint16_t port;
};

enum JoinAddrKind { JA_LAN, JA_VPN, JA_INTERNET };   /* RFC 1918, 100.64.0.0/10 (Tailscale's), the rest */

std::string  joincode_encode(const std::vector<JoinAddr> &addrs);
/* A code, or an address typed in its place ("192.168.1.20" or "192.168.1.20:7000"; the port defaults to
   default_port). false and *why (a sentence for the player) when it's neither. */
bool         joincode_decode(const std::string &text, uint16_t default_port, std::vector<JoinAddr> *out, std::string *why);
JoinAddrKind joinaddr_kind(uint32_t ip);
std::string  joinaddr_text(const JoinAddr &a);   /* "192.168.1.20:7000" */
