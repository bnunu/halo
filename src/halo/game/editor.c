/* editor_stubs — re-implementations of the (empty/stub) editor entry points.
 * In the 2276 build these are inert: the level editor is compiled out, so the
 * initialize/dispose entry points are empty and the predicates report false. */

bool game_in_editor(void)
{
  return false;
}

bool editor_should_exit(void)
{
  return false;
}

void editor_initialize(void)
{
}

void editor_initialize_for_new_map(void)
{
}

void editor_dispose(void)
{
}

void editor_dispose_from_old_map(void)
{
}
