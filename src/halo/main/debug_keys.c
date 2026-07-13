/* re-implemented from cachebeta.xbe disassembly */

void debug_keys_initialize(void)
{
  int count = 0;
  if (debug_keys_table[0].unk_0) {
    for (debug_key_t *key = debug_keys_table;; key++) {
      if (key->down) {
        *key->down = 0;
      }
      count++;
      if (!key[1].unk_0) {
        break;
      }
    }
  }
  int size = ((count + 0x1f) >> 5) * 4;
  global_debug_key_down = debug_malloc(size, 0, __FILE__, __LINE__);
  assert_halt(global_debug_key_down);
  csmemset(global_debug_key_down, 0, size);
}

void debug_keys_dispose(void)
{
  if (global_debug_key_down) {
    debug_free(global_debug_key_down, __FILE__, __LINE__);
    global_debug_key_down = NULL;
  }
}
