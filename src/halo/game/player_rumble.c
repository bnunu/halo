/* Auto-drafted inert stub re-implementations (target = bare ret). */

void rumble_dispose(void)
{
}

void rumble_initialize(void)
{
  rumble_globals = game_state_malloc("rumble", NULL, 0x82c);
}

void rumble_initialize_for_new_map(void)
{
  csmemset(rumble_globals, 0, 0x82c);
}
