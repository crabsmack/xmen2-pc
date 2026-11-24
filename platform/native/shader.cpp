/* shader.cpp - RetroArch shader presets (.slangp) on the played picture: the shader_preset option.
 *
 * librashader (github.com/SnowflakePowered/librashader, MPL-2.0) reads the preset and runs its passes;
 * it is loaded at run time from the library beside the game (librashader.dll / librashader.so, through
 * librashader_ld.h, MIT), so without it the option says so once and the picture is the plain one.
 * Each platform's runtime on SDL's own renderer (host_sdl.cpp make_renderer):
 *
 * Windows, Direct3D 11. SDL 2 hands out its device but neither its textures nor its back buffer, so:
 *   the frame goes into a texture of ours on SDL's device (the w x h of the picture, made again when
 *   the view's size changes: 320 or 400 wide);
 *   the output is the back buffer's render target view, the one SDL binds for its own draws (a black
 *   fill over the cleared window, flushed, then OMGetRenderTargets);
 *   SDL caches the pipeline state it set (shaders, input layout, blend, samplers), and librashader
 *   changes all of it, so the chain runs in a device context state of its own
 *   (ID3D11DeviceContext1::SwapDeviceContextState), and SDL finds its state as it left it.
 *
 * Linux (and other non-Windows), OpenGL: SDL's "opengl" renderer (make_renderer asks for it when a preset
 * is set) on a context of at least 3.3. librashader's GL runtime draws into a texture, not the window, so:
 *   the frame goes into a texture of ours (as on Windows);
 *   the chain draws into an output texture of ours the size of the picture's place in the window, which
 *   a framebuffer blit then copies into the window's (default) framebuffer;
 *   SDL caches the GL state it set (program, bound textures, blend, viewport, scissor, framebuffer,
 *   client-side vertex arrays that need no buffer bound), so everything the chain can change is read
 *   before it and put back after (GlState).
 *
 * The hotkey messages (host_osd) are drawn after the chain by host_sdl.cpp, so they aren't shaded.
 * A preset that fails to load or a frame that fails: one stderr line, then the plain picture (never a
 * message box in play). Headless runs open no window, so no chain: traces and digests are untouched.
 *
 * Its cost: GPU timestamps around each chain frame, summed up at exit (`[native] shader: ...`).
 */
#include "shader.h"

#include <SDL.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <vector>

#include "host_sdl.h"
#include "native.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d11_1.h>
#define LIBRA_RUNTIME_D3D11
#define XMEN2_LIBRA_FILE "librashader.dll"
#else
#include <SDL_opengl.h>
#define LIBRA_RUNTIME_OPENGL
#define XMEN2_LIBRA_FILE "librashader.so"
#endif
#include "external/librashader/librashader_ld.h"

/* ---- both platforms: the library, the preset's path, errors, the cost summary ---- */

