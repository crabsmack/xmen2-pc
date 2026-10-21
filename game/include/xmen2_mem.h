/* xmen2_mem.h - the one switch between the compilers (and memory models) of the hand-written C.
 *
 * Every game/c/<ADDR>.c is compiled in three modes:
 *
 *   MPW  (applec): Apple MPW C 3.3, -mbg off -b, the game's own compiler. The object code
 *        must equal the ROM's bytes (xmen2disasm tools/cc.py --match-recomp). Game memory
 *        is the machine's own memory: plain C types and pointers.
 *   PC   (C++17): inside the recomp runner. Game memory is still the emulated 68000's
 *        (work RAM, ROM; big-endian), because the generated code of the functions not yet
 *        written in C shares it. The types below make plain-looking C read and write it.
 *   NATIVE (C++17, XMEN2_NATIVE; the XMen2Native runner, phase 5): where the native types
 *        land, game state in host structs and pointers off the emulated machine. Today it IS
 *        PC mode (it builds on it and changes nothing yet). XMen2Native is XMen2Stepped plus
 *        game/runtime/export.cpp, which writes the game state out as the 68000 image (work
 *        RAM, VRAM, CRAM, VSRAM) its sync rows are computed from, so the stepped-gen
 *        references still decide equality. A global moved to native storage here keeps its
 *        entry in the export table (tools/export_table.py, from xmen2_globals.h), which then
 *        serializes it from the native storage by its declared type.
 *
 * Rules for C written against these headers (lint: xmen2check.py lint):
 *   - s8 u8 s16 u16 s32 u32 for every value that lives in game memory: struct fields,
 *     globals, whatever a pointer points at. Plain short/long/int for locals and value
 *     parameters (they live on the host).
 *   - Pointers are plain C pointers (T *). A pointer stored IN game memory (a field, a
 *     global) is PTR(T); a function pointer stored there has a type made by FNTYPE().
 *   - Hardware is reached through the platform layer's names (platform/xmen2_platform.h),
 *     never by address.
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
/* FN_AT(StateFn, p): code at p (ROM the C has no function for, e.g. an AI script's jmp
   thunk) as a stored function pointer of type `type`. */
#define FN_AT(type, p)         ((type)(p))
/* FN_DATA(f): the address of function f passed as data (a void *), e.g. a V-blank hook given
   to Timer_SetVBlankHookCb as its timer argument (Hologram_Init). */
#define FN_DATA(f)             ((void *)(f))
/* A class method slot. */
typedef long (*AnyFn)();

/* ROM data the code reaches the way the original did: by naming it as code (MPW gives
   such a name a jump-table entry; the real address is the jmp target at entry+2). */
#define CLASS(code)            (*(struct Class **)((char *)(code) + 2))
#define CODEPTR(code)          (*(void **)((char *)(code) + 2))
/* A ROM string: MPW -b places the literal in the code, as the original. */
#define STR(name, text)        (text)

/* The address the 68000 bus sees for p, as a number (a DMA source, a sound-driver pointer),
   and back: a pointer to T at a 68000 address computed as a number (work RAM or ROM). */
#define ADDR68(p)              ((unsigned long)(p))
#define AT68(T, a)             ((T *)(a))

#else
/* ======================================================================== */
/* PC: C++ over the emulated 68000 memory                                    */
/* ======================================================================== */
#ifndef __cplusplus
#error "hand-written C builds as C++ on PC (xmen2_mem.h)"
#endif
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

/* MPW C's long is 32 bits, and so was the PC's (Windows is LLP64). An LP64 host (Linux) makes it
   64, and the C's long arithmetic then wraps elsewhere: map it to int for every file that includes
   this header, which therefore comes after all of that file's system headers. */
#if defined(__LP64__)
#define long int
#endif
static_assert(sizeof(long) == 4, "xmen2_mem.h: the C's long is 32 bits");

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
    /* explicit: `c ? 5 : field` then converts the field to its T (as C promotes it), not 5 to a
       be<T> (both ways make ?: ambiguous, which MSVC let through and GCC rejects) */
    explicit be(T v) { set(v); }
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
#if !defined(__LP64__)   /* there long is int (above) */
    operator long() const { return (long)(int32_t)d0; }
    operator unsigned long() const { return (unsigned long)d0; }
