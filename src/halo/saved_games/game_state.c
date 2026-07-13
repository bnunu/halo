data_t *game_state_data_new(char *name, __int16 maximum_count, __int16 size)
{
  data_t *data; // esi
  int allocation_size; // [esp-Ch] [ebp-18h]

  allocation_size = data_allocation_size(maximum_count, size);
  data = (data_t *)game_state_malloc(name, "data array", allocation_size);
  data_initialize(data, name, maximum_count, size);
  return data;
}

void game_state_dispose_from_old_map(void)
{
}

void game_state_initialize_for_new_map(void)
{
  xbox_game_state_globals.unk_4 = 1;
  xbox_game_state_globals.unk_5 = 0;
  xbox_game_state_globals.unk_8 = 0xffffffff;
  csmemset(xbox_game_state_globals.header, 0, sizeof(game_state_header_t));
  csstrcpy(xbox_game_state_globals.header->map_path, tag_get_name(dword_326a08));
  csstrcpy(xbox_game_state_globals.header->build_version, "01.10.12.2276");
  xbox_game_state_globals.header->player_spawn_count = player_spawn_count;
  xbox_game_state_globals.header->difficulty = game_difficulty_level();
  xbox_game_state_globals.header->unk_128 = cache_file_get_checksum();
  xbox_game_state_globals.header->unk_0 = xbox_game_state_globals.unk_0;
}
