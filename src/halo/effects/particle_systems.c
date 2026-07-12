/* Auto-drafted inert stub re-implementations (target = bare ret). */

void particle_systems_initialize(void)
{
  particle_system_data = game_state_data_new("particle systems", 0x40, 0x158);
  particle_system_particle_data =
    game_state_data_new("particle system particles", 0x200, 0x80);
}

void particle_systems_initialize_for_new_map(void)
{
  data_make_valid(particle_system_data);
  data_make_valid(particle_system_particle_data);
}

void particle_systems_dispose(void)
{
}
