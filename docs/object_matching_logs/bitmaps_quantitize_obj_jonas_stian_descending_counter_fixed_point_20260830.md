# `bitmaps_quantitize.obj` Stian descending-counter fixed point (2026-08-30)

## Scope and baseline

This bounded closeout ran only in the isolated
`agent/light-volumes-closure-20260830` worktree. Canonical was read-only at
`6a73e6adf90b2e57e769dca9be2761b1c8ee11c5`; its Quantitize source and base
ledger are byte-equivalent to this worktree's starting files.

The hardened gate reports two exact functions and one residual:

| Function | Target/base padded bytes | Relocations | Strict state |
| --- | ---: | ---: | --- |
| `_bitmap_quantitize` | 592/592 | 28/28 | exact |
| `_code_0006d5d0` | 32/32 | 0/0 | exact |
| `_code_0006d5f0` (`row_dither`) | 464/464 | 0/0 | residual |

All target-owned `.rdata`, `.data`, and `.bss` remain exact. The target
`row_dither` normalized SHA-256 is
`a5feef0c98bc8565991a9cf9722e0258d2bd556e23fc0326891aa0ce0ebca5db`;
the restored production hash is
`7afe02e297bf06dd682a9c44bfd00aacf39c2f7ea0c0f45acce838dc9b696da4`.
Both have zero relocations.

The complete target/base disassembly agrees except at inner-loop offsets
`+0x178` through `+0x17B`:

```text
January: inc eax; add ebx, 2; add ecx, 2
baseline: add ebx, 2; inc eax; add ecx, 2
```

`EAX` is the channel induction value, `EBX` is the minimum-error pointer, and
`ECX` is the optimizer's next-row channel pointer. Frame, CFG, branches, data
accesses, and every other instruction are exact.

Before compiling, the original `bitmaps_quantitize_obj.md`, its complete Git
history, `C:/tmp/bitmaps-quantitize-agent/ROW_DITHER_PROVENANCE_REPORT.md`,
and canonical freshness audit commit
`935e6d944bb3f226c768390123e33681e65c3abd` were read in full. The prior
1,571-worktree/4,643-object census classifies all ten historical source hashes
and all existing candidate objects. Neither direct statement ordering,
two-loop reversal, declaration order, barriers, pragmas, aliases, explicit
pointer dependencies, nor the prohibited diagnostic-assembly families was
repeated.

## Fresh authenticated donor premise

The authenticated Stian tree contains a readable implementation that the
earlier search missed because it retains an address-derived placeholder name:

`C:/Users/isabe/Documents/Codex/2026-07-13/i-w/work/stianeklund-halo-reference/src/halo/bitmaps/bitmaps.c`

`FUN_0007ef80` begins at the exact October donor address `0x7EF80`. Unlike the
production reconstruction, its inner error loop derives each channel as
`4 - i` from a descending counter and has no source-level channel increment.
The compiler must therefore synthesize the ascending channel induction. This
is a materially new natural-C scheduling premise: a generated EAX increment
could legally be placed before the two explicit pointer advances, exactly as
January does.

Workspace searches found no prior Quantitize candidate containing
`CHANNEL_COUNT - channels_remaining`, `4 - channels_remaining`, or the
equivalent Stian descending-index spelling. HCEA, punpckhdq, Pastudan,
halopc-restored, and the remaining authenticated mirrors add no competing C
implementation or original local record.

## Single bounded candidate

The one candidate adapted only the Stian induction premise. It retained the
existing UB-free typed accesses and exact surrounding function, but replaced
the error loop's explicit `channel_index` with
`CHANNEL_COUNT - channels_remaining`. This kept the candidate legal House C89:
no pointer subtraction across allocations, raw offsets, assembly, `volatile`,
`register`, pragma, intrinsic, barrier, pun, undefined behavior, byte forcing,
or compiler-flag change was used.

The result retained 464 padded bytes, zero relocations, and both exact sibling
functions, but regressed structurally:

- 458 meaningful / 464 padded bytes;
- normalized SHA-256
  `9691818f52c7a9df9ed8f5f9a55a3e82726b5e837955dd64a6b99fea47ef803b`;
- first divergence at `+0x0F`, where January homes the outer pixel index at
  `[EBP-8]` but the shortened explicit-index lifetime changes multiple frame
  homes;
