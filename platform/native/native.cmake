# XMen2Native, the native port's runner (xmen2disasm docs/phases/P5-port.md): the hand C in
# xmen2_mem.h's NATIVE mode, its bridge (tools/bridge.py --mode native: wrappers, the function
# table, typed calls through stored pointers), the memory layer (game/runtime/native_rt.cpp), the
# state exporter, and the platform's own machine and host side (platform/native). From the
# engine only what the platform keeps (P6): the video model and the Z80 core. No 68000 CPU,
# scheduler, dispatch, generated code or emulated work RAM is compiled in; `xmen2check.py build
# --native` reads its objects and link map to prove it. Only target-level definitions here:
# per-source ones would stop /MP (one cl call per file).
set(XMEN2_NATIVE_BRIDGE_DIR "${CMAKE_BINARY_DIR}/bridge_native")
add_executable(XMen2Native
    ${XMEN2_HANDC_SOURCES}
    "${XMEN2_NATIVE_BRIDGE_DIR}/xmen2_bridge.cpp"
    "${CMAKE_SOURCE_DIR}/game/runtime/native_rt.cpp"
    "${CMAKE_SOURCE_DIR}/game/runtime/export.cpp"
    "${CMAKE_SOURCE_DIR}/game/runtime/crash_win.cpp"
    "${CMAKE_SOURCE_DIR}/game/runtime/state_place.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/machine.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/main.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/options.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/widescreen.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/present.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/game_entry.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/host_sdl.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/controls.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/state.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/players.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/sound2.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/sample_retime.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/capture.cpp"
    # the sound machines' chips: the engine's YM2612 and SN76489 wrappers, each machine's compiled
    # through its own file. Never the engine's files themselves: MSBuild would then credit everything
    # a copy reads after including one to that source, and an engine change rebuilt no copy.
    "${CMAKE_SOURCE_DIR}/platform/native/snd1_chips.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/snd1_psg.c"
    "${CMAKE_SOURCE_DIR}/platform/native/snd2_chips.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/snd2_psg.c"
    "${CMAKE_SOURCE_DIR}/platform/native/mus_chips.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/mus_psg.c"
    "${CMAKE_SOURCE_DIR}/platform/native/prt_chips.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/prt_psg.c"
    "${CMAKE_SOURCE_DIR}/platform/native/p34_chips.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/p34_psg.c"
    "${CMAKE_SOURCE_DIR}/game/runtime/players34.cpp"
    # the game's fiber (phase 7 R2): the engine's snapshottable fibers on vendored minicoro
    "${RUNNER_ROOT}/fiber_compat.c"
    "${RUNNER_ROOT}/video/genesis_vdp.c"
    "${RUNNER_ROOT}/external/superzazu/z80.c"
    # the sound chips and their mixer (played runs only)
    "${RUNNER_ROOT}/audio.c"
    "${RUNNER_ROOT}/chip_trace.c"
    "${RUNNER_ROOT}/audio/event_queue.c"
    "${RUNNER_ROOT}/audio/mixer.c"
    "${RUNNER_ROOT}/audio/fm_shadow.cpp"
    "${RUNNER_ROOT}/audio/observability.c"
    "${RUNNER_ROOT}/audio/audio_shadow.c"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_opn.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_ssg.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_adpcm.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_pcm.cpp")
# The port's VDP renderer: the engine's BATCHED (tile-row spans and a table-driven compositor, the
# same pixels as SCALAR: the engine's renderer differential, and every --fb comparison of this runner
# with the recomp runners' references), about 1.5x faster, which high_fps's frames in between need.
# SCALAR is the control. The recomp runners keep the engine's default (GENESIS_VDP_PLANE_IMPL).
set(XMEN2_NATIVE_VDP "BATCHED" CACHE STRING "XMen2Native's VDP renderer: BATCHED or SCALAR")
set_property(CACHE XMEN2_NATIVE_VDP PROPERTY STRINGS BATCHED SCALAR)
target_compile_definitions(XMen2Native PRIVATE XMEN2_NATIVE=1
    GENESIS_BATCHED_PLANES=$<STREQUAL:${XMEN2_NATIVE_VDP},BATCHED>)
target_include_directories(XMen2Native PRIVATE
    "${RUNNER_ROOT}"
    "${RUNNER_ROOT}/include"
    "${RUNNER_ROOT}/external/ymfm/src"
    "${RUNNER_ROOT}/video"
    "${RUNNER_ROOT}/external/superzazu"
    "${CMAKE_SOURCE_DIR}/game/include"
    "${CMAKE_SOURCE_DIR}/game/runtime"
    "${CMAKE_SOURCE_DIR}/platform"
    "${CMAKE_SOURCE_DIR}/platform/native"
    "${CMAKE_BINARY_DIR}/native")
target_link_libraries(XMen2Native PRIVATE SDL2::SDL2)
if(WIN32)
    target_link_libraries(XMen2Native PRIVATE comdlg32 dwmapi)   # host_sdl.cpp's ROM picker, XMEN2_PRESENT_TRACE
endif()

# The launcher (platform/native/launcher.cpp, GitHub issue #1): Dear ImGui v1.92.9b (vendored, MIT) on SDL2's
# renderer, and its two fonts (Karla, SIL OFL 1.1; Roboto Medium, Apache 2.0) built into the program as byte
# arrays, written here at configure time (launcher_fonts.h in the build's native/).
set(XMEN2_IMGUI "${CMAKE_SOURCE_DIR}/platform/native/external/imgui")
target_sources(XMen2Native PRIVATE
    "${CMAKE_SOURCE_DIR}/platform/native/launcher.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/joincode.cpp" "${CMAKE_SOURCE_DIR}/platform/native/room.cpp"
    "${XMEN2_IMGUI}/imgui.cpp" "${XMEN2_IMGUI}/imgui_draw.cpp" "${XMEN2_IMGUI}/imgui_tables.cpp"
    "${XMEN2_IMGUI}/imgui_widgets.cpp" "${XMEN2_IMGUI}/backends/imgui_impl_sdl2.cpp"
    "${XMEN2_IMGUI}/backends/imgui_impl_sdlrenderer2.cpp")
target_include_directories(XMen2Native PRIVATE "${XMEN2_IMGUI}" "${XMEN2_IMGUI}/backends")
if(WIN32)
    target_link_libraries(XMen2Native PRIVATE iphlpapi)   # the launcher's list of this PC's addresses
endif()
set(XMEN2_FONTS_H "${CMAKE_BINARY_DIR}/native/launcher_fonts.h")
set(XMEN2_FONTS_BODY "/* launcher_fonts.h - generated by platform/native/native.cmake from platform/native/external/fonts */\n#pragma once\n")
foreach(XMEN2_FONT "k_font_karla:Karla-Regular.ttf" "k_font_roboto:Roboto-Medium.ttf")
    string(REPLACE ":" ";" XMEN2_FONT "${XMEN2_FONT}")
    list(GET XMEN2_FONT 0 XMEN2_FONT_VAR)
    list(GET XMEN2_FONT 1 XMEN2_FONT_FILE)
    set(XMEN2_FONT_PATH "${CMAKE_SOURCE_DIR}/platform/native/external/fonts/${XMEN2_FONT_FILE}")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${XMEN2_FONT_PATH}")
    file(READ "${XMEN2_FONT_PATH}" XMEN2_FONT_HEX HEX)
    string(REGEX REPLACE "([0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f])" "\\1\n" XMEN2_FONT_HEX "${XMEN2_FONT_HEX}")
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," XMEN2_FONT_HEX "${XMEN2_FONT_HEX}")
    string(APPEND XMEN2_FONTS_BODY "static const unsigned char ${XMEN2_FONT_VAR}[] = {\n${XMEN2_FONT_HEX}};\n")
endforeach()
file(CONFIGURE OUTPUT "${XMEN2_FONTS_H}" CONTENT "${XMEN2_FONTS_BODY}" @ONLY)

# Phase 7 (P7-multiplayer.md R1): rollback netplay through the engine's libraries, linked directly
# (recomp-net: sessions, transports, the rollback driver; rbengine: the snapshot ring), with
# XMen2Native's own host table (platform/native/netplay.cpp). ICE (R8) needs libjuice, which
# recomp-net fetches (a pinned, hashed release) when the engine has no copy.
option(XMEN2_NETPLAY_ICE "Netplay over ICE (libjuice)" ON)
set(RNET_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RNET_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RNET_BUILD_MODULE OFF CACHE BOOL "" FORCE)
set(RBE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RNET_ENABLE_ICE ${XMEN2_NETPLAY_ICE} CACHE BOOL "" FORCE)
if(NOT TARGET recomp_net)
    add_subdirectory("${RECOMP_ROOT}/external/recomp-net" "${CMAKE_BINARY_DIR}/recomp-net" EXCLUDE_FROM_ALL)
endif()
if(NOT TARGET retcomm_rbengine)
    add_subdirectory("${RECOMP_ROOT}/external/rbengine" "${CMAKE_BINARY_DIR}/rbengine" EXCLUDE_FROM_ALL)
endif()
target_sources(XMen2Native PRIVATE "${CMAKE_SOURCE_DIR}/platform/native/netplay.cpp")
target_link_libraries(XMen2Native PRIVATE recomp_net retcomm_rbengine)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # the game's arithmetic wraps as the 68000's does, and its memory is reached through
    # reinterpreting casts of game-memory types: no optimisation may assume otherwise
    target_compile_options(XMen2Native PRIVATE -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks)
    # the game's fiber: no shadow stacks (its switch replaces the stack pointer), and page-by-page
    # probes of big frames so one never steps over the stack's guard (fiber_compat.h)
    target_compile_options(XMen2Native PRIVATE -fstack-clash-protection)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
        target_compile_options(XMen2Native PRIVATE -fcf-protection=none)
    endif()
endif()
if(MSVC)
    target_compile_options(XMen2Native PRIVATE /W3 /WX- /utf-8 /wd4102 /wd4996)
    target_link_options(XMen2Native PRIVATE "/MAP:$<TARGET_FILE_DIR:XMen2Native>/XMen2Native.map" "/CETCOMPAT:NO"
        "/ENTRY:xm_entry")   # state files in other processes: XMEN2_GS_COOKIE (game/runtime/state_place.cpp)
    add_custom_command(TARGET XMen2Native POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RUNNER_ROOT}/external/SDL2/SDL2-2.28.5/lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:XMen2Native>"
        COMMENT "Copying SDL2.dll")
