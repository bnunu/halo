/* re-implemented from cachebeta.xbe disassembly */

#define NUMBER_OF_COLLISION_TIME_PERIODS 3

void collision_log_initialize(void)
{
  csmemset(collision_log_periods, 0, sizeof(collision_log_periods));
  assert_halt_msg(
    global_current_collision_user_depth < 32,
    "global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH");
  word_5a8c80[global_current_collision_user_depth++] = 0;
}

/* private in the target (0x14d080, si-register calling convention);
   reimplemented as a static helper */
static void collision_log_begin_period(__int16 time_period)
{
  assert_halt_msg(collision_usage_current_period == -1,
                  "collision_usage_current_period == NONE");
  assert_halt_msg(
    time_period >= 0 && time_period < NUMBER_OF_COLLISION_TIME_PERIODS,
    "(time_period >= 0) && (time_period < NUMBER_OF_COLLISION_TIME_PERIODS)");
  csmemset(collision_log_current, 0, sizeof(collision_log_current));
  collision_usage_current_period = time_period;
}

void collision_log_continue_period(__int16 time_period)
{
  collision_log_begin_period(time_period);
}

void collision_log_end_period(void)
{
  assert_halt_msg(
    collision_usage_current_period >= 0 &&
      collision_usage_current_period < NUMBER_OF_COLLISION_TIME_PERIODS,
    "(collision_usage_current_period >= 0) && (collision_usage_current_period "
    "< NUMBER_OF_COLLISION_TIME_PERIODS)");
  collision_log_current[0] = 1;
  csmemcpy(collision_log_periods[collision_usage_current_period],
           collision_log_current, sizeof(collision_log_current));
  collision_usage_current_period = -1;
}
