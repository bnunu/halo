/* re-implemented from cachebeta.xbe disassembly */

void scenario_initialize(void)
{
  scenario_globals = game_state_malloc("scenario globals", NULL, 0x100);
}

scenario_t *global_scenario_get(void)
{
  assert_halt(global_scenario);
  return global_scenario;
}
