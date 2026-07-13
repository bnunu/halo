/* re-implemented from cachebeta.xbe disassembly */

void encounters_initialize(void)
{
  encounter_data = game_state_data_new("encounter", 0x80, 0x6c);
  assert_halt_msg(encounter_data, "encounter_data");
  squad_array = game_state_malloc("squad", "squad", 0x8000);
  assert_halt_msg(squad_array, "squad_array");
  platoon_array = game_state_malloc("platoon", "platoon", 0x1000);
  assert_halt_msg(platoon_array, "platoon_array");
  pursuit_data = game_state_data_new("ai pursuit", 0x100, 0x28);
  assert_halt_msg(pursuit_data, "pursuit_data");
}

void encounters_dispose(void)
{
}

void encounters_dispose_from_old_map(void)
{
  data_make_invalid(encounter_data);
  data_make_invalid(pursuit_data);
}
