/* re-implemented from cachebeta.xbe disassembly */

/* NOTE: at 0x12a1e0 the "client" dispose deletes the object at 0x46e8bc via
   network_game_server_delete, and at 0x12a2a0 the "server" dispose deletes
   0x46e8c0 via network_game_client_delete — the kb.json names may be swapped
   relative to the objects they own; implemented faithfully per address. */

void dispose_global_network_game_client(void)
{
  if (dword_46e8bc) {
    network_game_server_delete(dword_46e8bc);
    dword_46e8bc = NULL;
    byte_46e8c5 = 0;
  }
}

void dispose_global_network_game_server(void)
{
  if (dword_46e8c0) {
    network_game_client_delete(dword_46e8c0);
    dword_46e8c0 = NULL;
  }
  byte_46e8c6 = 0;
}

void network_game_abort(void)
{
  byte_46e8c6 = 1;
}
