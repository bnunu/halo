bool cinematic_can_be_skipped(void)
{
  return cinematic_globals->can_be_skipped;
}

void cinematic_dispose(void)
{
}

bool cinematic_in_progress(void)
{
  return cinematic_globals->unk_0[9];
}

void cinematic_dispose_from_old_map(void)
{
  cinematic_globals->unk_0[8] = 0;
  cinematic_globals->unk_0[9] = 0;
}

void cinematic_initialize_for_new_map(void)
{
  csmemset(cinematic_globals, 0, 0x1c);
  csmemset((char *)cinematic_globals + 0xc, -1, 0x10);
}