#endif
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
uint32_t sp_reserve(uint32_t bytes);   /* A7 -= bytes (even); the new A7 */
/* NO_RETURN_VALUE (xmen2_macros.h): a C function ended where the original returns no value.
   g_frame is the running C function's call (the bridge numbers them), g_novalue the call that
   ended so, g_retnv whether the call that just returned did (its slot entry leaves d0 alone). */
extern uint32_t g_frame, g_seq, g_novalue;
extern bool g_retnv;
void     sp_restore(uint32_t a7);
/* The stack region of game memory: the original kept some locals whose addresses go into game
   memory (a lift, a palette, select slots) on its stack, so the C places them where the original's
   frame had them: frame_alloc(n) takes n bytes (even) below the mark, frame_release(mark) gives
   back everything below it. */
uint32_t frame_mark(void);
uint32_t frame_alloc(uint32_t bytes);   /* the new mark: the bytes' address */
void     frame_release(uint32_t mark);

inline bool in_game_memory(const void *p)
{
    uintptr_t u = (uintptr_t)p;
    return u - (uintptr_t)g_ram < 0x10000u || u - (uintptr_t)g_ram_ffff < 0x10000u
        || u - (uintptr_t)g_rom < 0x400000u;
}

/* The arguments of one call into 68000 code. A pointer to host memory (a C function's local,
   e.g. a SprInst passed by address) travels on the 68000 stack: copied below A7 before the
   call, copied back after it, A7 restored. Game-memory types go as their bytes (they are the
   68000 layout); a plain scalar (char, short, long) in the 68000's byte order and size. A
   `void *` can't: its size is unknown. */
inline uint32_t g_call68_to;   /* the 68000 code being called (for the errors below) */
struct Scratch {
    uint32_t sp0 = 0;
    int nc = 0;
    struct { void *host; uint32_t a, n; bool scalar; } copy[8];
    ~Scratch()
    {
        for (int i = 0; i < nc; i++) {
            if (!copy[i].scalar) { memcpy(copy[i].host, host(copy[i].a), copy[i].n); continue; }
            if (copy[i].n == 1) { *(uint8_t *)copy[i].host = m68k_read8(copy[i].a); continue; }
            if (copy[i].n == 2) { uint16_t v = m68k_read16(copy[i].a); memcpy(copy[i].host, &v, 2); continue; }
            uint32_t v = m68k_read32(copy[i].a);
            memcpy(copy[i].host, &v, 4);
        }
        if (nc) sp_restore(sp0);
    }
    template <class A> uint32_t arg(A a)
    {
        if constexpr (std::is_pointer<A>::value && !std::is_function<typename std::remove_pointer<A>::type>::value) {
            if (!a || in_game_memory((const void *)a)) return addr((const void *)a);
            typedef typename std::remove_cv<typename std::remove_pointer<A>::type>::type T;
            if constexpr (std::is_void<T>::value) {
                fatal("call68 to $%06X: a host void pointer can't be passed to 68000 code (its size is unknown)",
                      (unsigned)g_call68_to);
            } else if constexpr (std::is_arithmetic<T>::value) {
                static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4, "call68: a 1, 2 or 4 byte scalar");
                if (nc == 8) fatal("call68: too many host pointers");
                if (!nc) sp0 = sp_reserve(0);
                uint32_t a68 = sp_reserve(sizeof(T) < 2 ? 2u : (uint32_t)sizeof(T));
                if (sizeof(T) == 1) m68k_write8(a68, *(const uint8_t *)a);
                else if (sizeof(T) == 2) { uint16_t v; memcpy(&v, (const void *)a, 2); m68k_write16(a68, v); }
                else { uint32_t v; memcpy(&v, (const void *)a, 4); m68k_write32(a68, v); }
                copy[nc].host = (void *)a; copy[nc].a = a68; copy[nc].n = (uint32_t)sizeof(T); copy[nc].scalar = true;
                nc++;
                return a68;
            } else {
                if (nc == 8) fatal("call68: too many host pointers");
                uint32_t n = ((uint32_t)sizeof(T) + 1u) & ~1u;
                if (!nc) sp0 = sp_reserve(0);
                uint32_t a68 = sp_reserve(n);
                memcpy(host(a68), (const void *)a, sizeof(T));
                copy[nc].host = (void *)a; copy[nc].a = a68; copy[nc].n = (uint32_t)sizeof(T); copy[nc].scalar = false;
                nc++;
                return a68;
            }
        } else {
            return slot(a);
        }
    }
};
template <class... A> inline ret68 call68(uint32_t a, A... args)
{
    g_call68_to = a & 0xFFFFFFu;
    Scratch sc;
    uint32_t s[sizeof...(A) + 1] = { sc.arg(args)... };
    return ret68{ call_slots(a, s, (int)sizeof...(A)) };
}
#ifdef XMEN2_NATIVE
/* NATIVE (phase 5 P3): a call through a function value stored in game memory runs the C function
   at that value directly, through a thunk typed by its prototype (bridge.py: call_native and its
   table). An argument travels as narg: a value as the slot the original pushed (slot()); a pointer
   to host memory (a caller's local) as itself, with `stack` = the bytes call68 reserves for its
   copy, so that the stack region keeps the original's addresses (xm::frame_alloc). */
