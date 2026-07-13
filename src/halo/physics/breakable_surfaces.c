/* Auto-drafted inert stub re-implementations (target = bare ret). */

void breakable_surfaces_initialize(void)
{
  assert_halt_msg(!breakable_surface_globals, "!globals");
  breakable_surface_globals = (breakable_surface_globals_t *)game_state_malloc(
    "breakable surface globals", NULL, sizeof(breakable_surface_globals_t));
}

void breakable_surfaces_initialize_for_new_map(void)
{
  assert_halt_msg(breakable_surface_globals, "globals");
  breakable_surface_globals->unk_0 = 1;
  for (int i = 0; i < 16; i++) {
    csmemset(breakable_surface_globals->unk_1[i], -1,
             sizeof(breakable_surface_globals->unk_1[i]));
    for (int j = 0; j < 0x100; j++) {
      breakable_surface_globals->unk_204[i][j] = 1.0f;
    }
  }
}

void breakable_surfaces_dispose(void)
{
}

void breakable_surfaces_dispose_from_old_map(void)
{
}
