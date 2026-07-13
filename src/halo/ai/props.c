/* re-implemented from cachebeta.xbe disassembly */

void props_initialize(void)
{
  prop_data = game_state_data_new("prop", 0x300, 0x138);
  assert_halt_msg(prop_data, "prop_data");
}

void props_dispose(void)
{
}
