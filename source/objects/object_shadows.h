/*
OBJECT_SHADOWS.H

header included in hcex build.
*/

#ifndef __OBJECT_SHADOWS_H
#define __OBJECT_SHADOWS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct object_shadow
{
	real object_bounding_radius;
	real_rectangle3d bounds;
	short count;
	short unknown1;
};

/* ---------- prototypes/OBJECT_SHADOWS.C */

boolean object_build_shadow(
	long object_index,
	void const *context,
	struct object_shadow *shadow);

/* ---------- globals */

/* ---------- public code */

#endif // __OBJECT_SHADOWS_H