namespace {

libra_instance_t L;
bool s_lib_tried, s_lib_ok;
bool s_on = true;                       /* F8 */
std::string s_name;                     /* the preset's file name, for the hotkey's message */
size_t s_frames;
int s_fails;                            /* failed frames in a row */
std::vector<float> s_us;                /* GPU time per timed chain frame */
uint64_t s_frames_total;

void say_error(const char *what, libra_error_t e)
{
    char *msg = nullptr;
    if (e && L.error_write(e, &msg) == 0 && msg) {
        fprintf(stderr, "[native] shader_preset: %s: %s\n", what, msg);
        L.error_free_string(&msg);
    } else {
        fprintf(stderr, "[native] shader_preset: %s\n", what);
    }
    if (e) L.error_free(&e);
}

void summary(void)
{
    if (!s_frames_total) return;
    std::vector<float> v = s_us;
    std::sort(v.begin(), v.end());
    auto pct = [&](double p) { return v.empty() ? 0.0 : (double)v[(size_t)(p * (double)(v.size() - 1))]; };
    fprintf(stderr, "[native] shader: %llu frames through the chain, GPU time per frame (us, %zu timed): p50 %.0f p99 %.0f max %.0f\n",
            (unsigned long long)s_frames_total, v.size(), pct(0.5), pct(0.99), v.empty() ? 0.0 : (double)v.back());
}

bool load_library(void)
{
    if (s_lib_tried) return s_lib_ok;
    s_lib_tried = true;
    /* Windows looks beside the program first; on Linux the program's RUNPATH ($ORIGIN, native.cmake)
       makes dlopen look there */
    L = librashader_load_instance();
    s_lib_ok = L.instance_loaded;
    if (!s_lib_ok)
        fprintf(stderr, "[native] shader_preset: " XMEN2_LIBRA_FILE " (ABI %d) not found beside the game: the plain picture\n",
                LIBRASHADER_CURRENT_ABI);
    return s_lib_ok;
}

/* the preset as given, else beside the game (a relative path in xmen2.ini means the game's folder) */
std::string resolve(const std::string &p)
{
    if (FILE *f = fopen(p.c_str(), "rb")) { fclose(f); return p; }
    std::string q = options_exe_dir() + "/" + p;
    if (FILE *f = fopen(q.c_str(), "rb")) { fclose(f); return q; }
    return p;
}

/* the start of shader_open on both: the path, the library, the exit summary; false: nothing to do */
bool open_common(std::string *path)
{
    std::string want = options_shader_preset();
    if (want.empty() || !load_library()) return false;
    static bool exit_hook;
    if (!exit_hook) { exit_hook = true; atexit(summary); }
    *path = resolve(want);
    size_t s = path->find_last_of("/\\");
    s_name = s == std::string::npos ? *path : path->substr(s + 1);
    return true;
}

bool chain_loaded(void);

}  // namespace

void shader_toggle(void)
{
    if (!chain_loaded()) {
        host_osd(options_shader_preset().empty() ? "NO SHADER PRESET SET" : "SHADER PRESET NOT LOADED");
        return;
    }
    s_on = !s_on;
    std::string m = s_on ? "SHADER: " + s_name : std::string("SHADER: OFF");
    if (m.size() > 47) m = m.substr(0, 44) + "...";
    for (char &c : m) c = (char)toupper((unsigned char)c);
    host_osd(m.c_str());
}

#ifdef _WIN32
/* ---- Windows: Direct3D 11 ---- */

namespace {

ID3D11Device *s_dev;
ID3D11DeviceContext *s_ctx;
ID3D11DeviceContext1 *s_ctx1;
ID3DDeviceContextState *s_state;        /* the chain's own pipeline state */
libra_d3d11_filter_chain_t s_chain;
ID3D11Texture2D *s_in;
ID3D11ShaderResourceView *s_in_srv;
int s_in_w, s_in_h;

/* GPU time per chain frame: timestamp queries, read a few frames later */
struct Timing { ID3D11Query *dis, *t0, *t1; bool pending; };
Timing s_q[4];
int s_qi;

bool chain_loaded(void) { return s_chain != nullptr; }

template <class T> void rel(T *&p) { if (p) { p->Release(); p = nullptr; } }

void timing_read(bool wait)
{
    for (Timing &q : s_q) {
        if (!q.pending) continue;
        D3D11_QUERY_DATA_TIMESTAMP_DISJOINT d;
        UINT64 a, b;
        UINT flags = wait ? 0 : D3D11_ASYNC_GETDATA_DONOTFLUSH;
        if (s_ctx->GetData(q.dis, &d, sizeof d, flags) != S_OK) { if (wait) q.pending = false; continue; }
        if (s_ctx->GetData(q.t0, &a, sizeof a, flags) != S_OK || s_ctx->GetData(q.t1, &b, sizeof b, flags) != S_OK) continue;
        q.pending = false;
        if (!d.Disjoint && d.Frequency && b >= a) s_us.push_back((float)((double)(b - a) * 1e6 / (double)d.Frequency));
    }
}

}  // namespace