endif()

# shader_preset (platform/native/shader.cpp): librashader's C headers are vendored (MIT,
# platform/native/external/librashader); the library itself (MPL-2.0) is loaded at run time from
# beside the program. Windows: librashader.dll, the upstream release unmodified, pinned by hash and
# fetched once into the build tree. Linux: librashader.so, built from the same release's source by
# tools/librashader_linux.py (upstream publishes no Linux library; `xmen2check.py native-build` runs
# it) into a per-version cache, copied from there; the program's RUNPATH ($ORIGIN) lets dlopen find
# it beside the program. Without it (offline, no cargo, XMEN2_LIBRASHADER off) the build goes on and
# the option says the library is missing.
target_sources(XMen2Native PRIVATE "${CMAKE_SOURCE_DIR}/platform/native/shader.cpp")
option(XMEN2_LIBRASHADER "librashader beside the program, for shader_preset" ON)
if(UNIX AND NOT APPLE)
    target_link_libraries(XMen2Native PRIVATE ${CMAKE_DL_LIBS})   # dlopen (librashader_ld.h)
    set_target_properties(XMen2Native PROPERTIES BUILD_RPATH "$ORIGIN" INSTALL_RPATH "$ORIGIN")
endif()
if(UNIX AND NOT APPLE AND XMEN2_LIBRASHADER)
    if(DEFINED ENV{XMEN2_LIBRASHADER_CACHE})
        set(XMEN2_LS_SO "$ENV{XMEN2_LIBRASHADER_CACHE}/librashader.so")
    else()
        set(XMEN2_LS_SO "$ENV{HOME}/.cache/xmen2/librashader-0.12.0/librashader.so")
    endif()
    if(EXISTS "${XMEN2_LS_SO}")
        add_custom_command(TARGET XMen2Native POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${XMEN2_LS_SO}" "$<TARGET_FILE_DIR:XMen2Native>/librashader.so"
            COMMENT "Copying librashader.so")
    else()
        message(WARNING "no ${XMEN2_LS_SO} (python3 tools/librashader_linux.py builds it): shader_preset will say the library is missing")
    endif()
