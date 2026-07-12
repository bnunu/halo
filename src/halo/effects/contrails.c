/* re-implemented from cachebeta.xbe disassembly */

void contrails_initialize(void)
{
  contrail_data = game_state_data_new("contrail", 0x100, 0x44);
  contrail_point_data = game_state_data_new("contrail point", 0x400, 0x38);
  if (!contrail_data || !contrail_point_data) {
    contrail_data = NULL;
    contrail_point_data = NULL;
    error(0, "couldn't allocate contrail globals");
  }
}

void contrails_initialize_for_new_map(void)
{
  data_make_valid(contrail_data);
  data_make_valid(contrail_point_data);
}

void contrails_dispose_from_old_map(void)
{
  data_make_invalid(contrail_point_data);
  data_make_invalid(contrail_data);
}

void contrails_dispose(void)
{
  if (contrail_point_data) {
    contrail_point_data = NULL;
  }
  if (contrail_data) {
    contrail_data = NULL;
  }
}
