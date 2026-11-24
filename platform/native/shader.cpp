/* shader.cpp - RetroArch shader presets (.slangp) on the played picture: the shader_preset option.
 *
 * librashader (github.com/SnowflakePowered/librashader, MPL-2.0) reads the preset and runs its passes;
 * it is loaded at run time from librashader.dll beside the game (librashader_ld.h, MIT), so without it
 * the option says so once and the picture is the plain one. Windows only for now: the D3D11 runtime on
 * SDL's own Direct3D 11 renderer (host_sdl.cpp make_renderer).
 *
 * SDL 2 hands out its device but neither its textures nor its back buffer, so:
 *   the frame goes into a texture of ours on SDL's device (the w x h of the picture, made again when
 *   the view's size changes: 320 or 400 wide);
 *   the output is the back buffer's render target view, the one SDL binds for its own draws (a black
 *   fill over the cleared window, flushed, then OMGetRenderTargets);
 *   SDL caches the pipeline state it set (shaders, input layout, blend, samplers), and librashader
 *   changes all of it, so the chain runs in a device context state of its own
 *   (ID3D11DeviceContext1::SwapDeviceContextState), and SDL finds its state as it left it.
 * The hotkey messages (host_osd) are drawn after the chain by host_sdl.cpp, so they aren't shaded.
 * A preset that fails to load or a frame that fails: one stderr line, then the plain picture (never a
 * message box in play). Headless runs open no window, so no chain: traces and digests are untouched.
 *
 * Its cost: GPU timestamps around each chain frame, summed up at exit (`[native] shader: ...`).
 */
#include "shader.h"

#include <SDL.h>
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
#include "external/librashader/librashader_ld.h"

namespace {

libra_instance_t L;
bool s_lib_tried, s_lib_ok;
bool s_on = true;                       /* F8 */
std::string s_name;                     /* the preset's file name, for the hotkey's message */
ID3D11Device *s_dev;
ID3D11DeviceContext *s_ctx;
ID3D11DeviceContext1 *s_ctx1;
ID3DDeviceContextState *s_state;        /* the chain's own pipeline state */
libra_d3d11_filter_chain_t s_chain;
ID3D11Texture2D *s_in;
ID3D11ShaderResourceView *s_in_srv;
int s_in_w, s_in_h;
size_t s_frames;
int s_fails;                            /* failed frames in a row */

/* GPU time per chain frame: timestamp queries, read a few frames later */
struct Timing { ID3D11Query *dis, *t0, *t1; bool pending; };
Timing s_q[4];
int s_qi;
std::vector<float> s_us;
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
    L = librashader_load_instance();
    s_lib_ok = L.instance_loaded;
    if (!s_lib_ok)
        fprintf(stderr, "[native] shader_preset: librashader.dll (ABI %d) not found beside the game: the plain picture\n",
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

}  // namespace

void shader_open(SDL_Renderer *ren)
{
    shader_close();
    std::string want = options_shader_preset();
    if (want.empty() || !load_library()) return;
    static bool exit_hook;
    if (!exit_hook) { exit_hook = true; atexit(summary); }
    std::string path = resolve(want);
    size_t s = path.find_last_of("/\\");
    s_name = s == std::string::npos ? path : path.substr(s + 1);
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
    rel(dev1);
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

void shader_toggle(void)
{
    if (!s_chain) {
        host_osd(options_shader_preset().empty() ? "NO SHADER PRESET SET" : "SHADER PRESET NOT LOADED");
        return;
    }
    s_on = !s_on;
    std::string m = s_on ? "SHADER: " + s_name : std::string("SHADER: OFF");
    if (m.size() > 47) m = m.substr(0, 44) + "...";
    for (char &c : m) c = (char)toupper((unsigned char)c);
    host_osd(m.c_str());
}

#else  /* not Windows: the first build has the Direct3D 11 runtime only */

void shader_open(SDL_Renderer *)
{
    static bool said;
    if (!said && !options_shader_preset().empty()) {
        said = true;
        fprintf(stderr, "[native] shader_preset: Windows only in this build: the plain picture\n");
    }
}
void shader_close(void) {}
bool shader_draw(SDL_Renderer *, const uint32_t *, int, int, int, const SDL_Rect *) { return false; }
void shader_toggle(void) { host_osd("SHADER PRESETS: WINDOWS ONLY"); }

#endif
