/* joincode.cpp - join codes: see joincode.h */
#include "joincode.h"

#include <stdio.h>
#include <stdlib.h>

namespace {

const char k_alpha[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
const uint16_t k_default_port = 7000;

uint8_t crc8(const std::vector<uint8_t> &b, size_t n)
{
    uint8_t c = 0;
    for (size_t i = 0; i < n; i++) {
        c ^= b[i];
        for (int k = 0; k < 8; k++) c = (uint8_t)(c & 0x80 ? (c << 1) ^ 0x07 : c << 1);
    }
    return c;
}

int digit(char ch)
{
    if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 32);
    if (ch == 'O') ch = '0';
    if (ch == 'I' || ch == 'L') ch = '1';
    for (int i = 0; i < 32; i++)
        if (k_alpha[i] == ch) return i;
    return -1;
}

/* an address's form in the code: 0 all four bytes, 1 192.168.a.b (2), 2 10.a.b.c (3), 3 100.64-127.b.c (3) */
int form(uint32_t ip)
{
    if ((ip >> 16) == 0xC0A8) return 1;
    if ((ip >> 24) == 10) return 2;
    if ((ip >> 22) == (100u << 2 | 1)) return 3;
    return 0;
}
const int k_form_bytes[4] = {4, 2, 3, 3};

/* "a.b.c.d[:port]" */
bool parse_address(const std::string &t, uint16_t default_port, JoinAddr *out)
{
    unsigned a, b, c, d, port = default_port;
    int used = -1;
    if (t.find(':') == std::string::npos) {
        if (sscanf(t.c_str(), "%u.%u.%u.%u%n", &a, &b, &c, &d, &used) != 4) return false;
    } else if (sscanf(t.c_str(), "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &used) != 5) return false;
    if (used != (int)t.size()) return false;
    if (a > 255 || b > 255 || c > 255 || d > 255 || port < 1 || port > 65535) return false;
    out->ip = a << 24 | b << 16 | c << 8 | d;
    out->port = (uint16_t)port;
    return true;
}

}   // namespace

std::string joincode_encode(const std::vector<JoinAddr> &addrs)
{
    size_t n = addrs.size() > 4 ? 4 : addrs.size();
    if (!n) return "";
    bool shared = true;
    for (size_t i = 1; i < n; i++) shared &= addrs[i].port == addrs[0].port;
    bool dflt = shared && addrs[0].port == k_default_port;
    std::vector<uint8_t> b;
    b.push_back((uint8_t)(0x20 | (dflt ? 0x10 : 0) | (shared ? 0 : 0x08) | (n - 1)));
    uint8_t forms = 0;
    for (size_t i = 0; i < n; i++) forms |= (uint8_t)(form(addrs[i].ip) << (6 - 2 * i));
    b.push_back(forms);
    if (shared && !dflt) { b.push_back((uint8_t)(addrs[0].port >> 8)); b.push_back((uint8_t)addrs[0].port); }
    for (size_t i = 0; i < n; i++) {
        int f = form(addrs[i].ip), nb = k_form_bytes[f];
        uint32_t v = f == 3 ? addrs[i].ip & 0x3FFFFF : addrs[i].ip;
        for (int k = nb - 1; k >= 0; k--) b.push_back((uint8_t)(v >> (8 * k)));
        if (!shared) { b.push_back((uint8_t)(addrs[i].port >> 8)); b.push_back((uint8_t)addrs[i].port); }
    }
    b.push_back(crc8(b, b.size()));
    std::string chars;
    uint32_t acc = 0;
    int bits = 0;
    for (uint8_t v : b) {
        acc = acc << 8 | v;
        bits += 8;
        while (bits >= 5) { chars += k_alpha[(acc >> (bits - 5)) & 31]; bits -= 5; }
    }
    if (bits) chars += k_alpha[(acc << (5 - bits)) & 31];
    std::string out = "XM2";
    for (size_t i = 0; i < chars.size(); i++) {
        if (i % 4 == 0) out += '-';
        out += chars[i];
    }
    return out;
}

bool joincode_decode(const std::string &text, uint16_t default_port, std::vector<JoinAddr> *out, std::string *why)
{
    out->clear();
    std::string t;
    for (char ch : text)
        if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') t += ch;
    if (t.empty()) { *why = "Paste the host's join code."; return false; }
    JoinAddr a;
    if (t.find('.') != std::string::npos) {
        if (parse_address(t, default_port, &a)) { out->push_back(a); return true; }
        *why = "That isn't a join code or an address (an address looks like 192.168.1.20:7000).";
        return false;
    }
    std::string c;
    for (char ch : t)
        if (ch != '-') c += ch;
    if (c.size() >= 3 && (c[0] == 'X' || c[0] == 'x') && (c[1] == 'M' || c[1] == 'm') && c[2] == '2') c.erase(0, 3);
    std::vector<uint8_t> b;
    uint32_t acc = 0;
    int bits = 0;
    for (char ch : c) {
        int v = digit(ch);
        if (v < 0) { *why = "That code has a character no join code has: copy it again."; return false; }
        acc = acc << 5 | (uint32_t)v;
        bits += 5;
        if (bits >= 8) { b.push_back((uint8_t)(acc >> (bits - 8))); bits -= 8; }
    }
    const char *incomplete = "That code isn't complete or has a typo: copy the whole code again.";
    /* the spare bits after the last byte are zero, the header's format is 1 */
    if (b.size() < 2 || (b[0] & 0xE0) != 0x20 || (acc & ((1u << bits) - 1))) { *why = incomplete; return false; }
    size_t n = (b[0] & 3) + 1;
    bool dflt = b[0] & 0x10, shared = !(b[0] & 0x08);
    if (dflt && !shared) { *why = incomplete; return false; }
    size_t len = 2 + (shared && !dflt ? 2 : 0) + 1;
    for (size_t i = 0; i < n; i++) len += k_form_bytes[(b[1] >> (6 - 2 * i)) & 3] + (shared ? 0 : 2);
    if (b.size() != len || crc8(b, len - 1) != b[len - 1] || (b[1] & ((1u << (8 - 2 * n)) - 1) & 0xFF)) {
        *why = incomplete;
        return false;
    }
    size_t p = 2;
    uint16_t port = k_default_port;
    if (shared && !dflt) { port = (uint16_t)(b[2] << 8 | b[3]); p = 4; }
    for (size_t i = 0; i < n; i++) {
        int f = (b[1] >> (6 - 2 * i)) & 3;
        uint32_t v = 0;
        for (int k = 0; k < k_form_bytes[f]; k++) v = v << 8 | b[p++];
        a.ip = f == 1 ? 0xC0A80000u | v : f == 2 ? 0x0A000000u | v : f == 3 ? 0x64400000u | (v & 0x3FFFFF) : v;
        if (!shared) { port = (uint16_t)(b[p] << 8 | b[p + 1]); p += 2; }
        a.port = port;
        out->push_back(a);
    }
    return true;
}

JoinAddrKind joinaddr_kind(uint32_t a)
{
    if ((a >> 24) == 10 || (a >> 20) == 0xAC1 || (a >> 16) == 0xC0A8) return JA_LAN;
    if ((a >> 22) == (100u << 2 | 1)) return JA_VPN;
    return JA_INTERNET;
}

std::string joinaddr_text(const JoinAddr &a)
{
    char s[32];
    snprintf(s, sizeof s, "%u.%u.%u.%u:%u", a.ip >> 24, (a.ip >> 16) & 255, (a.ip >> 8) & 255, a.ip & 255, a.port);
    return s;
}
