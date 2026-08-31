/*
OBSERVER.H

header included in hcex build.
*/

#ifndef __OBSERVER_H
#define __OBSERVER_H
#pragma once

/* ---------- headers */

#include "camera/static_camera.h"
#include "objects/objects.h"

/* ---------- constants */

enum
{
	OBSERVER_SIGNATURE = 'rad!'
};

/* ---------- macros */

/* ---------- structures */

struct observer_result
{
	real_point3d position;
	struct location location;
	real_vector3d velocity;
	real_vector3d forward;
	real_vector3d up;
	real field_of_view;
};

struct observer_command
{
	struct camera_command camera;
	byte parameter_flags[NUMBER_OF_CAMERA_PARAMETERS];
	byte pad4D[3];
	real parameter_timers[NUMBER_OF_CAMERA_PARAMETERS];
};

struct observer
{
	long header_signature;
	void *unknown4;
	struct observer_command command;
	boolean updated_for_frame;
	boolean unknown71;
	byte pad72[2];
	struct observer_result result;
	real_point3d position;
	real_vector3d focus_offset;
	real focus_distance;
	real field_of_view;
	real_vector3d forward;
	real_vector3d up;
	byte unknownE8[0x1B0];
	long trailer_signature;
};

typedef char observer_command_size_assert[
	sizeof(struct observer_command) == 0x68 ? 1 : -1];
typedef char observer_size_assert[
	sizeof(struct observer) == 0x29C ? 1 : -1];
typedef char observer_result_offset_assert[
	offsetof(struct observer, result) == 0x74 ? 1 : -1];
typedef char observer_position_offset_assert[
	offsetof(struct observer, position) == 0xB0 ? 1 : -1];
typedef char observer_command_timer_offset_assert[
	offsetof(struct observer, command.camera.timer) == 0x50 ? 1 : -1];

/* ---------- prototypes/OBSERVER.C */

void observer_initialize(
	void);
void observer_dispose_from_old_map(
	void);
void observer_initialize_for_new_map(
	void);

struct observer_result const *observer_get_camera(
	short local_player_index);

void observer_up_from_forward(
	real_vector3d const *forward,
	real_vector3d *up);

/* ---------- globals */

/* ---------- public code */

#endif // __OBSERVER_H
