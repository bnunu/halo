/* re-implemented from cachebeta.xbe disassembly */

void update_server_delete(void)
{
  if (update_server_globals.queues) {
    data_dispose(update_server_globals.queues);
    update_server_globals.queues = NULL;
  }
  update_server_globals.initialized = 0;
  update_server_globals.unk_4 = 0;
  if (dword_45b260) {
    data_dispose(dword_45b260);
    dword_45b260 = NULL;
  }
  dword_45b1d4 = 0;
  byte_45b1d0 = 0;
  dword_45b1d8 = -1;
}
