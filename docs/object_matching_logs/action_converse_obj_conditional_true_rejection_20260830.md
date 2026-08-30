# `action_converse.obj` conditional-`TRUE` closeout (2026-08-30)

## Scope and authority

This bounded closeout starts from local commit
`bcea0a36ab0102915eabebb491276df0c27858d3` on isolated branch
`jonas/ima-adpcm-fresh-packet-20260830`. Production scope was limited to the
single 256-byte residual `_action_converse_perform` in
`source/ai/action_converse.c` and one genuinely fresh natural-C lifetime
premise. No other source, parked entry, object, or fixed exclusion was changed.

The immutable January target is
`build/split/source/ai/action_converse.obj`. The hardened COFF comparator is
strict authority, including relocation types and destinations. Compilation
used the repository's pinned XDK 3911 VC7 edge and normal per-unit flags.

The campaign board reports `8 / 9` strict-C owners and `640 / 896` padded code
bytes. `_action_converse_perform` is byte-identical only because its current
source contains inline assembly; `config/parked.json` correctly classifies it
as `asm-implemented`, so it receives no C-reconstruction credit.

## Validated target and baselines

| Shape | Meaningful / padded | Relocations | Normalized SHA-256 | Status |
| --- | ---: | ---: | --- | --- |
| January target | `248 / 256` | 8 | `7a3fad8fb69307c132558b906832d4d31f0334734fc76d18fe773189b5a38d5d` | authority |
| restored assembly baseline | `248 / 256` | 8 | `7a3fad8fb69307c132558b906832d4d31f0334734fc76d18fe773189b5a38d5d` | strict bytes, parked/no C credit |
| historical natural C | `244 / 256` | 8 | `288862a662942eb5b03ee84809dbd6b6aea1c023d52915f2bc373192e7c212cb` | nonexact |
| E01 conditional-lifetime C | `244 / 256` | 8 | `288862a662942eb5b03ee84809dbd6b6aea1c023d52915f2bc373192e7c212cb` | rejected |

The exact target relocation sequence is:

| Offset | Type | Destination |
| ---: | ---: | --- |
| 4 | `DIR32` | `_actor_data` |
| 17 | `REL32` | `_datum_get` |
| 67 | `REL32` | `_prop_get_base_by_unit_index` |
| 107 | `DIR32` | `_prop_data` |
| 114 | `REL32` | `_datum_get` |
| 157 | `DIR32` | `__real@3f333333` (`0.7f`) |
| 186 | `REL32` | `_actor_move_halt` |
| 220 | `REL32` | `_actor_move_to_prop` |

Both natural-C shapes preserve all eight relocation identities and order, but
their offsets are `4, 16, 63, 103, 110, 153, 183, 216`. The first true
divergence is target `+0x08: push ebx`; natural C starts `push esi` there and
never reserves `EBX`.

The residue is completely classified. January emits `mov ebx,1` between the
run-to-prop comparison and its flag-consuming branch, pushes `EBX` twice for
the two true arguments to `prop_get_base_by_unit_index`, and stores `BL` to
both `in_range` and `failed`. VC7 instead emits two `push 1` and two immediate
byte stores for ordinary C. Everything after those instruction-length changes
is the same control/data topology plus displacement drift.

## Provenance and artifact census

The census was completed before editing.

- Every reachable source blob for `source/ai/action_converse.c` was deduplicated
  and inspected. The only implemented natural-C family is the historical
  `true_value` body introduced at `b4651a63...`; the later
  `fcade4ab...`/`e1de8768...`/`2e781110...` lineage replaces its four constant
  uses with inline assembly and produces the exact bytes.
- The original parked record at `65da8997...` says five grounded natural
  shapes had already reduced the function to the same equal-size/equal-reloc
  register-allocation tie. Those source spellings were not preserved, so this
  ledger does not invent them.
- Dedicated worktrees and branches were inspected:
  `work/action-converse-closeout`, `work/action-converse-final-2`,
  `work/lanes/action-converse-final`, `action-converse-controls`,
  `action-converse-leaves`, and `actor-layout-converse`. Their surviving
  natural object is the `288862a6...` family above; the final branches contain
  the known assembly body.
- Workspace-wide source and ledger searches found no additional Claude scratch
  body. The numerous other worktrees contain copies of one of the two
  reachable source families.
- Punp's authenticated current and full-history checkouts contain only the
  empty action-converse skeleton. Stian and Pastudan contain no implementation
  donor for this function.
