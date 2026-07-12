/* re-implemented from cachebeta.xbe disassembly */

void debug_keys_dispose(void)
{
  if (dword_46d928) {
    debug_free(dword_46d928, __FILE__, __LINE__);
    dword_46d928 = NULL;
  }
}
