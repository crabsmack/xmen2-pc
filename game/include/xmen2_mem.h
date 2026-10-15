/* xmen2_mem.h - the one switch between the two compilers of the hand-written C.
 *
 * Every game/c/<ADDR>.c is compiled twice:
 *
 *   MPW  (applec): Apple MPW C 3.3, -mbg off -b, the game's own compiler. The object code
 *        must equal the ROM's bytes (xmen2disasm tools/cc.py --match-recomp). Game memory
 *        is the machine's own memory: plain C types and pointers.
 *   PC   (C++17): inside the recomp runner. Game memory is still the emulated 68000's
 *        (work RAM, ROM; big-endian), because the generated code of the functions not yet
 *        written in C shares it. The types below make plain-looking C read and write it.
 *
 * Rules for C written against these headers (lint: xmen2check.py lint):
 *   - s8 u8 s16 u16 s32 u32 for every value that lives in game memory: struct fields,
 *     globals, whatever a pointer points at. Plain short/long/int for locals and value
 *     parameters (they live on the host).
 *   - Pointers are plain C pointers (T *). A pointer stored IN game memory (a field, a
 *     global) is PTR(T); a function pointer stored there has a type made by FNTYPE().
 *   - Hardware is reached through the names in this file, never by address.
 *   - No register access: the C never sees the 68000.
 *
 * PC mode, how pointers keep their 68000 value: the 68000 has 24 address bits, so the same
 * work-RAM byte is $00FFxxxx or $FFFFxxxx depending on how the original code computed the
 * pointer (A5-relative globals give $FFFFxxxx, absolute constants $00FFxxxx), and the
 * original stores both forms in RAM. The runner maps work RAM twice (g_ram = $00FFxxxx,
 * g_ram_ffff = $FFFFxxxx, the same memory): a host pointer into either view converts back
 * to exactly the 68000 value the original had, and pointer comparisons behave as on the
 * 68000 (the two forms compare unequal there too).
 */
#ifndef XMEN2_MEM_H
#define XMEN2_MEM_H

#if defined(applec) || defined(XMEN2_MPW)
/* ======================================================================== */
/* MPW C 3.3                                                                 */
/* ======================================================================== */
#define XMEN2_MPW 1

typedef signed char    s8;
typedef unsigned char  u8;
typedef short          s16;
typedef unsigned short u16;
typedef long           s32;
typedef unsigned long  u32;

#define PTR(T)                 T *
/* FNTYPE(StateFn, short) declares StateFn, a function pointer kept in game memory: old-style
   (any arguments, as the original C declared them), returning short. */
#define FNTYPE(name, ret)      typedef ret (*name)()
/* A class method slot. */
typedef long (*AnyFn)();

/* ROM data the code reaches the way the original did: by naming it as code (MPW gives
   such a name a jump-table entry; the real address is the jmp target at entry+2). */
#define CLASS(code)            (*(struct Class **)((char *)(code) + 2))
#define CODEPTR(code)          (*(void **)((char *)(code) + 2))
/* A ROM string: MPW -b places the literal in the code, as the original. */
#define STR(name, text)        (text)

/* Hardware (68000 addresses) */
#define HW8(a)                 (*(volatile u8 *)(a))
#define HW16(a)                (*(volatile s16 *)(a))
#define HW32(a)                (*(volatile s32 *)(a))

#else
/* ======================================================================== */
/* PC: C++ over the emulated 68000 memory                                    */
/* ======================================================================== */
#ifndef __cplusplus
#error "hand-written C builds as C++ on PC (xmen2_mem.h)"
#endif
#include <stddef.h>
#include <stdint.h>
#include <type_traits>

extern "C" {
extern uint8_t *g_ram;        /* work RAM as $00FFxxxx (the engine's view) */
extern uint8_t *g_ram_ffff;   /* the same RAM as $FFFFxxxx */
extern uint8_t  g_rom[0x400000];
uint8_t  m68k_read8(uint32_t addr);
uint16_t m68k_read16(uint32_t addr);
uint32_t m68k_read32(uint32_t addr);
void     m68k_write8(uint32_t addr, uint8_t v);
void     m68k_write16(uint32_t addr, uint16_t v);
void     m68k_write32(uint32_t addr, uint32_t v);
}

