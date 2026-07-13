/* re-implemented from cachebeta.xbe disassembly */

char stack_walk(__int16 skip_frames)
{
  return stack_walk_internal(NULL, skip_frames + 1, NULL);
}
