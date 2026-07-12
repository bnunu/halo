/* re-implemented from cachebeta.xbe disassembly */

void contrails_dispose_from_old_map(void)
{
  data_make_invalid(contrail_data);
  data_make_invalid(contrail_data2);
}

void contrails_dispose(void)
{
  if (contrail_data) {
    contrail_data = NULL;
  }
  if (contrail_data2) {
    contrail_data2 = NULL;
  }
}
