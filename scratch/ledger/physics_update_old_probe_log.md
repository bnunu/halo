# _physics_update_old probe log (lane claude/decals-physics-20260924)

Base TU: scratch/ph/lb_real_all.c (Lane B try_w3_real_all.c: real point_from_line3d calls).
Harness: scratch/lane/fvar.py; census tools reloc_segments.py, slot_census.py, leafswaps.py.
All rows are gate.py strict verdicts; nothing here is exact (zero credit).

| id | evidence | change | result |
|----|----------|--------|--------|
| base | Lane B | real point_from_line3d | [size 5184!=5168, sha] |
| A2 | /Od 0x7be7aa vector_from_points3d | local_position via helper | inert alone; worse on Lall (LEAF 7->11) |
| B1 | /Od 0x7bf2b0 dot(v,v) | at-rest test dot_product3d(&v,&v) | ACCEPT: exact lowering at site, 1625->1619 insns |
| B2 | /Od | collision normal dot arg order | inert (superseded by L6) |
| C  | /Od 0x7bfc34 add_vectors3d | new_position via helper | worse on Lall (LEAF 7->12) |
| A1 | /Od 0x7be675 add_vectors3d(total_torque) | REJECTED: January copies magic_torque (3 dword moves) |
| P3 | /Od 0x7bf56e..f3 | no probe_length local; radius+antigrav_height passed and recomputed | ACCEPT: padded 5168 = January, [sha] only |
| F1 | /Od 0x7beca8 je->ice arm second | `!= _material_ice`, normal arm first | ACCEPT: segments 42/44 -6/+5 -> -1/0 |
| I1 | /Od [ebp-0x264] distinct counter | inertia loop declares its own short counter | ACCEPT: frame slots -0x44..-0x54 now = January; January homes it in dead magic_force slot [ebp+0x14] |
| I2/I3 | /Od 0x7bfa23..a91 / 0x7bf9e7 | moment expr order; dot(&radius,&axis) | inert alone, kept in L-all |
| G1 | /Od 0x7be990 | gravity*depth/ground_depth ... *mass | INERT (VC7 canonicalises the product) |
| L-all | /Od named scalar slots -0xd4,-0x13c/140,-0x15c,-0x174,-0x190/194,-0x1ac/1b0,-0x1b4,-0x244/248 | named locals per /Od | 198 -> 189 diff blocks; LEAF sites 12 -> 7 |
| D2 | /Od 0x7bfd89 dot(&up,&forward) | end-of-function dot arg order | LEAF 7 -> 5 (best: fv_LD2.obj, 187 blocks) |

Best: LD2 = B1+P3+F1+I1+L-all+D2. Remaining: frame rotation physics ptr (-8) vs total_torque (-0x14..-0xc);
LEAF at 0x3bc/0x3d8 (loop-head cross product) and 0xc81/0xc95/0xcb2 (loop-end cross product);
~24 REG and ~86 OTHER blocks (x87/int interleave placement, e.g. pmpd null test hoisted at 0x44c vs Jan 0x4a3;
flags store-forwarding at Jan 0x973; normalize3d reciprocal consumption at Jan 0x10aa; antigrav point copy order z,x,y).
Scalar local NAMES are not attested (no RTC for scalars; HCEX.pdb enregisters scalars) - probe names only.
HCEX.pdb real names: mass_points_at_rest_count / _on_ground_count / _on_volatile_surface_count / _in_water_count,
position, powered_world_matrix, collision, point, vector, torque_axle, new_position, new_location, axis.
