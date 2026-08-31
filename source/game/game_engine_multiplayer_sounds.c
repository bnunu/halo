/*
GAME_ENGINE_MULTIPLAYER_SOUNDS.C

symbols in this file:
000A1460 0060:
	__game_engine_play_multiplayer_sound (0000)
000A14C0 0020:
	_push_queued_sound (0000)
000A14E0 0050:
	_game_engine_update_multiplayer_sound (0000)
000A1530 0080:
	_get_sound_length_in_ticks (0000)
000A15B0 0050:
	_game_engine_play_multiplayer_sound (0000)
000A1600 0030:
	_game_engine_intialize_queued_sounds (0000)
002DE530 002b:
	_sound_is_queueable (0000)
0043EB78 002c:
	_mp_sound_queue_count (0000)
	_mp_sound_queue (0004)
*/

/* ---------- headers */

#include "cseries.h"

#include "game/game_globals.h"
#include "objects/objects.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"

/* ---------- constants */

enum multiplayer_information_sound
{
	_multiplayer_sound_play_ball,
	_multiplayer_sound_game_over,
	_multiplayer_sound_one_minute_to_win,
	_multiplayer_sound_30_seconds_to_win,
	_multiplayer_sound_red_team_minute_to_win,
	_multiplayer_sound_red_team_30_seconds_to_win,
	_multiplayer_sound_blue_team_minute_to_win,
	_multiplayer_sound_blue_team_30_seconds_to_win,
	_multiplayer_sound_blue_team_has_the_flag,
	_multiplayer_sound_blue_team_flag_returned,
	_multiplayer_sound_blue_team_score,
	_multiplayer_sound_red_team_has_the_flag,
	_multiplayer_sound_red_team_flag_returned,
	_multiplayer_sound_red_team_score,
	_multiplayer_sound_double_kill,
	_multiplayer_sound_triple_kill,
	_multiplayer_sound_killtacular,
	_multiplayer_sound_running_riot,
	_multiplayer_sound_killing_spree,
	_multiplayer_sound_oddball,
	_multiplayer_sound_race,
	_multiplayer_sound_slayer,
	_multiplayer_sound_capture_the_flag,
	_multiplayer_sound_warthog,
	_multiplayer_sound_ghost,
	_multiplayer_sound_scorpion,
	_multiplayer_sound_countdown_timer,
	_multiplayer_sound_teleporter_activate,
	_multiplayer_sound_flag_failure,
	_multiplayer_sound_countdown_for_respawn,
	_multiplayer_sound_hill_move,
	_multiplayer_sound_player_respawn,
	_multiplayer_sound_team_king_of_the_hill,
	_multiplayer_sound_team_oddball,
	_multiplayer_sound_team_race,
	_multiplayer_sound_team_slayer,
	_multiplayer_sound_king_of_the_hill,
	_multiplayer_sound_blue_team_ctf,
	_multiplayer_sound_red_team_ctf,
	_multiplayer_sound_hill_contested,
	_multiplayer_sound_hill_controlled,
	_multiplayer_sound_hill_occupied,
	_multiplayer_sound_countdown_timer_end,
	_multiplayer_sound_ting,
	NUMBER_OF_MULTIPLAYER_INFORMATION_SOUNDS
};

enum
{
	MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS = 5,
	MULTIPLAYER_SOUND_QUEUE_INITIAL_DELAY_TICKS = 2 * TICKS_PER_SECOND,
};

/* ---------- macros */

/* ---------- structures */

struct queued_mp_sound
{
	long sound_index;
	long delay_ticks;
};

struct multiplayer_sound_queue
{
	long count;
	struct queued_mp_sound sounds[MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS];
};

struct game_globals_multiplayer_sound_view
{
	byte unused[0x164];
	struct tag_block multiplayer_information;
};

typedef char verify_multiplayer_sound_queue_size[sizeof(struct multiplayer_sound_queue) == 0x2C ? 1 : -1];
typedef char verify_game_globals_multiplayer_information_offset[
	offsetof(struct game_globals_multiplayer_sound_view, multiplayer_information) == 0x164 ? 1 : -1];

