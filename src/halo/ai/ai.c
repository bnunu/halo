/* re-implemented from cachebeta.xbe disassembly */

void ai_initialize(void)
{
  ai_globals = (ai_globals_t *)game_state_malloc("ai globals", NULL,
                                                 sizeof(ai_globals_t));
  assert_halt_msg(ai_globals, "ai_globals");
  csmemset(ai_globals, 0, sizeof(ai_globals_t));
  ai_debug_initialize();
  ai_profile_initialize();
  path_initialize();
  actors_initialize();
  props_initialize();
  encounters_initialize();
  ai_script_initialize();
  ai_communication_initialize();
  actor_moving_initialize();
}

void ai_dispose(void)
{
  ai_communication_dispose();
  ai_script_dispose();
  encounters_dispose();
  props_dispose();
  actors_dispose();
  path_dispose();
  ai_profile_dispose();
  ai_debug_dispose();
}
