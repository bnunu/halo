"""Find code references to a string in cachebeta.xbe and disassemble context.

Usage: python dev/strref.py <repo_dir> <substring> [context_before=0x60]
"""
import sys, re, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from xbe import Xbe

repo, want = sys.argv[1], sys.argv[2]
before = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0x60

xbe = Xbe.from_file(f"{repo}/halo-patched/cachebeta.xbe")
secs = [(s.header.virtual_addr, s.header.virtual_size, s.data)
        for s in xbe.sections.values()]

def rd(va, n):
    for b, sz, d in secs:
        if b <= va < b + sz:
            return d[va - b:va - b + n]
    return b""

# find string VAs
str_vas = []
wb = want.encode()
for b, sz, d in secs:
    i = d.find(wb)
    while i >= 0:
        # walk back to string start
        j = i
        while j > 0 and d[j-1] >= 0x20 and d[j-1] < 0x7f:
            j -= 1
        str_vas.append(b + j)
        i = d.find(wb, i + 1)

md = Cs(CS_ARCH_X86, CS_MODE_32)
for va in sorted(set(str_vas)):
    pat = struct.pack("<I", va)
    for b, sz, d in secs:
        k = d.find(pat)
        while k >= 0:
            ref = b + k
            label = rd(va, 70).split(b'\0')[0].decode('ascii', errors='replace')
            label = label.encode('ascii', errors='replace').decode()
            print(f"\n=== ref {ref:#x} -> str {va:#x} {label!r}")
            code = rd(ref - before, before + 0x10)
            for ins in md.disasm(code, ref - before):
                mark = " <<<" if ins.address <= ref < ins.address + ins.size else ""
                print(f"{ins.address:8x}: {ins.mnemonic:8} {ins.op_str}{mark}")
            k = d.find(pat, k + 1)
