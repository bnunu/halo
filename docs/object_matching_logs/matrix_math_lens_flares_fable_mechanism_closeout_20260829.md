# matrix_math.obj + structure_lens_flares.obj — mechanism closeout (2026-08-29)

Session: fable/exact-finish-20260829, forked from eed61170. Baseline re-verified
before any experiment: matrix_math 34 exact + `_matrix4x3_multiply` residual
(352/336, sha cea7023c), structure_lens_flares 3 exact +
`_build_structure_lens_flares` residual (4288/4336, 156/156 relocs, frame 0xf8
vs 0x10c). No production source was changed in this session; every experiment
ran through scratch harnesses (`scratch/mm2.py`, `scratch/lab.py`) against
copies. Both park entries stand, now with decoded mechanisms rather than
empirical plateaus.

## Summary of verdicts

| lead | verdict |
|---|---|
| 1. matrix movaps floor | **Proven structural.** Mechanism decoded (below). 8 movaps is the exact floor for every expression-shaped source; 0 movaps requires named `__m128` locals, which force the dynamic-alignment preamble the target lacks. |
| 2. lens scale-arm window | Deprioritized as moot for the gate (see lead 3 arithmetic): even a perfect seam flip leaves the 7 conversion sites diverging, so the function cannot gate exact under the no-asm rule. |
| 3. lens 0x14 frame gap | **Closed by exact arithmetic.** 0xc = fast_ftol temps (banned), 0x8 = the two adjudicated scheduler-tie slots. No recoverable local exists. |
| 4. deeper c2 instrumentation | Not spent; the prior adjudication (no pop/keep node field, encode-time liveness) already covers the remaining ties, and leads 1/3 closed without it. |
| 5. genuine Bungie source on disk | **Negative, thorough.** Bounded sweep of Downloads/Desktop/Documents/Codex/C:\ top level: only reconstructions and campaign artifacts. New corroboration: the halocea reconstruction (C:\Users\isabe\Documents\Codex\reference\halocea) self-documents HCEA's original `matrix4x3_multiply` as hand-written VMX/AltiVec inline assembly with a scalar `_cpp` twin. |
| 6. fast_ftol policy | Unchanged; owner decision. Evidence restated below with the 7-site frame arithmetic. |

## matrix_math: the movaps floor is mechanistic

Three probe results (VC7 13.00.9254, campaign flags, `scratch/mm2.py probes`):

1. **Named `__m128` locals force the dynamic-alignment preamble even when
   register-only.** A two-local, zero-spill probe emits
   `push ebx / mov ebx,esp / push ecx / and esp,0xfffffff0 / ...`. The January
   target has a standard `push ebp / mov ebp,esp / sub esp,8` prologue, so
   every named-local form is excluded at the first instruction.
2. **Comma-joined store intrinsics drop the first store.**
   `_mm_store_ss(q,v), _mm_storeh_pi(p,v);` emits only the `movhps` store; the
   `movss` store vanishes (probe P3 vs P3b). Consequence recorded below under
   "defect report".
3. **Expression evaluation is right-to-left (intrinsics are frontend calls)
   with a working-register discipline**: each new leaf load targets the current
   working register; any live temp is first copied up (`movaps`). Therefore any
   row of the form `bi*fwd + bj*left + bk*up` costs exactly 2 copies (the first
   product is live across the second broadcast load; the running value is live
   across the third), for every association/nesting of the sum.

