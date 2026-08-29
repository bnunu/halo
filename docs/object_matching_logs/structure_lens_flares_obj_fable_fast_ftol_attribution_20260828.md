# `structure_lens_flares.obj` fast_ftol attribution and legal-topology recovery

## Scope and result

This wave answers the standing highest-leverage question for
`_build_structure_lens_flares`: what makes January materialize seven narrowed
float results through 32-bit float homes before integer conversion. The answer
is now proven, mechanically and by provenance: the seven sites are inline
expansions of the historical `__asm` helper `fast_ftol(float d)` from
`cseries.h`. No ordinary-C spelling can reproduce them on this compiler.

Three legal source recoveries discovered while proving this are retained. The
production candidate improves from 4192 bytes / 156 relocations / 3843
documented normalized distance (3807 re-measured this wave) to 4192 / 156 /
3690, with the byte-level LCS distance improving 2842 → 2536. The three
sibling functions and all owned data remain strict-exact; the relocation
destination/type sequence remains equal. The object remains `NonMatching`; no
prohibited technique was introduced into production source.

## The conversion-width proof

The retained residual's principal gap is seven float→integer sites (four grid
bounds, three packed direction bytes). January emits, at every site:

```
call  _ceil / _floor
fstp  dword ptr [ebp-SHARED_FLOAT_HOME]   ; honored narrowing to 32-bit float
fld   dword ptr [ebp-SHARED_FLOAT_HOME]
fistp dword ptr [ebp-DISTINCT_INT_HOME]   ; 32-bit fistp
mov   cx/dl, word/byte ptr [ebp-DISTINCT_INT_HOME]
```

Measured this wave on the exact XDK 3911 CL 13.00.9254 with the campaign
flags, every C conversion collapses to `fistp qword` through one shared
temporary:

- every destination type — `(short)`, `(long)`, `(int)`, `(char)`,
  `(unsigned short)`, `(unsigned long)`, `(unsigned char)`;
- every staging topology — direct casts, named `real` locals (single and
  reused), distinct `long` intermediates, real→long→short chains, pointer
  (address-taken) staging, aggregate-member staging, union members, bitfield
  destinations, parameter reassignment (`z = z * 255.f`), inline C helper
  returns, float-typed call returns;
- every relevant flag family — `/O1`, `/O2`, `/Od`, `/Op`, `/Oi`, `/Ow`,
  `/G5`, `/Za`, each with `/QIfist`. `/Op` restores the `fstp/fld dword`
  narrowing for named-variable spellings but still emits `fistp qword`.

`tools/flag_provenance_census.py` already records the same width law:
`/QIfist` always converts through a 64-bit `fistp` regardless of source or
destination width. A 32-bit inline `fistp` is therefore never compiler output
on this toolchain.

## Provenance of the true spelling

- Historical `source/cseries/cseries.h`, restored verbatim by non-ancestor
  commit `5093ac1a` ("match bsp3d (#49)") and placed there by HCEX PDB
  evidence, defines `__inline long fast_ftol(float d)` with body
  `__asm { fld d; fistp result }`, beside the portable `fast_ftol_C`.
- January's `actor_combat.obj` contains a standalone 32-byte `_fast_ftol`
  with exactly that body; `hud_draw.obj` contains `_fast_ftol_C` (a
  hand-written branchless `fist/fisub/sbb` truncation — also never compiler
  output). VC7 13.00 both inlines the `__asm` helper and can emit the
  out-of-line copy.
- A tree-wide scan (833 split objects) found 137 inline `fistp` instructions
  in January game code, all dword; our candidate build has 16, all qword,
  confined to the four `/QIfist`-configured files. Zero `Matching` objects
  contain any inline `fistp`: no byte-proven ordinary-C spelling exists
  anywhere in matched territory.
- The stian donor lift (build-2276 target) reconstructs this exact function
  with `x87_round_to_int` (the same bare `fld/fistp dword` helper) at all
  seven sites and annotates the 2276 binary's identical staged sequence; its
  sibling `compress_real_to_int16` annotation decodes the same
  narrow-through-float-home motif byte-for-byte. The four unwritten
  `rasterizer_geometry.obj` compress helpers in our own target exhibit it in
  32–112-byte isolation.

