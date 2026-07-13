/* re-implemented from cachebeta.xbe disassembly */

void real_math_reset_precision(void)
{
  _control87(0x9001f, 0xfffff);
}
