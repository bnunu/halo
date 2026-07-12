/* re-implemented from cachebeta.xbe disassembly */

void terminal_gets_end(void *state)
{
  if (state == dword_46c414) {
    dword_46c414 = NULL;
  }
}
