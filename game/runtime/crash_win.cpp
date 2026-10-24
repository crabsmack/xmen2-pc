/* crash_win.cpp - a last word before a runner dies of a host exception (Windows).
 *
 * The 68000 reads address 0 harmlessly where the PC dereferences null, so a C function that
 * misses such a case kills the runner with an access violation and no message. A vectored
 * handler calls the report a runner installs (verify.cpp: the tests open and the C function
 * running), then lets the exception go on as before.
 *
 * Every runner (xm_install_crash_trace) also prints, for an exception nobody handles, where it
 * happened and the host stack above it: `[crash] exception C0000005 at XMen2Verify.exe+0x1234
 * (func_01F578+0x2c)`, then `[crash]  #1 ...` per frame, unwound with the x64 unwind tables (no
 * PDB needed) and named from the linker's map ($XMEN2_CRASH_MAP, else the .map beside the
 * runner). XMEN2_CRASH_FRAME=N makes the runner fault at runner frame N (xm_crash_test): the test
 * that the report works. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

static void (*s_report)(unsigned long code, void *addr);

static LONG CALLBACK on_exception(EXCEPTION_POINTERS *e)
{
    DWORD code = e->ExceptionRecord->ExceptionCode;
    if (s_report && (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_STACK_OVERFLOW
                     || code == EXCEPTION_INT_DIVIDE_BY_ZERO)) {
        void (*r)(unsigned long, void *) = s_report;
        s_report = nullptr;   /* once */
        r(code, e->ExceptionRecord->ExceptionAddress);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" void xm_install_crash_report(void (*report)(unsigned long code, void *addr))
{
    s_report = report;
    AddVectoredExceptionHandler(1, on_exception);
}

/* ---- the stack trace ---------------------------------------------------------------------- */

static std::vector<std::pair<uint64_t, std::string>> s_syms;   /* (rva, name), sorted */
static bool s_syms_read;

static void read_map(void)
{
    s_syms_read = true;
    char path[MAX_PATH * 2] = "";
    if (const char *m = getenv("XMEN2_CRASH_MAP"); m && *m) {
        snprintf(path, sizeof path, "%s", m);
    } else {
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        char *dot = strrchr(path, '.');
        if (dot) strcpy(dot, ".map");
    }
    FILE *f = fopen(path, "r");
    if (!f) return;
    char line[2048];
    unsigned long long preferred = 0x140000000ull;
    while (fgets(line, sizeof line, f)) {
        unsigned long long p;
        if (sscanf(line, " Preferred load address is %llx", &p) == 1) { preferred = p; continue; }
        unsigned sec, off;
        char name[1024];
        unsigned long long va;
        if (sscanf(line, " %x:%x %1023s %llx", &sec, &off, name, &va) == 4 && va > preferred)
            s_syms.push_back({ va - preferred, name });
    }
    fclose(f);
    std::sort(s_syms.begin(), s_syms.end());
}

/* "module+0xRVA (symbol+0xoff)" */
static std::string where(uint64_t pc)
{
    HMODULE mod = nullptr;
    char mpath[MAX_PATH] = "?";
    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCSTR)(uintptr_t)pc, &mod) || !mod) {
        char b[64];
        snprintf(b, sizeof b, "0x%llx", (unsigned long long)pc);
        return b;
    }
    GetModuleFileNameA(mod, mpath, sizeof mpath);
    const char *base = strrchr(mpath, '\\');
    base = base ? base + 1 : mpath;
    uint64_t rva = pc - (uint64_t)(uintptr_t)mod;
    char b[1400];
    snprintf(b, sizeof b, "%s+0x%llx", base, (unsigned long long)rva);
    std::string s = b;
    if (mod == GetModuleHandleA(nullptr)) {
        if (!s_syms_read) read_map();
        auto it = std::upper_bound(s_syms.begin(), s_syms.end(), std::make_pair(rva, std::string("\x7f")));
        if (it != s_syms.begin()) {
            --it;
            snprintf(b, sizeof b, " (%s+0x%llx)", it->second.c_str(), (unsigned long long)(rva - it->first));
            s += b;
        }
    }
    return s;
}

static void walk(CONTEXT c)
{
    for (int i = 1; i < 32 && c.Rip; i++) {
        DWORD64 image = 0;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(c.Rip, &image, nullptr);
        if (!fn) {   /* a leaf: its return address is on top of the stack */
            c.Rip = *(DWORD64 *)c.Rsp;
            c.Rsp += 8;
        } else {
            PVOID handler;
            DWORD64 frame;
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, image, c.Rip, fn, &c, &handler, &frame, nullptr);
        }
        if (!c.Rip) break;
        fprintf(stderr, "[crash]  #%d %s\n", i, where(c.Rip).c_str());
    }
}

static void walk_guarded(const CONTEXT *c)
{
    __try {
        walk(*c);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        fprintf(stderr, "[crash]  (the stack walk stopped: unreadable stack)\n");
    }
}

static LONG WINAPI on_unhandled(EXCEPTION_POINTERS *e)
{
    static volatile LONG once;
    if (InterlockedExchange(&once, 1)) return EXCEPTION_CONTINUE_SEARCH;
    fprintf(stderr, "[crash] exception %08lX at %s\n", (unsigned long)e->ExceptionRecord->ExceptionCode,
            where((uint64_t)(uintptr_t)e->ExceptionRecord->ExceptionAddress).c_str());
    walk_guarded(e->ContextRecord);
    fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" void xm_install_crash_trace(void)
{
    static bool done;
    if (done) return;
    done = true;
    SetUnhandledExceptionFilter(on_unhandled);
}
#else
extern "C" void xm_install_crash_report(void (*)(unsigned long, void *)) {}
extern "C" void xm_install_crash_trace(void) {}
#endif

/* XMEN2_CRASH_FRAME=N: fault at runner frame N (the crash report's own test) */
extern "C" void xm_crash_test(uint64_t frame)
{
    static long long at = -2;
    if (at == -2) {
        const char *e = getenv("XMEN2_CRASH_FRAME");
        at = e && *e ? atoll(e) : -1;
    }
    if ((long long)frame == at) {
        fprintf(stderr, "[crash] XMEN2_CRASH_FRAME=%lld: faulting on purpose\n", at);
        fflush(stderr);
        *(volatile int *)(uintptr_t)0 = 0;
    }
}
