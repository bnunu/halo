/*
RECORDED_ANIMATION_INITIALIZE.H
*/

#ifndef __RECORDED_ANIMATION_INITIALIZE_H
#define __RECORDED_ANIMATION_INITIALIZE_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct unit_control_data
{
	char animation_state;
	char aiming_speed;
	unsigned short control_flags;
	short weapon_index;
	short grenade_index;
	short zoom_level;
	unsigned short pad;
	real_vector3d throttle;
	real primary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
};

/* ---------- prototypes/RECORDED_ANIMATION_INITIALIZE.C */

void recorded_animation_byteswap_unit_control(byte **playback_stream, byte unit_version);

void recorded_animation_initialize_unit_control(
	struct unit_control_data *control,
	byte **playback_stream,
	byte unit_version);

void recorded_animation_write_unit_control(
	struct unit_control_data const *control,
	byte **playback_stream,
	byte unit_version);

/* ---------- globals */

/* ---------- public code */

#endif // __RECORDED_ANIMATION_INITIALIZE_H
