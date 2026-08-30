# `circular_queue.obj` early-success result packet (2026-08-30)

## Result

This packet started from local campaign commit
`0bd66c9c0b8c293d13c4fbb6bc46bad487bde6c0` and investigated the sole
non-exact owner in `source/memory/circular_queue.obj`,
`_circular_queue_dequeue_data`.

One genuinely fresh, natural C89 lifetime placement was compiled once.  It was
not exact, so it was rejected and the production source was restored
byte-for-byte.  The object remains **7/8 functions exact**, representing
`592/848` target padded code bytes.  All `251/251` owned `.rdata` bytes remain
strict exact.

## Evidence census

The following canonical material was read in full before editing:

- `docs/object_matching_logs/circular_queue_obj.md`
- `research/circular_queue_obj/EXPERIMENTS.md`
- `research/circular_queue_obj/compare.py`
- `research/circular_queue_obj/find_bl_donors.py`
- `research/circular_queue_obj/october_probe.py`
- `docs/exact_match_acceleration_playbook.md`
- `tools/campaign/README.md`
- `source/memory/circular_queue.c` and `.h`

All commits touching the translation unit and all 18 circular-queue-named refs
and surviving worktrees were recensused, including dirty working-copy state.
The apparent dirty source files were line-ending/index refreshes with empty Git
diffs.  Reachable ref tips reduce to the already-known source blobs:

- `192160e09018e9135210bd92222a4a8317c56a4c`: current production source;
- `ef56002b0cc42474d23b7aa79aad9d6e79ad3833` and
  `a07b6f457c2281f5bf29452b89db49e442a8299a`: the same production body with
  older comments;
- `6ae3b910353c02a58d37e1699cf1f1c0e575bc4a`: the superseded outer
  `result && advance` writeback topology;
- `e5b9c9b7357a45f00f5aa79960fcd81b1823df10`: the original stub family.

No worktree contains an untracked candidate object or unrecorded source body.
The latest evidence commits (`73ce982b0`/`634fd66a7`, `c5514492d`,
`d63b25044`/`53e1dab90`, `df529faf6`/`307f77efc`, and `fd0055c17`) agree with
the canonical N01-N37 ledger.

The October 2001 beta at VA `0x00118fb0` remains an exact machine-topology
donor, but it contains no source/local records.  The complete cached HCEA
`simple_circular_queue_*` family was read; it is a later fixed-element queue
with a different structure and API, not a source donor for this byte queue.
The cached punpckhdq/Stian/Pastudan families and a fresh public exact-symbol/
assert-string search exposed no implementation body.  January's PDB still
provides only the public symbol.

## Frozen baseline

The strict whole-TU baseline was frozen in
`build/circular_queue_regression_manifest.json` before the experiment.

| Owner | Padded size T/B | Relocs T/B | Normalized SHA-256 | State |
|---|---:|---:|---|---|
| `_circular_queue_reset` | `16/16` | `0/0` | `43bd388fa461cee21e94369e90a18459b65c36ef9b0d20ec68f3a589abce1ab6` | exact |
| `_circular_queue_validate` | `112/112` | `6/6` | `f6a98f7f32feb7dfe05b503ecbb463faba7ae62effff0c1f2c3c50486d7e3da6` | exact |
| `_circular_queue_new` | `96/96` | `4/4` | `89dac4b56b758d67e5607d1e4346c9de20725c95d37544110eae68c6b4adb76e` | exact |
| `_circular_queue_delete` | `48/48` | `3/3` | `90fd5b668d0661b3aa3043550b0670181fd87af81e097237e1b38b8452bb4d10` | exact |
| `_circular_queue_size` | `32/32` | `1/1` | `691530e94ba7f3f2ce922e1dfc1a4e03bb081af0f78d43f1baa572b9e96b531d` | exact |
| `_circular_queue_free_space` | `48/48` | `1/1` | `fa1cb9aa5c44d00dce56b74bdcb3380f3d03cec7d37a3a36c231cd09d806b38d` | exact |
| `_circular_queue_queue_data` | `240/240` | `12/12` | `cfe1f5adce7a13f339612b68dfde5bb5b11a32845abd9295a3ae960048a5545a` | exact |
| `_circular_queue_dequeue_data` | `256/240` | `12/12` | target `2981341a22238a314c0f885d555149dc279257b6109242e674f38ccfb0ccd1f9`; base `40adf3be1fa79c0b9aaf6b169e37da2e59aa4e306efef68a9573b7aa7ec8dede` | non-exact |

The residual has 222 differing normalized bytes and begins at `+0x4`.  Its 12
relocation destinations/types/addends match, but the different prologue and
register topology shift the target/base relocation addresses to:

```text
target: 14,45,50,55,62,72,123,164,193,198,203,210
base:   12,43,48,53,60,70,119,160,189,194,199,206
```

The five exact zero-relocation `.rdata` COMDATs are:

