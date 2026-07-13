/* re-implemented from cachebeta.xbe disassembly */

void ai_debug_initialize(void)
{
  csmemset(&ai_debug_globals, 0, sizeof(ai_debug_globals_t));
  ai_debug_globals.unk_38 = -1;
  ai_debug_globals.unk_34 = -1;
  ai_debug_globals.unk_f4 = 1;
  ai_debug_globals.unk_a5 = true;
  if (!actor_debug_array) {
    actor_debug_array = debug_malloc(0x657c00, 0, __FILE__, __LINE__);
  }
  if (!actor_path_debug_array) {
    actor_path_debug_array = debug_malloc(0x394f80, 0, __FILE__, __LINE__);
  }
  assert_halt_msg(actor_debug_array && actor_path_debug_array,
                  "actor_debug_array && actor_path_debug_array");
}

void ai_debug_dispose(void)
{
  if (actor_debug_array) {
    debug_free(actor_debug_array, __FILE__, __LINE__);
    actor_debug_array = NULL;
  }
  if (actor_path_debug_array) {
    debug_free(actor_path_debug_array, __FILE__, __LINE__);
    actor_path_debug_array = NULL;
  }
}
