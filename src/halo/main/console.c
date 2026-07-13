/* Auto-drafted inert stub re-implementations (target = bare ret). */

void console_initialize(void)
{
  console_color[0] = flt_31f9b8[0];
  console_color[1] = flt_31f9b8[1];
  console_color[2] = flt_31f9b8[2];
  console_color[3] = flt_31f9b8[3];
  csstrcpy(console_prompt_text, "halo( ");
  byte_46d018 = 0;
  word_46d91e = -1;
  word_46d91c = 0;
  word_46d920 = -1;
}

void console_initialize_for_new_map(void)
{
}

void console_dispose(void)
{
  if (byte_46cf60) {
    terminal_gets_end(&byte_46cf64);
    byte_46cf60 = 0;
  }
}
