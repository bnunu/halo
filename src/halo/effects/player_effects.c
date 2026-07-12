/* re-implemented from cachebeta.xbe disassembly */

void player_effect_initialize(void)
{
  player_effect_globals = (player_effect_globals_t *)game_state_malloc(
    "player effects", NULL, sizeof(player_effect_globals_t));
  assert_halt(player_effect_globals);
}

void player_effect_initialize_for_new_map(void)
{
  csmemset(player_effect_globals, 0, sizeof(player_effect_globals_t));
  player_effect_globals->unk_3c0 = -1;
  player_effect_globals->unk_3e8 = game_time_get();
}

void player_effect_dispose(void)
{
}

void player_effect_dispose_from_old_map(void)
{
}
