/* re-implemented from cachebeta.xbe disassembly */

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
