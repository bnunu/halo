/* re-implemented from cachebeta.xbe disassembly */

void particles_dispose_from_old_map(void)
{
  data_make_invalid(particle_data);
}

void particles_dispose(void)
{
  if (particle_data) {
    particle_data = NULL;
  }
}
