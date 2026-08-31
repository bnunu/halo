/*
OBSERVER.C

symbols in this file:
00078970 0040:
	_code_00078970 (0000)
000789B0 0010:
	_observer_initialize (0000)
000789C0 0130:
	_code_000789c0 (0000)
00078AF0 0050:
	_observer_initialize_for_new_map (0000)
00078B40 0010:
	_observer_dispose_from_old_map (0000)
00078B50 0090:
	_observer_get_camera (0000)
00078BE0 0080:
	_observer_command_has_finished (0000)
00078C60 0070:
	_observer_reconnect_to_structure_bsp (0000)
00078CD0 01d0:
	_code_00078cd0 (0000)
00078EA0 0200:
	_code_00078ea0 (0000)
000790A0 0050:
	_observer_obsolete_position (0000)
000790F0 0090:
	_observer_up_from_forward (0000)
00079180 0080:
	_code_00079180 (0000)
00079200 00e0:
	_code_00079200 (0000)
000792E0 0040:
	_code_000792e0 (0000)
00079320 03b0:
	_observer_set_camera (0000)
000796D0 0410:
	_code_000796d0 (0000)
00079AE0 05a0:
	_code_00079ae0 (0000)
0007A080 0620:
	_code_0007a080 (0000)
0007A6A0 0120:
	_code_0007a6a0 (0000)
0007A7C0 02f0:
	_code_0007a7c0 (0000)
0007AAB0 0070:
	_code_0007aab0 (0000)
0007AB20 0840:
	_code_0007ab20 (0000)
0007B360 0050:
	_code_0007b360 (0000)
0007B3B0 00a0:
	_code_0007b3b0 (0000)
0007B450 0130:
	_observer_update (0000)
00256F3C 001c:
	_rdata_00256f3c (0000)
00256F58 0021:
	??_C@_0CB@MBFPPGME@c?3?2halo?2SOURCE?2camera?2observer?4c@ (0000)
00256F80 0084:
	??_C@_0IE@MHMDJBI@observer?9?$DOresult?4location?4cluste@ (0000)
00257004 0028:
	??_C@_0CI@NBOHAIIP@observer?9?$DOvelocities?4n?$FLparameter@ (0000)
0025702C 0009:
	??_C@_08LOHDAPLH@forward1?$AA@ (0000)
00257038 0004:
	??_C@_03BAPGBPKP@up1?$AA@ (0000)
00257040 0042:
	??_C@_0EC@LAMMDMMJ@?$CFs?0?5?$CFs?3?5assert_valid_real_vector@ (0000)
00257084 0009:
	??_C@_08KHGIDOPG@forward0?$AA@ (0000)
00257090 0004:
	??_C@_03JONCOOO@up0?$AA@ (0000)
00257098 0008:
	__real@3fb99999a0000000 (0000)
002570A0 002d:
	??_C@_0CN@JLJEHJNH@valid_field_of_view?$CIobserver?9?$DOfi@ (0000)
002570D0 0035:
	??_C@_0DF@POOMEEJD@valid_world_real_point3d?$CI?$CGobserv@ (0000)
00257108 0025:
	??_C@_0CF@HBCBGNBI@valid_focus_distance?$CIfocus_dista@ (0000)
00257130 0043:
	??_C@_0ED@DKCKLPPD@valid_world_real_point3d?$CI?$CIreal_p@ (0000)
00257174 0013:
	??_C@_0BD@EHDGCMIA@?$CGobserver?9?$DOforward?$AA@ (0000)
00257188 000e:
	??_C@_0O@NJKILIMK@?$CGobserver?9?$DOup?$AA@ (0000)
00257198 002a:
	??_C@_0CK@OGOGINPI@valid_world_real_point3d?$CI?$CGfocus_@ (0000)
002571C4 001d:
	??_C@_0BN@FGDILBJM@?$CBobserver?9?$DOupdated_for_frame?$AA@ (0000)
002571E8 0062:
	??_C@_0GC@DNGKAJPP@observer?9?$DOheader_signature?$DN?$DNOBSE@ (0000)
002DCC6C 0016:
	_data_002dcc6c (0000)
0031D4B8 0a74:
	_bss_0031d4b8 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "observer.h"

#include "game/players.h"
#include "networking/network_connection.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"

/* ---------- constants */

enum
{
	OBSERVER_SIGNATURE = 'rad!',

	NUMBER_OF_OBSERVER_COMMAND_PARAMETERS = 5,

	NUMBER_OF_OBSERVER_VELOCITIES = 11
};

#define DEFAULT_OBSERVER_FIELD_OF_VIEW DEGREES_TO_RADIANS(50.f)

/* ---------- macros */

/* ---------- structures */

