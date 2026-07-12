/* re-implemented from cachebeta.xbe disassembly */

void game_sound_initialize(void)
{
  object_looping_sound_data =
    game_state_data_new("object looping sounds", 0x400, 0x34);
  game_sound_globals = game_state_malloc("game sound globals", NULL, 8);
}

void game_sound_dispose(void)
{
  if (object_looping_sound_data) {
    object_looping_sound_data = NULL;
  }
}
