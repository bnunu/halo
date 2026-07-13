/* re-implemented from cachebeta.xbe disassembly */

#define NUMBER_OF_SOUND_CLASSES 51

void sound_classes_dispose_from_old_map(void)
{
}

void sound_classes_dispose(void)
{
  sound_class_data = NULL;
}

void sound_classes_initialize(void)
{
  sound_class_data = (sound_class_t *)game_state_malloc(
    "sound classes", NULL, NUMBER_OF_SOUND_CLASSES * sizeof(sound_class_t));
}

void sound_classes_initialize_for_new_map(void)
{
  for (__int16 index = 0; index < NUMBER_OF_SOUND_CLASSES; index++) {
    assert_halt_msg(index >= 0 && index < NUMBER_OF_SOUND_CLASSES,
                    "index>=0 && index<NUMBER_OF_SOUND_CLASSES");
    assert_halt(sound_class_data);
    sound_class_data[index].unk_4 = 1.0f;
    sound_class_data[index].unk_0 = 1.0f;
    sound_class_data[index].unk_8 = 0;
  }
}
