"""The last 4 lines: cross-product operand roles, re-tested in the f1 basin.

Every earlier cross verdict was measured in the flat- or grouped-header
basin. The TU-local grouped distance changed the basin, so those verdicts
are void and must be re-run (basin law).
"""
import subprocess, sys

BASE = "scratch/witness_fl_f1_local_grouped.c"
T = "\t"
PROTO = "static void breakable_surface_effect(\n"
CALL = "cross_product3d(&s_normal, &surface_plane.n, &t_normal);"

def helper(body):
    return ("static __inline real_vector3d *breakable_surface_cross(\n"
            "\treal_vector3d const *a,\n"
            "\treal_vector3d const *b,\n"
            "\treal_vector3d *result)\n"
            "{\n" + body + "\treturn result;\n}\n\n")

STD = (T+"real k = a->i*b->j - a->j*b->i;\n"
       +T+"real j = a->k*b->i - a->i*b->k;\n"
       +T+"real i = a->j*b->k - a->k*b->j;\n"
       +T+"result->i = i;\n"+T+"result->j = j;\n"+T+"result->k = k;\n")
BFIRST = (T+"real k = a->i*b->j - a->j*b->i;\n"
          +T+"real j = b->i*a->k - a->i*b->k;\n"
          +T+"real i = a->j*b->k - b->j*a->k;\n"
          +T+"result->i = i;\n"+T+"result->j = j;\n"+T+"result->k = k;\n")
TWOTEMP = (T+"real k = a->i*b->j - a->j*b->i;\n"
           +T+"real j = a->k*b->i - a->i*b->k;\n"
           +T+"result->i = a->j*b->k - a->k*b->j;\n"
           +T+"result->j = j;\n"+T+"result->k = k;\n")
GROUPED = (T+"real k = (a->i*b->j) - (a->j*b->i);\n"
           +T+"real j = (a->k*b->i) - (a->i*b->k);\n"
           +T+"real i = (a->j*b->k) - (a->k*b->j);\n"
           +T+"result->i = i;\n"+T+"result->j = j;\n"+T+"result->k = k;\n")
AKLOCAL = (T+"real ak = a->k;\n"
           +T+"real k = a->i*b->j - a->j*b->i;\n"
           +T+"real j = ak*b->i - a->i*b->k;\n"
           +T+"real i = a->j*b->k - ak*b->j;\n"
           +T+"result->i = i;\n"+T+"result->j = j;\n"+T+"result->k = k;\n")

VARIANTS = {"g0_control": None, "g1_local_std": STD, "g2_local_bfirst": BFIRST,
            "g3_local_2temp": TWOTEMP, "g4_local_grouped": GROUPED,
            "g5_local_aklocal": AKLOCAL}


def run(tag, body):
    w = open(BASE, encoding="latin-1").read()
    if body is not None:
        w = w.replace(PROTO, helper(body) + PROTO, 1)
        w = w.replace(CALL, "breakable_surface_cross(&s_normal, &surface_plane.n, &t_normal);")
    src = "scratch/witness_g_%s.c" % tag
    open(src, "w", encoding="latin-1", newline="\n").write(w)
    out = "scratch/disas_g_%s.txt" % tag
    with open(out, "w") as f:
        subprocess.run(["python", "tools/campaign/gate.py", "source/physics/breakable_surfaces",
                        "--source", src, "--fn", "_breakable_surface_effect",
                        "--disas", "_breakable_surface_effect"], stdout=f, stderr=subprocess.STDOUT)
    txt = open(out, encoding="utf-8", errors="replace").read()
    if "COMPILE FAILED" in txt or "Traceback" in txt:
        print("%-20s COMPILE FAILED" % tag, flush=True); return
    n = sum(1 for l in txt.splitlines() if "|" in l) - 1
    print("%-20s diff-lines=%d" % (tag, n), flush=True)


for tag in (sys.argv[1:] or list(VARIANTS)):
    run(tag, VARIANTS[tag])
