/* re-implemented from cachebeta.xbe disassembly */

void ai_profile_initialize(void)
{
  csmemset(&ai_profile_globals, 0, sizeof(ai_profile_globals_t));
  ai_profile_globals.unk_4 = true;
}

void ai_profile_dispose(void)
{
}

void ai_profile_initialize_for_new_map(void)
{
  csmemset(&ai_profile_globals.unk_c, 0, sizeof(ai_profile_globals.unk_c));
}

void ai_profile_dispose_from_old_map(void)
{
}