## End-to-end mechanism verification (lab only, not production)

Spelling the seven sites through the verbatim historical helper
(`s_min = (short)fast_ftol((real)ceil(PIN(...)));` etc.) in a scratch compile
reproduces the target's shape precisely and moves the function:

| candidate | padded bytes | relocs | frame | LCS byte distance |
| --- | ---: | ---: | ---: | ---: |
| legal baseline (start of wave) | 4192 / 4336 | 156 / 156 | 0xf8 (target 0x10c) | 2842 |
| + statement-level `__asm fld/fistp` | 4272 / 4336 | 156 / 156 | **0x10c exact** | 3126 |
| + verbatim inline `fast_ftol` instead | 4272 / 4336 | 156 / 156 | 0x10c | 2656 |
| + the three retained legal recoveries | 4256 / 4336 | 156 / 156 | 0x10c | 2312 |

The inline-function form beats statement-level asm by 470 LCS bytes: the
shared float home is the inlined parameter `d`'s slot and the distinct int
homes are each expansion's `result` local, exactly as in January. The
relocation destination/type sequence stays equal throughout, and the frame
size becomes exact the moment the helper shape is present. The remaining
~80 bytes / 2312 LCS distance of the best diagnostic are a whole-function
callee-saved permutation (January: `edi`=structure_bsp, `ebx`=points,
`esi`=projected_points; ours rotates it), the induced frame-slot packing
permutation, and per-use reload scheduling around the outer loops — the
allocation-cascade class with no admissible source lever, and moot for
production since the diagnostic itself is inadmissible.

## Retained legal recoveries (production)

1. **Triangle batch write** (`point_count += 3`): January copies all three
   coplanar-group vertices, then performs the three projections at
   `point_count`, `+1`, `+2`, then advances the counter once (`add reg,3` in
   the target). Interleaved copy/project/increment was measured farther.
   LCS 2842 → ~2600 territory on the legal stream; on the asm-normalized
   stream −148.
2. **Cluster marker fields are unsigned** (`word
   first_lens_flare_marker_index; word lens_flare_marker_count;`): the
   target's validation loop reads them with `movzx`/unsigned branches.
3. **`projection_axis` / `projection_sign` carry no initializers**: both are
   assigned on the `point_count == 0` path before any use; January carries no
   constant-store pair at the loop head.

Combined production effect: 4192 → 4192 bytes (size-neutral), normalized
differing bytes 3807 → 3690, LCS distance 2842 → 2536, relocations 156/156
with equal destination/type sequence, three siblings exact, full build and
all audits green.

## Measured and rejected this wave (do not retry)

- Chained-zero reordering and four separate zero statements in the
  `lens_flare_spacing == 0` arm: byte-neutral / 2-byte noise.
- Swapping the two `shader_get_and_verify_type` locals per case: −76 LCS
  (worse); the target's `add eax,0x38` mutate shape does not come from call
  order.
- Named reciprocal local (`real inverse_spacing = 1.f / lens_flare_spacing`)
  with per-bound multiply: byte-identical — VC7 already synthesizes the
  shared reciprocal (`fld1; fdiv; fst dword`) from the plain divisions; the
  January reciprocal is not source-observable.
- Hoisting `s_count`/`s_grid` to the hull scope: byte-identical.
- `if`-spelled min/max clamps replacing `MIN`/`MAX` in the hull loop:
  mixed-sign noise (−21 positional, +2 LCS); not retained.
- Every conversion staging family and flag listed in the width proof above.

## Remaining boundary and next step

The first differing region of the improved legal candidate is unchanged in
kind: at each of the seven sites the legal build emits
`call _ceil; fistp qword [shared]` (10 bytes) where January emits the
19-byte staged `fast_ftol` expansion; the 144-byte size gap and the frame
delta (0xf8 vs 0x10c) are accounted for by those expansions plus their
downstream encoding effects.