namespace xm {

[[noreturn]] void fatal(const char *fmt, ...);

/* 68000 address -> host pointer (0 -> null). */
inline void *host(uint32_t a)
{
    if (!a) return nullptr;
    uint32_t a24 = a & 0xFFFFFFu;
    if (a24 >= 0xFF0000u) {
        uint32_t top = a >> 24;
        if (top == 0x00u) return g_ram + (a24 & 0xFFFFu);
        if (top == 0xFFu) return g_ram_ffff + (a24 & 0xFFFFu);
    } else if (a == a24 && a24 < 0x400000u) {
        return g_rom + a24;
    }
    fatal("xm::host: $%08X is not a work-RAM or ROM address", (unsigned)a);
}

/* host pointer -> the 68000 address it stands for (null -> 0). */
inline uint32_t addr(const void *p)
{
    if (!p) return 0;
    uintptr_t u = (uintptr_t)p;
    if (u - (uintptr_t)g_ram < 0x10000u)      return 0x00FF0000u + (uint32_t)(u - (uintptr_t)g_ram);
    if (u - (uintptr_t)g_ram_ffff < 0x10000u) return 0xFFFF0000u + (uint32_t)(u - (uintptr_t)g_ram_ffff);
    if (u - (uintptr_t)g_rom < 0x400000u)     return (uint32_t)(u - (uintptr_t)g_rom);
    fatal("xm::addr: host pointer %p is not in game memory", p);
}

/* A big-endian scalar in game memory: reads and writes like the T it holds. */
template <class T>
struct be {
    static_assert(sizeof(T) == 2 || sizeof(T) == 4, "be<T>: 16 or 32 bits");
    uint8_t b[sizeof(T)];