struct narg { uint32_t v; const void *p; uint32_t stack; };
template <class A> inline narg make_narg(A a)
{
    if constexpr (std::is_pointer<A>::value && !std::is_function<typename std::remove_pointer<A>::type>::value) {
        if (!a || in_game_memory((const void *)a)) return narg{ addr((const void *)a), nullptr, 0u };
        typedef typename std::remove_cv<typename std::remove_pointer<A>::type>::type T;
        uint32_t n = 0;
        if constexpr (std::is_arithmetic<T>::value) n = sizeof(T) < 2 ? 2u : (uint32_t)sizeof(T);
        else if constexpr (!std::is_void<T>::value) n = ((uint32_t)sizeof(T) + 1u) & ~1u;
        return narg{ 0u, (const void *)a, n };
    } else {
        return narg{ slot(a), nullptr, 0u };
    }
}
uint32_t call_native(uint32_t a, const narg *args, int n);
template <class... A> inline ret68 calln(uint32_t a, A... args)
{
    narg s[sizeof...(A) + 1] = { make_narg(args)... };
    return ret68{ call_native(a, s, (int)sizeof...(A)) };
}
#endif
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
    /* (StateFn)self->cls->method[n]: a stored pointer cast to another stored type keeps its value */
    template <class Q> explicit fnp(const fnp<Q> &o) { v = o.raw(); }
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
#ifdef XMEN2_NATIVE
        if constexpr (std::is_void<R>::value) (void)calln(v.get(), a...);
        else return (R)calln(v.get(), a...);
#else
        if constexpr (std::is_void<R>::value) (void)call68(v.get(), a...);
        else return (R)call68(v.get(), a...);
#endif
    }
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

/* The original wrote 
egister where it wanted a variable in a register (it decides MPW's
   register choice, so matching keeps it); C++17 has no register storage class. */
#define register

#define PTR(T)                 xm::bptr<T>
#define FNTYPE(name, ret)      typedef xm::fnp<ret> name
typedef xm::fnp<xm::ret68> AnyFn;   /* the result converts to what the caller casts it to */
namespace xm {
template <class F> inline F fn_at(const void *p) { F f; f.v = addr(p); return f; }
}
#define FN_AT(type, p)         (xm::fn_at<type>(p))
/* a host pointer standing for the function's 68000 value (fn_value), so addr() gives it back */
#define FN_DATA(f)             (xm::host(xm::fn_value((const void *)(f))))

#define CLASS(code)            ((struct Class *)(code))
#define CODEPTR(code)          ((void *)(code))
#define STR(name, text)        ((char *)(name))

#define ADDR68(p)              ((unsigned long)xm::addr(p))
#define AT68(T, a)             ((T *)xm::host((uint32_t)(a)))

#ifdef XMEN2_NATIVE
/* ------------------------------------------------------------------------ */
/* NATIVE (phase 5) on top of PC mode: nothing differs yet. Native storage    */
/* and host types go here, each with its writer in the export table.          */
/* ------------------------------------------------------------------------ */
#endif

#endif /* PC */

/* ======================================================================== */
/* Hardware (all modes): the platform layer's names and API                  */
/* ======================================================================== */
#include "xmen2_platform.h"
#define WORK_RAM_START 0xFF0000L        /* first byte of the 68000's work RAM: the heap's base (Level_Load) */

/* A loop waiting for a flag only an interrupt handler sets (`while (!g_FrameTick) IRQ_WAIT();`):
   on the 68000 the interrupt simply arrives, so MPW compiles nothing for it; on PC the
   frame-synchronous runner takes the V-int there (plat_wait_vint). */
#ifdef XMEN2_MPW
#define IRQ_WAIT()
#else
#define IRQ_WAIT()             plat_wait_vint()
#endif

#endif /* XMEN2_MEM_H */
