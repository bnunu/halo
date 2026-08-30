# structure_lens_flares — the two-byte frontier (2026-08-30, session 3)

Branch fable/exact-finish-20260829. Continuing under the directive "assume
everything is recoverable C; if stuck do more research." All work in scratch
harnesses; production untouched.

## Headline

The witness now stands **2 bytes from byte-exact** (one instruction pair):
`base8 + pn1 + fi4` = size 4336/4336, relocs 156/156 exact sequence, frame
0x10c, **349/349 frame-slot refs exact, lcsd 2, one differing instruction pair**
(tie B, the project_point3d arg-push vs position.x fadd interleave). The clean
all-legitimate subset (`base8 + pn1`, dropping the one byte-forcing lever) is
lcsd 8. Only the seven `__asm fast_ftol` conversions remain as the non-legal-C
element (first-hand re-confirmed, session 2).

## What this session found

The masked diff tools (mdiff, slotmap) had been **hiding a third difference**:
they mask `[ebp-X]` slots and don't track reused positive param slots
(`[ebp+8]`), so a commutative fld/fmul swap at 0x6c9 aligned away. Base8 in fact
had THREE differences, not two:

1. **tie A @0e28** — `mov [s_count],eax` (int store) vs `fld [t_plane.n.j]`
   (FP, a LICM-hoisted `t_plane.n.j*t_distance` product). int/FP interleave.
2. **0x6c9 (×3)** — `point_from_line3d`'s `direction.k*offset` multiply: which
   operand is loaded to st(0). Commutative-load order.
3. **tie B @0e6a** — `push ebx` (projection_sign, ancient in ebx) vs
   `fadd [origin.x]` (position.x completion). int/FP interleave at the
   project_point3d call boundary.

### LEGITIMATE lever discovered — definition-position via decl/init split

`pn1_off_def_late`: in the marker cluster-search block, declare `real offset;`
with the other locals and **assign `offset = 1.f/65536.f;` after the three
`direction.{i,j,k}=…` statements** (valid C89; natural "initialize the search
step right before the search loop" ordering). This shifts `offset`'s c2
definition position later than `direction`'s, flipping the commutative-load
preference in `point_from_line3d` to January's (load offset first). **Fixes all
three 0x6c9 sites**; `base8 + pn1` = lcsd 20 → 8, blocks 7 → 4, slots
345 → 348. Pure statement reordering, arithmetically identical — not
byte-forcing. This is the first genuinely NEW clean recovery on these residual
ties and confirms the def-position mechanism the dbg32 pass named.

### tie A — fixable, but only by a byte-forcing lever

`fi4_tgrid_dbl`: `t_distance = (real)((double)t_grid * lens_flare_spacing)`
flips tie A (`base8 + pn1 + fi4` = lcsd 2). But it is **arithmetically inert**
on x87 (80-bit internal; verified byte-identical instruction multiset to base8)
— it only manipulates c2's type-tracking to shift scheduling. `units.c`, whose
tgr/cpmd ties were closed with the "FP-intermediate TYPE" lever, contains ZERO
`(double)` casts, so there is no owner precedent for inserted double casts.
**Classified byte-forcing; excluded from the clean witness.** It is retained
only as proof that tie A is source-reachable (which the identical-compiler
theorem already guarantees).

### tie B — the last holdout

`push ebx` (projection_sign) vs `fadd [origin.x]` at the project_point3d call.
projection_sign is ancient (callee-saved ebx, ready ~0x500 bytes earlier); the
fadd becomes ready one op prior. January defers the push; we emit it first.
Resisted this session: pn1-style def-position on offset/origin/s_distance/
position/projected_position/projection_sign (inert or copy-propagated);
fi4-style doubles on s_grid/s_distance/ds_x/position.x/origin (regress);
call-arg expression variants (sign!=0, casts, pointer temps — inert or
regress); b12 removal (regress); t-term/position reorderings (regress). ~40
distinct attempts across sessions/agents. It is the project_point3d arg-push
scheduling interleaved with the caller's position FP — a genuine low-level
tie-break.

## The identical-compiler theorem (why this is not "impossible")

C2.Dll and the campaign flags are byte-identical to January's (primary-artifact
proven). Identical compiler + identical flags ⇒ identical source yields
identical output. Our witness differs from January by exactly these ties, so a
source form that reproduces January's order provably EXISTS — these are not
"below source visibility" in the absolute sense, only not-yet-found. This
session converted one of the three (0x6c9) from "not found" to "found and
clean," strengthening the position that the remaining two are likewise
reachable; the front-end simply normalizes most candidate spellings to the same
IR, so the differentiating construct is narrow.

## State of the witness

| stack | size | relocs | frame | slots | lcsd | legit? |
|---|---|---|---|---|---|---|
| base8 (session 2) | 4336 | 156 seq | 0x10c | 345/345 | 20 | clean + fast_ftol |
| base8 + pn1 | 4336 | 156 seq | 0x10c | 348/348 | 8 | **clean + fast_ftol** |
| base8 + pn1 + fi4 | 4336 | 156 seq | 0x10c | 349/349 | **2** | fi4 byte-forcing |

fast_ftol (7 sites) is the only non-legal-C element in all rows; under the
no-asm rule the object cannot gate regardless. The rows measure how completely
January's source is otherwise reconstructed: with pn1, the entire 4336-byte
function is reproduced to **one instruction pair** save the seven conversions,
using ordinary readable C.

## Recommendation / open items

- **pn1 is a real recovery** — a natural statement ordering. If production ever
  moves to the fast_ftol convention, land pn1 with the rest.
- **tie A**: no clean lever found; fi4 demonstrates reachability but is
  byte-forcing. A legitimate def-position construct for the s_count-store /
  hoisted-t-product interleave is the remaining hunt (the LICM hoist point is
  the control variable; source levers tried so far do not move it).
- **tie B**: the one genuinely open tie-break; next instrument is dbg32b on the
  project_point3d arg-push scheduling (the dbg32 mechanistic pass this session
  mapped the emitter as a pure DAG-order serializer, so the ready-list seeding
  is where a source hook, if any, lives).
- **matrix_math**: unchanged; `__asm` witness compiles all 37 symbols exact, no
  C path (mechanism-proven).
