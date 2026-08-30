"""Declaration order / scope of the two vectors.

Mechanism found by the C2 frame walk: nothing in lowering CHOOSES between
commutative operands - the head of the operand chain becomes the load, and
that chain order is canonicalised upstream.  If the canonical order follows
symbol order, then the DECLARATION ORDER and SCOPE of the two vectors is the
lever, and it is ordinary readable C.
"""
import subprocess, sys

BASE = "scratch/witness_mirror2.c"
T = "\t"
INNER = T*5 + "real_vector3d t_normal;\n" + T*5 + "real_vector3d s_normal;\n"
FUNC_ANCHOR = T + "real_plane3d surface_plane;\n"

VARIANTS = {
    "d0_control":      [],
    # swap the two inner declarations
    "d1_swap_inner":   [(INNER, T*5 + "real_vector3d s_normal;\n" + T*5 + "real_vector3d t_normal;\n")],
    # hoist both to function scope, BEFORE surface_plane
    "d2_func_before":  [(INNER, ""),
                        (FUNC_ANCHOR, T + "real_vector3d t_normal;\n" + T + "real_vector3d s_normal;\n" + FUNC_ANCHOR)],
    # hoist both to function scope, AFTER surface_plane
    "d3_func_after":   [(INNER, ""),
                        (FUNC_ANCHOR, FUNC_ANCHOR + T + "real_vector3d t_normal;\n" + T + "real_vector3d s_normal;\n")],
    # hoist, swapped, before
    "d4_func_before_s":[(INNER, ""),
                        (FUNC_ANCHOR, T + "real_vector3d s_normal;\n" + T + "real_vector3d t_normal;\n" + FUNC_ANCHOR)],
    # hoist, swapped, after
    "d5_func_after_s": [(INNER, ""),
                        (FUNC_ANCHOR, FUNC_ANCHOR + T + "real_vector3d s_normal;\n" + T + "real_vector3d t_normal;\n")],
    # move surface_plane down into the inner arm instead
    "d6_plane_inner":  [(FUNC_ANCHOR, ""),
                        (INNER, T*5 + "real_plane3d surface_plane;\n" + INNER)],
}


def run(tag, edits):
    w = open(BASE, encoding="latin-1").read()
    for find, repl in edits:
        if find not in w:
            print("%-18s PATTERN MISSING" % tag, flush=True); return
        w = w.replace(find, repl, 1)
    src = "scratch/witness_dc_%s.c" % tag
    open(src, "w", encoding="latin-1", newline="\n").write(w)
    out = "scratch/disas_dc_%s.txt" % tag
    with open(out, "w") as f:
        subprocess.run(["python", "tools/campaign/gate.py", "source/physics/breakable_surfaces",
                        "--source", src, "--fn", "_breakable_surface_effect",
                        "--disas", "_breakable_surface_effect"], stdout=f, stderr=subprocess.STDOUT)
    txt = open(out, encoding="utf-8", errors="replace").read()
    if "COMPILE FAILED" in txt or "Traceback" in txt:
        print("%-18s COMPILE FAILED" % tag, flush=True); return
    n = sum(1 for l in txt.splitlines() if "|" in l) - 1
    print("%-18s diff-lines=%d" % (tag, n), flush=True)


for tag in (sys.argv[1:] or list(VARIANTS)):
    run(tag, VARIANTS[tag])
