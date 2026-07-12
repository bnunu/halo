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
