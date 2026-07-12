/* re-implemented from cachebeta.xbe disassembly */

void profile_render_start(void)
{
  unsigned int lo, hi;
  word_448dda = 0;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  dword_449c68 = lo;
  dword_449c6c = hi;
}
