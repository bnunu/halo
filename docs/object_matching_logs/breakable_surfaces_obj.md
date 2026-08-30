# `breakable_surfaces.obj` exact-match strategy log

## Scope and provenance

- Source TU: `source/physics/breakable_surfaces.c`
- Target object: `source/physics/breakable_surfaces.obj`
- Target build: Halo January 2002 Xbox build
- Compiler: XDK 3911 Microsoft C/C++ 13.00.9254.1
- Flags: `/O2 /Oy- /Ow /QIfist /DDEBUG /Dxbox`
- Comparator: hardened COFF function and section comparison
- Status: active closeout lane; no matching credit has been granted

The rough October source at
`C:\Users\isabe\Downloads\halo-main (1)\halo-main\src\halo\physics\breakable_surfaces.c`
is a topology hint only. January machine code, relocations, and data ownership
remain authoritative.

## Current validated state (updated 2026-08-30, session 3)

- Functions: `11/12` strict exact.
- Only residual: `_breakable_surface_effect`.
- Target/current padded size: `0xFC0/0xF90` (`4032/3984`) — the honest
  deficit is exactly the four forbidden `fast_ftol` expansions; the earlier
  `4032/4032` state rested on compensating union stores and is superseded.
- Target/current relocations: `117/117`, with the same semantic destinations.
- Target normalized SHA-256:
  `510486a7d0a1f2fc1dfb8f0c47990c57f83ce1030ef5e4db8dfc42943dc56bfa`
- Current normalized SHA-256:
  `3e92357588ee17cc138699dbfa7476727653565b02d04460b1815093b9141e81`
- All 11 protected sibling functions remain strict exact.

The current source is the recovered origin-mirror topology (2026-08-30
part 2): natural block scoping with no union, `real_point2d jitter`,
inner-scope `new_particle_data particle`, `real u` interpolate argument,
`rectangle2d bounds`, `vertex_point` lvalues, and the four conversions
spelled `(short)(long)(real)ceil/floor(PIN(...))` in place of January's
assembly `fast_ftol`. The analysis witness built from the same text plus the
verbatim helper reaches **20 differing instructions** at identical size,
relocations, frame and stack homes (session 3 corrected an earlier figure
of 4, which depended on a shared-header edit the whole board refutes).

## Accepted controls retained in source

### E09: traversal/audio scratch union

January reuses one `0x58`-byte region for traversal scratch and the later
`sound_location`. The retained C89 union models the measured layout:

- traversal `s_offset`, `t_offset`, two projected points, and eight 2D
  vertices;
- audio view with `sound_location` at union offset `0x2C`;
- both views end at the same byte.

This is defined lifetime-based storage reuse, not aliasing or byte padding.

### E18: `rectangle2d` grid bounds

Replacing four unrelated short locals with `rectangle2d grid_bounds` moved
the candidate from `0xF90` to the exact target size `0xFC0` while preserving
`117/117` relocations and every exact sibling.

The native structure layout is exactly the order proved by January:

| Structure field | January meaning | January home |
|---|---|---:|
| `grid_bounds.y0` | `t_min` | `-0x5C` |
| `grid_bounds.x0` | `s_min` | `-0x5A` |
| `grid_bounds.y1` | `t_max` | `-0x58` |
| `grid_bounds.x1` | `s_max` | `-0x56` |

This is an evidence-backed type recovery and remains in production even
though the function is not yet exact.

## Current stack/codegen finding

E18 makes every large aggregate start exactly 16 bytes above the January
home:

| Region | Target | E18 |
|---|---:|---:|
| `surface_queue[1024]` | `-0x1240` | `-0x1230` |
| `surface_vertices3d[8]` | `-0x240` | `-0x230` |
| traversal/audio scratch | `-0x1E0` | `-0x1D0` |
| `new_particle_data` | `-0x188` | `-0x178` |

The missing 16 bytes are four January dword integer-conversion homes. For
each grid bound January emits:

```asm
call  _ceil/_floor
fstp  dword ptr [ebp-real_home]
add   esp, 8
fld   dword ptr [ebp-real_home]
fistp dword ptr [ebp-distinct_integer_home]
mov   r16, word ptr [ebp-distinct_integer_home]
mov   word ptr [ebp-grid_bound], r16
```

E18 instead emits `fistp qword` to one shared compiler temporary. The target
therefore rounds through single precision and owns four distinct dword
conversion homes; the present compiler forwards through every normal
readable-C spelling tested.

