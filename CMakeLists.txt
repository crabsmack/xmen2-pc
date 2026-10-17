cmake_minimum_required(VERSION 3.20)
project(XMen2Recomp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# The Visual Studio generator's --parallel only runs projects side by side; /MP compiles a
# project's own files in parallel (XMen2Stepped and XMen2Verify have ~3,200 hand-C files each).
if(MSVC)
    add_compile_options(/MP)
    # No identical-code folding: game functions are values the game stores and compares (state
    # functions, hooks), so two with the same code need two addresses. Release links fold by
    # default, and DropRocket_StateFly became BrainRocket_StateFly (both re-arm themselves).
    add_link_options(/OPT:REF /OPT:NOICF)
endif()

# ---------------------------------------------------------------------------
# X-Men 2: Clone Wars, statically recompiled with segagenesisrecomp.
#
# Engine: the pinned segagenesisrecomp submodule (fork crabsmack/segagenesisrecomp),
# or GENESIS_RECOMP_ROOT for engine development. Game-owned inputs live in game/:
# game.toml + the discovery files tools/sync_disasm.py generates from the pinned
# xmen2disasm submodule, the GameSpec (xmen2_spec.c), and the ROM (gitignored,
# placed by tools/common.py). Generated C is build output (build/generated/xmen2/),
# regenerated from the ROM by the engine's CMake helper; never edit it.
# ---------------------------------------------------------------------------

set(GENESIS_RECOMP_ROOT "" CACHE PATH "Optional segagenesisrecomp checkout (default: the submodule)")
if(GENESIS_RECOMP_ROOT)
    set(RECOMP_ROOT "${GENESIS_RECOMP_ROOT}")
else()
    set(RECOMP_ROOT "${CMAKE_SOURCE_DIR}/segagenesisrecomp")
endif()
set(RUNNER_ROOT "${RECOMP_ROOT}/runner")

option(GEN_ENABLE_TRACE "Build the TCP cmd server + observability rings" ON)
if(GEN_ENABLE_TRACE)
    add_compile_definitions(GEN_ENABLE_TRACE=1)
endif()

if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${RUNNER_ROOT}/external/SDL2/SDL2-2.28.5/cmake")
endif()
find_package(SDL2 REQUIRED)

include("${RECOMP_ROOT}/cmake/GenesisRecompGenerated.cmake")
genesisrecomp_runner_sources(XMEN2_RUNNER_SOURCES TRACE ${GEN_ENABLE_TRACE})

set(XMEN2_INCLUDE_DIRS
    "${RUNNER_ROOT}"
    "${RUNNER_ROOT}/include"
    "${RUNNER_ROOT}/external/ymfm/src"
    "${RUNNER_ROOT}/external/superzazu"
    "${RUNNER_ROOT}/external/clowncommon"
    "${RUNNER_ROOT}/video"
    "${RECOMP_ROOT}/recompiler/src"
    "${CMAKE_SOURCE_DIR}/game"
    "${CMAKE_SOURCE_DIR}/game/include")
set(XMEN2_COMMON_DEFS INCLUDE_GENERATED_CODE=1 ENABLE_RECOMPILED_CODE=1 OWN_BACKEND=1
    GENESIS_GAME_VERSION="${PROJECT_VERSION}")

# One runner executable: the engine's runner sources, the game spec and SOURCES.
function(xmen2_runner TARGET PORT)
    cmake_parse_arguments(XR "" "" "DEFS;SOURCES;DEPENDS" ${ARGN})
    add_executable(${TARGET} ${XMEN2_RUNNER_SOURCES} ${XR_SOURCES} "${CMAKE_SOURCE_DIR}/game/xmen2_spec.c")
    if(XR_DEPENDS)
        add_dependencies(${TARGET} ${XR_DEPENDS})
    endif()
    genesisrecomp_runner_target(${TARGET})
    target_compile_definitions(${TARGET} PRIVATE ${XMEN2_COMMON_DEFS} DEFAULT_DEBUG_PORT=${PORT} ${XR_DEFS})
    target_include_directories(${TARGET} PRIVATE ${XMEN2_INCLUDE_DIRS})
    target_link_libraries(${TARGET} PRIVATE SDL2::SDL2main SDL2::SDL2)
    if(WIN32)
        target_link_libraries(${TARGET} PRIVATE ws2_32 comdlg32)
    endif()
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE /W3 /WX- /utf-8 /wd4102 /wd4996)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${RUNNER_ROOT}/external/SDL2/SDL2-2.28.5/lib/x64/SDL2.dll"
                "$<TARGET_FILE_DIR:${TARGET}>"
            COMMENT "Copying SDL2.dll")
    endif()
