/* Auto-drafted inert stub re-implementations (target = bare ret). */

void director_dispose(void)
{
}

void director_initialize(void)
{
  director_globals = game_state_malloc("director scripting", NULL, 4);
  *(char *)director_globals = 0;
}
