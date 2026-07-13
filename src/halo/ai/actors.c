/* re-implemented from cachebeta.xbe disassembly */

void actors_initialize(void)
{
  actor_types_initialize();
  actor_data = game_state_data_new("actor", 0x100, 0x724);
  assert_halt_msg(actor_data, "actor_data");
  swarm_data = game_state_data_new("swarm", 0x20, 0x98);
  assert_halt_msg(swarm_data, "swarm_data");
  swarm_component_data = game_state_data_new("swarm component", 0x100, 0x40);
  assert_halt_msg(swarm_component_data, "swarm_component_data");
}

void actors_dispose(void)
{
}
