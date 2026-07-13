/* re-implemented from cachebeta.xbe disassembly */

void ai_script_initialize(void)
{
}

void ai_script_dispose(void)
{
}

void ai_script_initialize_for_new_map(void)
{
}

void ai_script_dispose_from_old_map(void)
{
}

void ai_deselect(void)
{
  if (ai_globals->unk_1) {
    ai_debug_select(-1, -1);
  }
}

void ai_reconnect(void)
{
}
