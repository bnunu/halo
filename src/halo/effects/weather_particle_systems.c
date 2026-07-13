/* re-implemented from cachebeta.xbe disassembly */

void weather_particle_systems_initialize(void)
{
  weather_particle_data = data_new("weather particles", 0x200, 0x54);
  if (!weather_particle_data) {
    error(0, "couldn't allocate weather particle system globals.");
  }
}

void weather_particle_systems_initialize_for_new_map(void)
{
  for (__int16 local_player_index = 0;
       local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
       local_player_index++) {
    assert_halt_msg(local_player_index >= 0 &&
                      local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS,
                    "local_player_index>=0 && "
                    "local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS");
    weather_globals.players[local_player_index].unk_0.value = -1;
  }
  weather_globals.unk_0 = 0;
  data_make_valid(weather_particle_data);
}

void weather_particle_systems_dispose(void)
{
  if (weather_particle_data) {
    data_dispose(weather_particle_data);
    weather_particle_data = NULL;
  }
}

void weather_particle_systems_dispose_from_old_map(void)
{
  if (weather_particle_data->valid) {
    data_make_invalid(weather_particle_data);
  }
}
