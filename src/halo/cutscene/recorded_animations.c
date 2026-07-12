/* re-implemented from cachebeta.xbe disassembly */

void recorded_animations_dispose(void)
{
  if (dword_44df0c) {
    debug_free(dword_44df0c, __FILE__, __LINE__);
    dword_44df0c = NULL;
  }
}

void recorded_animations_dispose_from_old_map(void)
{
  data_make_invalid(recorded_animation_data);
}
