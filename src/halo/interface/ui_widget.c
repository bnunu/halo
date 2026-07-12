void ui_widgets_disable_pause_game(int duration_ticks)
{
  assert_halt(duration_ticks >= 0);
  dword_46CC44 = duration_ticks;
}

void ui_widgets_safe_to_load(bool a1)
{
}

void display_error_when_main_menu_loaded(int16_t error)
{
  if (word_46cc48 == -1) {
    word_46cc48 = error;
  }
}
