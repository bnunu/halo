"""Extract all assert(expr) call sites from cachebeta.xbe.

The debug build calls 0x8d9f0 (display_assert) with args pushed:
    push halt; push line; push file_str; push expr_str; call 0x8d9f0
Usage: python dev/asserts.py <repo_dir> [file_substr]
Prints: code_addr  file:line  expr
"""
import sys, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from xbe import Xbe

repo = sys.argv[1]
want = sys.argv[2] if len(sys.argv) > 2 else ""

xbe = Xbe.from_file(f"{repo}/halo-patched/cachebeta.xbe")
sections = [(s.header.virtual_addr, s.header.virtual_size, s.data)
            for s in xbe.sections.values()]

def read_va(va, n):
    for base, size, data in sections:
        if base <= va < base + size:
            off = va - base
            return data[off:off + n]
    return b""

def read_cstr(va, maxlen=512):
    b = read_va(va, maxlen)
    i = b.find(b"\0")
    if i <= 0:
        return None
    try:
        return b[:i].decode("ascii")
    except UnicodeDecodeError:
        return None

ASSERT_FN = 0x8d9f0

# find the .text-like section containing the assert fn
for base, size, data in sections:
    if not (base <= ASSERT_FN < base + size):
        continue
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.skipdata = True
    pushes = []  # (addr, imm or None)
    for ins in md.disasm(data, base):
        if ins.mnemonic == "push":
            try:
                imm = int(ins.op_str, 16) if ins.op_str.startswith("0x") else int(ins.op_str)
            except ValueError:
                imm = None
            pushes.append((ins.address, imm))
            if len(pushes) > 8:
                pushes.pop(0)
        elif ins.mnemonic == "call":
            try:
                tgt = int(ins.op_str, 16)
            except ValueError:
                pushes.clear()
                continue
            if tgt == ASSERT_FN and len(pushes) >= 3:
                expr_a = pushes[-1][1]
                file_a = pushes[-2][1]
                line = pushes[-3][1]
                expr = read_cstr(expr_a) if expr_a else None
                fpath = read_cstr(file_a) if file_a else None
                if expr and fpath and (want.lower() in fpath.lower()):
                    print(f"{ins.address:8x}  {fpath.split(chr(92))[-1]}:{line}  {expr}")
            pushes.clear()
        elif ins.mnemonic not in ("mov", "lea", "add", "sub"):
            # any control flow etc. resets tracked pushes conservatively
            if ins.mnemonic.startswith("j") or ins.mnemonic in ("ret", "retn"):
                pushes.clear()
