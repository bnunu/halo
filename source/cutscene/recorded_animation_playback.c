/*
RECORDED_ANIMATION_PLAYBACK.C

symbols in this file:
00081EF0 0090:
	_apply_animation_state (0000)
00081F80 0090:
	_apply_aiming_speed (0000)
00082010 0090:
	_apply_control_flags (0000)
000820A0 0090:
	_apply_weapon_index (0000)
00082130 00a0:
	_apply_throttle (0000)
000821D0 0040:
	_update_controller_char (0000)
00082210 0040:
	_update_controller_short (0000)
00082250 0040:
	_uncompress_vector_from_controller (0000)
00082290 0200:
	_apply_vector_char_difference (0000)
00082490 0200:
	_apply_vector_short_difference (0000)
00082690 0040:
	_recorded_animation_initialize_event_stream (0000)
000826D0 0040:
	_recorded_animation_initialize_event_stream_with_size (0000)
00082710 01f0:
	_recorded_animation_apply_event_stream (0000)
00082900 0010:
	_byte_swap_recording_stream (0000)
00258B64 001d:
	??_C@_0BN@BFIMCFJJ@vector_short_difference_data?$AA@ (0000)
00258B84 001c:
	??_C@_0BM@PGDPGFD@vector_char_difference_data?$AA@ (0000)
00258BA0 0014:
	??_C@_0BE@DKBHBGID@throttle_event_data?$AA@ (0000)
00258BB4 0018:
	??_C@_0BI@KMHECDMB@weapon_index_event_data?$AA@ (0000)
00258BCC 0019:
	??_C@_0BJ@FKCNELO@control_flags_event_data?$AA@ (0000)
00258BE8 0018:
	??_C@_0BI@LIFDCNFO@aiming_speed_event_data?$AA@ (0000)
00258C00 001b:
	??_C@_0BL@KNDJMAJG@animation_state_event_data?$AA@ (0000)
00258C1C 0032:
	??_C@_0DC@OJKPBELH@header?9?$DOevent_type?$DN?$DN_playback_an@ (0000)
00258C50 0008:
	??_C@_07MMBOFDEG@control?$AA@ (0000)
00258C58 0036:
	??_C@_0DG@CLKBONJL@c?3?2halo?2SOURCE?2cutscene?2recorded@ (0000)
00258C90 002f:
	??_C@_0CP@MMJOFKBA@header?9?$DOevent_type?$DN?$DN_playback_ai@ (0000)
00258CC0 0030:
	??_C@_0DA@LDGBKFOB@header?9?$DOevent_type?$DN?$DN_playback_co@ (0000)
00258CF0 002f:
	??_C@_0CP@PKMHGEMN@header?9?$DOevent_type?$DN?$DN_playback_we@ (0000)
00258D20 002b:
	??_C@_0CL@JMFIPONF@header?9?$DOevent_type?$DN?$DN_playback_th@ (0000)
00258D4C 0004:
	__real@3b4de32e (0000)
00258D50 0092:
	??_C@_0JC@CIBGBOCG@header?9?$DOevent_type?$DO?$DN_playback_ve@ (0000)
00258DE8 0094:
	??_C@_0JE@EDOLGDPM@header?9?$DOevent_type?$DO?$DN_playback_ve@ (0000)
00258E7C 0029:
	??_C@_0CJ@FIDPKLHA@header?9?$DOevent_type?$DMNUMBEROF?$CIappl@ (0000)
00258EA8 001d:
	??_C@_0BN@GHBCOJNB@time_delta?$DOUNSIGNED_CHAR_MAX?$AA@ (0000)
00258EC8 002c:
	??_C@_0CM@IOCGHDGB@time_delta?$DO1?$CG?$CGtime_delta?$DM?$DNUNSIGN@ (0000)
00258EF4 0011:
	??_C@_0BB@LIMNNLMB@?$CKplayback_stream?$AA@ (0000)
00258F08 0010:
	??_C@_0BA@IBBGIMIK@playback_stream?$AA@ (0000)
00258F18 0006:
	??_C@_05CDKPFCKO@ticks?$AA@ (0000)
002DCF20 0110:
	_apply_funcs (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cutscene/recorded_animation_initialize.h"
#include "memory/byte_swapping.h"
#include "math/real_math.h"

#undef memcpy
#include <string.h>

/* ---------- constants */