void shader_open(SDL_Renderer *ren)
{
    shader_close();
    std::string path;
    if (!open_common(&path)) return;
    s_dev = SDL_RenderGetD3D11Device(ren);   /* AddRef'd; null: not the Direct3D 11 renderer */
    if (!s_dev) { fprintf(stderr, "[native] shader_preset: the renderer isn't Direct3D 11: the plain picture\n"); return; }
    s_dev->GetImmediateContext(&s_ctx);
    ID3D11Device1 *dev1 = nullptr;
    D3D_FEATURE_LEVEL fl = s_dev->GetFeatureLevel();
    /* the state object's threading must be the device's (SDL makes it single-threaded) */
    UINT sflags = (s_dev->GetCreationFlags() & D3D11_CREATE_DEVICE_SINGLETHREADED) ? D3D11_1_CREATE_DEVICE_CONTEXT_STATE_SINGLETHREADED : 0;
    HRESULT hr = s_dev->QueryInterface(__uuidof(ID3D11Device1), (void **)&dev1);
    if (SUCCEEDED(hr)) hr = s_ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&s_ctx1);
    if (SUCCEEDED(hr)) hr = dev1->CreateDeviceContextState(sflags, &fl, 1, D3D11_SDK_VERSION, __uuidof(ID3D11Device1), nullptr, &s_state);
    rel(dev1);
    if (FAILED(hr)) {
        fprintf(stderr, "[native] shader_preset: no Direct3D 11.1 context state (0x%08lx): the plain picture\n", (unsigned long)hr);
        shader_close();
        return;
    }
    libra_shader_preset_t preset = nullptr;
    if (libra_error_t e = L.preset_create(path.c_str(), &preset)) {
        say_error(("can't load " + path + ", the plain picture").c_str(), e);
        shader_close();
        return;
    }
    filter_chain_d3d11_opt_t opt = {};
    opt.version = LIBRASHADER_CURRENT_VERSION;
    ID3DDeviceContextState *prev = nullptr;
    s_ctx1->SwapDeviceContextState(s_state, &prev);   /* loading the lookup textures uses the context */
    libra_error_t e = L.d3d11_filter_chain_create(&preset, s_dev, &opt, &s_chain);
    s_ctx1->SwapDeviceContextState(prev, nullptr);
    rel(prev);
    if (preset) L.preset_free(&preset);   /* the chain took it (null) unless it failed; 0.12's free panics on null */
    if (e) {
        say_error(("can't compile " + path + ", the plain picture").c_str(), e);
        s_chain = nullptr;
        shader_close();
        return;
    }
    for (Timing &q : s_q) {
        D3D11_QUERY_DESC d = { D3D11_QUERY_TIMESTAMP_DISJOINT, 0 };
        s_dev->CreateQuery(&d, &q.dis);
        d.Query = D3D11_QUERY_TIMESTAMP;
        s_dev->CreateQuery(&d, &q.t0);
        s_dev->CreateQuery(&d, &q.t1);
        q.pending = false;
    }
    s_fails = 0;
    fprintf(stderr, "[native] shader_preset: %s\n", path.c_str());
}

void shader_close(void)
{
    if (s_ctx) timing_read(true);
    for (Timing &q : s_q) { rel(q.dis); rel(q.t0); rel(q.t1); q.pending = false; }
    if (s_chain) L.d3d11_filter_chain_free(&s_chain);
    s_chain = nullptr;
    rel(s_in_srv);
    rel(s_in);
    s_in_w = s_in_h = 0;
    rel(s_state);
    rel(s_ctx1);
    rel(s_ctx);
    rel(s_dev);
}

