/* re-implemented from cachebeta.xbe disassembly */

void ai_communication_initialize(void)
{
  int16_t dialogue_count = 0;
  do {
    dialogue_count++;
  } while (dialogue_definitions[dialogue_count].unk_0 != -1);
  word_331f08 = dialogue_count;
  if (!dialogue_status_table) {
    dialogue_status_table =
        game_state_malloc("ai communication dialogue", NULL, dialogue_count * 16);
    assert_halt_msg(dialogue_status_table,
                    "ai_communication_initialize: unable to allocate comm "
                    "dialogue status table");
  }

  int16_t reply_count = 0;
  do {
    reply_count++;
  } while (reply_definitions[reply_count].unk_0 != -1);
  word_331f10 = reply_count;
  if (!reply_status_table) {
    reply_status_table =
        game_state_malloc("ai communication replies", NULL, reply_count * 16);
    assert_halt_msg(reply_status_table,
                    "ai_communication_initialize: unable to allocate comm "
                    "reply status table");
  }

  for (int16_t vocalization = 0; vocalization < 0x39; vocalization++) {
    word_632500[vocalization] = -1;
    for (int16_t index = 0; dialogue_definitions[index].unk_0 != -1; index++) {
      if (dialogue_definitions[index].unk_0 == vocalization) {
        word_632500[vocalization] = index;
        break;
      }
    }
  }

  conversation_data = game_state_data_new("ai conversation", 8, 0x64);
  assert_halt_msg(conversation_data, "conversation_data");
}

void ai_communication_dispose(void)
{
}

void ai_communication_dispose_from_old_map(void)
{
  data_make_invalid(conversation_data);
}
