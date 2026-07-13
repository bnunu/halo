/* re-implemented from cachebeta.xbe disassembly */

void bink_playback_initialize(void)
{
  csmemset(&bink_playback_globals, 0, sizeof(bink_playback_globals));
  RADSetMemory(bink_playback_memory_allocate, bink_playback_memory_free);
  bink_playback_globals.initialized = 1;
}

void bink_playback_dispose(void)
{
  if (bink_playback_globals.initialized) {
    bink_playback_stop();
    csmemset(&bink_playback_globals, 0, sizeof(bink_playback_globals));
  }
}