## Experiment matrix

Every reverted experiment preserved all 11 exact siblings unless explicitly
noted.

| ID | Source or compiler experiment | Result | Decision |
|---|---|---|---|
| E01 | Move `position` and `position_2d_test` inward | byte-neutral | reverted |
| E02 | Move both vertex arrays into the `while` scope | byte-neutral | reverted |
| E03 | Declare the 2D vertex array later | byte-neutral | reverted |
| E04 | Remove the duplicate queue-read shadow | byte-neutral | reverted |
| E05 | Hoist random offsets to function scope | byte-neutral | reverted |
| E06 | One named `real` between ceil/floor and cast | byte-neutral; store forwarded | reverted |
| E07 | Group grid bounds and loop indices | byte-neutral | reverted |
| E08 | True sibling traversal/audio scopes | frame `0x1228`; proved lifetime reuse | superseded by E09 |
| E09 | Meaningful `0x58` C89 scratch union | size `0xF90`, frame `0x122C` | retained |
| E10 | Remove explicit `long` from the grid casts | byte-neutral | reverted |
| E11 | Static grid-index helper | caller byte-neutral; unwanted helper COMDAT | reverted |
| E12 | Named `real` plus four named long homes | optimizer eliminated homes | reverted |
| E13 | Aggregate containing rounding/conversion fields | optimizer eliminated homes | reverted |
| E14 | Address escape / compiler-barrier shape | did not produce target chain | reverted |
| E15 | Conversion-home array plus barrier | did not produce target chain | reverted |
| E16 | Pointer helper plus barrier | did not produce target chain | reverted |
| E17 | `__forceinline` float ceil/floor wrappers | caller byte-neutral; two unwanted COMDATs | reverted |
| E18 | Native `rectangle2d grid_bounds` | exact `0xFC0`, `117/117`; correct bound layout | retained |
| E19 | `real_rectangle2d` staging plus E18 | byte-identical to E18 | reverted |
| E20 | Four named long homes plus E18 | byte-identical to E18 | reverted |
| E21 | Historical function-scope bounds plus `long` loop indices | regressed to `0xF70`; hash `f32f3d8f...` | reverted |
| E22 | Move only `rectangle2d grid_bounds` to function scope | byte-identical to E18 | reverted |
| E23 | Change only the two grid loop indices to `long` | same `0xF70` regression as E21 | reverted |
| A01 | Compile E18 as C++ (`/TP`) | function byte-neutral; decorated symbols | analysis only |
| A02 | `/TP` with explicit `ceilf`/`floorf` | unwanted helpers; caller byte-neutral | analysis only |
| A03 | Remove `/Ow` | byte-neutral | analysis only |
| A04 | Enable `/Op` | `4320` bytes, `114` relocs; broad regression | rejected |
| A05 | Minimal direct/named/four-value float-to-short fixtures | every form collapsed to shared `fistp qword`; no target motif | analysis only |
| A06 | Four legal union-backed long/short conversion objects | also collapsed to shared `fistp qword` | analysis only |

## Corpus and compiler audits

- A relocation-aware scan of all 833 January split objects found the complete
  float-home conversion motif only eight times: four here and four in
  nonexact `_build_structure_lens_flares`.
- A scan of 467 rebuilt base objects found zero occurrences.
- `_build_structure_lens_flares` is not a reconstructed-C donor: target/base
  is `4336/4000` bytes and `156/157` relocations.
- All locally available compatible compiler sets are byte-identical
  VC7 13.00.9254.1 binaries. No alternate compatible compiler/QFE is locally
  available.
- Upstream history commit
  `95e6477328dd7742175074de5eab0a20c3e7bde0` provided a real source-shape
  lead: function-scope bounds and `long` grid indices. E21-E23 showed that
  bound scope is neutral and the historical `long` index type causes the
  same `0xF70` regression, so it is not the January topology.
- The reproducible `round_probe.c` fixture proves that direct, named-real,
  named-long, four-output, and four-longs-first forms all lower through one
  shared `fistp qword` home under the campaign compiler.

Reproducible evidence is under
`research/breakable_surfaces_closeout/`, especially
`STACK_SLOT_LIFETIME_MAP.md` and `donor_census/REPORT.md`.

## Do not repeat

- E01-E17 and E19-E23, including cosmetic variations with equivalent
  lifetimes.