There is no next ordinary-C experiment: the conversion-width law closes the
space. The one remaining move is an owner adjudication — whether
`fast_ftol`, as HCEX-attested original source with a byte-exact standalone
twin in January's own `actor_combat.obj`, is admissible under the asm policy
(the actor_looking checkpoint reached the same fork and likewise deferred).
If admitted, this wave's lab recipe (helper verbatim in `cseries.h`, the
seven call spellings, `/QIfist` dropped as then-inert) starts from 4256/4336
with frame and relocation identity already exact; the same helper unblocks
the recorded staged-dword residuals in `breakable_surfaces`,
`actor_looking`, `player_rumble`, `decals`, `hud_draw`, and the unwritten
`rasterizer_geometry` compress family. If not admitted, this function is
asm-blocked by target evidence and should be treated like the
asm-forced family.

## Second wave (2026-08-28/29): topology recovery to the tie frontier

A follow-up wave recovered eleven further January source shapes, all landed in
production as ordinary C. Metrics are for `_build_structure_lens_flares`
against the 4,336-byte target with `fast_ftol` present during the grind and
re-measured legal-only at the end:

1. Nested `dynamic_array_get_element` calls (the edge lookup inline inside the
   point lookup's index argument, designator re-read from the triangle
   pointer): reached exact 4,336 padded size for the first time.
2. `rectangle2d grid_bounds` replaces the four `short` grid bounds: word
   cells, packed y0/x0/y1/x1 layout, chain zero-arm store order, dword-pair
   count loads — the sites became byte-identical to January's.
3. Spacing-test polarity flipped (`!= 0.f` first, zero arm sunk cold).
4. `if (... > 0)` on the marker-count validation test.
5. `cross_product3d(&plane.n, &s_axis, ...)` — arguments reversed; the fsubp
   product structure proves January computed cross(normal, tangent). This is
   a semantic correction to the reconstruction (t-axis sign).
6. `plane3d_from_points(&plane, triangle_point2, triangle_point1,
   triangle_point0)` — the push stream proves the points passed reversed
   (plane winding correction, flagged deliberately).
7. Distributed hull dots: `origin_s`/`origin_t` hoisted once,
   `dot(point, axis) - origin_dot` per point; `relative` never existed.
8. Guardless do-while for the grid hull walk plus a `for` min/max loop
   (pointer-walk countdown, memory-resident origin dots).
9. Shader cases: reference from the first `shader_get_and_verify_type` call
   (add-in-place), spacing from an inline second call.
10. `vertices` hoisted out of the surface loop (single pre-call read);
    marker-phase `long marker_index`/`long marker_write_index` hoisted with
    inits ahead of the decode loop; `triangle_points[3]` decomposed into
    three scalar pointers; `t_offset` deleted in favor of compiler-hoisted
    loop invariants; grid init statement orders; min/max spelled value-first
    (`MIN(s, bounds.x0)`); count-on-the-left search-loop equality; the
    literal 65536 marker warning argument.

With `fast_ftol` at the seven sites this candidate measures 4,320/4,336
padded bytes, 156/156 relocations with the destination/type sequence exact,
26 masked diff blocks (~29 target instructions), and the frame at 0x104
versus 0x10c. Without `fast_ftol` (production, legal): 4,256/4,336, 156/156,
normalized positional distance 1,890 (wave start: 3,807), LCS byte distance
1,308 (wave start: 2,842). Three siblings and all data remain strict-exact.

## The measured tie frontier (do not re-sweep)

Instruction-level decode agents censused both frames completely and swept the
remaining clusters. All of the following are measured inert or regressive:
scope hoists to function scope for fourteen locals (each adds instructions or
is byte-identical); `normalize3d(vector_from_points3d(...))` nesting; explicit
member subtractions; origin-dot statement/decl-init respellings; every
commutative dot/argument/operand order (fully normalized); do-while vs for vs
while vs guarded-do for the min/max loop except the landed form; search-loop
init placements; result-init split; named reciprocals; point2d origin pair.

The remaining residual decomposes into three certified compiler-state ties
with January-corpus twins on BOTH sides:

- the lens-flare search head first-scratch-choice (January loads the count
  into ecx leaving eax free for a shared zero; we load it into eax) — same
  axis as the units `start_action` family;
- the normalize seam keep-vs-fold (January keeps the reciprocal stacked with
  a shared join pop; we fold destructively — January itself folds in
  `decals@04c0` and keeps in `path_obstacle_avoidance@0125` from identical
  statement classes);
