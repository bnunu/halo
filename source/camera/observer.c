/*
OBSERVER.C

symbols in this file:
00078970 0040:
	_observer_get (0000)
000789B0 0010:
	_observer_initialize (0000)
000789C0 0130:
	_observer_clear (0000)
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
	_observer_update_accelerations (0000)
00078EA0 0200:
	_observer_update_velocities (0000)
000790A0 0050:
	_observer_obsolete_position (0000)
000790F0 0090:
	_observer_up_from_forward (0000)
00079180 0080:
	_observer_apply_rotational_displacement (0000)
00079200 00e0:
	_observer_collision_test_with_t (0000)
000792E0 0040:
	_observer_collision_test_differential (0000)
00079320 03b0:
	_observer_set_camera (0000)
000796D0 0410:
	_observer_update_command (0000)
00079AE0 05a0:
	_observer_update_polynomial (0000)
0007A080 0620:
	_observer_update_positions (0000)
0007A6A0 0120:
	_observer_rotational_displacement (0000)
0007A7C0 02f0:
	_observer_check_penetration (0000)
0007AAB0 0070:
	_observer_find_displacement (0000)
0007AB20 0840:
	_observer_postcheck (0000)
0007B360 0050:
	_observer_update_displacements (0000)
0007B3B0 00a0:
	_observer_pass_time (0000)
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
	_observer_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "observer.h"

#include "game/players.h"
#include "networking/network_connection.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct observer_globals
{
	real seconds_elapsed;
	struct observer observers[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
};

/* ---------- prototypes */

static struct observer *observer_get(
	short local_player_index);
static void observer_clear(
	struct observer *observer);
static void observer_apply_rotational_displacement(
	real_vector3d const *rotational_displacement,
	real_vector3d *forward,
	real_vector3d *up);

/* ---------- globals */

static struct observer_globals observer_globals;

/* ---------- public code */

void observer_initialize(
	void)
{
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
	struct observer *observer;
	short parameter_index;

	observer = observer_get(local_player_index);
	if (observer->command.camera.timer != 0.f)
		return FALSE;

	for (parameter_index = 0;
		parameter_index < NUMBER_OF_CAMERA_PARAMETERS;
		parameter_index++)
	{
		if (observer->command.parameter_timers[parameter_index] != 0.f)
			return FALSE;
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
			struct observer *observer = observer_get(local_player_index);

			scenario_location_from_point(
				&observer->result.location,
				&observer->result.position);
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
	real_vector3d perpendicular;

	perpendicular.i = forward->j;
	perpendicular.j = -forward->i;
	perpendicular.k = 0.f;
	if (normalize3d(&perpendicular) == 0.f)
	{
		perpendicular.i = 1.f;
		perpendicular.j = perpendicular.k = 0.f;
	}

	cross_product3d(&perpendicular, forward, up);

	return;
}

/* ---------- private code */

static struct observer *observer_get(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\camera\\observer.c",
		0x72,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	return &observer_globals.observers[local_player_index];
}

static void observer_clear(
	struct observer *observer)
{
	observer->forward = *global_forward3d;
	observer->up = *global_up3d;
	observer->field_of_view = DEGREES_TO_RADIANS(50);
	observer->result.position = *global_origin3d;
	observer->result.location.cluster_index = NONE;
	observer->result.location.leaf_index = NONE;
	observer->result.velocity = *global_zero_vector3d;
	observer->result.forward = *global_forward3d;
	observer->result.up = *global_up3d;
	observer->result.field_of_view = DEGREES_TO_RADIANS(50);
	csmemset(&observer->command, 0, sizeof(observer->command));
	observer->command.camera.forward = observer->forward;
	observer->command.camera.up = observer->up;
	observer->command.camera.field_of_view = observer->field_of_view;
	observer->trailer_signature = OBSERVER_SIGNATURE;
	observer->header_signature = OBSERVER_SIGNATURE;
	observer->updated_for_frame = TRUE;
	observer->unknown71 = FALSE;

	return;
}

static void observer_apply_rotational_displacement(
	real_vector3d const *rotational_displacement,
	real_vector3d *forward,
	real_vector3d *up)
{
	real_vector3d axis = *rotational_displacement;
	real angle = normalize3d(&axis);

	if (angle != 0.f)
	{
		real sine_angle = sine(angle);
		real cosine_angle = cosine(angle);

		rotate_vector_about_axis(forward, &axis, sine_angle, cosine_angle);
		rotate_vector_about_axis(up, &axis, sine_angle, cosine_angle);
	}

	return;
}
