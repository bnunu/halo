/* re-implemented from cachebeta.xbe disassembly */

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