bool shader_draw(SDL_Renderer *ren, const uint32_t *fb, int stride, int w, int h, const SDL_Rect *dst)
{
    if (!s_chain || !s_on || w <= 0 || h <= 0 || dst->w <= 0 || dst->h <= 0) return false;
    if (w != s_in_w || h != s_in_h) {
        rel(s_in_srv);
        rel(s_in);
        D3D11_TEXTURE2D_DESC d = {};
        d.Width = (UINT)w; d.Height = (UINT)h; d.MipLevels = 1; d.ArraySize = 1;
        d.Format = DXGI_FORMAT_B8G8R8A8_UNORM;   /* the frame's ARGB8888, alpha 0xFF */
        d.SampleDesc.Count = 1; d.Usage = D3D11_USAGE_DEFAULT; d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (FAILED(s_dev->CreateTexture2D(&d, nullptr, &s_in)) || FAILED(s_dev->CreateShaderResourceView(s_in, nullptr, &s_in_srv))) {
            fprintf(stderr, "[native] shader_preset: no %dx%d input texture: the plain picture\n", w, h);
            shader_close();
            return false;
        }
        s_in_w = w; s_in_h = h;
    }
    s_ctx->UpdateSubresource(s_in, 0, nullptr, fb, (UINT)stride * 4, 0);
    /* the back buffer's view: SDL binds it for a draw of its own */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, nullptr);
    SDL_RenderFlush(ren);
    ID3D11RenderTargetView *rtv = nullptr;
    s_ctx->OMGetRenderTargets(1, &rtv, nullptr);
    if (!rtv) return false;
    timing_read(false);
    Timing &q = s_q[s_qi];
    bool timed = q.dis && q.t0 && q.t1 && !q.pending;
    if (timed) { s_ctx->Begin(q.dis); s_ctx->End(q.t0); }
    ID3DDeviceContextState *prev = nullptr;
    s_ctx1->SwapDeviceContextState(s_state, &prev);
    libra_viewport_t vp = { (float)dst->x, (float)dst->y, (uint32_t)dst->w, (uint32_t)dst->h };
    frame_d3d11_opt_t o = {};
    o.version = LIBRASHADER_CURRENT_VERSION;
    o.frame_direction = 1;
    o.total_subframes = 1;
    o.current_subframe = 1;
    o.frames_per_second = 59.92f;
    libra_error_t e = L.d3d11_filter_chain_frame(&s_chain, s_ctx, s_frames++, s_in_srv, rtv, &vp, nullptr, &o);
    s_ctx1->SwapDeviceContextState(prev, nullptr);
    rel(prev);
    rel(rtv);
    if (timed) { s_ctx->End(q.t1); s_ctx->End(q.dis); q.pending = true; s_qi = (s_qi + 1) % 4; }
    if (e) {
        say_error("a frame failed", e);
        if (++s_fails >= 3) { fprintf(stderr, "[native] shader_preset: off for this run\n"); shader_close(); }
        return false;
    }
    s_fails = 0;
    s_frames_total++;
    return true;
}

#else
/* ---- Linux and the rest: OpenGL ---- */

