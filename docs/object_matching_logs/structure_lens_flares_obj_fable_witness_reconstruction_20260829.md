# structure_lens_flares — witness reconstruction to 4 instructions (2026-08-29, session 2)

Branch fable/exact-finish-20260829. Directive: exhaust the research needed to
understand and unlock the residuals. Everything below ran in scratch harnesses
(msr/mdiff/sweep2/pairs/triples + two 4-agent workflow rounds, ~15,000 measured
compiles total); production source is untouched.

## Headline

Starting from the parked state (4288/4336, 33 diff blocks in the old ledger),
the fast_ftol witness basin was re-measured on the current base and then driven,
through seven recovered source shapes, to:

**4336/4336 bytes, 156/156 relocations in exact sequence, frame 0x10c exact,
345/345 frame-slot references exact, 1327/1327 instructions, 4 instructions
differing (two int-vs-FP encode-order transpositions, lcsd 20).**

Sibling functions and data stay strict-exact throughout. The one non-legal
ingredient remains the historical `__inline fast_ftol` (`__asm fld/fistp`)
helper from January's cseries.h — everything else recovered this session is
ordinary, readable C, and several shapes are plainly more idiomatic than what
they replaced. This is as close to a constructive proof of January's source as
the campaign has ever produced for a parked function.

## The recovered January source structure (witness stack, in order)

1. `z3_fastftol_now` — the historical fast_ftol at the 7 conversion sites
   (grid_bounds x0/y0/x1/y1, marker direction[0..2]). NOT legal under the
   no-asm rule; the sole reason the witness cannot land.
2. `a3geom_plane` — **January used two `real_plane3d` locals `s_plane`/`t_plane`**
   (axis = `.n`, origin projection = `.d`), with bounds seeded and hull
   distances computed via the existing `plane3d_distance_to_point` inline.
   Proof: the two 16-byte {n.i,n.j,n.k,d} blocks land exactly on January's
   frame cells (-0x34..-0x28 and -0x88..-0x7c) and the whole 60-slot frame
   permutation collapses (slots 73/338 -> 324/339). This kills the landed
   real_point2d origin_projection aggregate for this basin.
3. `b12_plane` — the marker x-term through a scoped `real ds_x` temp.
4. `a3geom_avgassign` — origin averaging spelled `origin.x = origin.x * (1.f /
   (real)hull_count);` (explicit form; assignment-op form emitted differently).
5. `a3geom_ptrtemp` — `real_point3d const *point` temp for the hull-loop
   distance calls (activated only in combination — found by pair lottery).
6. `e1seam_planefull` — the s-plane built in a local `real_plane3d s_temp`
   (vector_from_points3d/normalize3d/cross_product3d/d-dot all on s_temp) then
   **one total-aggregate copy `s_plane = s_temp;`**. The 16-byte copy coalesces
   to zero bytes only when TOTAL (all four fields assigned first); it flips the
   normalize scale-arm keep-vs-consume tie, the magnitude j,k,i order, and the
   cross interleave to January's shapes — the mechanism-grade confirmation that
   these "certified ties" are steered by definition positions, reachable from
   source via coalescing copies.
7. `z6_pair_dowhile` — the lens-flare search loop as January's rotated
   iteration idiom (attested verbatim in matched units.c
   unit_postprocess_node_matrices): `lens_flare_index = 0; if (count > 0) {
   long element = 0; do { ... ++lens_flare_index; element = lens_flare_index; }
   while (element < count); }` — in this basin it also fixed the hull-dot
   component orders (i,j,k / i,k,j) as a side effect.
8. `z14_elem_outside` — the `long element` declared+zeroed BEFORE the count
   guard (adjacent to the short init). This finally reproduces the T@0244
   unfolded guard byte-for-byte including the `lea ecx,[ecx]` loop-alignment
   pad, and the 02f9 pad echo heals with it.

Basin dependence ruled everything: z6 was byte-inert until e1seam landed; the
avgassign/ptrtemp pair was inert as singles; the n05 axis-temp that first
flipped the seam was superseded by the plane discovery. Every lever was
re-measured per basin.

## The remaining 4 instructions

Two same-size transpositions in the marker preheader/argument region:

- T@0e28: January `mov [ebp-0x44],eax` (s_count home store) one FP-slot
  earlier than ours relative to `fld t_plane.n.j`.
- T@0e6a: January `fadd [origin.x]` before `push ebx`; ours after.

Both are int-filler placement slots in the x87 stream. Negative evidence at
these two sites across three basins: ~110 targeted variants (temps, statement
splits/orders, operand swaps, decl orders, scope moves, staging — logs
a2args/e4tail/x-,v-,w-series), plus 4186 pairs and 2500 triples of the
92-member applicable corpus on the final basin: zero flips; every real IR
change perturbs other bytes first. This matches the campaign's
adjudicated-encode-tie family (c2-pogo: definition-position driven, no
spill-cost/per-node field). A quads lottery and the dbg32 watchpoint route
(per the c2-pogo playbook) are the only remaining instruments.

## The T@0244 decode (for the record)

January's "materialized zero" guard is the hand-rotated iteration idiom above.
The isolation ran bidirectionally on units.c (the only TU with both guard
shapes in one matched function): pre-guard zero store necessary (u3), cached
block pointer / for-vs-do form / live-out / element twin / break all
non-discriminant (u1/u2/u4/u5/u6); on the lens side, every one-variable
spelling folds (z5/z8, s1/s4/s11, i3/z2, p1-p9), the long-copy web (p11) flips
the guard but costs a home, and only the pair idiom with the pre-guard long
zero (z6+z14) reproduces January at zero cost — in the e1seam basin.

## Production (no-asm) implications — owner decision

The legal seven-edit subset measured against the production baseline
(no fast_ftol, /QIfist retained):

| candidate | size | masked blocks | lcsd | slots |
|---|---|---|---|---|
| current production base | 4288/4336 | 47 | 1254 | 39/306 |
| + legal witness stack | 4272/4336 | **28** | 1068 | 140/313 |

Better on the primary judge (masked blocks, -19) and on bytes/slots; 16 bytes
farther on padded size (the conversion sites inline smaller without the
helper's temps). Mixed verdict — presented, not landed, per the
no-speculative-edits rule.

## Policy statement, sharpened

The strict no-asm gate remains blocked by exactly one construct: the seven
`fld dword; fistp dword` fast_ftol expansions (conversion-width law, primary-
artifact proven). Everything else that was ever attributed to "vendored
assembly" in this function has now been recovered as ordinary C. If the owner
grants the upstream punpckhdq/halo convention (historical `__asm fast_ftol` in
cseries.h), structure_lens_flares is **4 instructions from byte-exact**, with
those 4 in the certified-tie class that basin evolution has repeatedly flipped
for free — and the two prior "impossible" ties (scale arm, magnitude order)
did exactly that this session.

matrix_math is unchanged from the morning closeout: the `__asm` witness
compiles all 37 symbols byte-exact; no C path exists (mechanism-proven);
scalar-C control emits pure x87.

## Artifacts

Witness stack JSONs: scratch/{z3_fastftol_now, a3geom_plane, b12_plane,
a3geom_avgassign, a3geom_ptrtemp, e1seam_planefull, z6_pair_dowhile,
z14_elem_outside}.json (apply in order); stacked source snapshot
scratch/_m_z14_elem_outside.c; agent logs scratch/agents/*.md; session log
scratch/agents/session_c_keystone_log.md.
