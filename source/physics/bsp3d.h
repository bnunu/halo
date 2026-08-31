/*
BSP3D.H

header included in hcex build.
*/

#ifndef __BSP3D_H
#define __BSP3D_H
#pragma once


/* ---------- headers */

#include "tag_files/tag_groups.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bsp3d
{
	struct tag_block nodes;
	struct tag_block planes;
};

struct bsp3d_node
{
	long plane_designator;
	long children[2];
};

typedef char bsp3d_node_size_check[
	sizeof(struct bsp3d_node) == 0xC ? 1 : -1];

typedef void (*bsp3d_line_leaf_proc)(
	real_point3d const *point0,
	real_point3d const *point1,
	long leaf_index,
	void *user_data);

typedef void (*bsp3d_polygon_leaf_proc)(
	real_point3d const *vertices,
	short vertex_count,
	long leaf_index,
	long on_node_designator,
	void *user_data);

/* ---------- prototypes/BSP3D.C */

long bsp3d_test_point(struct bsp3d const *bsp, long node_index, union real_point3d const *point);

long bsp3d_clip_line_to_leaves(
	struct bsp3d const *bsp,
	long node_index,
	real_point3d const *p0,
	real_point3d const *p1,
	bsp3d_line_leaf_proc handler,
	void *user_data);

long bsp3d_clip_polygon_to_leaves(
	struct bsp3d const *bsp,
	long root_node_index,
	real_point3d const *vertices,
	short vertex_count,
	real epsilon,
	bsp3d_polygon_leaf_proc handler,
	void *user_data);

/* ---------- globals */

/* ---------- public code */

__inline real_plane3d *bsp3d_get_plane_from_designator(
	struct bsp3d const *bsp,
	long plane_designator,
	real_plane3d *result)
{
	real_plane3d* plane = TAG_BLOCK_GET_ELEMENT(&bsp->planes, plane_designator & LONG_MAX, real_plane3d);

	if (plane_designator<0)
	{
		plane3d_negate(plane, result);
	}
	else
	{
		*result = *plane;
	}

	return result;
}

#endif // __BSP3D_H