namespace {

/* the GL entry points this file calls, from SDL_GL_GetProcAddress (the game links no GL library) */
struct Gl {
    void (APIENTRY *GetIntegerv)(GLenum, GLint *);
    const GLubyte *(APIENTRY *GetString)(GLenum);
    GLenum (APIENTRY *GetError)(void);
    GLboolean (APIENTRY *IsEnabled)(GLenum);
    void (APIENTRY *Enable)(GLenum);
    void (APIENTRY *Disable)(GLenum);
    void (APIENTRY *GetFloatv)(GLenum, GLfloat *);
    void (APIENTRY *GetBooleanv)(GLenum, GLboolean *);
    void (APIENTRY *GenTextures)(GLsizei, GLuint *);
    void (APIENTRY *DeleteTextures)(GLsizei, const GLuint *);
    void (APIENTRY *BindTexture)(GLenum, GLuint);
    void (APIENTRY *TexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *);
    void (APIENTRY *TexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *);
    void (APIENTRY *TexParameteri)(GLenum, GLenum, GLint);
    void (APIENTRY *PixelStorei)(GLenum, GLint);
    void (APIENTRY *Viewport)(GLint, GLint, GLsizei, GLsizei);
    void (APIENTRY *Scissor)(GLint, GLint, GLsizei, GLsizei);
    void (APIENTRY *ColorMask)(GLboolean, GLboolean, GLboolean, GLboolean);
    void (APIENTRY *ClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
    PFNGLACTIVETEXTUREPROC ActiveTexture;
    PFNGLBLENDFUNCSEPARATEPROC BlendFuncSeparate;
    PFNGLBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNGLUSEPROGRAMPROC UseProgram;
    PFNGLBINDBUFFERPROC BindBuffer;
    PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    PFNGLBINDSAMPLERPROC BindSampler;
    PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
    PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
    PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
    PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
    PFNGLBLITFRAMEBUFFERPROC BlitFramebuffer;
    PFNGLGENQUERIESPROC GenQueries;
    PFNGLDELETEQUERIESPROC DeleteQueries;
    PFNGLBEGINQUERYPROC BeginQuery;
    PFNGLENDQUERYPROC EndQuery;
    PFNGLGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGLGETQUERYOBJECTUI64VPROC GetQueryObjectui64v;
};
Gl G;

SDL_Window *s_win;
SDL_GLContext s_glctx;
libra_gl_filter_chain_t s_chain;
GLuint s_in, s_out, s_fbo;              /* the frame, the chain's output, the framebuffer the blit reads */
int s_in_w, s_in_h, s_out_w, s_out_h;
const int k_units = 3;                  /* the texture units SDL's GL renderer uses (YUV takes three) */

/* GPU time per chain frame: GL_TIME_ELAPSED queries, read a few frames later */
struct Timing { GLuint q; bool pending; };
Timing s_q[4];
int s_qi;

bool chain_loaded(void) { return s_chain != nullptr; }

const void *gl_loader(const char *name) { return SDL_GL_GetProcAddress(name); }

bool load_gl(void)
{
    bool ok = true;
#define XM_GL(field, name) do { *(void **)&G.field = SDL_GL_GetProcAddress(name); if (!G.field) { \
        fprintf(stderr, "[native] shader_preset: no %s in this OpenGL: the plain picture\n", name); ok = false; } } while (0)
    XM_GL(GetIntegerv, "glGetIntegerv"); XM_GL(GetString, "glGetString"); XM_GL(GetError, "glGetError");
    XM_GL(IsEnabled, "glIsEnabled"); XM_GL(Enable, "glEnable"); XM_GL(Disable, "glDisable");
    XM_GL(GetFloatv, "glGetFloatv"); XM_GL(GetBooleanv, "glGetBooleanv");
    XM_GL(GenTextures, "glGenTextures"); XM_GL(DeleteTextures, "glDeleteTextures"); XM_GL(BindTexture, "glBindTexture");
    XM_GL(TexImage2D, "glTexImage2D"); XM_GL(TexSubImage2D, "glTexSubImage2D"); XM_GL(TexParameteri, "glTexParameteri");
    XM_GL(PixelStorei, "glPixelStorei"); XM_GL(Viewport, "glViewport"); XM_GL(Scissor, "glScissor");
    XM_GL(ColorMask, "glColorMask"); XM_GL(ClearColor, "glClearColor");
    XM_GL(ActiveTexture, "glActiveTexture"); XM_GL(BlendFuncSeparate, "glBlendFuncSeparate");
    XM_GL(BlendEquationSeparate, "glBlendEquationSeparate"); XM_GL(UseProgram, "glUseProgram");
    XM_GL(BindBuffer, "glBindBuffer"); XM_GL(BindVertexArray, "glBindVertexArray"); XM_GL(BindSampler, "glBindSampler");
    XM_GL(GenFramebuffers, "glGenFramebuffers"); XM_GL(DeleteFramebuffers, "glDeleteFramebuffers");
    XM_GL(BindFramebuffer, "glBindFramebuffer"); XM_GL(FramebufferTexture2D, "glFramebufferTexture2D");
    XM_GL(CheckFramebufferStatus, "glCheckFramebufferStatus"); XM_GL(BlitFramebuffer, "glBlitFramebuffer");
    XM_GL(GenQueries, "glGenQueries"); XM_GL(DeleteQueries, "glDeleteQueries"); XM_GL(BeginQuery, "glBeginQuery");
    XM_GL(EndQuery, "glEndQuery"); XM_GL(GetQueryObjectiv, "glGetQueryObjectiv");
    XM_GL(GetQueryObjectui64v, "glGetQueryObjectui64v");
#undef XM_GL
    return ok;
}

/* the GL state SDL's renderer relies on (it caches what it set) and the chain may change */
struct GlState {
    GLint program, vao, array_buf, active, tex[k_units], sampler[k_units], draw_fb, read_fb;
    GLint viewport[4], scissor[4], blend_src_rgb, blend_dst_rgb, blend_src_a, blend_dst_a, blend_eq_rgb, blend_eq_a;
    GLint unpack_row, unpack_align;
    GLboolean blend, scissor_on, cull, depth, stencil, srgb, mask[4];
    GLfloat clear[4];