endfunction()

# ---------------------------------------------------------------------------
# The timed runner (phase 2; phase 4 D1): all generated code, cycle-exact. Its generated
# code is one object library, shared with the verification runner.
# ---------------------------------------------------------------------------
genesisrecomp_add_generated_sources(XMEN2_GENERATED xmen2 "${CMAKE_SOURCE_DIR}/game" xmen2.bin)
add_library(xmen2_timed_code OBJECT ${XMEN2_GENERATED})
add_dependencies(xmen2_timed_code ${XMEN2_GENERATED_TARGET})
target_compile_definitions(xmen2_timed_code PRIVATE ${XMEN2_COMMON_DEFS})
target_include_directories(xmen2_timed_code PRIVATE ${XMEN2_INCLUDE_DIRS})
if(MSVC)
    target_compile_options(xmen2_timed_code PRIVATE /W3 /WX- /utf-8 /wd4102 /wd4996)
endif()
xmen2_runner(XMen2Recomp 4382 SOURCES $<TARGET_OBJECTS:xmen2_timed_code>)

# ---------------------------------------------------------------------------
# Phase 4. Hand-written C (game/c) joins a runner through the bridge tools/bridge.py writes
# (xmen2check.py build runs it first): build/bridge for the stepped runner, build/bridge_verify
# for the verification runner. handc.cmake lists a wrapper per C file (build/bridge/handc/
# <ADDR>.cpp) that renames its function to hc_<Name> (the bridge's <Name> counts the entry and
# calls it); the wrappers share one set of compile options, so /MP compiles them in parallel.
# ---------------------------------------------------------------------------
set(XMEN2_BRIDGE_DIR "${CMAKE_BINARY_DIR}/bridge")
set(XMEN2_VERIFY_BRIDGE_DIR "${CMAKE_BINARY_DIR}/bridge_verify")
set(XMEN2_HANDC_SOURCES "")
include("${XMEN2_BRIDGE_DIR}/handc.cmake" OPTIONAL)

# The stepped runner (D2): the same game.toml generated with --untimed (the exact-mode code
# shape without XI/XC timing), scheduled by logical position from a timed run's replay log
# (GENESIS_REPLAY_IN). Functions written in C are left out (--skip): their slot entries
# take every call.
genesisrecomp_add_generated_sources(XMEN2S_GENERATED xmen2s "${CMAKE_SOURCE_DIR}/game" xmen2.bin
    EXTRA_ARGS --untimed --prefix xmen2s --skip "${XMEN2_BRIDGE_DIR}/handc_skip.txt"
    EXTRA_DEPENDS "${XMEN2_BRIDGE_DIR}/handc_skip.txt")
xmen2_runner(XMen2Stepped 4383 DEFS XMEN2_STEPPED=1 DEPENDS ${XMEN2S_GENERATED_TARGET}
    SOURCES ${XMEN2S_GENERATED} ${XMEN2_HANDC_SOURCES}
            "${XMEN2_BRIDGE_DIR}/xmen2_bridge.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/handc_rt.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/asm_conv.cpp")

# The same stepped runner with ALL generated code (no hand C): the reference D2/D6 compare the
# C against (xmen2disasm P4-decomp.md decision 5).
genesisrecomp_add_generated_sources(XMEN2G_GENERATED xmen2g "${CMAKE_SOURCE_DIR}/game" xmen2.bin
    EXTRA_ARGS --untimed --prefix xmen2g)
xmen2_runner(XMen2SteppedGen 4385 DEFS XMEN2_STEPPED=1 DEPENDS ${XMEN2G_GENERATED_TARGET}
    SOURCES ${XMEN2G_GENERATED})

# The verification runner (D4 `verify`): the timed runner with ALL generated code, plus the
# hand-written C run beside it as shadow tests (game/runtime/verify.cpp).
xmen2_runner(XMen2Verify 4384 DEFS XMEN2_VERIFY=1
    SOURCES $<TARGET_OBJECTS:xmen2_timed_code> ${XMEN2_HANDC_SOURCES}
            "${XMEN2_VERIFY_BRIDGE_DIR}/xmen2_bridge.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/handc_rt.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/asm_conv.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/verify.cpp"
            "${CMAKE_SOURCE_DIR}/game/runtime/crash_win.cpp")