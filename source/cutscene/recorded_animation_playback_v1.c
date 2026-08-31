/*
RECORDED_ANIMATION_PLAYBACK_V1.C

symbols in this file:
00082910 00b0:
	_apply_animation_state (0000)
000829C0 00b0:
	_apply_aiming_speed (0000)
00082A70 00b0:
	_apply_control_flags (0000)
00082B20 00b0:
	_apply_weapon_index (0000)
00082BD0 00c0:
	_apply_throttle (0000)
00082C90 00c0:
	_apply_facing_vector (0000)
00082D50 00c0:
	_apply_aiming_vector (0000)
00082E10 00c0:
	_apply_looking_vector (0000)
00082ED0 0110:
	_apply_angle_vector (0000)
00082FE0 0100:
	_apply_multi_vector (0000)
000830E0 0020:
	_recorded_animation_initialize_event_stream_v1 (0000)
00083100 0110:
	_recorded_animation_apply_event_stream_v1 (0000)
00083210 0010:
	_byte_swap_recording_stream_v1 (0000)
00258F20 001a:
	??_C@_0BK@OHDHHHHJ@angle_vector_set_event_v1?$AA@ (0000)
00258F3C 001a:
	??_C@_0BK@LHEBCLGN@multi_vector_set_event_v1?$AA@ (0000)
00258F58 0016:
	??_C@_0BG@OPCBPKCE@throttle_set_event_v1?$AA@ (0000)
00258F70 001a:
	??_C@_0BK@HLNNIFOD@weapon_index_set_event_v1?$AA@ (0000)
00258F8C 001b:
	??_C@_0BL@PAKDCOLD@control_flags_set_event_v1?$AA@ (0000)
00258FA8 001a:
	??_C@_0BK@GKLMCKBG@aiming_speed_set_event_v1?$AA@ (0000)
00258FC4 001d:
	??_C@_0BN@LHCEPMPP@animation_state_set_event_v1?$AA@ (0000)
00258FE4 0013:
	??_C@_0BD@EKJKBAHO@animation_event_v1?$AA@ (0000)
00258FF8 0036:
	??_C@_0DG@KFHJEKLB@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
00259030 000e:
	??_C@_0O@DDIBHFNF@anim_event_v1?$AA@ (0000)
00259040 0039:
	??_C@_0DJ@OHKNEDDJ@c?3?2halo?2SOURCE?2cutscene?2recorded@ (0000)
0025907C 0033:
	??_C@_0DD@JBDMMKBH@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
002590B0 0034:
	??_C@_0DE@CNFIJCNC@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
002590E4 0033:
	??_C@_0DD@KHGFPEMK@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
00259118 002f:
	??_C@_0CP@NOOIADDJ@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
00259148 0034:
	??_C@_0DE@EJOEFJGM@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
0025917C 0034:
	??_C@_0DE@GOFIOKDN@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
002591B0 0035:
	??_C@_0DF@JGMNCGMJ@anim_event_v1?9?$DOtype?$DN?$DN_playback_v@ (0000)
002591E8 007a:
	??_C@_0HK@BLBLFAF@anim_event_v1?9?$DOtype?$DO?$DN_playback_v@ (0000)
00259268 007f:
	??_C@_0HP@IJICEFOE@anim_event_v1?9?$DOtype?$DO?$DN_playback_v@ (0000)
002DD030 0130:
	_apply_funcs (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cutscene/recorded_animation_initialize.h"
#include "math/real_math.h"
#include "memory/byte_swapping.h"

/* ---------- constants */

enum
{
	_playback_v1_nothing,
	_playback_v1_end,
	_playback_v1_animation_state_set,
	_playback_v1_aiming_speed_set,
	_playback_v1_control_flags_set,
	_playback_v1_weapon_index_set,
	_playback_v1_throttle_set,
	_playback_v1_vectors_synchronize,
	_playback_v1_vectors_desynchronize,
	_playback_v1_facing_vector_set,
	_playback_v1_aiming_vector_set,
	_playback_v1_looking_vector_set,
	_playback_v1_facing_aiming_vector_set,
	_playback_v1_facing_looking_vector_set,
	_playback_v1_aiming_looking_vector_set,
	_playback_v1_facing_aiming_looking_vector_set,
	_playback_v1_facing_angles_set,
	_playback_v1_aiming_angles_set,
	_playback_v1_looking_angles_set,
	_playback_v1_facing_aiming_angles_set,
	_playback_v1_facing_looking_angles_set,
	_playback_v1_aiming_looking_angles_set,
	_playback_v1_facing_aiming_looking_angles_set,
	NUMBER_OF_RECORDED_ANIMATION_V1_EVENTS
};

/* ---------- macros */

/* ---------- structures */

struct animation_playback_controller;

struct recorded_animation_event_v1
{
	short type;
	word time_delta;
};

struct recorded_animation_state_set_event_v1
{
	struct recorded_animation_event_v1 event;
	byte animation_state;
	byte unused5;
};

