void hud_load(bool a1)
{
  __int16 v1;

  if (a1) {
    v1 = hud_globals->unk_3d8;
  } else {
    v1 = hud_globals->unk_3da;
  }
  scripted_hud_messages_clear();
  if (v1 != -1) {
    hud_print_message(local_player_get_next(-1), hud_get_item_string(v1));
  }
}

wchar_t *hud_get_item_string(int index)
{
  if (hud_globals->item_message_text.value != -1) {
    int *string_list = tag_get(0x75737472 /* 'ustr' */,
                               hud_globals->item_message_text.value);
    if (string_list && index >= 0 && index < *string_list) {
      return unicode_string_list_get_string(
        hud_globals->item_message_text.value, index);
    }
  }
  return dword_2f66bc;
}