endif()
if(WIN32 AND XMEN2_LIBRASHADER)
    set(XMEN2_LS_VER "0.12.0")
    set(XMEN2_LS_ZIP "${CMAKE_BINARY_DIR}/_deps/librashader-x86_64-windows-v${XMEN2_LS_VER}-optimized.zip")
    set(XMEN2_LS_DIR "${CMAKE_BINARY_DIR}/_deps/librashader-${XMEN2_LS_VER}")
    if(NOT EXISTS "${XMEN2_LS_DIR}/librashader.dll")
        file(DOWNLOAD
            "https://github.com/SnowflakePowered/librashader/releases/download/librashader-v${XMEN2_LS_VER}/librashader-x86_64-windows-v${XMEN2_LS_VER}-optimized.zip"
            "${XMEN2_LS_ZIP}"
            EXPECTED_HASH SHA256=521fe0f364bfa705883f9e99fb1733a3a24f0f403bcc5591b6b78f6cff289183
            STATUS XMEN2_LS_STATUS)
        list(GET XMEN2_LS_STATUS 0 XMEN2_LS_RC)
        if(XMEN2_LS_RC EQUAL 0)
            file(ARCHIVE_EXTRACT INPUT "${XMEN2_LS_ZIP}" DESTINATION "${XMEN2_LS_DIR}" PATTERNS "librashader.dll")
            file(REMOVE "${XMEN2_LS_ZIP}")
        else()
            message(WARNING "librashader ${XMEN2_LS_VER} not fetched (${XMEN2_LS_STATUS}): shader_preset will say the library is missing")
        endif()
    endif()
    if(EXISTS "${XMEN2_LS_DIR}/librashader.dll")
        add_custom_command(TARGET XMen2Native POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${XMEN2_LS_DIR}/librashader.dll" "$<TARGET_FILE_DIR:XMen2Native>"
            COMMENT "Copying librashader.dll")
    endif()
endif()
