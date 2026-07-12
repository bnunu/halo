/* re-implemented from cachebeta.xbe disassembly */

void effects_dispose_from_old_map(void)
{
  data_make_invalid(effect_data);
  data_make_invalid(effect_data2);
}

void effects_dispose(void)
{
  if (effect_data) {
    effect_data = NULL;
  }
  if (effect_data2) {
    effect_data2 = NULL;
  }
}