| Logical bytes | Normalized SHA-256 | Contents |
|---:|---|---|
| 46 | `c671d36a6ddf25152b4cdb1f2fb75917529d09b9cb55d997b1a99b5025e8d130` | corruption format string |
| 39 | `7bb74ce48b4230918f0336668893e44ab42036e839d992f9d6878661f6bb0662` | source path |
| 65 | `b897082a5404f6a92d9b0c855c09d4431d5bfd5f2d985d1ea1196753b6aded90` | write-offset assertion |
| 52 | `7af78a1d6e6d778ffdcf376eb96e8119b693888759aef19fba40863a7dd52218` | data/size assertion |
| 49 | `08a109861dd5fdee4328ab5373f9c29c6077706988118c4d5162f1e9d7e46c5f` | read-offset assertion |

January's defining allocation is:

```text
push ebx; push esi; push edi
mov edi,queue
xor bl,bl
...
cmp data_size,available
jg failure
...
mov ebx,read_offset
...
success: mov al,1; ret
failure: mov al,bl; ret
```

Thus the false byte's EBX live range ends on the success edge exactly where the
long `read_offset` EBX live range begins.

## N38: assign success before the read-offset lifetime

N01-N37 did not test the most direct source lifetime corresponding to that
machine join: kill the initialized false value immediately on entry to the
success arm, before `read_offset` is initialized.  The candidate moved only
the existing assignment:

```c
if (data_size <= circular_queue_size(queue))
{
	result = TRUE;
	read_offset = queue->read_offset;
	/* unchanged copy/assert/advance body */
}
return result;
```

This is natural typed C89.  It adds no helper, alias, qualifier, intrinsic,
barrier, address-taking, raw offset, type pun, undefined behavior, or synthetic
dependency.  It was selected because its source lifetime ends `result` exactly
where January begins `read_offset`, while avoiding N18's loop and nested-inline
fixed point.

The candidate remained `240` padded bytes with 12 relocations at the baseline
addresses.  Its normalized SHA-256 was
`ec45d3733e1a2367104716c1f7b17d3f4961abd835462ce55c75819b72d42196`;
221 normalized bytes differed, one fewer than baseline but nowhere near exact.
All seven inherited exact owners and all five `.rdata` owners remained exact.

VC7 still kept `queue` in EBX, `read_offset` in EDI, and folded false into a
late `xor al,al`.  The only material change was the tail:

```text
+0xD5 mov al,[advance]
+0xD8 test al,al
+0xDA mov al,1
+0xDC je +0xE8
+0xDE mov [ebx+8],edi
...
+0xE6 xor al,al
```

This is not January's dual `mov al,1` / `mov al,bl` epilogue and does not create
the required disjoint EBX lifetime.  The strict whole-TU gate reported the
seven siblings `still_exact`, the dequeue owner `changed_nonexact`, and no
failures or warnings.  With no exact owner gain, N38 was rejected and the
production source was restored to Git blob
`192160e09018e9135210bd92222a4a8317c56a4c` and baseline object hash
`40adf3be1fa79c0b9aaf6b169e37da2e59aa4e306efef68a9573b7aa7ec8dede`.

## Restored-state validation

- The focused whole-TU gate reports all seven inherited functions
  `still_exact`, the restored dequeue owner unchanged non-exact, and zero
  failures or warnings.
- Full `halobetacache_build`, `libcmt_build`, `semantic_progress`, and
  `progress` gates pass.
- The semantic audit covers 470 units and 4,957 functions: 4,841 semantic
  exact, 4,851 accepted exact, and zero unit errors.
- Object admission reports zero candidates, contradictions, and revocations.
- Park validation reports 12 active, zero stale, and zero invalid.
- `python -m pytest -q` reports 212/212 passing tests.
- The strict board remains 277/619 exact objects, 667,863/1,922,669 padded
  bytes, and 4,823/8,246 exact functions.
- `git diff --check` passes.  The Units sentinel is unchanged at Git blob
  `c000b070f97406b480ae33d7e7f6f11def3ae69b`.

## Claude-ready boundary

Do not repeat N38 or any N01-N37 family.  In particular, moving the success
assignment anywhere within the copy/advance arm does not establish January's
BL-to-EBX coalescing; the early placement merely rotates the final AL branch.
The single-iteration N18 family remains the only legal source shape known to
create the exact BL/EBX lifetime, and it still adds a nested validator clone
(`336` bytes, 17 relocations).  Preventing that clone through noinline,
address-taking, or wrappers either destroys the lifetime or regresses the exact
private EAX ABI.

Reopen only with original/same-lineage source, local-variable or line records,
an exact donor that demonstrates the lifetime without changing a private ABI,
or a general readable-C control that independently prevents the nested clone.
Cosmetic declaration, branch, return, result-placement, loop, wrapper, barrier,
or function-order variants are exhausted.

Absolute handoff pointers:

- production source:
  `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\work\game-allegiance-closeout-20260830\source\memory\circular_queue.c`
- canonical ledger:
  `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\work\game-allegiance-closeout-20260830\docs\object_matching_logs\circular_queue_obj.md`
- experiment transcript:
  `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\work\game-allegiance-closeout-20260830\research\circular_queue_obj\EXPERIMENTS.md`
- this N38 ledger:
  `C:\Users\isabe\Documents\Codex\2026-07-13\i-w\work\game-allegiance-closeout-20260830\docs\object_matching_logs\circular_queue_obj_jonas_early_success_result_rejection_20260830.md`
