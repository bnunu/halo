# CLAUDE.md — Halo: Combat Evolved Decompilation (working context)

> This file is auto-loaded by Claude Code. It is the operational contract for working
> on this repo. Read it fully before touching anything. A longer narrative of how the
> project got to its current state is in `HANDOFF.md` and `../DECOMP_WORKLOG.md`.

## What this project is

An open-source **functional re-implementation ("decompilation")** of the original Xbox
launch title *Halo: Combat Evolved*. It is the `halo-re/halo` research project.

We reverse-engineer functions from a specific build's executable and re-implement them in
C. The build compiles our C and **patches redirects** into the original executable so our
implementations run instead of the originals. This is a **functional** decomp (redirect
patching), **not** a strict byte-matching decomp.

The target executable is the **Halo Xbox debug build 2276** ("cachebeta"):
- File: `halo-patched/cachebeta.xbe` (already in place)
- MD5: `c7869590a1c64ad034e49a5ee0c02465`
- Version string: `01.10.12.2276` (build dated Oct 12, 2001)
- It is a **debug** build (richer symbols/asserts), NOT any retail disc. Do not swap in a
  retail `default.xbe` — the addresses in `kb.json` are absolute VAs into THIS exact file.

## Current state (keep this updated)

- **162 / 336** declared functions implemented (patched redirects counted from the build log).
  Note: the earlier "106" figure over-counted — `cutscene/cinematics.c` existed but was
  missing from `src/CMakeLists.txt` and silently never compiled (fixed).
- **~55** data globals declared in `kb.json` `<common>`. Key identifications this session:
  the full `data.c` API (`data_new` 0x1194d0, `data_dispose` 0x119520, `data_delete_all`
  0x119720, `data_make_valid` 0x119b20), `debug_malloc`/`debug_free` (0x8ee60/0x8ef70,
  new `debug_memory.obj`), `terminal_gets_end` (0xe3560), `update_server_globals`
  (0x4570c0, source `game/player_queues_new.c`).
- Build is **green** on Windows (LLVM 22 + CMake + Ninja via winget; see below).
- **Git + fork are live**: branch `decomp-batch-1` on top of `upstream/main`, pushed to
  fork `origin` = github.com/bnunu/halo (`upstream` = github.com/halo-re/halo). gh CLI
  authenticated as bnunu. PRs not yet opened — coordinate scope with the maintainers first.

## Key files

- `kb.json` — the knowledge base. Every function/global has an absolute address. To add a
  function you implement it in `src/…` AND declare any globals it touches here under the
  `<common>` object's `data` array (`{"decl": "...", "addr": "0x..."}`).
- `src/types.h` — **struct definitions. Always confirm field offsets here. NEVER guess a
  struct field — a wrong field is a wrong decompilation.**
- `src/CMakeLists.txt` — list of compiled sources. New `.c` files must be added here.
- `tools/knowledge.py` — generates `decl.h`, import `.def`, and thunks from `kb.json`.
- `tools/patch.py` — patches the compiled output into the XBE → `halo-patched/default.xbe`.
- `halo-patched/cachebeta.xbe` — the target (required to build). Do not commit it (copyright).

## How to build (the interface)

Requirements: **clang + lld (`lld-link`) + cmake + python3** with these exact pins:
`pefile~=2023.2.7`, `pyxbe~=1.0.2`, `libclang~=16.0.0` (see `requirements.txt`).

Configure + build (LLVM toolchain path):
```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=toolchains/llvm.cmake
cmake --build build
```
Success looks like `Patching "<fn>" ... redirect to re-implementation` lines and
`Build complete`, producing `halo-patched/default.xbe`.

### On Windows (this machine — verified working)
Installed via winget: `LLVM.LLVM` (22.1.8), `Kitware.CMake` (4.4), `Python.Python.3.12`,
`Ninja-build.Ninja`, `GitHub.cli`. None are on PATH in fresh shells; prepend:
`C:\Program Files\LLVM\bin`, `C:\Program Files\CMake\bin`,
`C:\Users\isabe\AppData\Local\Programs\Python\Python312`.
Python deps: `pip install -r requirements.txt` plus `setuptools<81` (newer setuptools
removed `pkg_resources`, which `tools/check_requirements.py` imports) and `capstone`
(for analysis scripts). Configure (note the extra flags — no MSVC CRT is installed, so
CMake's compiler test must build a static lib, and ninja isn't on PATH):
```
cmake -B build -S . -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchains/llvm.cmake \
  -DCMAKE_MAKE_PROGRAM=C:/Users/isabe/AppData/Local/Microsoft/WinGet/Packages/Ninja-build.Ninja_Microsoft.Winget.Source_8wekyb3d8bbwe/ninja.exe \
  -DPython3_EXECUTABLE=C:/Users/isabe/AppData/Local/Programs/Python/Python312/python.exe \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
cmake --build build
```
The built exe is `build/halo` (no extension). The project also supports MSVC via
`cmake -AWin32 -Bbuild -S .` if Visual Studio is ever installed.

