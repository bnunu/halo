void display_assert(const char *reason, const char *filepath, int lineno,
                    bool halt)
{
  if (halt) {
    stack_walk(0);
  }
  error(2, "EXCEPTION %s in %s,#%d: %s", halt ? "halt" : "warn", filepath,
        lineno, reason ? reason : "<no reason given>");
}

#ifdef strlen
#undef strlen
#endif

int csstrlen(const char *s1)
{
  int size;

  assert_halt(s1);
  size = strlen(s1);
  assert_halt(size >= 0 && size < MAXIMUM_STRING_SIZE);

  return size;
}

void *csstrncpy(char *destination, const char *source, size_t size)
{
  assert_halt_msg(destination && source, "s1 && s2");
  assert_halt_msg(size < MAXIMUM_STRING_SIZE,
                  "size>=0 && size<MAXIMUM_STRING_SIZE");
  return strncpy(destination, source, size);
}
