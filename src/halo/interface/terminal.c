/* re-implemented from cachebeta.xbe disassembly */

/* 0x124-byte "terminal output" line datum */
typedef struct {
  __int16 identifier;
  __int16 unk_2;
  int previous; /* 0x4 */
  int next;     /* 0x8 */
  char unk_c[0x114];
  int age;      /* 0x120, update ticks since printed */
} terminal_output_t;

void terminal_initialize(void)
{
  terminal_output_data = data_new("terminal output", 0x20,
                                  sizeof(terminal_output_t));
  byte_46c404 = 1;
  data_make_valid(terminal_output_data);
  dword_46c414 = NULL;
  dword_46c40c = -1;
  dword_46c410 = -1;
}

void terminal_gets_end(void *state)
{
  if (state == dword_46c414) {
    dword_46c414 = NULL;
  }
}

/* private in the target (0xe3410, edi-register calling convention);
   reimplemented as a static helper */
static void terminal_output_delete_line(int line_index)
{
  terminal_output_t *line = datum_get(terminal_output_data, line_index);
  if (line->next != -1) {
    terminal_output_t *next = datum_get(terminal_output_data, line->next);
    next->previous = line->previous;
  } else {
    dword_46c410 = line->previous;
  }
  if (line->previous != -1) {
    terminal_output_t *previous = datum_get(terminal_output_data,
                                            line->previous);
    previous->next = line->next;
  } else {
    dword_46c40c = line->next;
  }
  datum_delete(terminal_output_data, line_index);
}

void terminal_output_update(void)
{
  int line_index = dword_46c40c;
  while (line_index != -1) {
    terminal_output_t *line = datum_get(terminal_output_data, line_index);
    int next = line->next;
    if (++line->age > 150) {
      terminal_output_delete_line(line_index);
    }
    line_index = next;
  }
}

bool terminal_update(void)
{
  bool result = false;
  if (byte_46c404) {
    result = terminal_gets_update();
    if (!console_is_active()) {
      terminal_output_update();
    }
  }
  return result;
}