- The authenticated HCEA donor at
  `research/halocea/src/action_converse_perform.c` confirms the timeslice gate,
  cached unit-to-prop resolution, two true acquisition flags, distance tests,
  halt behavior, move failure, and returned `failed` byte. It also independently
  confirms the converse state offsets and `0.7f` threshold. Its local pointer
  and literal-`1u` spelling is semantic evidence, not an exact x86 donor.

The target disassembly and HCEA donor agree that no missing call, field,
branch, float comparison, or relocation remains. The blocker is only VC7's
choice to materialize a shared constant in callee-saved `EBX`.

## The single fresh premise

The historical natural source initialized `true_value = TRUE` before testing
`actor->meta.timeslice`. January does not materialize the shared one until
after entering the timeslice arm. The sole fresh premise therefore shortened
the value's real lifetime by leaving the local uninitialized outside the arm
and assigning it immediately inside:

```c
boolean true_value;

actor = actor_get(actor_index);
state_data = &actor->state.action_data.converse;
if (actor->meta.timeslice)
{
	true_value = TRUE;
	/* The proven natural converse body uses true_value four times here. */
}
```

The rest of E01 used the target's already-proven natural control topology:
resolve the prop only when absent, range-test it, return after halting, jump to
the shared return when movement succeeds, and otherwise store `true_value` to
`failed`. The assembly-only offset enum was removed with the assembly body.
The spelling is typed, defined, readable C89 and changes no behavior.

E01 compiled once. VC7 constant-propagated the conditional assignment and
reproduced the historical natural object exactly:

| ID | Premise | Size T/C | Relocs T/C | Candidate hash | First divergence | Siblings | Decision |
| --- | --- | ---: | ---: | --- | --- | --- | --- |
| E01 | assign shared `TRUE` only inside timeslice arm | `248/244` meaningful, `256/256` padded | `8/8` identities, offsets drift | `288862a662942eb5b03ee84809dbd6b6aea1c023d52915f2bc373192e7c212cb` | `+0x08`: target saves `EBX`; candidate saves `ESI` | all eight siblings remained exact | reverted |

The result falsifies the conditional-lifetime premise: moving the assignment
across the timeslice branch is inert under this compiler. It neither reserves
`EBX` nor changes any byte of the known natural family.

## Disposition and Claude-ready blocker

E01 was reverted with the exact inverse patch. Production source again hashes
to blob `3d0d4ddf8017e0c0beaefff83510157ca587f12d`; the rebuilt assembly baseline
is strict exact. There is no implementation delta. The board remains
`8 / 9`, `640 / 896`, with `_action_converse_perform` correctly parked as
`asm-implemented`.

Do not repeat:

- the historical `boolean true_value = TRUE` family;
- E01's declaration-before-branch/assignment-inside-branch spelling;
- a target-style `goto` versus nested `if (!actor_move_to_prop)` rewrite;
  both already canonicalize to `288862a6...` in natural C;
- HCEA's literal `1u` spelling, which gives the same immediate-constant class;
  or
- source spellings allegedly belonging to the old five-row matrix unless the
  actual text is recovered. The old ledger did not preserve it.

Claude should reopen this owner only with one of these concrete new premises:

1. an authenticated Bungie/HCEA-era original-source spelling that gives the
   shared true value a non-constant dependency while proving it is exactly
   one at all four uses;
2. a recovered local/PDB record that proves the original variable's type,
   scope, and live range;
3. a surviving compiled artifact with a distinct natural-C source blob and
   exact target hash; or
4. a new ordinary-C dataflow in which a runtime-derived boolean naturally
   remains in `EBX` across `_prop_get_base_by_unit_index` and the later stores,
   without adding calls, memory accesses, relocations, or undefined behavior.

The first comparison for any new candidate should be the `+0x08` prologue and
`+0x2c` materialization site. If it does not emit `push ebx` and `mov ebx,1`,
stop before tuning the exact suffix. Preserve the eight exact sibling owners
and the single `0.7f` const owner.

Assembly, volatility, `register`, pragmas, intrinsics, attributes, barriers,
raw offsets/addresses, pointer or union puns, undefined behavior, synthetic
anchors, alternate flags/tools, object-byte patching, comparator exceptions,
and fuzzy credit remain forbidden.

## Verification

After restoration, the focused whole-TU regression gate passes with all nine
byte-level owners in `still_exact`; the residual stays excluded from C credit
solely by its `asm-implemented` park. Full consolidated and protected gates
are recorded after this documentation commit is built.