enum
{
	_playback_end = 1,
	_playback_animation_state_set,
	_playback_aiming_speed_set,
	_playback_control_flags_set,
	_playback_weapon_index_set,
	_playback_throttle_set,
	_playback_vector_char_difference_set,
	_playback_vector_short_difference_set = 15,
	NUMBER_OF_RECORDED_ANIMATION_EVENTS = 23
};

enum
{
	_time_delta_zero,
	_time_delta_one,
	_time_delta_byte,
	_time_delta_word,
};

enum
{
	_control_vector_facing_bit,
	_control_vector_aiming_bit,
	_control_vector_looking_bit,
	NUMBER_OF_CONTROL_VECTORS,
};

/* ---------- macros */

/* ---------- structures */

struct direction_playback_controller
{
	short yaw;
	short pitch;
};

struct animation_event_header
{
	byte time_delta : 2;
	byte event_type : 6;
};

struct vector_char_difference_data
{
	char delta_yaw;
	char delta_pitch;
};

struct vector_short_difference_data
{
	short delta_yaw;
	short delta_pitch;
};

struct animation_playback_controller
{
	struct direction_playback_controller facing_control;
	struct direction_playback_controller aiming_control;
	struct direction_playback_controller looking_control;
};

typedef void (*recorded_animation_apply_proc)(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);

/* ---------- prototypes */

