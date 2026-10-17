/* genesis_build_info.h for reproducible runners (CMakeLists.txt XMEN2_REPRODUCIBLE): found before
 * the engine's generated one, which holds `git describe` of the engine and this repo and so
 * changes the executable at every commit, code or not. The runner's evidence files still carry
 * its exact identity: the fingerprint of its own executable (engine runtime_evidence.c). */
#define GENESIS_BUILD_ENGINE_GIT "reproducible (see the executable fingerprint)"
#define GENESIS_BUILD_GAME_GIT "reproducible (see the executable fingerprint)"
