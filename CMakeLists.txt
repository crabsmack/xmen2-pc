cmake_minimum_required(VERSION 3.20)
project(XMen2Recomp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

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
genesisrecomp_add_generated_sources(XMEN2_GENERATED xmen2 "${CMAKE_SOURCE_DIR}/game" xmen2.bin)
genesisrecomp_runner_sources(XMEN2_RUNNER_SOURCES TRACE ${GEN_ENABLE_TRACE})

add_executable(XMen2Recomp
    ${XMEN2_RUNNER_SOURCES}
    ${XMEN2_GENERATED}
    "${CMAKE_SOURCE_DIR}/game/xmen2_spec.c")
add_dependencies(XMen2Recomp ${XMEN2_GENERATED_TARGET})
genesisrecomp_runner_target(XMen2Recomp)

target_compile_definitions(XMen2Recomp PRIVATE
    INCLUDE_GENERATED_CODE=1
    ENABLE_RECOMPILED_CODE=1
    OWN_BACKEND=1
    DEFAULT_DEBUG_PORT=4382
    GENESIS_GAME_VERSION="${PROJECT_VERSION}")
target_include_directories(XMen2Recomp PRIVATE
    "${RUNNER_ROOT}"
    "${RUNNER_ROOT}/include"
    "${RUNNER_ROOT}/external/ymfm/src"
    "${RUNNER_ROOT}/external/superzazu"
    "${RUNNER_ROOT}/external/clowncommon"
    "${RUNNER_ROOT}/video"
    "${RECOMP_ROOT}/recompiler/src"
    "${CMAKE_SOURCE_DIR}/game")
target_link_libraries(XMen2Recomp PRIVATE SDL2::SDL2main SDL2::SDL2)
if(WIN32)
    target_link_libraries(XMen2Recomp PRIVATE ws2_32 comdlg32)
endif()
if(MSVC)
    target_compile_options(XMen2Recomp PRIVATE /W3 /WX- /utf-8 /wd4102 /wd4996)
    add_custom_command(TARGET XMen2Recomp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RUNNER_ROOT}/external/SDL2/SDL2-2.28.5/lib/x64/SDL2.dll"
            "$<TARGET_FILE_DIR:XMen2Recomp>"
        COMMENT "Copying SDL2.dll")
endif()
