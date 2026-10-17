/* crash_win.cpp - a last word before the runner dies of a host exception (Windows).
 *
 * The 68000 reads address 0 harmlessly where the PC dereferences null, so a C function that
 * misses such a case kills the runner with an access violation and no message. A vectored
 * handler calls the report a runner installs (verify.cpp: the tests open and the C function
 * running), then lets the exception go on as before. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

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
#else
extern "C" void xm_install_crash_report(void (*)(unsigned long, void *)) {}
#endif
