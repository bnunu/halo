/* Auto-drafted inert stub re-implementations (target = bare ret). */

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
