__int16 player_ui_get_single_player_local_player_controller(
  __int16 local_player_index)
{
  assert_halt_msg(local_player_index >= 0 && local_player_index < 4, "invalid local player index");
  return word_46BFC4[(__int16)local_player_index];
}

void player_ui_dispose(void)
{
  csmemset(player_ui_globals, 0, 0x230);
}

void player_ui_initialize(void)
{
  int i;

  csmemset(player_ui_globals, 0, 0x230);
  for (i = 0; i < 4; i++) {
    char *profile = &player_ui_globals[(__int16)i * 0x38];
    assert_halt_msg(profile, "profile");
    csmemset(profile, 0, 0x30);
    *(__int16 *)(profile + 0x18) = -1;
    profile[0x28] = 0;
    profile[0x29] = 0;
    *(int *)(profile + 0x30) = -1;
    word_46BFC4[(__int16)i] = -1;
  }
  *(int *)&player_ui_globals[0x158] = -1;
  player_ui_globals[0x22c] = 1;
}