static void apply_animation_state(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_aiming_speed(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_control_flags(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_weapon_index(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_throttle(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_vector_char_difference(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);
static void apply_vector_short_difference(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream);

/* ---------- globals */

static recorded_animation_apply_proc apply_funcs[NUMBER_OF_RECORDED_ANIMATION_EVENTS] =
{
	NULL,
	NULL,
	apply_animation_state,
	apply_aiming_speed,
	apply_control_flags,
	apply_weapon_index,
	apply_throttle,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_char_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
	apply_vector_short_difference,
};

static byte_swap_code animation_state_event_data_bs_codes[] = { _1byte };

static struct byte_swap_definition animation_state_event_data_bs_definition =
	{ "animation_state_event_data", sizeof(byte), animation_state_event_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code aiming_speed_event_data_bs_codes[] = { _1byte };

static struct byte_swap_definition aiming_speed_event_data_bs_definition =
	{ "aiming_speed_event_data", sizeof(byte), aiming_speed_event_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code control_flags_event_data_bs_codes[] = { _2byte };

static struct byte_swap_definition control_flags_event_data_bs_definition =
	{ "control_flags_event_data", sizeof(short), control_flags_event_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code weapon_index_event_data_bs_codes[] = { _2byte };

static struct byte_swap_definition weapon_index_event_data_bs_definition =
	{ "weapon_index_event_data", sizeof(short), weapon_index_event_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code throttle_event_data_bs_codes[] = { _4byte, _4byte };

static struct byte_swap_definition throttle_event_data_bs_definition =
	{ "throttle_event_data", sizeof(real_vector2d), throttle_event_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code vector_char_difference_data_bs_codes[] = { _1byte, _1byte };

static struct byte_swap_definition vector_char_difference_data_bs_definition =
	{ "vector_char_difference_data", sizeof(struct vector_char_difference_data), vector_char_difference_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code vector_short_difference_data_bs_codes[] = { _2byte, _2byte };

static struct byte_swap_definition vector_short_difference_data_bs_definition =
	{ "vector_short_difference_data", sizeof(struct vector_short_difference_data), vector_short_difference_data_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

/* ---------- public code */

void recorded_animation_initialize_event_stream(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *unit_control,
	byte **playback_stream,
	byte unit_control_data_version)
{
	recorded_animation_initialize_unit_control(
		unit_control,
		playback_stream,
		unit_control_data_version);

	memcpy(animation_state, *playback_stream, sizeof(*animation_state));
	*playback_stream += sizeof(*animation_state);

	return;
}

void recorded_animation_initialize_event_stream_with_size(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *unit_control,
	byte **playback_stream)
{
	memcpy(unit_control, *playback_stream, sizeof(*unit_control));
	*playback_stream += sizeof(*unit_control);

	memcpy(animation_state, *playback_stream, sizeof(*animation_state));
	*playback_stream += sizeof(*animation_state);

	return;
}

boolean recorded_animation_apply_event_stream(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	long *ticks,
	byte const **playback_stream)
{
	struct animation_event_header const *header;
	word time_delta;
	word header_size;
	recorded_animation_apply_proc apply;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 0x113, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 0x114, ticks);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 0x115, playback_stream);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 0x116, *playback_stream);

	for (;;)
	{
		header = (struct animation_event_header const *)*playback_stream;
		header_size = 0;
		switch (header->time_delta)
		{
		case _time_delta_zero:
			time_delta = 0;
			header_size = 1;
			break;

		case _time_delta_one:
			time_delta = 1;
			header_size = 1;
			break;

		case _time_delta_byte:
			time_delta = *((byte const *)header + 1);
			header_size = 2;
			match_assert(
				"c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c",
				0x12D,
				time_delta>1&&time_delta<=UNSIGNED_CHAR_MAX);
			break;

		case _time_delta_word:
			memcpy(&time_delta, (byte const *)header + 1, sizeof(time_delta));
			header_size = 3;
			match_assert(
				"c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c",
				0x132,
				time_delta>UNSIGNED_CHAR_MAX);
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c",
				0x135,
				!"unreachable");
			break;
		}

		if (*ticks < time_delta || header->event_type == _playback_end)
			break;

		*playback_stream += header_size;
		match_assert(
			"c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c",
			0x13B,
			header->event_type<NUMBEROF(apply_funcs));

		apply = apply_funcs[header->event_type];
		if (apply)
		{
			apply(animation_state, control, header, playback_stream);
		}

		*ticks -= time_delta;
	}

	if (header->event_type == _playback_end && *ticks == time_delta)
		return FALSE;

	return TRUE;
}

void byte_swap_recording_stream(
	void *stream,
	long stream_size,
	byte unit_control_data_version)
{
	return;
}

/* ---------- private code */

static void apply_animation_state(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	byte const *event_data = *playback_stream;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 25, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 25, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 25, header->event_type==_playback_animation_state_set);

	control->animation_state = *event_data;
	(*playback_stream)++;

	return;
}

static void apply_aiming_speed(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	byte const *event_data = *playback_stream;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 26, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 26, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 26, header->event_type==_playback_aiming_speed_set);

	control->aiming_speed = *event_data;
	(*playback_stream)++;

	return;
}

static void apply_control_flags(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	byte const *event_data = *playback_stream;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 27, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 27, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 27, header->event_type==_playback_control_flags_set);

	memcpy(&control->control_flags, event_data, sizeof(control->control_flags));
	*playback_stream += sizeof(control->control_flags);

	return;
}

static void apply_weapon_index(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	byte const *event_data = *playback_stream;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 28, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 28, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 28, header->event_type==_playback_weapon_index_set);

	memcpy(&control->weapon_index, event_data, sizeof(control->weapon_index));
	*playback_stream += sizeof(control->weapon_index);

	return;
}

static void apply_throttle(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	byte const *event_data = *playback_stream;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 33, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 35, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 36, header->event_type==_playback_throttle_set);

	memcpy(&control->throttle, event_data, sizeof(real_vector2d));
	control->throttle.k = 0.0f;
	*playback_stream += sizeof(real_vector2d);

	return;
}

static void update_controller_char(
	struct vector_char_difference_data const *event_data,
	struct direction_playback_controller *control)
{
	control->yaw += event_data->delta_yaw;
	if (control->yaw > 1000)
	{
		control->yaw -= 1000;
	}
	else if (control->yaw < -1000)
	{
		control->yaw += 1000;
	}
	control->pitch += event_data->delta_pitch;

	return;
}

static void update_controller_short(
	struct vector_short_difference_data const *event_data,
	struct direction_playback_controller *control)
{
	control->yaw += event_data->delta_yaw;
	if (control->yaw > 1000)
	{
		control->yaw -= 1000;
	}
	else if (control->yaw < -1000)
	{
		control->yaw += 1000;
	}
	control->pitch += event_data->delta_pitch;

	return;
}

static void uncompress_vector_from_controller(
	real_vector3d *vector,
	struct direction_playback_controller const *controller)
{
	real_euler_angles2d angles;

	angles.yaw = (real)controller->yaw * 0.0031415927f;
	angles.pitch = (real)controller->pitch * 0.0031415927f;
	vector3d_from_euler_angles2d(vector, &angles);

	return;
}

static void apply_vector_char_difference(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	void const *serialized_event_data = *playback_stream;
	struct vector_char_difference_data const *event_data = serialized_event_data;
	word event_type;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 100, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 102, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 103, header->event_type>=_playback_vector_char_difference_set&&header->event_type-_playback_vector_char_difference_set<FLAG(NUMBER_OF_CONTROL_VECTORS));

	event_type = header->event_type - _playback_vector_char_difference_set;
	if (event_type & FLAG(_control_vector_facing_bit))
	{
		update_controller_char(event_data, &animation_state->facing_control);
		uncompress_vector_from_controller(&control->facing_vector, &animation_state->facing_control);
	}

	if (event_type & FLAG(_control_vector_aiming_bit))
	{
		if (event_type & FLAG(_control_vector_facing_bit))
		{
			animation_state->aiming_control = animation_state->facing_control;
			control->aiming_vector = control->facing_vector;
		}
		else
		{
			update_controller_char(event_data, &animation_state->aiming_control);
			uncompress_vector_from_controller(&control->aiming_vector, &animation_state->aiming_control);
		}
	}

	if (event_type & FLAG(_control_vector_looking_bit))
	{
		if (event_type & FLAG(_control_vector_facing_bit))
		{
			animation_state->looking_control = animation_state->facing_control;
			control->looking_vector = control->facing_vector;
			*playback_stream += sizeof(struct vector_char_difference_data);
			return;
		}

		if (event_type & FLAG(_control_vector_aiming_bit))
		{
			animation_state->looking_control = animation_state->aiming_control;
			control->looking_vector = control->aiming_vector;
			*playback_stream += sizeof(struct vector_char_difference_data);
			return;
		}

		update_controller_char(event_data, &animation_state->looking_control);
		uncompress_vector_from_controller(&control->looking_vector, &animation_state->looking_control);
	}

	*playback_stream += sizeof(struct vector_char_difference_data);

	return;
}

static void apply_vector_short_difference(
	struct animation_playback_controller *animation_state,
	struct unit_control_data *control,
	struct animation_event_header const *header,
	byte const **playback_stream)
{
	void const *serialized_event_data = *playback_stream;
	struct vector_short_difference_data const *event_data = serialized_event_data;
	short event_type;

	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 160, control);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 162, header);
	match_assert("c:\\halo\\SOURCE\\cutscene\\recorded_animation_playback.c", 163, header->event_type>=_playback_vector_short_difference_set&&header->event_type-_playback_vector_short_difference_set<FLAG(NUMBER_OF_CONTROL_VECTORS));

	event_type = header->event_type - _playback_vector_short_difference_set;
	if (event_type & FLAG(_control_vector_facing_bit))
	{
		update_controller_short(event_data, &animation_state->facing_control);
		uncompress_vector_from_controller(&control->facing_vector, &animation_state->facing_control);
	}

	if (event_type & FLAG(_control_vector_aiming_bit))
	{
		if (event_type & FLAG(_control_vector_facing_bit))
		{
			animation_state->aiming_control = animation_state->facing_control;
			control->aiming_vector = control->facing_vector;
		}
		else
		{
			update_controller_short(event_data, &animation_state->aiming_control);
			uncompress_vector_from_controller(&control->aiming_vector, &animation_state->aiming_control);
		}
	}

	if (event_type & FLAG(_control_vector_looking_bit))
	{
		if (event_type & FLAG(_control_vector_facing_bit))
		{
			animation_state->looking_control = animation_state->facing_control;
			control->looking_vector = control->facing_vector;
			*playback_stream += sizeof(struct vector_short_difference_data);
			return;
		}

		if (event_type & FLAG(_control_vector_aiming_bit))
		{
			animation_state->looking_control = animation_state->aiming_control;
			control->looking_vector = control->aiming_vector;
			*playback_stream += sizeof(struct vector_short_difference_data);
			return;
		}

		update_controller_short(event_data, &animation_state->looking_control);
		uncompress_vector_from_controller(&control->looking_vector, &animation_state->looking_control);
	}

	*playback_stream += sizeof(struct vector_short_difference_data);

	return;
}
