/* re-implemented from cachebeta.xbe disassembly */

wchar_t *ustrncpy(wchar_t *dest, wchar_t *src, size_t count)
{
  assert_halt_msg(dest && src, "dest && src");
  assert_halt_msg(count < 0x8000,
                  "(count >= 0) && (count < MAXIMUM_STRING_SIZE)");
  return wcsncpy(dest, src, count);
}
