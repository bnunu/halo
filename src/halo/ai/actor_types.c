/* re-implemented from cachebeta.xbe disassembly */

#define NUMBER_OF_ACTOR_TYPES 16
#define NUMBER_OF_ACTOR_PURSUIT_SETTINGS 3

/* the original (0x3a600, inlined from actor_type_definitions.h) takes
   actor_type in ax and cannot be redirect-patched; reimplemented as a
   static helper for our internal callers */
static actor_type_definition_t *actor_type_definition_get(int16_t actor_type)
{
  assert_halt_msg(actor_type >= 0 && actor_type < NUMBER_OF_ACTOR_TYPES,
                  "actor_type>=0 && actor_type<NUMBER_OF_ACTOR_TYPES");
  assert_halt_msg(actor_type_definitions[actor_type],
                  "actor_type_definitions[actor_type]");
  assert_halt_msg(actor_type_definitions[actor_type]->name,
                  "actor_type_definitions[actor_type]->name");
  assert_halt_msg(actor_type_definitions[actor_type]->decide_action,
                  "actor_type_definitions[actor_type]->decide_action");
  assert_halt_msg(
      actor_type_definitions[actor_type]->when_to_search_at_target <
          NUMBER_OF_ACTOR_PURSUIT_SETTINGS,
      "actor_type_definitions[actor_type]->when_to_search_at_target < "
      "NUMBER_OF_ACTOR_PURSUIT_SETTINGS");
  assert_halt_msg(actor_type_definitions[actor_type]->when_to_pursue <
                      NUMBER_OF_ACTOR_PURSUIT_SETTINGS,
                  "actor_type_definitions[actor_type]->when_to_pursue < "
                  "NUMBER_OF_ACTOR_PURSUIT_SETTINGS");
  assert_halt_msg(actor_type_definitions[actor_type]->when_to_search_pursuit <
                      NUMBER_OF_ACTOR_PURSUIT_SETTINGS,
                  "actor_type_definitions[actor_type]->when_to_search_pursuit "
                  "< NUMBER_OF_ACTOR_PURSUIT_SETTINGS");
  return actor_type_definitions[actor_type];
}

void actor_types_initialize(void)
{
  for (int16_t actor_type = 0; actor_type < NUMBER_OF_ACTOR_TYPES;
       actor_type++) {
    actor_type_definition_get(actor_type);
  }
}
