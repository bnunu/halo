void players_initialize(void)
{
  player_data = game_state_data_new("players", 16, sizeof(player_data_t));
  team_data = game_state_data_new("teams", 16, sizeof(team_data_t));
  players_globals = (players_globals_t *)game_state_malloc(
    "players globals", NULL, sizeof(players_globals_t));
  csmemset(players_globals->local_players, -1,
           sizeof(players_globals->local_players));
  players_globals->unk_0.value = -1;
  players_globals->local_player_count = 0;
  player_control_globals = (player_control_globals_t *)game_state_malloc(
    "player control globals", 0, sizeof(player_control_globals_t));
}

void players_dispose_from_old_map(void)
{
  data_make_invalid(player_data);
  data_make_invalid(team_data);
}

void players_dispose(void)
{
  if (player_data) {
    player_data = NULL;
  }
  if (team_data) {
    team_data = NULL;
  }
  if (players_globals) {
    players_globals = NULL;
  }
}

__int16 local_player_count(void)
{
  return players_globals->local_player_count;
}

__int16 local_player_get_next(__int16 player_index)
{
  __int16 result = -1;
  for (__int16 i = 0; i < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; i++) {
    if (players_globals->local_players[i].value != -1 && i > player_index &&
        (i < result || result == -1)) {
      result = i;
    }
  }
  return result;
}
