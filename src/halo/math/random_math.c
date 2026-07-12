/* Auto-drafted inert stub re-implementations (target = bare ret). */

void random_seed_debug_log(bool a1)
{
}

void lock_global_random_seed(void)
{
  ++dword_46e3f0;
}

void unlock_global_random_seed(void)
{
  assert_halt_msg(dword_46e3f0 > 0,
                  "unmatched call to unlock_random_seed() somewhere");
  --dword_46e3f0;
}

int *get_global_random_seed_address(void)
{
  if (game_engine_running()) {
    assert_halt_msg(
      !dword_46e3f0,
      "you should not be using global random(); use local random() instead");
  }
  return &global_random_seed;
}
