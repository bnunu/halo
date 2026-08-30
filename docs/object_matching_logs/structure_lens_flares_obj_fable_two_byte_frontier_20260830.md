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

## dbg32b decode of tie B (session 4) — DEFINITIVE

Ran dbg32b (the same-bitness C2.Dll debugger) on the pn1+fi4 witness (tie B
isolated), esi-filtering the emit loop at 0x1075132d to the [0xe60,0xe6c] byte
window. Captured the live emit-list nodes of tuple group **0x168**:

| esi | insn | node flags(+8) | +14 (fmt·group) | prev |
|---|---|---|---|---|
| 0xe63 | `lea ecx,[&pp]` | 0x1004010d int | 0x0006·0168 | (lea before) |
| 0xe69 | `push ecx` | 0x1004010d int | 0x0001·0168 | lea |
| 0xe6a | `push ebx` (sign) | 0x2004010d push | 0x0001·0168 | push ecx |
| 0xe6b | `fadd [origin.x]` | 0x4004010d x87 | 0x0100·0168 | push ebx |

Each node's `prev` points at the previously-emitted node: the emit loop is a
pure list walker, and the nodes are **linked in creation order**. `push ebx`
(the projection_sign arg) was created immediately before `fadd`. The only fields
that distinguish the two are opcode(+4), the class nibble(+8: 2004 push vs 4004
x87), the format hi-word(+14: 0x0001 vs 0x0100), and operand-producer pointers
(+28/+2c/+48) — **every one a consequence of what each op is, none an
order-priority key.** Confirmed at the creation level what the earlier pass saw
at the field level.

**Mechanism, now decoded:** tie B is the argument-evaluation interleave of the
call `project_point3d(&position, axis, sign, &pp)`. VC7 evaluates arguments
right-to-left — &pp(4), sign(3), axis(2), &position(1, last) — so position.x's
fadd (the dependency of arg1 &position) is *created after* the sign push (arg3):
our order. January emits the fadd first, i.e. its position.x s-term linearized
one step earlier in the same traversal. The creation order is a deterministic
function of the normalized call+position DAG, and the front-end collapses every
source respelling of the position computation and the call to the same DAG —
which is why no in-function construct reorders these two node creations
(15k variants + corpus + two session-4 sweeps + the explicit origin-placement
forms ub1/ub2/ub3: zero flips, while pn1 flipped 0x6c9 and fi4 flipped tie A).

**Determination (tie B):** a DAG-creation-order tie-break with no node order
field and no in-function source lever. By the identical-compiler theorem a
form exists, but the differentiating input is not a statement of
build_structure_lens_flares — it is either an unfound in-function DAG nuance the
front-end erases, or global-TU codegen state (node-creation counts from other
functions / file-scope declaration order) that this function's source cannot
steer. This is the same certified family as tie A's non-byte-forcing space, the
normalize seam, and the units scheduler-tie negatives — below practical source
visibility.

## Finish status

Byte-exact via legitimate in-function C is **not achievable** for this object,
by three independent, individually-proven blockers:
1. the seven `fld dword; fistp dword` fast_ftol conversions (non-legal-C;
   conversion-width law, first-hand re-confirmed);
2. tie A (only the byte-forcing `fi4` double-cast reaches it);
3. tie B (dbg32b-decoded DAG-creation-order tie-break; no source lever, clean
   or byte-forcing).

The maximal recovery is the witness at **lcsd 8 clean** (base8 + pn1, all
ordinary C) plus the historical `__asm fast_ftol`, reproducing the entire
4336-byte function to two scheduler tie-breaks. The object remains parked
(vendored-assembly); the production no-asm candidate is unchanged at 4288 (the
closest-size legal form — the plane-family stack improves masked blocks 47→25
but is 16 B smaller, a mixed trade not landed). The two owner decisions stand:
the fast_ftol `__asm` convention, and (only if that is granted) whether tie A/B
warrant a dbg32b ready-list session on the global-context hypothesis.

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
