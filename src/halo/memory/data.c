int data_allocation_size(__int16 count, __int16 size)
{
  return size * count + sizeof(data_t);
}

void data_initialize(data_t *data, char *name, __int16 maximum_count,
                     __int16 size)
{
  assert_halt(maximum_count > 0);
  assert_halt(size > 0);
  assert_halt(name);
  assert_halt(data);

  csmemset(data, 0, sizeof(*data));
  csstrncpy(data->name, name, sizeof(data->name) - 1);
  data->maximum_count = maximum_count;
  data->size = size;
  data->magic = 0x64407440;
  data->data = &data[1];
  data->valid = 0;
}

data_t *data_new(char *name, __int16 maximum_count, __int16 size)
{
  data_t *data =
    debug_malloc(size * maximum_count + sizeof(data_t), 0, __FILE__, __LINE__);
  if (data) {
    data_initialize(data, name, maximum_count, size);
  }
  return data;
}

void data_dispose(data_t *data)
{
  data_verify(data);
  csmemset(data, 0, sizeof(*data));
  debug_free(data, __FILE__, __LINE__);
}

void data_make_invalid(data_t *data)
{
  data_verify(data);
  data->valid = 0;
}

void data_delete_all(data_t *data)
{
  data_verify(data);
  assert_halt(data->valid);
  data->current_count = 0;
  data->actual_count = 0;
  data->next_index = 0;
  csstrncpy((char *)&data->next_identifier, data->name, 2);
  data->next_identifier |= 0x8000;
  for (__int16 i = 0; i < data->maximum_count; i++) {
    *(int16_t *)((char *)data->data + data->size * i) = 0;
  }
}

void data_make_valid(data_t *data)
{
  data_verify(data);
  data->valid = 1;
  data_delete_all(data);
}

void data_iterator_new(data_iter_t *iter, data_t *data)
{
  data_verify(data);
  assert_halt(data->valid);

  iter->data = data;
  iter->cookie = (unsigned int)data ^ 0x69746572;
  iter->index = 0;
  iter->datum_handle = -1;
}

void *data_iterator_next(data_iter_t *iterator)
{
  int16_t index; // cx
  void *result; // eax
  int handle; // edx
  size_t size; // [esp+14h] [ebp+8h]

  assert_halt_msg(iterator->cookie == ((int)iterator->data->name ^ 0x69746572),
                  "uninitialized iterator passed to iterator_next()");
  data_verify(iterator->data);
  assert_halt(iterator->data->valid);

  index = iterator->index;
  size = iterator->data->size;
  result = (char *)iterator->data->data + iterator->data->size * index;
  if (index >= iterator->data->current_count) {
    iterator->index = index;
    return 0;
  } else {
    while (1) {
      handle = index++ | (*(__int16 *)result << 16);
      if (*(_WORD *)result)
        break;
      result = (char *)result + size;
      if (index >= iterator->data->current_count) {
        iterator->index = index;
        return 0;
      }
    }
    iterator->datum_handle = handle;
    iterator->index = index;
  }
  return result;
}