    void save(void)
    {
        G.GetIntegerv(GL_CURRENT_PROGRAM, &program);
        G.GetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        G.GetIntegerv(GL_ARRAY_BUFFER_BINDING, &array_buf);
        G.GetIntegerv(GL_ACTIVE_TEXTURE, &active);
        for (int u = 0; u < k_units; u++) {
            G.ActiveTexture(GL_TEXTURE0 + u);
            G.GetIntegerv(GL_TEXTURE_BINDING_2D, &tex[u]);
            G.GetIntegerv(GL_SAMPLER_BINDING, &sampler[u]);
        }
        G.ActiveTexture((GLenum)active);
        G.GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_fb);
        G.GetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_fb);
        G.GetIntegerv(GL_VIEWPORT, viewport);
        G.GetIntegerv(GL_SCISSOR_BOX, scissor);
        G.GetIntegerv(GL_BLEND_SRC_RGB, &blend_src_rgb);
        G.GetIntegerv(GL_BLEND_DST_RGB, &blend_dst_rgb);
        G.GetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src_a);
        G.GetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst_a);
        G.GetIntegerv(GL_BLEND_EQUATION_RGB, &blend_eq_rgb);
        G.GetIntegerv(GL_BLEND_EQUATION_ALPHA, &blend_eq_a);
        G.GetIntegerv(GL_UNPACK_ROW_LENGTH, &unpack_row);
        G.GetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_align);
        blend = G.IsEnabled(GL_BLEND);
        scissor_on = G.IsEnabled(GL_SCISSOR_TEST);
        cull = G.IsEnabled(GL_CULL_FACE);
        depth = G.IsEnabled(GL_DEPTH_TEST);
        stencil = G.IsEnabled(GL_STENCIL_TEST);
        srgb = G.IsEnabled(GL_FRAMEBUFFER_SRGB);
        G.GetBooleanv(GL_COLOR_WRITEMASK, mask);
        G.GetFloatv(GL_COLOR_CLEAR_VALUE, clear);
    }

    void restore(void)
    {
        G.UseProgram((GLuint)program);
        G.BindVertexArray((GLuint)vao);
        G.BindBuffer(GL_ARRAY_BUFFER, (GLuint)array_buf);   /* 0: SDL's vertex arrays are client memory */
        for (int u = 0; u < k_units; u++) {
            G.ActiveTexture(GL_TEXTURE0 + u);
            G.BindTexture(GL_TEXTURE_2D, (GLuint)tex[u]);
            G.BindSampler((GLuint)u, (GLuint)sampler[u]);   /* a sampler left bound would override SDL's nearest filter */
        }
        G.ActiveTexture((GLenum)active);
        G.BindFramebuffer(GL_DRAW_FRAMEBUFFER, (GLuint)draw_fb);
        G.BindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)read_fb);
        G.Viewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        G.Scissor(scissor[0], scissor[1], scissor[2], scissor[3]);
        G.BlendFuncSeparate((GLenum)blend_src_rgb, (GLenum)blend_dst_rgb, (GLenum)blend_src_a, (GLenum)blend_dst_a);
        G.BlendEquationSeparate((GLenum)blend_eq_rgb, (GLenum)blend_eq_a);
        G.PixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row);
        G.PixelStorei(GL_UNPACK_ALIGNMENT, unpack_align);
        auto set = [](GLenum cap, GLboolean on) { if (on) G.Enable(cap); else G.Disable(cap); };
        set(GL_BLEND, blend);
        set(GL_SCISSOR_TEST, scissor_on);
        set(GL_CULL_FACE, cull);
        set(GL_DEPTH_TEST, depth);
        set(GL_STENCIL_TEST, stencil);
        set(GL_FRAMEBUFFER_SRGB, srgb);
        G.ColorMask(mask[0], mask[1], mask[2], mask[3]);
        G.ClearColor(clear[0], clear[1], clear[2], clear[3]);
    }
};

void drain_errors(void) { for (int i = 0; i < 16 && G.GetError() != GL_NO_ERROR; i++) {} }