- C++ mode, `ceilf` wrappers, `/Ow` removal, or `/Op`.
- Adding guessed padding merely to force the missing 16 frame bytes.
- Treating `_build_structure_lens_flares` as an exact donor.
- Inline assembly, `volatile` byte forcing, undefined behavior, object-byte
  patches, or compiler-flag changes (the recommended `/QIfist` removal is an
  owner reconciliation action, not a lane experiment).
- 2026-08-30 additions: hunting a C spelling for the four `fistp dword`
  chains (mechanism-proven impossible — see the conversion-width law);
  re-measuring `surface_vertices3d` scope (byte-inert in both basins);
  landing the W3 offset flow while the unit still builds with `/QIfist`
  (regresses the checkpoint to 3984); re-sweeping the witness's remaining
  dot-order and frame-placement ties outside a funded lottery session.

## 2026-07-28 provenance audit: January PDB, HCEX PDB, and local source corpus

A second provenance pass found a strong source-semantics lead but no new
legal-C code-generation lever.

### January PDB negative result

`C:\Users\isabe\Downloads\Halo Combat Evolved (Jan 14, 2002 prototype)\cachebeta.pdb`
names `\halo\objects\halobetacache\breakable_surfaces.obj` as module stream
154. The complete module stream is only 132 bytes: one `S_OBJNAME` record,
one compiler record (`Microsoft (R) Optimizing Compiler`), and four zero tail
bytes. It contains no function, local, type, or line records, so it cannot
recover the grid-bound declarations, any rounded `real`, or the four
conversion homes. The raw compiler record must not be treated as proof of an
unrecorded precision flag.

### HCEX source-line and code-generation evidence

The later debug and release symbol files are:

- `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\research\hcea_jun2011_prototype\payload\Halo CE Anniversary (Jun 24 2011)\HCEX.pdb`
- `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\research\hcea_jun2011_prototype\payload\Halo CE Anniversary (Jun 24 2011)\HCEX_Release.pdb`

They identify `breakable_surface_effect` as source
`d:\projects\code\hcex\sources\physics\breakable_surfaces.c`, source MD5
`CEACA3EE6F74879855CD2005D8B6C1AC`. Debug RVA/length/frame are
`0x00D0C1A0/0xE68/0x1360`; release RVA is `0x01743190` with the same
length/frame. Neither PDB contains injected source.

DIA line records map the four bounds to four consecutive source statements:
lines 423-426 are the two `ceil` assignments followed by the two `floor`
assignments; line 421 is the nonzero-density test and line 440 begins the
grid loop. There are no intervening source lines for a separate named-real
assignment/conversion pair. The surviving named-local records also contain
no rounded-real/grid aggregate or scalar grid-bound names, although
optimized-away locals cannot be ruled out from that absence alone.

PowerPC disassembly of the decoded debug image at flat RVA `0xD0C924` is
stronger semantic evidence. Each libm result is immediately rounded from
double to float with `frsp` (first at `0xD0C954`), then converted with
`fctiwz`/`stfd` and sign-extended to a short; the four results remain scalar
GPR values. This is consistent with four direct
`(short)(real)ceil/floor(...)` assignments and inconsistent with a required
separate named `real` statement. PowerPC cannot distinguish the optional
intermediate `(long)` because both `int` and `long` are 32-bit there.

### Source-corpus and macro negatives

A hash census of 747 local `breakable_surfaces.c` files found 11 unique
contents and no copy matching HCEX MD5
`CEACA3EE6F74879855CD2005D8B6C1AC`. Repository history contains
reconstruction commits only, not an authentic Bungie source blob. The
October file at
`C:\Users\isabe\Downloads\halo-main (1)\halo-main\src\halo\physics\breakable_surfaces.c`
is a decompiler reconstruction: it defines replacement `breakable_floor` and
`breakable_ceil` functions and has grid comments/order inconsistent with the
machine-code topology, so its named `float val` is not provenance.

The active type and macro audit found `typedef float real` in
`source/math/real_math.h` and ordinary ternary `PIN` in
`source/cseries/cseries.h`; no real-to-short rounding macro exists in the
recovered source. The closest source sibling,
`source/structures/structure_lens_flares.c`, spells all four bounds directly
as `(short)(real)ceil/floor(PIN(...))`, and its January target has the same
float-home conversion motif, but that function is itself nonexact and cannot
serve as a strict donor.

### Verdict and do-not-repeat expansion

