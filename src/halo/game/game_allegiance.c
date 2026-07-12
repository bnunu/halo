/* Auto-drafted inert stub re-implementations (target = bare ret). */

void game_allegiance_dispose(void)
{
}

void game_allegiance_dispose_from_old_map(void)
{
}

void game_allegiance_initialize(void)
{
  game_allegiance_globals = game_state_malloc("game allegiance globals", NULL, 0xb4);
  csmemset(game_allegiance_globals, 0, 0xb4);
}
