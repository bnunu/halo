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
