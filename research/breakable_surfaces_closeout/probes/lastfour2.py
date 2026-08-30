"""Last 4: change the KIND of lvalue each cross operand is.

The chain head becomes the load. Text order and helper structure cannot
move it, but binding an operand through a pointer local changes the operand
node the front end builds, which is a different axis.
"""
import subprocess, sys
BASE = "scratch/witness_fl_f1_local_grouped.c"
T = "\t"
CALL = T*5 + "cross_product3d(&s_normal, &surface_plane.n, &t_normal);\n"
DECL = T*5 + "real_vector3d s_normal;\n"

V = {
 "h0_control": None,
 "h1_plane_ptr": (DECL, DECL + T*5 + "real_vector3d const *plane_normal;\n",
                  CALL, T*5 + "plane_normal = &surface_plane.n;\n"
                        + T*5 + "cross_product3d(&s_normal, plane_normal, &t_normal);\n"),
 "h2_both_ptr": (DECL, DECL + T*5 + "real_vector3d const *plane_normal;\n"
                       + T*5 + "real_vector3d const *edge_normal;\n",
                 CALL, T*5 + "plane_normal = &surface_plane.n;\n"
                       + T*5 + "edge_normal = &s_normal;\n"
                       + T*5 + "cross_product3d(edge_normal, plane_normal, &t_normal);\n"),
 "h3_normal_ptr": (DECL, DECL + T*5 + "real_vector3d const *edge_normal;\n",
                   CALL, T*5 + "edge_normal = &s_normal;\n"
                         + T*5 + "cross_product3d(edge_normal, &surface_plane.n, &t_normal);\n"),
}

def run(tag, spec):
    w = open(BASE, encoding="latin-1").read()
    if spec:
        d_old, d_new, c_old, c_new = spec
        assert d_old in w and c_old in w, tag
        w = w.replace(d_old, d_new, 1).replace(c_old, c_new, 1)
    src = "scratch/witness_h_%s.c" % tag
    open(src, "w", encoding="latin-1", newline="\n").write(w)
    out = "scratch/disas_h_%s.txt" % tag
    with open(out, "w") as f:
        subprocess.run(["python", "tools/campaign/gate.py", "source/physics/breakable_surfaces",
                        "--source", src, "--fn", "_breakable_surface_effect",
                        "--disas", "_breakable_surface_effect"], stdout=f, stderr=subprocess.STDOUT)
    t = open(out, encoding="utf-8", errors="replace").read()
    if "COMPILE FAILED" in t or "Traceback" in t:
        print("%-16s COMPILE FAILED" % tag, flush=True); return
    print("%-16s diff-lines=%d" % (tag, sum(1 for l in t.splitlines() if "|" in l) - 1), flush=True)

for tag in (sys.argv[1:] or list(V)):
    run(tag, V[tag])