- the error-loop preheader creates a separate pointer home, and the tail
  becomes `add [ebp-4],2; add ebx,2; add ecx,2; inc eax; cmp eax,4` rather than
  January's four-register maintenance sequence.

Thus XDK 3911 does not preserve the descending counter as a paired
increment/decrement induction. It materializes different pointer and loop
state, moving far more than the two residual instructions. The candidate
provides no strict gain and was restored immediately. Restored production
source raw SHA-256 is
`1ddbe04004450954182c6161eddf87558122b180969a47acfbe60de2f646a1b1`.

## Durable boundary and Claude-ready handoff

Do not repeat this descending-counter/`4-i` donor family. It is distinct from
the older source-order matrix and now has a measured compiler outcome. The
object remains 2/3 strict exact with all data exact and no production-source
change from this packet.

Reopen only with original Bungie statement/local provenance, a legal C89 donor
that is strict exact under XDK 3911 CL `13.00.9254.1` and the campaign flags,
or a new legal scheduling control that changes only the EAX/EBX ordering while
preserving the ECX placement and every other byte.

Claude must read these paths before any further work:

- `C:/Users/isabe/Documents/Codex/2026-07-13/i-w/work/rasterizer-model-wrapper-alias-20260829/docs/object_matching_logs/bitmaps_quantitize_obj.md`
- `C:/Users/isabe/Documents/Codex/2026-07-13/i-w/work/rasterizer-model-wrapper-alias-20260829/docs/object_matching_logs/rasterizer_transparent_geometry_bitmaps_quantitize_obj_jonas_closure_freshness_audit_20260830.md`
- this ledger;
- `C:/tmp/bitmaps-quantitize-agent/ROW_DITHER_PROVENANCE_REPORT.md`
- `C:/Users/isabe/Documents/Codex/2026-07-13/i-w/work/rasterizer-model-wrapper-alias-20260829/source/bitmaps/bitmaps_quantitize.c`
- `C:/Users/isabe/Documents/Codex/2026-07-13/i-w/work/stianeklund-halo-reference/src/halo/bitmaps/bitmaps.c`

Any future candidate must preserve both exact sibling functions, all owned
data/sections, zero-relocation `row_dither`, the protected Units sentinel, and
the complete whole-TU admission baseline.

## Restored validation

The rejected candidate was removed before final validation. The restored
focused gate reports 2 exact, 1 residual, and 0 unwritten owners; the sole
residual remains `_code_0006d5f0` at 464 padded bytes. The restored source
SHA-256 is
`1ddbe04004450954182c6161eddf87558122b180969a47acfbe60de2f646a1b1`.

The complete `halobetacache_build`, `libcmt_build`, `progress`, and
`semantic_progress` gates pass. Final campaign measurements are 384/833 exact
objects, 4,812/11,060 exact functions, 610,646/2,198,102 exact code bytes, and
1,856,050/4,176,062 exact data bytes. The Halo partition is 282/468 exact
objects, 4,645/7,574 exact functions, 597,732/1,770,166 exact code bytes, and
1,850,858/3,923,451 exact data bytes. Semantic validation evaluates 4,957
functions across 470 units, with 4,841 semantic exact, 4,851 accepted exact,
131 hidden exact / 78,940 hidden bytes, and zero unit errors.

Additional restored gates pass:

- object admission: zero candidates, contradictions, and revocations;
- parked functions: 12 active, zero stale, and zero invalid;
- tooling tests: 212/212 pass;
- protected Units `_unit_preprocess_node_orientations`: strict exact at 1,920
  padded bytes, 87 ordered relocations, normalized SHA-256
  `5d5c8edc492fb8ab6ea83e1ccaa4cb2798da51ae4a17182fa848878bed05a7ed`;
- report SHA-256:
  `13c9f7e8ac93b0a6de047e0f40bc27408dd076504228e7e1211f2cc4fe526f4f`;
- semantic-report SHA-256:
  `9f948fc7de4a7d8d11f52ebbfb7f17126cb03ea893662921475c6e9484f66db0`;
- hardened comparator SHA-256:
  `d926e41b4d1b8eedd6ad8813615c32da452dcd8e95ad7fd3fa87ac5d82fe11a5`;
- `git diff --check`: clean.

There is no admitted production delta: 0 functions and 0 bytes. No push is
performed.
