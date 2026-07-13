/* re-implemented from cachebeta.xbe disassembly */

void game_sound_initialize(void)
{
  object_looping_sound_data =
    game_state_data_new("object looping sounds", 0x400, 0x34);
  game_sound_globals = game_state_malloc("game sound globals", NULL, 8);
}

void game_sound_initialize_for_new_map(void)
{
  if (object_looping_sound_data) {
    data_make_valid(object_looping_sound_data);
    game_sound_globals->unk_4 = 0xffffffff;
    game_sound_globals->unk_0 = 0;
  }
}

void game_sound_dispose_from_old_map(void)
{
  if (object_looping_sound_data && object_looping_sound_data->valid) {
    game_sound_stop_scripted_looping_sounds();
    data_make_invalid(object_looping_sound_data);
  }
}

void game_sound_dispose(void)
{
  if (object_looping_sound_data) {
    object_looping_sound_data = NULL;
  }
}
