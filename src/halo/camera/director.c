/* Auto-drafted inert stub re-implementations (target = bare ret). */

void director_dispose(void)
{
}

void director_initialize(void)
{
  director_globals = game_state_malloc("director scripting", NULL, 4);
  *(char *)director_globals = 0;
}

void director_dispose_from_old_map(void)
{
  for (__int16 local_player_index = 0;
       local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
       local_player_index++) {
    assert_halt_msg(local_player_index >= 0 &&
                      local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS,
                    "local_player_index>=0 && "
                    "local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS");
    director_players[local_player_index].unk_0 = 0;
    director_players[local_player_index].unk_bc = 1.0f;
    director_players[local_player_index].unk_b8 = 0;
  }
  *(char *)director_globals = 0;
}