The highest-likelihood original spelling is four direct scalar assignments
with an explicit `(real)` cast, probably without the reconstruction-only
`(long)`. That spelling, named-real forms, named-long homes,
aggregates/arrays, address escapes, helpers/wrappers, scope changes, C++
mode, and precision-flag probes have already been measured and do not make
the campaign VC7 emit January's chain. No genuinely untried legal-C
hypothesis now satisfies the reopen criterion. Do not repeat the
January-PDB private-record search, HCEX injected-source/source-hash search,
October-source audit, macro/type audit, or local source-hash census; reopen
only if an authentic source blob, a distinct compatible compiler/QFE, or a
strict donor artifact appears.

## 2026-08-30 mechanism adjudication, witness reconstruction, and landing

Session worktree `claude/breakable-surfaces-20260830` from canonical commit
`6bb7e2ec9`. Three results: the adjudicated E09/E18 source state is finally
landed on the canonical line, the four-home conversion motif is
mechanism-proven as inline-assembly `fast_ftol` expansions, and three new
witness-proven ordinary-C topology recoveries are landed in production. The
object remains `NonMatching` with zero credit.

### Canonical-line reconciliation

The canonical production blob predated this ledger's "current validated
state": the E09 union and E18 `rectangle2d` shape lived only on
`codex/breakable-surfaces-finish-20260728` (commit `d07bd0d58`), which is
not an ancestor of canon; the backfilled ledger described a state the tree
did not contain. Measured canonical baseline: residual `3952/4032`. Porting
the codex blob reproduced the documented checkpoint exactly
(`4032/4032`, `117/117`, normalized SHA-256
`fcf66a3ce196b5295b412824a98f874123effcdf9f3043e6a1cd9bb98bfcaef2`, 11/11
siblings), and this session landed it.

### Mechanism adjudication: the four homes are `fast_ftol` expansions

The 2026-08-28/29 lens-flares campaign proved the conversion-width law
(`/QIfist` lowers EVERY C float/double-to-integer conversion through one
64-bit `fistp qword`, across all destination types, stagings, and flags; a
32-bit inline `fistp` is never VC7 13.00.9254 compiler output) and
attributed the staged January motif to inline expansion of the historical
`__inline long fast_ftol(float d) { long result; __asm { fld d; fistp
result } return result; }` from `cseries.h` (verbatim in non-ancestor
commit `5093ac1a1`; HCEX-PDB-attested; byte-exact standalone twin in
January actor_combat.obj). See
`docs/object_matching_logs/structure_lens_flares_obj_jonas_policy_reconciled_20260829.md`.

This session verified the attribution byte-level for THIS object. January's
four sites decode as exactly the expansion shape:

- one shared single-precision float home `[ebp-8]` — the inlined parameter
  `d`, which also receives the `call _ceil/_floor` double result (the
  `fstp dword`/`fld dword` "round trip" is the float parameter store), and
  is later even reused as an integer loop temp;
- four distinct dword result homes `[ebp-0x120]`, `[ebp-0x118]`,
  `[ebp-0x108]`, `[ebp-0xFC]` — each expansion's `result` local;
- the `(short)` reads (`mov r16, word ptr [result]` or a full dword load
  then `mov word`) are the callers' truncations.

An analysis-only witness (never production; owner rule: no `__asm`, period)
respelled the four sites `(short)fast_ftol(ceil/floor(PIN(...)))` with the
verbatim helper. All four call sites immediately aligned at January's exact
section offsets and the frame became `0x1240` exact.

### Flag-provenance result for this unit

The witness is **byte-identical with and without `/QIfist`** (no compiler
conversion remains once the helper owns all four sites), so nothing in the
January object requires `/QIfist`, and the flag is provably unnecessary to
explain any January byte in this TU — the same conclusion the owner reached
when removing the lens-flares per-unit `/QIfist` exception. All 11 exact
siblings are also byte-identical without `/QIfist` (none contains a
conversion). Full decision table, campaign compiler:

| Configuration | Padded size | Relocations |
| --- | ---: | ---: |
| `/QIfist` (current config), landed source | 4032/4032 | 117/117 |
| `/QIfist`, landed source + W3 offset flow | 3984/4032 | 117/117 |
| uniform profile (no `/QIfist`), landed source | 4000/4032 | 121/117 (four `_ftol`) |
| uniform profile, landed source + W3 offset flow | 3952/4032 | 121/117 |
| fast_ftol witness, either flag state | 4032/4032 | 117/117 |