struct camera_command
{
	long flags;
	real_point3d position;
	real_vector3d offset;
	real depth;
	real field_of_view;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d velocity;
	real timer;
	byte reserved04C[0x08];
	real parameter_timers[NUMBER_OF_OBSERVER_COMMAND_PARAMETERS];
};

struct observer_velocities
{
	real n[NUMBER_OF_OBSERVER_VELOCITIES];
};

struct observer
{
	long header_signature;
	struct camera_command *command;
	struct camera_command target;
	boolean updated_for_frame;
	boolean initialized;
	byte pad072[2];
	struct observer_result result;
	byte reserved0B0[0x1C];
	real field_of_view;
	real_vector3d forward;
	real_vector3d up;
	struct observer_velocities velocities;
	byte reserved114[0x184];
	long trailer_signature;
};

struct observer_globals
{
	real dtime;
	struct observer local_players[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
};

typedef char camera_command_size_assert[
	sizeof(struct camera_command) == 0x68 ? 1 : -1];
typedef char observer_velocities_offset_assert[
	offsetof(struct observer, velocities) == 0xE8 ? 1 : -1];
typedef char observer_result_offset_assert[
	offsetof(struct observer, result) == 0x74 ? 1 : -1];
typedef char observer_size_assert[
	sizeof(struct observer) == 0x29C ? 1 : -1];
typedef char observer_globals_size_assert[
	sizeof(struct observer_globals) == 0xA74 ? 1 : -1];

/* ---------- prototypes */

static struct observer *observer_get(
	short local_player_index);
static void observer_clear(
	struct observer *observer);

/* ---------- globals */

extern struct observer_globals bss_0031d4b8;

/* ---------- private code */

static struct observer *observer_get(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\camera\\observer.c",
		0x72,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	return &bss_0031d4b8.local_players[local_player_index];
}

/* ---------- public code */

void observer_initialize(
	void)
{
	return;
}

static void observer_clear(
	struct observer *observer)
{
	observer->forward = *global_forward3d;
	observer->up = *global_up3d;
	observer->field_of_view = DEFAULT_OBSERVER_FIELD_OF_VIEW;

	observer->result.position = *global_origin3d;
	observer->result.location.cluster_index = NONE;
	observer->result.location.leaf_index = NONE;
	observer->result.velocity = *global_zero_vector3d;
	observer->result.forward = *global_forward3d;
	observer->result.up = *global_up3d;
	observer->result.field_of_view = DEFAULT_OBSERVER_FIELD_OF_VIEW;

	csmemset(&observer->target, 0, sizeof(observer->target));
	observer->target.forward = observer->forward;
	observer->target.up = observer->up;
	observer->target.field_of_view = observer->field_of_view;

	observer->trailer_signature = OBSERVER_SIGNATURE;
	observer->header_signature = OBSERVER_SIGNATURE;

	observer->updated_for_frame = TRUE;
	observer->initialized = FALSE;

	return;
}

void observer_initialize_for_new_map(
	void)
{
	short local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		observer_clear(observer_get(local_player_index));
	}

	return;
}

void observer_dispose_from_old_map(
	void)
{
	return;
}

struct observer_result const *observer_get_camera(
	short local_player_index)
{
	struct observer_result const *result = NULL;

	if (local_player_index != NONE)
	{
		struct observer *observer = observer_get(local_player_index);

		result = &observer->result;

		match_assert(
			"c:\\halo\\SOURCE\\camera\\observer.c",
			0x12D,
			observer->result.location.cluster_index>=NONE && observer->result.location.cluster_index<global_structure_bsp_get()->clusters.count);
	}

	return result;
}

