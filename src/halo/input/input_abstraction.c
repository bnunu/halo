/* re-implemented from cachebeta.xbe disassembly */

void input_abstraction_dispose(void)
{
  csmemset(input_abstraction_globals, 0, 0xdc);
}
