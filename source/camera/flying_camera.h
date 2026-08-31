/*
FLYING_CAMERA.H

header included in hcex build.
*/

#ifndef __FLYING_CAMERA_H
#define __FLYING_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct flying_camera
{
	real_point3d position;
	real_euler_angles3d facing;
	real field_of_view;
};

struct camera_command;
struct camera_control;

/* ---------- prototypes/FLYING_CAMERA.C */

void flying_camera_new(
	struct flying_camera *camera);
void flying_camera_new_from_point_and_vector(
	struct flying_camera *camera,
	real_point3d const *focus,
	real_vector3d const *orientation);
void flying_camera_update(
	struct flying_camera *camera,
	struct camera_control const *controls,
	struct camera_command *result);

/* ---------- globals */

/* ---------- public code */

#endif // __FLYING_CAMERA_H