### On Linux WITHOUT root (verified working — this is how it was built during handoff)
`apt` needs root, but `apt-get download` does not. Bootstrap a user-space toolchain:
```
mkdir -p /tmp/debs && cd /tmp/debs
apt-get download clang-14 lld-14 libllvm14 libclang-cpp14 \
    libclang-common-14-dev libclang1-14 llvm-14-linker-tools llvm-14
mkdir -p ~/llvm && for d in *.deb; do dpkg-deb -x "$d" ~/llvm; done
export PATH=~/llvm/usr/lib/llvm-14/bin:$HOME/.local/bin:$PATH
export LD_LIBRARY_PATH=~/llvm/usr/lib/llvm-14/lib:~/llvm/usr/lib/x86_64-linux-gnu
pip install --break-system-packages cmake pefile~=2023.2.7 pyxbe~=1.0.2 libclang~=16.0.0
```
Then the `cmake` commands above (cmake resolves to the pip one in `~/.local/bin`).

## The per-function workflow (do this for every function)

1. **Pick a frontier function** — one that is not yet implemented but is *called by* an
   implemented one. (Script: build addr→name from `kb.json`, disassemble implemented
   functions, collect call targets that aren't implemented.)
2. **Disassemble the target** at its `kb.json` address inside `cachebeta.xbe`.
   Tooling: Python + `capstone` (x86, 32-bit) for quick reads; the community standard is
   **Ghidra + the XboxDev `ghidra-xbe` extension** (github.com/XboxDev/ghidra-xbe).
3. **Confirm struct offsets in `src/types.h`.** If the function reads `[ptr+0x24]`, find
   which field is at 0x24. Do not guess.
4. **Recover string/const literals from the XBE** when needed (e.g. the `name` arg to
   `game_state_malloc` — read the null-terminated string at the pushed address).
5. **Declare referenced globals** in `kb.json` `<common>.data`. Naming: `byte_/word_/dword_`
   + hex addr for raw scalars; `data_t *name` for data handles; meaningful names when clear.
6. **Write faithful C** in the source file mapped by the function's `kb.json` object
   `source` field (create the file if needed and add it to `src/CMakeLists.txt`).
7. **Build** and confirm the function is patched and the build is green.
8. **Verify** with `llvm-objdump -dr --disassemble-symbols=_<fn> <obj>` and compare the
   instruction stream to the original disassembly.

## Faithfulness standard (READ THIS)

Judge a match by **identical logic/operations**, not identical bytes. Expected, acceptable
differences vs the target:
- **Import indirection**: our globals are reached via `mov eax,[__imp__global]; mov …,(eax)`
  instead of a direct absolute `mov …,[0xADDR]`. This is inherent and present in every
  accepted function.
- **Equivalent codegen**: compiler may coalesce two adjacent byte stores into one word
  store, use `cmp $0, mem` instead of `mov`/`test`, choose `add esp,4` vs `pop`, etc.
Final behavioral proof is running in xemu (secondary; see below).

## Conventions (match existing code)

- Asserts: `assert_halt(cond)`.
- Access globals through typed pointers declared in `kb.json`.
- Unknown struct fields are named `unk_<hexoffset>`; unknown raw globals `byte_/word_/dword_<addr>`.
- Prefer improving `types.h` structs (naming `unk_` fields) over raw-offset casts when you
  have enough evidence — several functions currently use casts as a stopgap (see Next steps).

## Compiler flags / gotchas

- Flags: `-Wall -Werror -target i386-pc-win32 -march=pentium3 -nostdlib -ffreestanding
  -fno-builtin -fno-exceptions -include src/common.h`.
- `-Wall` does NOT include `-Wunused-parameter`, so empty functions with unused params are fine.
- Non-void functions MUST return a value (`-Werror` will catch it).
- Watch pointer↔int: e.g. `dword_50548c` is declared `int` but actually holds a pointer —
  cast explicitly: `dword_50548c = (int)game_state_malloc(...)`.
- All `kb.json` addresses are absolute VAs into `cachebeta.xbe`.

## Runtime testing in xemu (secondary — do NOT prioritize over the build/diff loop)

xemu is set up on this machine. To boot our build:
1. Copy the full **2276 build directory** (default.xbe + maps/ + bink/ etc.), replace its
   `default.xbe` with our `halo-patched/default.xbe`.
2. Pack an ISO: `extract-xiso -c "<that dir>" out.iso`.
3. In xemu: **System Memory = 128 MiB** (required for the debug build), Internal
   resolution = 1x, a properly-formatted HDD image, Complex BIOS. Load the ISO, then
   **Machine → Reset** to boot (resetting from the menu is required each load).
Runtime is only worth doing once meaningful *behavioral* changes accumulate; the primary
verification is build + `llvm-objdump` diff.

## Next steps (recommended order)

1. **Refine structs** so raw-offset casts become named fields: `game_globals_t` (offsets 3,4
   used by `game_map_loading_in_progress`) and `players_globals_t` (offset 0x24 used by
   `local_player_count`). Split the `unk_N[]` arrays without changing total size.
2. Work the **medium control-flow** frontier functions one at a time.
3. Handle **FPU/float** functions (`render_camera_get_adjusted_field_of_view_tangent`,
   `point_physics_initialize_for_new_map`) — read the float constants from the XBE.
4. **Set up git**, fork `halo-re/halo`, and open PRs. Coordinate on the project Discord
   (a sourcing question is already posted there) so work isn't duplicated.
