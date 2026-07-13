/* re-implemented from cachebeta.xbe disassembly */

void ai_debug_dispose(void)
{
  if (dword_331f58) {
    debug_free(dword_331f58, __FILE__, __LINE__);
    dword_331f58 = NULL;
  }
  if (dword_331f5c) {
    debug_free(dword_331f5c, __FILE__, __LINE__);
    dword_331f5c = NULL;
  }
}
