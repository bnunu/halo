# breakable_surfaces closeout evidence

Reproducible evidence for the `_breakable_surface_effect` reconstruction
recorded in `docs/object_matching_logs/breakable_surfaces_obj.md`.

## 2026-08-30 part 2: the origin-mirror source recovery

Commit `fc47f9a15a0aea7f0081394c4d66db5c68ab5323` on the repository's own
`origin` remote (github.com/punpckhdq/halo) contains a
`source/physics/breakable_surfaces.c` from an earlier reconstruction lineage
that preserved the historical topology this repository had lost. It is NOT an
authentic Bungie blob (it carries `match_assert` and a `// TODO: doesn't
match` in its own headers), so it ranks as level-5 hypothesis evidence — but
its shapes were byte-verified here one at a time and all 11 sibling
functions gate strict-exact from its text unchanged.

Verified January shapes recovered from it (all now in production source):

- `real_point2d jitter` in an inner block, both products passed as
  expressions into `point_from_line3d` (explains the never-written
  `[ebp-0x1DC]` frame slot: it is `jitter.y`, elided but allocated);
- `struct new_particle_data particle` declared INSIDE the convex-hull block
  (fixes the particle-vs-temp-cluster frame placement wholesale);
- natural scoping instead of the E09 union: `surface_vertices2d` at while
  scope, `position_2d_test` in the s-loop, `s_normal`/`t_normal` in the
  first-vertex arm, `temp_2d` in the seed arm, `sound_location` plain in the
  epilogue block (the union was a compensating reconstruction device);
- `real u = real_local_random();` in a bare block for the
  `rgb_colors_interpolate` argument;
- `(tint_upper - tint_lower) * real_local_random()` alpha operand order
  (independently derived from the x87 popping form the day before, then
  found verbatim in the mirror);
- `rectangle2d bounds` naming, `real_point3d const *vertex_point` lvalues,
  `fast_ftol(ceil/floor(PIN(...)))` at the four conversion sites.

Two mirror shapes were REJECTED by January's bytes: its
`projection_from_vector3d` helper call (January open-codes the fabs block —
`test ah,1`/`jne` comparisons prove our spelling, and the attested assert
line anchors 266->348 span exactly 82 physical lines only with the
open-coded block) and its shared `bsp3d_get_plane_from_designator`
(January's `jns` at 0x177 proves our private helper's `& LONG_MIN` test).

Line-anchor validation: January's four in-function assert line numbers
(251/266/348/388, attested by the pushed constants in the binary) span
15/82/40 lines; the reconstruction spans 16/82/40 — byte-perfect spans in
the whole body, one line still unaccounted in the declaration head.

## witness_mirror_fast_ftol_20260830.c

Self-contained analysis-only witness (owner rule: no `__asm` in
production). With ONE additional analysis-only context lever in
`source/math/real_math.h`:

```
-	return a->i*b->i + a->j*b->j + a->k*b->k;
+	return a->i*b->i + (a->j*b->j + a->k*b->k);
```

the witness measures **4 differing instruction lines** against January
(1156/1156 instructions, 4032/4032 padded bytes, 117/117 relocations, frame
`0x1240`, every stack home identical):

```
python tools/campaign/gate.py source/physics/breakable_surfaces \
    --source research/breakable_surfaces_closeout/witness_mirror_fast_ftol_20260830.c \
    --fn _breakable_surface_effect --disas _breakable_surface_effect
```

The residual 4 lines are two commutative fld/fmul pairs in the
`cross_product3d` expansion (January folds `s_normal.i`, loading the plane
member first, in exactly the two `a->i` products).

The paren-grouping lever is fi4-class (scheduling-only, no provenance — the
lineage's grouped `triple_product3d` is explicitly marked "doesn't match" by
its own author) and is therefore NOT in production headers. Without it the
witness differs by 20 lines (the four distance-dot expansions reassociate
k,i,j instead of January's k,j,i, plus the same cross pair). Its byte-effect
proves a real, still-unfound upstream source difference exists
(identical-compiler theorem); the next instrument is reading the C2 IR node
numbers with `tools/c2dbg32` at the fld-choice site.

Measured-inert negatives in this basin (do not re-run): dot wrapper argument
order (both wrappers), reverse-spelled dot, grouped `magnitude_squared3d`,
2-temp vs 3-temp `cross_product3d` (byte-neutral here AND tree-neutral under
a full-board rebuild), b-first operand text in the two cross products
(changes other bytes, not the fold), nested
`normalize3d(vector_from_points3d(...))`, `fast_ftol` helper placement
(cseries.h vs TU), declaration-order permutations of
`s_normal`/`t_normal`/`next_vertex_point`, decl/init split of the else-arm
`s`/`t`, member-lvalue vs pointer-local vertex access.

Method warning that cost this session an afternoon: header-state experiments
MUST checksum the header per run (`scratch/header_matrix.py`) — a stale
header state produced a wrong lever attribution (the 2-temp cross was
credited with a fix that belonged to the paren lever).

## witness_fast_ftol_stack_20260830.json (superseded)

The 2026-08-30 part-1 edit-stack witness against the pre-mirror production
source. Historical: its W3/X1/X2/X3 recoveries are all subsumed by the
mirror topology (X3 verbatim-confirmed, X1 as `real u`, X2 superseded — the
mirror's plain sound test matches January in the new basin; W3 as `jitter`).
