/* re-implemented from cachebeta.xbe disassembly */

void weather_particle_systems_dispose_from_old_map(void)
{
  if (weather_particle_data->valid) {
    data_make_invalid(weather_particle_data);
  }
}
