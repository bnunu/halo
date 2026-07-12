/* re-implemented from cachebeta.xbe disassembly */

void event_manager_dispose(void)
{
  csmemset(event_manager_globals, 0, 0x108);
}

void event_manager_initialize(void)
{
  csmemset(event_manager_globals, 0, 0x108);
  *(unsigned int *)(event_manager_globals + 4) = system_milliseconds();
  event_manager_globals[0] = 1;
}