- the frame cell packing below -0x28 (aggregate-coalition pairings
  {direction+origin} and {test_point+position} in January versus different
  legal pairings in ours), which owns the residual size delta through
  disp8/disp32 encoding lengths and alignment-pad parity.

Adjudication route if ever reopened: c2 instrumentation per
`tools/c2dbg32/IR_LAYOUT.md` with the corpus contrasting pair above as the
minimal differential; not source sweeps.

## Production policy disposition

`fast_ftol` was reverted from production at wave end: the asm-implemented
parked class covers only byte-exact functions, and this function is not yet
exact, so carrying `__asm` in it would violate the no-asm rule without the
parking justification. Production is the maximal legal candidate. The
fast_ftol spelling (helper verbatim, seven call sites) is preserved in this
ledger and in the branch history (commits a06af868..) and reaches 4,320/4,336
the moment the owner grants the same adjudication the class's other eleven
members already have.

## Third wave note (2026-08-29, owner ruling)

The owner ruled that no `__asm` may enter production. `fast_ftol` remains
reverted; production is the maximal legal candidate (4,256/4,336, 156/156
with exact destination/type sequence). Under the no-asm rule the function is
provably uncompletable to byte-exactness: the seven conversion sites require
the inlined `__asm` helper (the conversion-width proof above), and even with
the helper the last ~29 instructions are compiler-state ties. Additional
negatives measured in the third wave before the ruling: TU function-order
permutation (inert — per-function compilation), decl-init to statement-init
splits for the material-block locals (fully normalized), all nine legal
statement-order permutations of the hull axis/dot block (each strictly
worse), and pairwise/triple combinations of fourteen scope hoists (none beat
the baseline masked distance of 64).

## c2 adjudication of the normalize seam (2026-08-29)

The seam was taken to the instrumented compiler (fresh `dbg32.exe`, the
campaign C2.DLL). Findings, in full at the branch's scratch ledger and
summarized here for the record:

- The seam is two coupled instances of one c2 x87 scheduler decision —
  consume-vs-keep for a shared FP operand (the freshly stored `s_axis.k` in
  the magnitude sum; the reciprocal across the three scale multiplies).
- The FP store node carries NO pop/keep field (live node dumps: opcode
  0x258, operand links only); `fst` vs `fstp` is computed at encode time
  from FP-stack liveness. The documented modrm-formation site 0x10744304
  never fires for this store — it dispatches through the FP-store jump-table
  handler at 0x10745628. New live-confirmed VAs: 0x107455e6 (byte emitter),
  0x10745628 (FP store case).
- A 24-site census of January's inlined normalize expansions shows the
  compiler itself lands on BOTH sides from identical statement classes
  (k,j,i dominant, 6 sites fused exactly like ours; the KEEP j,k,i shape is
  a two-site deviant), and our compiler reproduces January's other deviant
  orders wherever the surrounding context matches — the choice is a
  deterministic function of upstream IR context, not nondeterminism.
- No isolated source lever exists: nine targeted spellings are
  byte-identical; the one perturbing construct (a redundant
  `s_axis.k = s_axis.k;` self-assignment) flips only the magnitude arm, to
  the wrong order, and repartitions allocation globally.

## Basin-dependent levers (methodology correction)

A 1,822-combination lottery over the measured-inert edit pool found that
`real_point2d origin_projection` (aggregate origin dots) plus
`position.x = position.x + s_axis.i * s_distance` operand order — both
byte-identical when first measured — become active after the base changed:
on the then-current base they moved padded size and frame to exact target
values. Both are landed in production. **A "measured inert" verdict is only
valid within the basin where it was measured**; re-run surviving pool
members after every landing.

## The legal ceiling

With every recovery landed, the legal (no-asm) candidate stands at
4,288/4,336 padded bytes, 156/156 relocations with exact destination/type
sequence, ~60 masked-differing instructions, decomposing entirely into:
the seven `fast_ftol` sites (~25, unreachable from C by the conversion-width
proof), the two c2 scheduler ties above (~19, no isolated source lever,
instrumentation-verified), downstream multiply-order wobbles of those ties,
and alignment-pad parity. This is the proven ceiling of ordinary C on this
compiler for this function.
