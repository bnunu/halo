# HANDOFF — context for the next agent (Claude Code + Fable)

This is the narrative context behind the current state. `CLAUDE.md` is the operational
contract (build, workflow, conventions); read that first. This file explains the *why* and
the history so you don't re-derive or repeat dead ends.

## The pivot

Work so far was done in a Cowork session (Claude Opus). We're moving the ongoing grind to
Claude Code running Fable 5 for sustained, long-horizon autonomous work. Everything needed
is in this repo tree plus the two setup facts below. Nothing about the approach changes —
same repo, same `cachebeta.xbe`, same build, same per-function workflow.

## The single most important thing we learned: the target is a DEBUG build

The project builds against **`cachebeta.xbe`**, MD5 `c7869590a1c64ad034e49a5ee0c02465`,
version `01.10.12.2276`. We burned real time discovering that **no retail disc produces
this file**:
- Tested all three USA retail pressings' `default.xbe` — original `e7b18148…`,
  Rev 1 `5bcda9f4…`, Rev 2 `7f6cc290…`. All report version 2276 but **none match**, and
  none are address-compatible (we verified by checking whether `kb.json` addresses land on
  real function prologues — retail scored ~8%, i.e. noise).
- The real file is the **Halo Xbox debug build 2276**, aka "cachebeta"/"2276betaP.xbe",
  preserved on the Internet Archive as *"Halo (2276, Oct 12, 2001) Pre-Release Build"*.
  Inside that build, the file named `2276betaP.xbe` is the exact `c7869590…` match.
- It is already extracted and placed at `halo-patched/cachebeta.xbe`. Don't re-hunt discs.

Why a debug build: it retains symbols, assert strings, and cleaner codegen, which is why
`kb.json` has real function/file names. This is standard for game decomps.

## What the build actually is (verified working)

The full pipeline runs and produces `halo-patched/default.xbe`. On Linux without root we
bootstrapped clang-14/lld/llvm via `apt-get download` + `dpkg -x` into `~/llvm` (exact
commands in `CLAUDE.md`), plus pip for cmake + the pinned python deps. On Windows use
Visual Studio/CMake or installed LLVM. The compile step is the one that must run on a real
machine — the rest of the tooling (`knowledge.py`, `patch.py`) is pure Python.

## Verification approach (and the faithfulness standard)

We verify each function two ways: (1) it compiles/links/patches cleanly (build stays
green), and (2) `llvm-objdump -dr` of the compiled object matches the target's instruction
stream. Crucially this is a **functional** decomp — the compiled output legitimately
differs from the target by import indirection on globals and equivalent codegen choices
(store coalescing, cmp-immediate vs load+test). Do not chase byte-identical output.

## xemu (runtime testing) — set up, but keep it secondary

We set up xemu on this machine and got **retail Halo booting to the main menu**, which
proved the emulator works. Getting **our** patched build to boot required: a properly
formatted HDD image, **System Memory = 128 MiB** (debug builds need it), Internal
resolution 1x (it was cranked to 10x and black-screened everything), the Complex BIOS, and
`Machine → Reset` after loading a disc. The lesson we learned the hard way: **do not
prioritize runtime testing over the build/diff loop.** With mostly-inert functions there's
nothing for a boot test to reveal; runtime only matters once real behavior accumulates.

## What got done

- Repo obtained (source ZIP — **no git**; set up git + a fork before opening PRs).
- `cachebeta.xbe` sourced, verified, and placed.
- Full build working; toolchain reproducible.
- Community: the disassembler is **Ghidra + XboxDev/ghidra-xbe** (confirmed on the project
  Discord). A sourcing question is posted in the Discord's #general.
- **Implemented functions: 40 → 106.** Kinds: empty/inert stubs, flag getters/setters,
  counters, null-check+clear dispose functions, `data_make_invalid` dispose pairs,
  `csmemset` init/dispose, `game_state_malloc` initializers (with real name strings
  recovered from the XBE), and small predicates using confirmed struct fields.
- 32 data globals declared in `kb.json`.
- Every addition build-verified and spot-checked via diff.

## What's left / where to go next

The trivial/mechanical frontier is exhausted (~214 functions remain on the frontier). From
here it's genuine per-function reverse-engineering: bigger control flow, FPU/float math,
and structs that need proper field names instead of the raw-offset casts we used as
stopgaps in a couple of spots (`local_player_count`, `game_map_loading_in_progress`).
Recommended first move next session: flesh out `game_globals_t` / `players_globals_t` in
`src/types.h` so those functions use named fields, then take medium functions one at a time.

## Files touched this session (added under `src/halo/`)

cache/cache_files_windows.c, camera/director.c, camera/observer.c,
cutscene/recorded_animations.c, effects/{contrails,effects,particle_systems,particles,
player_effects,weather_particle_systems}.c, game/{cheats,editor,game_allegiance,
game_statistics,player_rumble,shell_stubs}.c, input/input_abstraction.c,
interface/{event_manager,progress_bar}.c, main/console.c, math/random_math.c,
networking/network_game_globals.c, physics/{breakable_surfaces,point_physics}.c,
scenario/scenario.c, shell.c, sound/{game_sound,sound_classes,sound_manager}.c.
Appended to existing: game/game_time.c, game/game_engine.c, game/players.c,
cutscene/cinematics.c, input/input_xbox.c. Plus many `<common>` data entries in `kb.json`.

## Pointers

- Progress site: https://blam.info/progress/  ·  Repo: https://github.com/halo-re/halo
- Disc variant reference: https://github.com/CYRiXplaysHalo/HaloCombatEvolvedDiscVariantInformation
- Ghidra XBE loader: https://github.com/XboxDev/ghidra-xbe
- Full chronological log: `../DECOMP_WORKLOG.md`
