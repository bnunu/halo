/* re-implemented from cachebeta.xbe disassembly */

void game_sound_dispose(void)
{
  if (game_sound_globals) {
    game_sound_globals = NULL;
  }
}
