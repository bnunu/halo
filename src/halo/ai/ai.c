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

void ai_initialize_for_new_map(void)
{
  csmemset(ai_globals, 0, sizeof(ai_globals_t));
  ai_globals->enabled = true;
  ai_globals->unk_2 = true;
  ai_globals->unk_8 = -1;
  ai_globals->grenades_enabled = true;
  ai_globals->dialogue_triggers_enabled = true;
  csmemset(&ai_globals->unk_14, -1, sizeof(ai_globals->unk_14));
  csmemset(&ai_globals->unk_1c, -1, sizeof(ai_globals->unk_1c));
  csmemset(&ai_globals->unk_24, -1, sizeof(ai_globals->unk_24));
  ai_debug_initialize_for_new_map();
  ai_profile_initialize_for_new_map();
  path_initialize_for_new_map();
  actors_initialize_for_new_map();
  props_initialize_for_new_map();
  encounters_initialize_for_new_map();
  ai_script_initialize_for_new_map();
  ai_communication_initialize_for_new_map();
  ai_globals->unk_132 = 0;
  ai_globals->unk_130 = 0;
  csmemset(&ai_globals->unk_134, 0, sizeof(ai_globals->unk_134));
  ai_globals->unk_1 = true;
}

void ai_set_enabled(bool enabled)
{
  assert_halt_msg(ai_globals, "ai_globals");
  ai_globals->enabled = enabled;
}

void ai_set_dialogue_triggers_enabled(bool enabled)
{
  assert_halt_msg(ai_globals, "ai_globals");
  ai_globals->dialogue_triggers_enabled = enabled;
}

void ai_set_grenades_enabled(bool enabled)
{
  assert_halt_msg(ai_globals, "ai_globals");
  ai_globals->grenades_enabled = enabled;
}

void ai_dispose_from_old_map(void)
{
  ai_communication_dispose_from_old_map();
  ai_script_dispose_from_old_map();
  encounters_dispose_from_old_map();
  props_dispose_from_old_map();
  actors_dispose_from_old_map();
  path_dispose_from_old_map();
  ai_profile_dispose_from_old_map();
  ai_debug_dispose_from_old_map();
  ai_globals->unk_1 = false;
}
