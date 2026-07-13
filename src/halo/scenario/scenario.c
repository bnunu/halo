/* re-implemented from cachebeta.xbe disassembly */

void scenario_initialize(void)
{
  scenario_globals = (scenario_globals_t *)game_state_malloc(
    "scenario globals", NULL, sizeof(scenario_globals_t));
}

void scenario_initialize_for_new_map(void)
{
  wind_initialize_for_new_map();
  csmemset(scenario_globals->unk_4, 0, sizeof(scenario_globals->unk_4));
  csmemcpy(scenario_globals->unk_b8, byte_2c1220,
           sizeof(scenario_globals->unk_b8));
  scenario_globals->unk_b4 = 0;
}

scenario_t *global_scenario_get(void)
{
  assert_halt(global_scenario);
  return global_scenario;
}
