/* re-implemented from cachebeta.xbe disassembly */

/* private in the target (0xd5070, esi-register calling convention);
   reimplemented as a static helper */
static hud_scripted_message_t *
hud_scripted_message_slot_get(hud_player_messaging_t *block, char char_code,
                              int id)
{
  hud_scripted_message_t *result = NULL;
  int oldest_time = 0x7fffffff;
  __int16 oldest_index = 0;
  for (unsigned __int16 index = 0; index < 4; index++) {
    hud_scripted_message_t *message = &block->scripted_messages[index];
    if ((id != -1 && id == message->unk_84 && char_code == message->unk_8a) ||
        !message->unk_82) {
      result = message;
      if (id == -1 || id == message->unk_84) {
        break;
      }
    } else if (message->time < oldest_time) {
      oldest_time = message->time;
      oldest_index = index;
    }
  }
  if (!result) {
    result = &block->scripted_messages[oldest_index];
  }
  return result;
}

void hud_print_message(__int16 player, wchar_t *message)
{
  if (player == -1) {
    return;
  }
  hud_player_messaging_t *block = &hud_messaging_state->players[player];
  hud_scripted_message_t *msg = hud_scripted_message_slot_get(block, 0, -1);
  ustrncpy(msg->text, message, 0x3f);
  msg->unk_84 = -1;
  msg->time = game_time_get();
  msg->unk_82 = 1;
  msg->unk_83 = hud_messaging_state->unk_1185++;
  block->unk_45e = 0;
}

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
