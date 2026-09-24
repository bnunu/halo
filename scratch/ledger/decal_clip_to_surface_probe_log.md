# _decal_clip_to_surface probe log (lane claude/decals-physics-20260924)

Canonical: [sha]; frame 0x70 vs January 0x6c; 35 diff blocks; zoffset nudge HAND-EXPANDED point_from_line3d.
Tools: scratch/dc/mk_c4.py (builds base TU), scratch/dc/clip_edits.py (all edits), scratch/lane/fvar.py.

| id | evidence | change | result |
|----|----------|--------|--------|
| C1 | /Od 0x567272 real call; lab forced-inline reproduces January 0x66a-0x6a0 byte-for-byte | real point_from_line3d at zoffset site | rule-compliant; stays OUT OF LINE (budget) until C7 |
| C3 | /Od 0x5669fb `je` epilogue with write-back inside | single-exit wrap | byte-neutral (budget neutral: early return == if-level) |
| C4 | /Od arm order + January assert-line anchors (1168->1365 gap 197 vs /Od 173; wrap-first would need ~+80) | `if (!update || angle<=min) {clip} else {wrap}` | layout-inert (VC7 emits wrap first either way) |
| scope | F2 slot-merge law; frame census: offset(8B, only .y homed) merges into a 4B temp entry (+4) instead of previous_point's entry | clipping-loop points in an inner block | frame 0x70 -> 0x6c = January |
| S4 | /Od declaration order (edge_index, edge_iteration first) + January init-store order + zero pin | outer: edge_index, edge_iteration, input_points, count, clip_flags; inner {current, previous; do-while} | zero pin matches |
| C7 | inline-gate lab: zoffset site inlines with <=3 condition levels, not 4/5; January test `test byte [+8],0xb` | `!(surface->flags & (FLAG(two_sided)|FLAG(invisible)|FLAG(breakable)))` | point_from_line3d INLINED; with S4: 4 diff blocks |
| tail | /Od tail order input_points, edge_index, edge_iteration++, previous | all 24 permutations | inert for the remaining swap (IENP kept) |
| B/W/U/T | /Od named sphere booleans, wrap counter; HCEX unsigned long clip_flags; texture_y scope | each and combined | inert |
| oracle | k unused / k used locals (LAB ONLY) | - | swap not ordinal-sensitive |

Best (S4+C7+C1+C3+C4): [sha] only, 571/571 insns, frame 0x6c, remaining = ONE two-instruction swap at Jan 0x4e1:
January `mov ecx,[ebp-0x14]` (edge_iteration) before `mov esi,[esi+edx*4+8]` (edge_index); ours the reverse.
Scheduler tie-break invariant under statement order, declaration order and local counts tried.
CANNOT LAND: emits _point_from_line3d COMDAT, allowed only for a strictly exact caller (folded-inline exception).
Open questions for the owner if it ever closes: the bare inner block and the flag-mask spelling are
inferred from the frame/inline laws, not attested by /Od (the /Od revision uses three TEST_FLAGs and a flat block).
