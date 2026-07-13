/* re-implemented from cachebeta.xbe disassembly */

void telnet_console_dispose(void)
{
  if (telnet_console_globals.initialized) {
    if (telnet_console_globals.unk_0) {
      transport_endpoint_delete(telnet_console_globals.unk_0);
    }
    if (telnet_console_globals.unk_4) {
      transport_endpoint_delete(telnet_console_globals.unk_4);
    }
  }
  csmemset(&telnet_console_globals, 0, sizeof(telnet_console_globals));
}