struct recorded_aiming_speed_set_event_v1
{
	struct recorded_animation_event_v1 event;
	byte aiming_speed;
	byte unused5;
};

struct recorded_control_flags_set_event_v1
{
	struct recorded_animation_event_v1 event;
	word control_flags;
};

struct recorded_weapon_index_set_event_v1
{
	struct recorded_animation_event_v1 event;
	short weapon_index;
};

struct recorded_throttle_set_event_v1
{
	struct recorded_animation_event_v1 event;
	real_vector2d throttle;
};

struct recorded_multi_vector_set_event_v1
{
	struct recorded_animation_event_v1 event;
	real_vector3d vector;
};

struct recorded_angle_vector_set_event_v1
{
	struct recorded_animation_event_v1 event;
	real_euler_angles2d angles;
};

typedef void (*recorded_animation_apply_event_v1_proc)(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);

/* ---------- prototypes */

static void apply_animation_state(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_aiming_speed(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_control_flags(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_weapon_index(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_throttle(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_facing_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_aiming_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_looking_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_angle_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);
static void apply_multi_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *event,
	byte const **playback_stream);

/* ---------- globals */

static recorded_animation_apply_event_v1_proc apply_funcs[NUMBER_OF_RECORDED_ANIMATION_V1_EVENTS] =
{
	NULL,
	NULL,
	apply_animation_state,
	apply_aiming_speed,
	apply_control_flags,
	apply_weapon_index,
	apply_throttle,
	NULL,
	NULL,
	apply_facing_vector,
	apply_aiming_vector,
	apply_looking_vector,
	apply_multi_vector,
	apply_multi_vector,
	apply_multi_vector,
	apply_multi_vector,
	apply_angle_vector,
	apply_angle_vector,
	apply_angle_vector,
	apply_angle_vector,
	apply_angle_vector,
	apply_angle_vector,
	apply_angle_vector,
};

static byte_swap_code animation_event_v1_bs_codes[] = { _2byte, _2byte };

static struct byte_swap_definition animation_event_v1_bs_definition =
	{ "animation_event_v1", sizeof(struct recorded_animation_event_v1), animation_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code animation_state_set_event_v1_bs_codes[] = { _1byte };

static struct byte_swap_definition animation_state_set_event_v1_bs_definition =
	{ "animation_state_set_event_v1", sizeof(struct recorded_animation_state_set_event_v1), animation_state_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code aiming_speed_set_event_v1_bs_codes[] = { _1byte };

static struct byte_swap_definition aiming_speed_set_event_v1_bs_definition =
	{ "aiming_speed_set_event_v1", sizeof(struct recorded_aiming_speed_set_event_v1), aiming_speed_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code control_flags_set_event_v1_bs_codes[] = { _2byte };

static struct byte_swap_definition control_flags_set_event_v1_bs_definition =
	{ "control_flags_set_event_v1", sizeof(struct recorded_control_flags_set_event_v1), control_flags_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code weapon_index_set_event_v1_bs_codes[] = { _2byte };

static struct byte_swap_definition weapon_index_set_event_v1_bs_definition =
	{ "weapon_index_set_event_v1", sizeof(struct recorded_weapon_index_set_event_v1), weapon_index_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code throttle_set_event_v1_bs_codes[] = { _4byte, _4byte };

static struct byte_swap_definition throttle_set_event_v1_bs_definition =
	{ "throttle_set_event_v1", sizeof(struct recorded_throttle_set_event_v1), throttle_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code multi_vector_set_event_v1_bs_codes[] = { _4byte, _4byte, _4byte };

static struct byte_swap_definition multi_vector_set_event_v1_bs_definition =
	{ "multi_vector_set_event_v1", sizeof(struct recorded_multi_vector_set_event_v1), multi_vector_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code angle_vector_set_event_v1_bs_codes[] = { _4byte, _4byte };

static struct byte_swap_definition angle_vector_set_event_v1_bs_definition =
	{ "angle_vector_set_event_v1", sizeof(struct recorded_angle_vector_set_event_v1), angle_vector_set_event_v1_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

/* ---------- public code */

void recorded_animation_initialize_event_stream_v1(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *unit_control,
	byte **playback_stream,
	byte unit_control_data_version)
{
	recorded_animation_initialize_unit_control(
		unit_control,
		playback_stream,
		unit_control_data_version);

	return;
}

boolean recorded_animation_apply_event_stream_v1(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	long *ticks,
	byte const **playback_stream)
{
	struct recorded_animation_event_v1 const *anim_event_v1;
	recorded_animation_apply_event_v1_proc apply;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0xA2, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0xA3, ticks);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0xA4, playback_stream);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0xA5, *playback_stream);

	anim_event_v1 = (struct recorded_animation_event_v1 const *)*playback_stream;
	while (*ticks >= anim_event_v1->time_delta)
	{
		if (anim_event_v1->type == _playback_v1_end)
			break;

		apply = apply_funcs[anim_event_v1->type];
		if (apply)
		{
			apply(control, anim_event_v1, playback_stream);
		}
		else
		{
			*playback_stream = (byte const *)(anim_event_v1 + 1);
		}

		*ticks -= anim_event_v1->time_delta;
		anim_event_v1 = (struct recorded_animation_event_v1 const *)*playback_stream;
	}

	if (anim_event_v1->type == _playback_v1_end &&
		*ticks == anim_event_v1->time_delta)
	{
		return FALSE;
	}

	return TRUE;
}

void byte_swap_recording_stream_v1(
	void *stream,
	long stream_size,
	byte unit_control_data_version)
{
	return;
}

/* ---------- private code */

static void apply_animation_state(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_animation_state_set_event_v1 const *event =
		(struct recorded_animation_state_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x19, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x19, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x19, anim_event_v1->type==_playback_v1_animation_state_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x19, playback_stream);

	control->animation_state = event->animation_state;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_aiming_speed(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_aiming_speed_set_event_v1 const *event =
		(struct recorded_aiming_speed_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1A, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1A, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1A, anim_event_v1->type==_playback_v1_aiming_speed_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1A, playback_stream);

	control->aiming_speed = event->aiming_speed;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_control_flags(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_control_flags_set_event_v1 const *event =
		(struct recorded_control_flags_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1B, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1B, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1B, anim_event_v1->type==_playback_v1_control_flags_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1B, playback_stream);

	control->control_flags = event->control_flags;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_weapon_index(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_weapon_index_set_event_v1 const *event =
		(struct recorded_weapon_index_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1C, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1C, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1C, anim_event_v1->type==_playback_v1_weapon_index_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x1C, playback_stream);

	control->weapon_index = event->weapon_index;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_throttle(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_throttle_set_event_v1 const *event =
		(struct recorded_throttle_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x21, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x22, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x23, anim_event_v1->type==_playback_v1_throttle_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x24, playback_stream);

	control->throttle.i = event->throttle.i;
	control->throttle.j = event->throttle.j;
	control->throttle.k = 0.f;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_facing_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_multi_vector_set_event_v1 const *event =
		(struct recorded_multi_vector_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2C, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2C, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2C, anim_event_v1->type==_playback_v1_facing_vector_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2C, playback_stream);

	control->facing_vector = event->vector;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_aiming_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_multi_vector_set_event_v1 const *event =
		(struct recorded_multi_vector_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2D, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2D, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2D, anim_event_v1->type==_playback_v1_aiming_vector_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2D, playback_stream);

	control->aiming_vector = event->vector;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_looking_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_multi_vector_set_event_v1 const *event =
		(struct recorded_multi_vector_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2E, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2E, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2E, anim_event_v1->type==_playback_v1_looking_vector_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x2E, playback_stream);

	control->looking_vector = event->vector;
	*playback_stream += sizeof(*event);

	return;
}

static void apply_angle_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_angle_vector_set_event_v1 const *event =
		(struct recorded_angle_vector_set_event_v1 const *)anim_event_v1;
	real_vector3d angle_vector;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x38, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x39, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x3A, anim_event_v1->type>=_playback_v1_facing_angles_set && anim_event_v1->type<=_playback_v1_facing_aiming_looking_angles_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x3B, playback_stream);

	vector3d_from_euler_angles2d(&angle_vector, &event->angles);
	if (anim_event_v1->type != _playback_v1_aiming_looking_angles_set)
	{
		control->facing_vector = angle_vector;
	}

	if (anim_event_v1->type != _playback_v1_facing_looking_angles_set)
	{
		control->aiming_vector = angle_vector;
	}

	if (anim_event_v1->type != _playback_v1_facing_aiming_angles_set)
	{
		control->looking_vector = angle_vector;
	}

	*playback_stream += sizeof(*event);

	return;
}

static void apply_multi_vector(
	struct unit_control_data *control,
	struct recorded_animation_event_v1 const *anim_event_v1,
	byte const **playback_stream)
{
	struct recorded_multi_vector_set_event_v1 const *event =
		(struct recorded_multi_vector_set_event_v1 const *)anim_event_v1;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x56, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x57, anim_event_v1);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x58, anim_event_v1->type>=_playback_v1_facing_aiming_vector_set&&anim_event_v1->type<=_playback_v1_facing_aiming_looking_vector_set);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback_v1.c", 0x59, playback_stream);

	if (anim_event_v1->type != _playback_v1_aiming_looking_vector_set)
	{
		control->facing_vector = event->vector;
	}

	if (anim_event_v1->type != _playback_v1_facing_looking_vector_set)
	{
		control->aiming_vector = event->vector;
	}

	if (anim_event_v1->type != _playback_v1_facing_aiming_vector_set)
	{
		control->looking_vector = event->vector;
	}

	*playback_stream += sizeof(*event);

	return;
}
