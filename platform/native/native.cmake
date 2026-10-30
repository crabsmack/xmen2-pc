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
    "${CMAKE_SOURCE_DIR}/platform/native/state.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/players.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/sound2.cpp"
    "${CMAKE_SOURCE_DIR}/platform/native/capture.cpp"
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
    "${RUNNER_ROOT}/audio/sn76489.c"
    "${RUNNER_ROOT}/audio/ym2612_ymfm.cpp"
    "${RUNNER_ROOT}/audio/fm_shadow.cpp"
    "${RUNNER_ROOT}/audio/observability.c"
    "${RUNNER_ROOT}/audio/audio_shadow.c"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_opn.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_ssg.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_adpcm.cpp"
    "${RUNNER_ROOT}/external/ymfm/src/ymfm_pcm.cpp")
target_compile_definitions(XMen2Native PRIVATE XMEN2_NATIVE=1
    GENESIS_BATCHED_PLANES=$<STREQUAL:${GENESIS_VDP_PLANE_IMPL},BATCHED>)
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
    target_link_libraries(XMen2Native PRIVATE comdlg32)   # host_sdl.cpp's ROM picker
endif()

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
