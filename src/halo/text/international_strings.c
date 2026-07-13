/* re-implemented from cachebeta.xbe disassembly */

/* 0x14-byte string reference element of a 'ustr' unicode string list */
typedef struct {
  int size;
  int unk_4;
  int unk_8;
  wchar_t *pointer;
  int unk_10;
} unicode_string_reference_t;

wchar_t *unicode_string_list_get_string(int tag_handle, __int16 index)
{
  if (tag_handle != -1) {
    int *list = tag_get(0x75737472 /* 'ustr' */, tag_handle);
    if (index >= 0 && index < *list) {
      unicode_string_reference_t *reference = tag_block_get_element(
        list, index, sizeof(unicode_string_reference_t));
      if (reference->size > 0) {
        wchar_t *string = reference->pointer;
        string[reference->size / sizeof(wchar_t) - 1] = 0;
        return string;
      }
    }
  }
  return L"<missing string>";
}
