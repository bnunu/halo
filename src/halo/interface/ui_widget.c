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

void ui_widgets_dispose(void)
{
  ui_widgets_close_all();
  if (ui_widget_globals->unk_4) {
    debug_free(ui_widget_globals->unk_4, __FILE__, __LINE__);
  }
  ui_widget_globals->unk_4 = NULL;
  ui_widget_globals->unk_8 = NULL;
  csmemset(byte_46cc20, 0, sizeof(byte_46cc20));
}

void *ui_widget_realloc(int a1, unsigned short a2, const char *a3,
                        unsigned int a4)
{
  return stack_memory_pool_realloc(ui_widget_globals, a1, a2, a3, a4);
}