void timing_read(bool wait)
{
    for (Timing &q : s_q) {
        if (!q.pending) continue;
        GLint ready = 0;
        G.GetQueryObjectiv(q.q, GL_QUERY_RESULT_AVAILABLE, &ready);
        if (!ready && !wait) continue;
        GLuint64 ns = 0;
        G.GetQueryObjectui64v(q.q, GL_QUERY_RESULT, &ns);
        q.pending = false;
        s_us.push_back((float)((double)ns / 1000.0));
    }
}

/* a texture of ours, RGBA8, w x h, nearest (the chain samples as each pass says, with its own samplers) */
GLuint make_tex(int w, int h)
{
    GLuint t = 0;
    G.GenTextures(1, &t);
    G.BindTexture(GL_TEXTURE_2D, t);
    G.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    G.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    G.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    G.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    G.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    G.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    return t;
}

bool make_current(void)
{
    if (SDL_GL_GetCurrentContext() == s_glctx) return true;
    return SDL_GL_MakeCurrent(s_win, s_glctx) == 0;
}

}  // namespace

void shader_open(SDL_Renderer *ren)
{
    shader_close();
    std::string path;
    if (!open_common(&path)) return;
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(ren, &info) != 0 || strcmp(info.name, "opengl") != 0) {
        fprintf(stderr, "[native] shader_preset: the renderer is %s, not OpenGL: the plain picture\n",
                SDL_GetRendererInfo(ren, &info) == 0 ? info.name : "unknown");
        return;
    }
    s_win = SDL_RenderGetWindow(ren);
    s_glctx = SDL_GL_GetCurrentContext();   /* the renderer's: making it made it current */
    if (!s_win || !s_glctx || !load_gl()) { shader_close(); return; }
    GLint major = 0, minor = 0;
    G.GetIntegerv(GL_MAJOR_VERSION, &major);
    G.GetIntegerv(GL_MINOR_VERSION, &minor);
    drain_errors();
    if (major * 10 + minor < 33) {
        const GLubyte *v = G.GetString(GL_VERSION);
        fprintf(stderr, "[native] shader_preset: OpenGL %s; presets need 3.3: the plain picture\n", v ? (const char *)v : "?");
        shader_close();
        return;
    }
    libra_shader_preset_t preset = nullptr;
    if (libra_error_t e = L.preset_create(path.c_str(), &preset)) {
        say_error(("can't load " + path + ", the plain picture").c_str(), e);
        shader_close();
        return;
    }
    filter_chain_gl_opt_t opt = {};
    opt.version = LIBRASHADER_CURRENT_VERSION;
    opt.glsl_version = 330;
    opt.use_dsa = false;   /* 4.5 only; 3.3 is enough without it */
    GlState st;
    st.save();
    libra_error_t e = L.gl_filter_chain_create(&preset, gl_loader, &opt, &s_chain);
    st.restore();
    drain_errors();
    if (preset) L.preset_free(&preset);   /* the chain took it (null) unless it failed; 0.12's free panics on null */
    if (e) {
        say_error(("can't compile " + path + ", the plain picture").c_str(), e);
        s_chain = nullptr;
        shader_close();
        return;
    }
    for (Timing &q : s_q) { G.GenQueries(1, &q.q); q.pending = false; }
    s_fails = 0;
    fprintf(stderr, "[native] shader_preset: %s (OpenGL %d.%d)\n", path.c_str(), (int)major, (int)minor);
}

void shader_close(void)
{
    if (s_glctx && G.GetIntegerv && make_current()) {
        timing_read(true);
        for (Timing &q : s_q) { if (q.q) G.DeleteQueries(1, &q.q); q.q = 0; q.pending = false; }
        if (s_chain) L.gl_filter_chain_free(&s_chain);
        if (s_fbo) G.DeleteFramebuffers(1, &s_fbo);
        if (s_in) G.DeleteTextures(1, &s_in);
        if (s_out) G.DeleteTextures(1, &s_out);
        drain_errors();
    }
    s_chain = nullptr;
    s_fbo = s_in = s_out = 0;
    s_in_w = s_in_h = s_out_w = s_out_h = 0;
    s_glctx = nullptr;
    s_win = nullptr;
}