boolean observer_command_has_finished(
	short local_player_index)
{
	struct observer *observer = observer_get(local_player_index);
	short parameter_index;

	if (observer->target.timer != 0.0f)
	{
		return FALSE;
	}

	for (parameter_index = 0;
		parameter_index < NUMBER_OF_OBSERVER_COMMAND_PARAMETERS;
		parameter_index++)
	{
		if (observer->target.parameter_timers[parameter_index] != 0.0f)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void observer_reconnect_to_structure_bsp(
	void)
{
	short local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		if (local_player_get_player_index(local_player_index) != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\camera\\observer.c",
				0x72,
				local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
			scenario_location_from_point(
				&bss_0031d4b8.local_players[local_player_index].result.location,
				&bss_0031d4b8.local_players[local_player_index].result.position);
		}
	}

	return;
}

void observer_obsolete_position(
	short local_player_index)
{
	observer_clear(observer_get(local_player_index));

	return;
}

void observer_up_from_forward(
	real_vector3d const *forward,
	real_vector3d *up)
{
	real_vector3d right;

	right.i = forward->j;
	right.j = -forward->i;
	right.k = 0.0f;

	if (normalize3d(&right) == 0.0f)
	{
		right.i = 1.0f;
		right.j = right.k = 0.0f;
	}

	cross_product3d(&right, forward, up);

	return;
}

static void observer_rotate_vectors(
	real_vector3d const *rotation,
	real_vector3d *forward,
	real_vector3d *up)
{
	real_vector3d axis = *rotation;
	real angle = normalize3d(&axis);

	if (angle != 0.0f)
	{
		real sine_value = sine(angle);
		real cosine_value = cosine(angle);

		rotate_vector_about_axis(forward, &axis, sine_value, cosine_value);
		rotate_vector_about_axis(up, &axis, sine_value, cosine_value);
	}

	return;
}

static boolean observer_test_line(
	real_point3d const *point0,
	real_point3d const *point1,
	boolean ignore_media,
	real *distance)
{
	boolean blocked = FALSE;
	unsigned long collision_flags =
		FLAG(_collision_test_front_facing_surfaces_bit) |
		FLAG(_collision_test_structure_bit) |
		FLAG(_collision_test_media_bit) |
		FLAG(_collision_test_objects_bit) |
		FLAG(_collision_test_objects_scenery_bit);
	struct collision_result collision;

	if (ignore_media)
	{
		collision_flags =
			FLAG(_collision_test_front_facing_surfaces_bit) |
			FLAG(_collision_test_structure_bit) |
			FLAG(_collision_test_objects_bit) |
			FLAG(_collision_test_objects_scenery_bit);
	}

	match_assert(
		"c:\\halo\\SOURCE\\camera\\observer.c",
		0x4B4,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = _collision_user_observer;

	if (collision_test_line(collision_flags, point0, point1, NONE, &collision))
	{
		*distance = collision.t;
		blocked = TRUE;
	}

	match_assert(
		"c:\\halo\\SOURCE\\camera\\observer.c",
		0x4BA,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return blocked;
}

static boolean observer_test_ray(
	real_point3d const *point0,
	real_point3d const *point,
	real_vector3d const *vector,
	real length,
	real *distance,
	boolean ignore_media)
{
	real_point3d point1;

	point1.x = point->x + length*vector->i;
	point1.y = point->y + length*vector->j;
	point1.z = point->z + length*vector->k;

	return observer_test_line(point0, &point1, ignore_media, distance);
}

void observer_set_camera(
	short local_player_index,
	struct camera_command *command)
{
	struct observer *observer = observer_get(local_player_index);

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\observer.c",
		0xE9,
		command &&
		(!(command->flags & FLAG(0)) ||
		(valid_real_vector3d_axes2(&command->forward, &command->up) &&
			valid_real(command->position.x) && command->position.x>=-5000.f && command->position.x<=5000.f &&
			valid_real(command->position.y) && command->position.y>=-5000.f && command->position.y<=5000.f &&
			valid_real(command->position.z) && command->position.z>=-5000.f && command->position.z<=5000.f &&
			valid_real(command->offset.i) && command->offset.i>=-5000.f && command->offset.i<=5000.f &&
			valid_real(command->offset.j) && command->offset.j>=-5000.f && command->offset.j<=5000.f &&
			valid_real(command->offset.k) && command->offset.k>=-5000.f && command->offset.k<=5000.f &&
			valid_real_vector3d(&command->velocity) &&
			valid_real(command->depth) && command->depth>=0.f && command->depth<=5000.f &&
			valid_real(command->field_of_view) && command->field_of_view>=0.001f && command->field_of_view<=_pi / 2.f &&
			valid_real(command->timer) && command->timer>=0.f && command->timer<=3600.f)),
		csprintf(
			temporary,
			"Invalid camera command.\nF: (%f, %f, %f) U: (%f, %f, %f)\nP: (%f, %f, %f) O: (%f, %f, %f)\nD: %f V: (%f, %f, %f), FOV: %f, T: %f, FL: %ld",
			command->forward.i,
			command->forward.j,
			command->forward.k,
			command->up.i,
			command->up.j,
			command->up.k,
			command->position.x,
			command->position.y,
			command->position.z,
			command->offset.i,
			command->offset.j,
			command->offset.k,
			command->depth,
			command->velocity.i,
			command->velocity.j,
			command->velocity.k,
			command->field_of_view,
			command->timer,
			command->flags));

	observer->command = command;
	observer->updated_for_frame = FALSE;

	if (!observer->initialized)
	{
		observer->initialized = TRUE;
		observer->command->timer = 0.0f;
		observer->command->flags |= FLAG(3);
		csmemset(observer->command->parameter_timers, 0, sizeof(observer->command->parameter_timers));
	}

	return;
}
