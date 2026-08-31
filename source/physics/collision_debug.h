/*
COLLISION_DEBUG.H

header included in hcex build.
*/

#ifndef __COLLISION_DEBUG_H
#define __COLLISION_DEBUG_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/COLLISION_DEBUG.C */

void collision_debug_render(
	void);

/* ---------- globals */

extern const real_point3d collision_debug_cube_vertices[NUMBER_OF_VERTICES_PER_CUBE];

extern boolean collision_debug_flag_front_facing_surfaces;
extern boolean collision_debug_flag_ignore_invisible_surfaces;
extern boolean collision_debug_flag_structure;
extern boolean collision_debug_flag_media;
extern boolean collision_debug_flag_objects;
extern real collision_debug_length;
extern long collision_debug_ignore_object_index;

extern real collision_debug_width;
extern real collision_debug_height;
extern boolean collision_debug_phantom_bsp;
extern boolean collision_debug_phantom_bsp_found;

/* ---------- public code */

#endif // __COLLISION_DEBUG_H