bool shader_draw(SDL_Renderer *ren, const uint32_t *fb, int stride, int w, int h, const SDL_Rect *dst)
{
    if (!s_chain || !s_on || w <= 0 || h <= 0 || dst->w <= 0 || dst->h <= 0) return false;
    int ow = 0, oh = 0;
    if (SDL_GetRendererOutputSize(ren, &ow, &oh) != 0 || oh <= 0) return false;
    SDL_RenderFlush(ren);   /* SDL's queued clear first */
    if (!make_current()) return false;
    GlState st;
    st.save();
    bool ok = true;
    if (w != s_in_w || h != s_in_h) {
        if (s_in) G.DeleteTextures(1, &s_in);
        s_in = make_tex(w, h);
        s_in_w = w; s_in_h = h;
    }
    if (dst->w != s_out_w || dst->h != s_out_h) {
        if (s_out) G.DeleteTextures(1, &s_out);
        s_out = make_tex(dst->w, dst->h);
        if (!s_fbo) G.GenFramebuffers(1, &s_fbo);
        G.BindFramebuffer(GL_READ_FRAMEBUFFER, s_fbo);
        G.FramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_out, 0);
        ok = G.CheckFramebufferStatus(GL_READ_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        s_out_w = dst->w; s_out_h = dst->h;
    }
    /* the frame: ARGB8888 words are B, G, R, A bytes in memory (alpha 0xFF) */
    G.ActiveTexture(GL_TEXTURE0);
    G.BindTexture(GL_TEXTURE_2D, s_in);
    G.PixelStorei(GL_UNPACK_ROW_LENGTH, stride);
    G.PixelStorei(GL_UNPACK_ALIGNMENT, 4);
    G.TexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, fb);
    G.PixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    libra_error_t e = nullptr;
    timing_read(false);
    Timing &q = s_q[s_qi];
    bool timed = q.q && !q.pending;
    if (ok) {
        if (timed) G.BeginQuery(GL_TIME_ELAPSED, q.q);
        libra_image_gl_t in = { s_in, GL_RGBA8, (uint32_t)w, (uint32_t)h };
        libra_image_gl_t out = { s_out, GL_RGBA8, (uint32_t)dst->w, (uint32_t)dst->h };
        frame_gl_opt_t o = {};
        o.version = LIBRASHADER_CURRENT_VERSION;
        o.frame_direction = 1;
        o.total_subframes = 1;
        o.current_subframe = 1;
        o.frames_per_second = 59.92f;
        e = L.gl_filter_chain_frame(&s_chain, s_frames++, in, out, nullptr, nullptr, &o);
        if (!e) {
            /* the output into the window: GL counts rows from the bottom, SDL's dst from the top. The chain
               draws the frame's first row at the output's row 0, as librashader's GL runtime does for a
               texture uploaded top row first, so the blit turns it over (y0 > y1). */
            G.BindFramebuffer(GL_READ_FRAMEBUFFER, s_fbo);
            G.BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            G.Disable(GL_SCISSOR_TEST);
            G.ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            int y_bottom = oh - (dst->y + dst->h);
            G.BlitFramebuffer(0, 0, dst->w, dst->h, dst->x, y_bottom + dst->h, dst->x + dst->w, y_bottom,
                              GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        if (timed) { G.EndQuery(GL_TIME_ELAPSED); q.pending = true; s_qi = (s_qi + 1) % 4; }
    }
    st.restore();
    GLenum gerr = G.GetError();
    drain_errors();
    if (!ok) {
        fprintf(stderr, "[native] shader_preset: no %dx%d output framebuffer: the plain picture\n", dst->w, dst->h);
        shader_close();
        return false;
    }
    if (e || gerr != GL_NO_ERROR) {
        if (e) say_error("a frame failed", e);
        else fprintf(stderr, "[native] shader_preset: a frame failed: GL error 0x%04x\n", (unsigned)gerr);
        if (++s_fails >= 3) { fprintf(stderr, "[native] shader_preset: off for this run\n"); shader_close(); }
        return false;
    }
    s_fails = 0;
    s_frames_total++;
    return true;
}

#endif
