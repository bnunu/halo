/* Auto-drafted inert stub re-implementations (target = bare ret). */

void breakable_surfaces_initialize(void)
{
  assert_halt_msg(!breakable_surface_globals, "!globals");
  breakable_surface_globals =
    game_state_malloc("breakable surface globals", NULL, 0x4204);
}

void breakable_surfaces_dispose(void)
{
}

void breakable_surfaces_dispose_from_old_map(void)
{
}
