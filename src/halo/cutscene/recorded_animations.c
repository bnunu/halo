/* re-implemented from cachebeta.xbe disassembly */

void recorded_animations_initialize_for_new_map(void)
{
  data_make_valid(recorded_animation_data);
  assert_halt(animation_threads_debug);
  csmemset(animation_threads_debug, 0, 0x400);
}

void recorded_animations_dispose(void)
{
  if (animation_threads_debug) {
    debug_free(animation_threads_debug, __FILE__, __LINE__);
    animation_threads_debug = NULL;
  }
}

void recorded_animations_dispose_from_old_map(void)
{
  data_make_invalid(recorded_animation_data);
}
