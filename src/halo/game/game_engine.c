
bool game_engine_running(void)
{
  return game_engine_globals != NULL;
}

void game_engine_dispose(void)
{
  if (game_engine_globals) {
    if (game_engine_globals->dispose) {
      game_engine_globals->dispose();
    }
    game_engine_globals = NULL;
  }
}

bool game_engine_force_single_screen(void)
{
  if (!game_engine_globals) {
    return false;
  }
  return dword_5aa730 >= 2 && dword_5aa730 <= 3;
}