Measured invariance across the full sum-tree shape space (committed macros,
same flags; `lea` counts include the dead address-computation artifacts of
re-spelled CSE'd loads):

| variant | size | movaps | shufps | lea | lcsd |
|---|---|---|---|---|---|
| base-committed `add(add(i,j),k)` | 352 | 8 | 13 | 23 | 556 |
| E1 `add(k,add(j,i))` | 352 | 8 | 13 | 23 | 552 |
| E2 `add(i,add(j,k))` | 352 | 8 | 13 | 23 | 556 |
| E3 `add(add(k,j),i)` | 352 | 8 | 13 | 23 | 552 |
| E4 shape-A, separate stores (semantically correct) | 384 | 8 | 13 | 26 | 572 |
| E5 shape-B, separate stores | 384 | 8 | 13 | 26 | 574 |
| January target | 336 | 0 | 15 | 3 | 0 |

E1/E3 do flip the product order to the target's i,j,k and give the rows the
target's exact registers (xmm0=forward, xmm1=left, xmm2=up), confirming order
is source-controllable — and the copy count still does not move, confirming
the floor is the discipline, not the order.

The target additionally shows properties outside anything our compiles ever
exhibit:

- **Zero movaps with three products computed before the first add** — requires
  per-value register binding without copies, i.e. the named-local path, whose
  preamble the target lacks. The two horns are exhaustive: an `__m128` value
  crossing statements is either a named local (preamble) or a re-spelled CSE'd
  expression (dead `lea` artifacts, 23-26 in ours, 3 real ones in the target).
- **Cross-row interleaving** (row1's first product and row2's b.up.j
  broadcast+mul are emitted inside row0/row1's streams). Every one of our
  compiles of every variant preserves strict source-statement order; nothing
  in this compiler reorders across the statement stream (consistent with the
  campaign's no-scheduler adjudication).
- First xmm def lands in xmm2 (up-row raw load) while later defs take
  xmm3/xmm4/xmm0 — inconsistent with the allocator behavior in all measured
  compiles.

Together with the prior evidence (AP-930 shuffle signatures 0x36/0x8f unique
in the whole image, HCEA PDB carrying a hand-written `_vmx` twin, 46,080-form
plateau, donor-topology floor) the park classification is correct and now
mechanism-proven: **`_matrix4x3_multiply` is vendored hand-scheduled SSE
assembly, unreachable from ordinary C under this compiler.**

### Defect report (owner action item, no edit made)

The owner-committed intrinsics base (restored verbatim from 2fdddb42) uses
`MATRIX4X3_STORE(p, v)` = `_mm_store_ss(...), _mm_storeh_pi(...)` inside a
comma expression. Probe P3 shows VC7 silently drops the `_mm_store_ss` in that
form: the committed 352-byte function **never writes result forward.i, left.i,
up.i, or position.x** (only the movhps halves are stored). The semantically
correct separate-statement form (E4) compiles to 384 bytes / 8 movaps and is
byte-farther from January. Decision is the owner's: keep the byte-closest
committed base (currently also the parked one), or accept +32 bytes for
semantic correctness in the parked placeholder. Nothing in this session
changed it.

## structure_lens_flares: the frame gap closes exactly

Fresh census on this session's builds (`scratch/regions/lf_{target,ours}.asm`):

- January has **7 `fistp dword` sites**, each the two-instruction fast_ftol
  body (`fld dword [ebp-0xc|-0x5c]; fistp dword [slot]`) with **7 distinct
  dword result temps** (-0xec, -0xe4, -0x108, -0xf0, -0xf4, -0xfc, -0x104)
  = 28 bytes of frame.
- Ours has the same 7 conversions as `/QIfist` `fistp qword` through **2
  shared qword temps** (-0x84, -0xb8) = 16 bytes.
- Conversion-machinery delta = 0xc. Cross-check: the prior session measured
  frame 0xf8 -> 0x104 (+0xc) when `__asm fast_ftol` was landed. Exact match.
- Remaining 0x8 = the two slots of January's KEEP-shape normalize (the
  pop-and-reload magnitude arm + kept-reciprocal join), i.e. the two c2
  scheduler ties already adjudicated inside the compiler (no pop/keep node
  field; encode-time FP-stack liveness).

So 0x14 = 0xc (banned fast_ftol convention) + 0x8 (certified compiler-state
tie). **There is no unattributed local and no recoverable frame delta.** Since
`sub esp, 0x10c` is among the first bytes of the function, no legal source can
gate exact while the conversion-width law stands; the seam ties are moot for
the strict gate.

### New in-basin negative

z2 (`scratch/z2_initless_for.json`): keep `short lens_flare_index = 0;`
declaration initializer, drop the `for` header init. Hypothesis was that an
earlier zero definition position would reproduce January's materialized zero
at T@0244 (`xor eax,eax; cmp ecx,eax; mov [ebp-0x10],eax`). Result: the
propagator crosses the switch — the store hoists to the declaration position,
the loop head degenerates to `test eax,eax; jle` with no store at all, and the
global masked distance regresses (ndist 1790 -> 1946, size unchanged 4288).
Reverted. Conclusion: VC7's constant propagation into the rotated loop guard
cannot be suppressed by init placement; January's register-zero at the
`lens_flares` search head implies an IR-context difference (count allocated to
ecx, zero node holding eax) that no init-placement spelling reproduces.

## Remaining first-differing regions (strict-gate view, ours vs January)

1. `sub esp, 0xf8` vs `0x10c` — fully attributed above.
2. T@0244 materialized zero at the lens_flare search-loop head — z2 negative
   above; believed coupled to the same upstream IR-state delta the normalize
   census identified.
3. T@0a65 normalize seam (both arms) — adjudicated compiler-state ties (prior
   session, dbg32-verified; not re-spent).
4. 7 conversion sites 0xca5-0xf56 — inlined fast_ftol, proven non-C.
5. Downstream ripples (cross/dot linearization, join shape, argument
   evaluation order near calls, tail nops) — all previously measured to move
   with the seam/conversion basin, not independently addressable.

## Proven impossible vs unattempted

Proven impossible under the rules (new this session in bold):

- Any C spelling of a 32-bit fistp (conversion-width law, prior).
- **Any expression-shaped intrinsic source with fewer than 8 movaps** (copy
  discipline; measured invariant over the complete sum-tree shape space).
- **Any named-`__m128` source without the alignment preamble** (P1).
- **A 352-byte semantically-correct comma-store form** (P3: the comma form is
  what makes 352, and it drops stores).
- Init-placement control of the T@0244 zero (z2, this basin).

Genuinely unattempted (and why left):

- Measuring the exact linearization-window extent against calls/branches
  (lead 2): would refine the seam model but cannot change the strict-gate
  verdict while the conversion sites stand; only worth spending if the owner
  ever grants the fast_ftol convention, at which point the seam becomes the
  live frontier again.
- Further c2 register-allocator instrumentation (lead 4): same conditional.

## Next steps if the owner rules

- **If fast_ftol is granted** (upstream punpckhdq/halo convention): lens
  flares lands at 4336/4336 with frame and reloc identity (prior direct
  measurement), ~28 other objects unblock, and the remaining spend target is
  the two seam ties + T@0244 context, with the window measurement (lead 2) the
  first experiment.
- **matrix_math**: no compiler path exists; the function stays a vendored
  kernel. The only open owner item is the store-dropping defect note above.
