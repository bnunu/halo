/* Auto-drafted inert stub re-implementations (target = bare ret). */

void sound_classes_dispose_from_old_map(void)
{
}

void sound_classes_dispose(void)
{
  dword_50548c = 0;
}

void sound_classes_initialize(void)
{
  dword_50548c = (int)game_state_malloc("sound classes", NULL, 0x264);
}
