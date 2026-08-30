"""Break the 20-line floor with TU-LOCAL changes only (no shared header).

The {j,k}-grouped dot fixes 16 of the 20 lines but is refuted tree-wide
because dot_product3d is shared. A helper local to THIS translation unit has
zero board impact, so the same association can be tested honestly.
"""
import subprocess, sys

BASE = "scratch/witness_mirror2.c"
T = "\t"
PROTO = "static void breakable_surface_effect(\n"

HELPER = (
 "/* Plane-distance with the j,k terms summed first.  January's four\n"
 "   expansions evaluate the sum in that association; the shared\n"
 "   dot_product3d must stay flat because 17 other exact functions depend\n"
 "   on it, so the association is expressed here, local to this unit. */\n"
 "static __inline real breakable_surface_plane_distance(\n"
 "\treal_plane3d const *plane,\n"
 "\treal_point3d const *point)\n"
 "{\n"
 "\treturn point->x*plane->n.i + (point->y*plane->n.j + point->z*plane->n.k) - plane->d;\n"
 "}\n"
 "\n")

CALLS = [
 ("plane3d_distance_to_point(&s_plane, vertex_point)",
  "breakable_surface_plane_distance(&s_plane, vertex_point)"),
 ("plane3d_distance_to_point(&t_plane, vertex_point)",
  "breakable_surface_plane_distance(&t_plane, vertex_point)"),
]

VARIANTS = {
    "f0_control": None,
    "f1_local_grouped": HELPER,
    "f2_local_flat": HELPER.replace(
        "point->x*plane->n.i + (point->y*plane->n.j + point->z*plane->n.k)",
        "point->x*plane->n.i + point->y*plane->n.j + point->z*plane->n.k"),
    "f3_local_plane_first": HELPER.replace(
        "point->x*plane->n.i + (point->y*plane->n.j + point->z*plane->n.k)",
        "plane->n.i*point->x + (plane->n.j*point->y + plane->n.k*point->z)"),
}


def run(tag, helper):
    w = open(BASE, encoding="latin-1").read()
    if helper is not None:
        assert PROTO in w
        w = w.replace(PROTO, helper + PROTO, 1)
        for a, b in CALLS:
            w = w.replace(a, b)
    src = "scratch/witness_fl_%s.c" % tag
    open(src, "w", encoding="latin-1", newline="\n").write(w)
    out = "scratch/disas_fl_%s.txt" % tag
    with open(out, "w") as f:
        subprocess.run(["python", "tools/campaign/gate.py", "source/physics/breakable_surfaces",
                        "--source", src, "--fn", "_breakable_surface_effect",
                        "--disas", "_breakable_surface_effect"], stdout=f, stderr=subprocess.STDOUT)
    txt = open(out, encoding="utf-8", errors="replace").read()
    if "COMPILE FAILED" in txt or "Traceback" in txt:
        print("%-20s COMPILE FAILED" % tag, flush=True)
        print(txt[-400:]); return
    n = sum(1 for l in txt.splitlines() if "|" in l) - 1
    print("%-20s diff-lines=%d" % (tag, n), flush=True)


for tag in (sys.argv[1:] or list(VARIANTS)):
    run(tag, VARIANTS[tag])