    T get() const
    {
        if constexpr (sizeof(T) == 2)
            return (T)(uint16_t)((b[0] << 8) | b[1]);
        else
            return (T)(((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | b[3]);
    }
    void set(T v)
    {
        uint32_t u = (uint32_t)v;
        if constexpr (sizeof(T) == 2) {
            b[0] = (uint8_t)(u >> 8); b[1] = (uint8_t)u;
        } else {
            b[0] = (uint8_t)(u >> 24); b[1] = (uint8_t)(u >> 16); b[2] = (uint8_t)(u >> 8); b[3] = (uint8_t)u;
        }
    }
    be() = default;
    be(T v) { set(v); }
    operator T() const { return get(); }
    be &operator=(T v) { set(v); return *this; }
    template <class U> be &operator+=(U v)  { set((T)(get() + v)); return *this; }
    template <class U> be &operator-=(U v)  { set((T)(get() - v)); return *this; }
    template <class U> be &operator*=(U v)  { set((T)(get() * v)); return *this; }
    template <class U> be &operator/=(U v)  { set((T)(get() / v)); return *this; }
    template <class U> be &operator%=(U v)  { set((T)(get() % v)); return *this; }
    template <class U> be &operator&=(U v)  { set((T)(get() & v)); return *this; }
    template <class U> be &operator|=(U v)  { set((T)(get() | v)); return *this; }
    template <class U> be &operator^=(U v)  { set((T)(get() ^ v)); return *this; }
    template <class U> be &operator<<=(U v) { set((T)(get() << v)); return *this; }
    template <class U> be &operator>>=(U v) { set((T)(get() >> v)); return *this; }
    be &operator++() { set((T)(get() + 1)); return *this; }
    be &operator--() { set((T)(get() - 1)); return *this; }
    T operator++(int) { T o = get(); set((T)(o + 1)); return o; }
    T operator--(int) { T o = get(); set((T)(o - 1)); return o; }
};

/* A pointer stored in game memory: the 68000 address, big-endian. */
template <class T>
struct bptr {
    be<uint32_t> v;

    T *get() const { return (T *)host(v.get()); }
    uint32_t raw() const { return v.get(); }
    bptr() = default;
    bptr(T *p) { v = addr(p); }
    operator T *() const { return get(); }
    T *operator->() const { return get(); }
    bptr &operator=(T *p) { v = addr(p); return *this; }
    bptr &operator=(decltype(nullptr)) { v = 0u; return *this; }
    bptr &operator=(int null) { if (null) fatal("pointer = %d", null); v = 0u; return *this; }   /* p = 0 */
    bptr &operator+=(long n) { v = v.get() + (uint32_t)(n * (long)sizeof(T)); return *this; }
    bptr &operator-=(long n) { v = v.get() - (uint32_t)(n * (long)sizeof(T)); return *this; }
    bptr &operator++() { return *this += 1; }
    bptr &operator--() { return *this -= 1; }
    T *operator++(int) { T *o = get(); *this += 1; return o; }
    T *operator--(int) { T *o = get(); *this -= 1; return o; }
};
template <>
struct bptr<void> {
    be<uint32_t> v;
    void *get() const { return host(v.get()); }
    uint32_t raw() const { return v.get(); }
    bptr() = default;
    bptr(void *p) { v = addr(p); }
    operator void *() const { return get(); }
    template <class U> operator U *() const { return (U *)get(); }
    bptr &operator=(void *p) { v = addr(p); return *this; }
    bptr &operator=(decltype(nullptr)) { v = 0u; return *this; }
    bptr &operator=(int null) { if (null) fatal("pointer = %d", null); v = 0u; return *this; }   /* p = 0 */
};

/* ---- calls into 68000 code (xmen2_bridge.cpp, generated) ------------------- */
/* One argument slot as the original pushed it: 4 bytes, integers extended per their
   type, pointers as their 68000 address. */
uint32_t fn_value(const void *hostfn);
template <class A> inline uint32_t slot(A a)
{
    if constexpr (std::is_pointer<A>::value && std::is_function<typename std::remove_pointer<A>::type>::value)
        return fn_value((const void *)a);                       /* a C function: its 68000 value */
    else if constexpr (std::is_pointer<A>::value || std::is_null_pointer<A>::value) return addr((const void *)a);
    else if constexpr (std::is_enum<A>::value) return (uint32_t)(int32_t)a;
    else if constexpr (std::is_integral<A>::value) return std::is_signed<A>::value ? (uint32_t)(int32_t)a : (uint32_t)a;
    else if constexpr (std::is_same<A, be<int16_t>>::value || std::is_same<A, be<int32_t>>::value) return (uint32_t)(int32_t)a.get();
    else if constexpr (std::is_same<A, be<uint16_t>>::value || std::is_same<A, be<uint32_t>>::value) return (uint32_t)a.get();
    else return a.raw();                                        /* bptr<>, fnp<>: the stored value */
}
/* The d0 a 68000 function returned, as the C type the caller expects. */
struct ret68 {
    uint32_t d0;
    template <class R> operator R *() const { return (R *)host(d0); }
    operator long() const { return (long)(int32_t)d0; }
    operator unsigned long() const { return (unsigned long)d0; }
    operator int() const { return (int)(int32_t)d0; }
    operator unsigned() const { return (unsigned)d0; }
    operator short() const { return (short)(uint16_t)d0; }
    operator unsigned short() const { return (unsigned short)d0; }
    operator signed char() const { return (signed char)(uint8_t)d0; }
    operator unsigned char() const { return (unsigned char)d0; }
    operator char() const { return (char)(uint8_t)d0; }
    operator bool() const { return d0 != 0; }
};
/* Call the 68000 code at `a` (generated, or C behind its slot entry) with argument slots
   pushed right to left, as an MPW C caller does. */
uint32_t call_slots(uint32_t a, const uint32_t *slots, int n);
template <class... A> inline ret68 call68(uint32_t a, A... args)
{
    uint32_t s[sizeof...(A) + 1] = { slot(args)... };
    return ret68{ call_slots(a, s, (int)sizeof...(A)) };
}
/* The 68000 value a function's address had where the current C function took it
   (direct, or its jump-table entry when the original crossed segments). */
uint32_t fn_value(const void *hostfn);
const void *fn_host(uint32_t a);   /* a C function at that value, or null */

/* A function pointer stored in game memory, old-style as the original declared it: it takes
   any C function (its 68000 value: fn_value) and calls with any arguments; R is the result. */
template <class R>
struct fnp {
    be<uint32_t> v;
    uint32_t raw() const { return v.get(); }
    fnp() = default;
    template <class F, class = typename std::enable_if<std::is_function<F>::value>::type>
    fnp(F *f) { v = fn_value((const void *)f); }
    template <class F, class = typename std::enable_if<std::is_function<F>::value>::type>
    fnp &operator=(F *f) { v = fn_value((const void *)f); return *this; }
    fnp &operator=(decltype(nullptr)) { v = 0u; return *this; }
    fnp &operator=(int null) { if (null) fatal("function pointer = %d", null); v = 0u; return *this; }
    explicit operator bool() const { return v.get() != 0; }
    template <class F> bool operator==(F *f) const { return v.get() == fn_value((const void *)f); }
    template <class F> bool operator!=(F *f) const { return !(*this == f); }
    bool operator==(decltype(nullptr)) const { return v.get() == 0; }
    bool operator!=(decltype(nullptr)) const { return v.get() != 0; }
    template <class Q> bool operator==(const fnp<Q> &o) const { return v.get() == o.v.get(); }
    const fnp &operator*() const { return *this; }
    template <class... A> R operator()(A... a) const
    {
        if constexpr (std::is_void<R>::value) (void)call68(v.get(), a...);
        else return (R)call68(v.get(), a...);
    }
};
/* ---- hardware: every access goes through the runner's 68000 bus ------------ */
struct hw8 {
    uint32_t a;
    operator uint8_t() const { return m68k_read8(a); }
    const hw8 &operator=(uint8_t v) const { m68k_write8(a, v); return *this; }
};
struct hw16 {
    uint32_t a;
    operator uint16_t() const { return m68k_read16(a); }
    const hw16 &operator=(uint16_t v) const { m68k_write16(a, v); return *this; }
};
struct hw32 {
    uint32_t a;
    operator uint32_t() const { return m68k_read32(a); }
    const hw32 &operator=(uint32_t v) const { m68k_write32(a, v); return *this; }
};

/* ROM bytes at a 68000 address (data the C names: xmen2_data.h). */
inline void *rom(uint32_t a) { return g_rom + a; }

}  /* namespace xm */

typedef int8_t                s8;
typedef uint8_t               u8;
typedef xm::be<int16_t>       s16;
typedef xm::be<uint16_t>      u16;
typedef xm::be<int32_t>       s32;
typedef xm::be<uint32_t>      u32;

/* The original wrote egister where it wanted a variable in a register (it decides MPW's
   register choice, so matching keeps it); C++17 has no register storage class. */
#define register

#define PTR(T)                 xm::bptr<T>
#define FNTYPE(name, ret)      typedef xm::fnp<ret> name
typedef xm::fnp<xm::ret68> AnyFn;   /* the result converts to what the caller casts it to */

#define CLASS(code)            ((struct Class *)(code))
#define CODEPTR(code)          ((void *)(code))
#define STR(name, text)        ((char *)(name))

#define HW8(a)                 (xm::hw8{ (uint32_t)(a) })
#define HW16(a)                (xm::hw16{ (uint32_t)(a) })
#define HW32(a)                (xm::hw32{ (uint32_t)(a) })

#endif /* PC */

/* ======================================================================== */
/* Hardware names (both modes)                                               */
/* ======================================================================== */
#define VDP_DATA       HW16(0xC00000)   /* VDP data port */
#define VDP_DATA32     HW32(0xC00000)
#define VDP_CTRL       HW16(0xC00004)   /* VDP control port: status (read), commands (write) */
#define VDP_CTRL32     HW32(0xC00004)
#define VDP_HV         HW16(0xC00008)   /* H/V counter */
#define PSG_PORT       HW8(0xC00011)    /* SN76489 */
#define IO_VERSION     HW8(0xA10001)    /* region / version register */
#define IO_DATA1       HW8(0xA10003)    /* pad port 1 data */
#define IO_DATA2       HW8(0xA10005)    /* pad port 2 data */
#define IO_CTRL1       HW8(0xA10009)    /* pad port 1 control */
#define IO_CTRL2       HW8(0xA1000B)    /* pad port 2 control */
#define Z80_BUSREQ     HW16(0xA11100)   /* Z80 bus request */
#define Z80_RESET      HW16(0xA11200)   /* Z80 reset */
#define TMSS_PORT      HW32(0xA14000)   /* TMSS "SEGA" */
#define Z80_RAM(off)   HW8(0xA00000 + (off))   /* Z80 RAM byte (bus held) */

#endif /* XMEN2_MEM_H */
