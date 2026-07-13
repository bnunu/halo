/* Auto-drafted inert stub re-implementations (target = bare ret). */

void game_allegiance_dispose(void)
{
}

void game_allegiance_dispose_from_old_map(void)
{
}

void game_allegiance_initialize(void)
{
  game_allegiance_globals = game_state_malloc("game allegiance globals", NULL, 0xb4);
  csmemset(game_allegiance_globals, 0, 0xb4);
}

void game_allegiance_initialize_for_new_map(void)
{
  assert_halt(game_allegiance_globals);
  game_allegiance_globals->unk_0 = 0;
  csmemset(game_allegiance_globals->unk_94, 0,
           sizeof(game_allegiance_globals->unk_94));
  csmemset(game_allegiance_globals->unk_a4, 0,
           sizeof(game_allegiance_globals->unk_a4));
  for (int team = 0; team < 10; team++) {
    int bit = team * 0xb;
    game_allegiance_globals->unk_a4[bit >> 5] |= 1u << (bit & 0x1f);
  }
}
