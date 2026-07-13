/* re-implemented from cachebeta.xbe disassembly */

void scripted_hud_messages_clear(void)
{
  for (int player = 0; player < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; player++) {
    for (int message = 0; message < 4; message++) {
      hud_messaging_state->players[player]
        .scripted_messages[message]
        .unk_82 = 0;
    }
  }
}
