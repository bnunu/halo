# lra_cache.obj - private-function naming + `_lra_allocate` diagnosis (2026-08-30)

State unchanged at **13/14 code functions exact**; `_lra_allocate` remains the
sole residual. This entry records a house-rule cleanup that is byte-neutral,
and a sharper diagnosis of the residual.

## Six placeholder names retired

The object carried six `code_<address>` private functions, which the naming
rule forbids. All six were identified from behaviour and call sites, renamed in
`config/symbols.json` (each with `"static": true`) and regenerated through
csplit:

| was | now | evidence |
| --- | --- | --- |
| `code_0010b9f0` | `lra_default_update_proc` | signature is exactly `lra_update_proc`; installed in `lra_new` when the caller passes none |
| `code_0010ba00` | `lra_default_delete_proc` | signature is exactly `lra_delete_proc`; installed the same way |
| `code_0010ba10` | `lra_delete_block` | calls `cache->delete_proc`, clears locked and sets deleted |
| `code_0010ba30` | `lra_verify_block` | sole body is the block-integrity `match_vassert` |
| `code_0010bab0` | `lra_verify_cache` | asserts cache signature/base/size, then verifies the last block |
| `code_0010bdf0` | `lra_block_offset` | verifies, then returns the block's byte offset from `base_address` |

Verified byte-neutral: 13 exact functions before and after, board unchanged at
277 objects / 4,829 functions.

## Source layout recovered from assert line anchors

The surviving `__FILE__`/line pairs pin January's ordering. Note that the
image addresses in the file header do **not** give source order - the linked
addresses and the line anchors disagree, so only the anchors are evidence:

| line | function |
| --- | --- |
| 86-126 | `lra_new` |
| 140-141 | `lra_dispose` |
| ~150-280 | `lra_flush`, then `lra_allocate` |
| 282 | `lra_free` |
| 298 | `lra_lock` |
| 314 | `lra_unlock` |
| 398 | `lra_verify_block` |
| 408-418 | `lra_verify_cache` |

So `lra_allocate` sits between `lra_flush` and `lra_free`, roughly lines
160-280, and the private helpers are defined at the *end* of the file, after
all public entry points.

## `_lra_allocate`: 368 against January's 400

The frame already matches (`sub esp, 0x14`, five slots) and so does the loop
topology. January's function is **larger**, and the extra 32 bytes are
accounted for:

January duplicates `first_deleted_block = NULL; number_of_passes = 0;` into
*both* arms of the `last_block ? last_block->next : NULL` test, materialising
`xor eax, eax` separately in each arm, and then spends that zero again for
`write_offset = 0`. Our build keeps `result`'s NULL live in `ESI` across the
whole prologue and reuses it for all of those stores, so the arms merge and
the immediate stores collapse.

This is the same mechanism found the same day in
`prime_numbers::_generate_prime_numbers`: **January pins a zero in a register
and spends it repeatedly; our build rematerialises immediates.** The two
objects are in unrelated subsystems, which is why a flag cause was tested.

### Controls run

- **Optimisation flags: refuted.** `/O1`, `/O2 /Os`, `/O1 /Ob2` all shrink the
  function to 319 bytes, `/Ox /Os` explodes it to 1255, and `/O2 /Og` leaves it
  at 368. Nothing moves toward 400. The campaign's `/O2 /Oy- /DDEBUG /Dxbox`
  is the closest of everything tested, so the gap is not a flag.
- **Ternary rewritten as explicit `if`/`else`: byte-identical.** The arm
  duplication is compiler tail-duplication, not a source shape.

### What would close it

A source shape in which `result` is memory-only rather than holding a live
zero in `ESI`, which would force the same rematerialisation January performs
and restore the duplicated arms. The slot permutation would likely follow:
January homes `first_deleted_block` at `-4`, `write_offset` at `-8` and
`last_block` at `-0xc`, while ours homes `write_offset` at `-4`,
`last_block` at `-8` and `first_deleted_block` at `-0xc`.

Not parked: parking requires equal size, and this function is 32 bytes short.