/* ---------- prototypes */

static void _game_engine_play_multiplayer_sound(
	long sound_index);
static void push_queued_sound(
	long sound_index,
	long delay_ticks);
static long get_sound_length_in_ticks(
	long sound_index);

/* ---------- globals */

static boolean sound_is_queueable[_multiplayer_sound_ting] =
{
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE,
	FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, FALSE,
};

/* the queue itself has external linkage in january: adding `static` to
mp_sound_queue_count lets VC7 prove the count survives the
_game_engine_play_multiplayer_sound() call and drops the reload in
game_engine_update_multiplayer_sound, and adding it to mp_sound_queue turns that
array's relocations section-relative.  sound_is_queueable is genuinely file-local. */
long mp_sound_queue_count = 0;
struct queued_mp_sound mp_sound_queue[
	MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS] = { 0 };

/* ---------- public code */

static void _game_engine_play_multiplayer_sound(
	long sound_index)
{
	struct game_globals_multiplayer_sound_view *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct tag_reference *sound;

	global_scenario_get();
	game_globals = (struct game_globals_multiplayer_sound_view *)scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);
	if (multiplayer_information && sound_index < multiplayer_information->sounds.count)
	{
		sound = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->sounds,
			sound_index,
			struct tag_reference);
		if (sound && sound->index != NONE)
			unspatialized_impulse_sound_new(sound->index, 1.0f);
	}

	return;
}

static void push_queued_sound(
	long sound_index,
	long delay_ticks)
{
	long queue_index = mp_sound_queue_count;

	if (queue_index < MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS)
	{
		mp_sound_queue[queue_index].sound_index = sound_index;
		mp_sound_queue[queue_index].delay_ticks = delay_ticks;
		mp_sound_queue_count++;
	}

	return;
}

void game_engine_update_multiplayer_sound(
	void)
{
	long i;
	long queue_count = mp_sound_queue_count;

	if (queue_count && --mp_sound_queue[0].delay_ticks == 0)
	{
		for (i = 1; i < queue_count; i++)
			mp_sound_queue[i - 1] = mp_sound_queue[i];

		queue_count--;
		mp_sound_queue_count = queue_count;
		if (queue_count)
			_game_engine_play_multiplayer_sound(mp_sound_queue[0].sound_index);
	}

	return;
}

static long get_sound_length_in_ticks(
	long sound_index)
{
	struct game_globals_multiplayer_sound_view *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct tag_reference *sound;

	global_scenario_get();
	game_globals = (struct game_globals_multiplayer_sound_view *)scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);
	if (!multiplayer_information)
		return 0;
	if (sound_index >= multiplayer_information->sounds.count)
		return 0;

	sound = TAG_BLOCK_GET_ELEMENT(
		&multiplayer_information->sounds,
		sound_index,
		struct tag_reference);
	if (!sound)
		return 0;
	if (sound->index == NONE)
		return 0;

	return (long)(sound_definition_get(sound->index)->longest_permutation_length * TICKS_PER_SECOND) / 1000;
}

void game_engine_play_multiplayer_sound(
	long sound_index)
{
	if (sound_is_queueable[sound_index])
	{
		push_queued_sound(
			sound_index,
			get_sound_length_in_ticks(sound_index) + 5);
		if (mp_sound_queue_count == 1)
			_game_engine_play_multiplayer_sound(sound_index);
	}
	else
	{
		_game_engine_play_multiplayer_sound(sound_index);
	}

	return;
}

void game_engine_intialize_queued_sounds(
	void)
{
	csmemset(
		mp_sound_queue,
		0,
		sizeof(mp_sound_queue));
	mp_sound_queue_count = 1;
	mp_sound_queue[0].sound_index = NONE;
	mp_sound_queue[0].delay_ticks = MULTIPLAYER_SOUND_QUEUE_INITIAL_DELAY_TICKS;

	return;
}

/* ---------- private code */