The current `4032/117` checkpoint therefore rests on two compensating
inauthenticities: the disproven flag emits a shared `fistp qword`, and the
E09 union member stores in the offset block supply roughly the bytes the
missing `fast_ftol` expansions would. Recommendation to the owner: when
reconciling this unit with the uniform profile (as done for lens-flares),
remove the per-unit `/QIfist` and land the W3 offset flow together; this
ledger records the honest measurements for that basin in advance. The flag
was deliberately left unchanged this session — that reconciliation is an
owner policy action, and the lens precedent shows it done as one.

### Witness reconstruction: three ordinary-C recoveries and the final gap

Bounded single-factor experiments in the witness basin (full stack:
`research/breakable_surfaces_closeout/witness_fast_ftol_stack_20260830.json`):

| Step | Content | Result |
| --- | --- | --- |
| W1 | helper + four `(short)fast_ftol(...)` sites | 4064; all four call sites at January offsets |
| W5 | + both random offsets stored to union members, products passed as expressions | 4032 size-exact; extra `fstp`/temp traffic vs January |
| W3 | s random stored to the union member, `real t_offset` plain local, both `point_from_line3d` factors passed as expressions | offset region instruction-for-instruction identical to January (unstored second random, `fxch st(1)`, hoisted `(real)t_index` spill); 4016 overall |
| W4 | both offsets plain locals | 4016; January stores the first random at the scratch base `[ebp-0x1E0]`, so the member store is right for s only |
| X1 | named `real random_value` = the `rgb_colors_interpolate` fifth argument | January's temp-store/reload/push of that random reproduced; large cascade (LCS replaced 53 -> 28) |
| X2 | named `long sound_definition_index` scoping the sound epilogue | January's registerized compare and the whole forward/velocity/game_location copy choreography reproduced |
| X3 | alpha PIN multiply spelled `(upper - lower) * real_local_random()` | January's non-popping `fmul st(1)` + deferred `fstp st(0)` in all three macro arms reproduced; instruction counts equalize |
| probe | `surface_vertices3d` function scope vs block scope | byte-identical in both basins (layout-inert) |

Final witness: padded `4032/4032`, relocations `117/117`, instructions
`1156/1156`, frame `0x1240`, siblings 11/11, every stack home identical,
and **20 differing instruction lines** (see the session-3 correction
below; an earlier figure of 4 in this session was produced by a refuted
shared-header edit). The complete remaining divergence is:

1. four dot-product accumulation orders (8 `fmul` lines): the same
   `dot_product3d` inline emits January's k,j,i in the four vertex-point
   distance expansions where ours emits k,i,j (both reassociated away from
   the written i,j,k; the first-vertex arm matches, so the choice is
   context-driven, not the inline's text);
2. frame placement: January allocates `new_particle_data` at `-0x188` with
   the conversion/staging temp cluster shallower (`-0x12C..-0xF8`); ours
   swaps the two groups, with a consequent mid-frame slot rotation.

Both are the certified allocation/scheduling tie classes from the
lens-flares campaign (per-basin lottery / definition-position territory).
They matter only to the inadmissible witness, so they were enumerated and
left unhunted.

### Production landing and gates

Landed in production: the ported E09/E18 state plus X1, X2, X3 — all
ordinary C, each individually January-proven in the witness basin, and
jointly size/relocation-preserving under the current config. Measured:
`4032/4032`, `117/117`, normalized SHA-256
`71d6cbcb08cc60c74cedb83be6993be23705fc458e2d66c4f3fb2bcf1b021b60`;
normalized-instruction LCS distance to January improved (replaced
175 -> 144, ours-only 23 -> 20). The W3 offset flow is deliberately NOT
landed while the unit builds with `/QIfist` (table above).

Gates, all pass: full ninja graph; semantic report 470 units / 4957
functions / 4840 semantic exact / 4850 accepted / 0 unit errors; campaign
progress 384/833 (halobetacache 282/468) unchanged except this unit's
internal improvement; admission audit 0 candidates / 0 contradictions /
0 revocations; parked audit 12 active / 0 stale / 0 invalid; tooling tests
212/212; fail-closed whole-TU regression manifest against the
canonical 6bb7e2ec9 baseline `ok: true` with zero failures/warnings,
all 11 siblings `still_exact`, and only the non-exact residual in
`changed_nonexact`; protected Units sentinel `_unit_preprocess_node_orientations`
1920 B / 87 relocs /
`5d5c8edc492fb8ab6ea83e1ccaa4cb2798da51ae4a17182fa848878bed05a7ed`;
`git diff --check` clean.

## 2026-08-30 part 2: origin-mirror source recovery and the witness

Directed continuation of the same session ("recover the human-readable
code; do not give up"). Origin-remote commit
`fc47f9a15a0aea7f0081394c4d66db5c68ab5323` (github.com/punpckhdq/halo — this
repository's own `origin`) preserves a `breakable_surfaces.c` from an
earlier reconstruction lineage. It is not an authentic Bungie blob (it
contains `match_assert` and its headers carry a `// TODO: doesn't match`),
so it is level-5 hypothesis evidence — but its topology byte-verified almost
wholesale, and all 11 siblings gate strict-exact from its text unchanged.

### What the mirror settled

- The E09 union was a compensating reconstruction device. January's layout
  falls out of natural block scoping: `surface_vertices2d` at while scope,
  `position_2d_test` in the s-loop, `s_normal`/`t_normal` in the
  first-vertex arm, `temp_2d` in the seed arm, plain `sound_location` in
  the epilogue.
- The offset pair is `real_point2d jitter` in an inner block with both
  products passed as expressions — `jitter.y` is consumed on the x87 stack
  and never stored, which is why January's `[ebp-0x1DC]` slot exists but is
  never written.
- `struct new_particle_data particle` is declared inside the convex-hull
  block; this alone resolved the particle-vs-temp-cluster frame placement
  and the mid-frame rotation from part 1.
- Part 1's X3 alpha operand order appears verbatim in the mirror
  (independent byte-derivation and source lineage agree); X1 is the
  mirror's `real u` block; X2 is superseded — the mirror's plain
  `breakable_surface->sound.index != NONE` test matches January in the new
  basin, so the named-long was a basin-local crutch and is removed.
- Rejected by January bytes: the mirror's `projection_from_vector3d` call
  (January open-codes the fabs block — `test ah,1`/`jne`) and its shared
  `bsp3d_get_plane_from_designator` (January needs our private helper's
  `& LONG_MIN`/`jns` shape).
- Line-anchor validation: January's attested assert lines 251/266/348/388
  span 15/82/40 source lines; the reconstruction spans 16/82/40. The two
  body spans are exact (the 82 requires the open-coded fabs block); one
  head line is still unaccounted.

### Witness state

`research/breakable_surfaces_closeout/witness_mirror_fast_ftol_20260830.c`
(mirror topology + verbatim `fast_ftol`) measures, under the campaign
compiler and flags, with one analysis-only context lever
(`dot_product3d` spelled `a->i*b->i + (a->j*b->j + a->k*b->k)`):

- padded `4032/4032`, relocations `117/117`, instructions `1156/1156`,
  frame `0x1240`, every stack home identical;
- **4 differing instruction lines**: two commutative `fld`/`fmul` pairs in
  the `cross_product3d` expansion (January folds `s_normal.i` in exactly
  the two `a->i` products, loading the plane member first).

Without the lever the witness differs by 20 lines (the four
`plane3d_distance_to_point` dot expansions reassociate k,i,j against
January's k,j,i, plus the same cross pair). The lever is fi4-class
(scheduling-only, no provenance) and is NOT in production headers; its
effect proves a genuine still-unfound upstream source difference
(identical-compiler theorem). Next instrument: read the C2 IR node numbers
at the fld-choice site with `tools/c2dbg32`.

Measured-inert negatives and the stale-header method warning are recorded
in `research/breakable_surfaces_closeout/README.md`. Notable corrections to
part 1's record: the mirror 2-temp `cross_product3d` is byte-neutral here
and tree-neutral under a full-board rebuild (an earlier 4-line reading of
it came from a stale header state), and header experiments must checksum
the header per run.

### Production landing and gates (part 2)

Production now carries the recovered mirror topology with the four
conversions spelled `(short)(long)(real)ceil/floor(PIN(...))` — the closest
ordinary-C stand-in for the forbidden `fast_ftol`. Headers are untouched.
Measured: `3984/4032`, `117/117`, normalized SHA-256
`3e92357588ee17cc138699dbfa7476727653565b02d04460b1815093b9141e81`,
siblings 11/11; normalized-instruction distance target-only 48 / ours-only
15 / replaced 120. The superseded `4032/4032` checkpoint is retained in
part 1 for the record; equal size from compensating stores is not evidence
of closeness.

Gates, all pass: full ninja graph twice (with and without the trial header
edit); whole-board per-object diff byte-identical both times (277/619
strict objects, 667,591 bytes, 4,822 functions); semantic report 470
units / 0 errors / 4850 accepted exact; admission audit 0/0/0; parked audit
12/0/0; tooling tests 212/212; Units sentinel exact; `git diff --check`
clean.

## 2026-08-30 session 3: C2-debugger pass, and a correction to the above

Directed continuation ("use the c2 debugger to solve the last 4
instructions"). It produced a real methodological correction rather than a
close, and the corrected number is worse than the one it started from.

### The "4 instructions" figure was wrong

Session 2 reported the witness at 4 differing lines using a
`dot_product3d` spelled `a->i*b->i + (a->j*b->j + a->k*b->k)`, and stated
its whole-board impact was nil. The board check behind that statement had
been run *after* the header was reverted, so it never tested the edit.
Re-measured with a full rebuild between each header state and the board:

| `dot_product3d` body | this function | whole board |
| --- | ---: | --- |
| flat (production) | 20 lines | **277 objects / 4822 fns** |
| grouped `i + (j + k)` | 4 lines | 271 / 4805 — 17 functions lost |
| k,j,i sequential accumulation | 4 lines | 271 / 4787 — 35 functions lost |

Both candidate levers are refuted by tree-wide evidence: they break
`source/math/real_math` itself plus objects, items, vehicles and units.
The flat form is *proven* by those functions. A corpus census over 66,930
objects / 9,989 commits independently found the grouped spelling has no
provenance in any preserved copy, while confirming that **no authentic
Bungie blob of these headers exists anywhere** — every copy is a
reconstruction, so absence of provenance is weak evidence either way and
the board is the real arbiter.

The accumulation form was not a guess: it is the lever the acceleration
playbook documents for exactly this symptom (it closed
`collision_prism_test_vector`). Here the board says it is wrong.

**Standing rule this establishes: a shared-header lever's local win is
meaningless until the whole board is rebuilt and diffed, and the board
numbers — not the local diff count — go in the ledger.**

### The true residual, precisely characterised

All 20 lines are one phenomenon: VC7 and January order commutative FP
operands/terms differently at five sites. Instruction counts, relocations,
frame size and every stack home already agree.

1. **Four `plane3d_distance_to_point` expansions** (0x3fc, 0x426, 0x45b,
   0x47f): both sides open the sum with the same term, then January adds
   the j term and finally the i term while ours adds i then j.
2. **One `cross_product3d` expansion** (0x37b, 0x389): for the two
   products containing `s_normal.k`, January loads the other operand and
   multiplies by `s_normal.k`; ours loads `s_normal.k`.

A 96-byte micro-probe (`scratch/micro.c::micro_m6`) reproduces item 2
exactly and shows VC7's default is *right-operand-loaded*, with the `a.k`
products as the deviation; `micro_m1`/`m2` prove local definition order is
not the driver. Roughly 40 further source shapes were measured inert in
the true basin (full list in the research README), including every operand
text order inside the helpers — VC7 canonicalizes commutative multiply
operands, so the helper text provably cannot move this.

### Debugger findings

`tools/c2dbg32/gen_config_cross.py` (new) points dbg32 at the probe
compile; the harness is confirmed working (C2 loads at its fixed base,
breakpoints trap, 36 hits on the arena allocator). Profiled over this
path: arena `0x10701000` 26 hits, byte emitter `0x107455e6` 70,
interference test `0x1070943b` 30, preferred-register push `0x1075fa55`
39, width-class write `0x10715873` 53 — and **zero** hits on every mapped
FP site (`0x10745628`, `0x10744304`, `0x10735135`), which were mapped from
an ai_debug compile and are not on this path. The FP commutative-order
decision is therefore in code not yet mapped; the byte emitter and the
allocator chain are the live anchors, and `micro_m6` is the right target
for that RE because it is 96 bytes and compiles in a second.

## Residual classification

Mechanism-proven vendored-assembly boundary (2026-08-30). The four January
conversion chains are inline expansions of the historical `__asm fast_ftol`
helper; no C spelling can emit them (`/QIfist` lowers every C conversion
through a shared `fistp qword`; the uniform profile emits `_ftol` calls).
Under the standing owner rule — no `__asm` in production, period — the
function is provably uncompletable, exactly like
`_build_structure_lens_flares`. Everything reachable by ordinary C has been
recovered and landed (the part-2 mirror topology). This is not strict
exactness and receives no completion credit.

## Reopen/continuation criteria

Reopen only if:

1. the owner changes the no-assembly rule or admits `fast_ftol`
   specifically — the archived witness then leaves 20 instructions at five
   commutative-order sites (four `plane3d_distance_to_point` term orders,
   one `cross_product3d` operand-role pair). Do not resume with source
   lotteries: ~40 shapes are measured inert and the helper text provably
   cannot move a commutative multiply. The next instrument is C2 RE against
   `scratch/micro.c::micro_m6`, a 96-byte probe that reproduces the tie;
2. the owner reconciles this unit's flags with the uniform profile — then
   remove `/QIfist` and re-measure against the honest table in the part-1
   section (the part-2 source is already the right topology for that
   basin); or
3. a distinct compatible compiler/QFE, an authentic source blob (HCEX MD5
   `CEACA3EE6F74879855CD2005D8B6C1AC` remains unfound), or provenance for
   the dot-grouping context factor appears.

## Completion status: two independent blockers, both measured

Stated plainly because the object has been dispatched repeatedly. Byte-exact
completion requires clearing BOTH of the following. Neither is an opinion.

**1. The four conversion sites need inline assembly.** January's
`fld dword`/`fistp dword` chains are inline expansions of the historical
`__asm fast_ftol`. The conversion-width law (measured across all destination
types, stagings and flags, and re-confirmed first-hand here) is that this
compiler lowers EVERY C float-to-integer conversion through a 64-bit
`fistp qword` under `/QIfist`, or through `_ftol` calls without it. No C
spelling reaches January's form. Production therefore sits 48 bytes short --
exactly four sites x 12 bytes -- and that gap is closed only by admitting the
verbatim helper, which the standing owner rule forbids.

**2. Twenty instructions remain even WITH the helper.** The analysis witness
(mirror topology + verbatim `fast_ftol`) reaches 4032/4032 bytes, 117/117
relocations, frame `0x1240`, 1156/1156 instructions and every stack home
identical -- and still differs at five commutative-order sites. Admitting the
assembly is therefore necessary but NOT sufficient: it would move production
from 3984 to 4032 bytes and leave the object still non-exact.

### Why the twenty are not a search failure

Roughly 70 source shapes were measured inert in the correct basin across
every structural class (helper text and structure, upstream producers,
caller structure, call-site order, dataflow, declaration order and scope).
A full C2 reverse-engineering pass then established the mechanism:

- the FP encoder only renders node fields (`0x107455e6`, node in `ebp`);
- the frame that builds the load takes the **head of the operand chain**
  (`0x10736518`-`0x1073653d`) and applies no rule;
- the one site that would order a commutative pair explicitly
  (`0x10760e84`) takes **zero hits**;
- so the order is canonicalised upstream, at or above the C1/C2 boundary.

That is a mechanistic explanation for the inert shapes rather than an
excuse: operand text order cannot matter because it is canonicalised away
before any of the mapped machinery runs. The prediction this yielded --
that symbol/declaration order would be the lever -- was tested and is also
inert.

### The only two things that change this

1. **An owner ruling admitting the verbatim `fast_ftol`.** Necessary for any
   exact outcome; on its own it yields 4032/4032 with 20 instructions left.
2. **Locating the C1 commutative canonicaliser.** The front end is now
   reachable (`dbg32` plants in any module; C1.Dll at `0x10600000` fires) and
   a working differential sampler exists, but C1 has no anchors and mapping
   it is a fresh multi-session effort with the failure modes recorded in
   `tools/c2dbg32/IR_LAYOUT.md`.

Until both are cleared the object stays `NonMatching`. Do not mark it
complete, and do not grant credit from size, plausibility, or effort spent.

## Disposition

`NonMatching` / rigorously parked at 11/12 exact functions. The landed
part-2 mirror topology is an evidence-backed reconstruction of the January
source — validated by the 20-instruction witness and the assert line-anchor
spans — not an object completion. Do not mark the object complete or grant
credit from equal size or semantic plausibility, and do not respend
implementation lanes here under the no-asm rule: the blocker is
mechanism-proven, not a missing source idea.
