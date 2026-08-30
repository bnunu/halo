# breakable_surfaces closeout evidence (2026-08-30 session)

Reproducible evidence for the `_breakable_surface_effect` mechanism
adjudication recorded in
`docs/object_matching_logs/breakable_surfaces_obj.md`.

## witness_fast_ftol_stack_20260830.json

The complete analysis-only witness edit stack. Apply in-memory with the gate
harness (never to production source — the helper is inline assembly and the
owner rule is no `__asm` in production, period):

```
python tools/campaign/gate.py source/physics/breakable_surfaces \
    --edits research/breakable_surfaces_closeout/witness_fast_ftol_stack_20260830.json \
    --fn _breakable_surface_effect --disas _breakable_surface_effect
```

Edit tags, in order:

| tag | content | January proof |
| --- | --- | --- |
| `helper` | verbatim `__inline long fast_ftol(float d)` `__asm fld d; fistp result` from historical `cseries.h` (non-ancestor commit `5093ac1a1`) | HCEX-attested; byte-exact standalone twin in January actor_combat.obj |
| `site0..3` | the four grid-bound conversions respelled `(short)fast_ftol(ceil/floor(PIN(...)))` | all four call sites align at January's exact section offsets |
| `w3` | offset block: `scratch.traversal.s_offset` store + plain `real t_offset` + both products passed as expressions into `point_from_line3d` | offset region instruction-for-instruction identical to January, including the unstored second random and `fxch st(1)` |
| `x1*` | named `real random_value` for the `rgb_colors_interpolate` fifth argument | January stores that random to a scalar temp and pushes a reloaded dword |
| `x2*` | named `long sound_definition_index` scoping the sound epilogue | January registerizes the index and the whole epilogue choreography follows |
| `x3alpha` | alpha PIN multiply spelled `(upper - lower) * real_local_random()` | January's non-popping `fmul st(1)` + later `fstp st(0)` = product owned by the left operand |

Witness measurement (campaign flags, XDK 3911 CL 13.00.9254.1):
padded 4032/4032, relocations 117/117, instructions 1156/1156, frame
`0x1240` exact, and byte-identical with and without `/QIfist`. Remaining
divergence: four dot-product accumulation orders (8 `fmul` lines,
January k,j,i vs ours k,i,j from the same `dot_product3d` inline), and the
frame placement of `new_particle_data` vs the conversion-temp cluster plus a
mid-frame slot rotation.

The `w3`/`x1`/`x2`/`x3` edits are ordinary C; `x1+x2+x3` are landed in
production. `w3` is deliberately NOT in production: under the current
per-unit `/QIfist` profile the union member stores compensate for the
missing fast_ftol expansion bytes, so landing the truer dataflow regresses
the checkpoint (4032 -> 3984). It becomes the correct production shape the
moment the flag profile is reconciled.
