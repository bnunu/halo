# `matrix_math.obj` vendored-kernel recovery — 35/35 byte-complete

## Scope and result

This wave executes the experiment the historical ledger
(`f4982274:docs/object_matching_logs/matrix_math_obj.md`) recorded as untried
and decisive: compiling a C + `__asm` form of `_matrix4x3_multiply` with the
campaign VC7 compiler. The first structurally complete candidate compiles to
the exact 336 target bytes, zero relocations, normalized SHA-256
`0436d82fe1070f30523ce8798bcce3b7625099794f5d0ab3fbf9a9c33802314e`, equal on
both sides of the hardened comparator.

`matrix_math.obj` is now **35/35 functions byte-identical** with all owned
data exact. The function is recorded in `config/parked.json` under the
standing `asm-implemented` class (the eleventh such entry; this function is
the vendored-assembly case that class's `stristr` entry cites as its own
precedent), so the strict C-reconstruction tally does not count it.
`config/config.json` index 226 moves to `Matching`, matching the
`cseries.c`/`object_shadows.c` precedent for objects whose only asm-attested
member is parked. `research/asm_credited_functions_audit.md` inventory is
updated to eleven.

## The recovered source shape

Everything the prior ledgers inferred from the bytes is confirmed by
compilation, and one new fact was recovered:

- `void matrix4x3_multiply(real_matrix4x3 const *a, real_matrix4x3 const *b,
  real_matrix4x3 *result)`, cdecl, no return value.
- Four pointer locals, in declaration order: `a_elements` = `a->n[0]`,
  `b_elements` = `b->n[0]`, `result_elements` = `result->n[0]`, and — the new
  fact, recovered from the one non-matching byte region of the first probe —
  **`a_scale` = `&a->scale`**. VC7 homes the first two at `[ebp-8]`/`[ebp-4]`
  and reuses the dead incoming parameter slots for the other two
  (`result_elements` → `[ebp+0xc]`, `a_scale` → `[ebp+8]`), reproducing the
  target's `mov [ebp+0xc], edx` / `mov [ebp+8], esi` stores exactly.
- One `__asm` block: the Intel AP-930 (June 1999, order 245045-001)
  `PIII_Mult00_3x3_3x3` kernel adapted for the 4x3 position row, reading the
  pointer locals into ecx/edx/eax (Intel's src1/src2/dst) and `a_scale` for
  the position-row scale broadcast. Every anomaly the ledger chased —
  asymmetric up-row load + `shufps 0x36`, `shufps 0x8f` before the third-row
  store, zero `movaps`, the software-pipelined schedule — is Intel's verbatim
  hand allocation.
- Compiled-C tail `result->scale = a->scale * b->scale;` — VC7 emits
  `fld [esi]; fmul [edi]; fstp [ebx]` interleaved with the register pops,
  byte-identical to January.

The probe history: the transcription with only the three row-pointer locals
compiled to 320/336 with a single missing instruction
(`mov [ebp+8], esi`) plus tail padding; modelling that store as the fourth
pointer local closed the function on the second compile. No flag changes; the
unit's campaign flags are untouched.

## Corroboration gathered this wave

- The stian donor lift (target: Oct-2001 debug 2276 XBE) documents the
  IDENTICAL prologue in that build — three `lea +4` row pointers spilled to
  `-0x8`/`-0x4` and the two parameter homes, then reloaded — so the wrapper
  shape is cross-build stable from 2276 to 2342. Its intrinsics
  reimplementation scores only 55.1% mnemonic / 17.8% operand under VC71 at
  119 vs 95 instructions, independently reconfirming the ledger's conclusion
  that intrinsics cannot reproduce the hand-scheduled kernel.
- The pastudan donor history contains no matrix4x3_multiply implementation
  evidence beyond scalar C.
- Prior bounded negatives stand un-retried per the fixed-point log: 46,080
  legal first-row forms, 40,320 store orders, named-`__m128` alignment
  preamble proof, loop forms, helper/inline forms — plateau ~293 normalized
  bytes.

## Policy accounting

The strict source policy forbids asm as a *reconstruction* technique; the
campaign's `asm-implemented` parked class exists precisely for functions
whose January source provably WAS assembly, keeping the bytes reproducible
while denying C-reconstruction credit. This function's kernel is vendored
Intel application-note assembly — the strongest possible attestation that the
original source contained `__asm` — and the class's own documentation names
matrix4x3_multiply as its precedent. The entry's unpark condition (replace
with C that gates exact) remains, and remains known-impossible under the
current compiler provenance.

## Gates

- `gate.py source/math/matrix_math --all`: 35 exact, 0 residual, 0 unwritten.
- Full build: pass. Parked validation: 14 active, 0 stale, 0 invalid.
- Admission audit: 0 candidates, 0 contradictions, 0 revocations.
- Semantic audit: 0 unit errors, 0 ordinary-rejected, accepted-exact 4599
  (+1 for this function).
- `git diff --check`: pass.

## Reversal (2026-08-29, owner ruling)

The owner ruled that no `__asm` may enter production, including under the
`asm-implemented` parked class. All landings from this ledger are reverted:
`matrix4x3_multiply` is removed from `source/math/matrix_math.c` (unwritten
again), the parked entry is withdrawn, configuration index 226 returns to
`NonMatching`, and the audit inventory returns to ten functions.

The compile-level attribution in this ledger remains valid as evidence: a C
wrapper (four pointer locals, including the newly recovered
`a_scale = &a->scale`) around the verbatim AP-930 kernel compiles to the
exact 336 bytes with the campaign toolchain. Under a strict no-asm rule the
function is provably uncompletable: the kernel is hand-scheduled vendor
assembly (already established by the 46,080-form intrinsic/C search, best
~293 normalized bytes away, and the zero-`movaps` proof), so the 2026-08-26
policy fixed point stands. Reopen only if the policy changes.

## Closure (2026-08-29): historical vendored-assembly park restored

The owner-approved completion state from commit `2fdddb42` ("matrix_math is
complete at 34/35 under the no-assembly rule", 2026-08-06) is restored
verbatim: the semantically correct intrinsics-based C implementation of
`matrix4x3_multiply` (352 bytes, 0 relocations, normalized SHA-256
`cea7023c…` — byte-identical to the historical base) and the
`vendored-assembly` parked entry with the AP-930 evidence. The units
integration had silently dropped both. The entry validates ACTIVE; the
object is complete under the no-assembly rule: 34 strict-exact functions,
all data exact, one function parked as vendored assembly with its external
origin identified and its 2026-08-28 byte-exact lab attribution referenced.
No `__asm` exists anywhere in the unit.
