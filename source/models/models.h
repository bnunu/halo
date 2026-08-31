/*
MODELS.H

header included in hcex build.
*/

#ifndef __MODELS_H
#define __MODELS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

enum
{
	_render_model_immediate_bit = 0,
	_render_model_shadow_bit,
	_render_model_no_planar_fog_bit,
	_render_model_first_person_bit,
	NUMBER_OF_RENDER_MODEL_FLAGS
};

/* ---------- macros */

/* ---------- structures */

struct render_lighting;

/* ---------- prototypes/MODELS.C */

void model_interpolate_node_orientations(
	struct model const *model,
	struct real_orientation *original_node_orientations,
	struct real_orientation *target_node_orientations,
	short frame_index,
	short frame_count);
void model_get_node_orientations(
	struct model const *model,
	struct real_orientation *node_orientations);

short model_find_marker(long model_index, char const *name);
struct real_matrix4x3 *model_get_default_inverse_matrix(
	struct model *model,
	short node_index);
short model_find_node(
	long model_index,
	char const *name);
short model_get_marker_by_name(
	long model_index,
	char const *name,
	byte const *region_permutations,
	short const *node_remapping_table,
	short node_count, 
	struct real_matrix4x3 const *node_matrices,
	boolean mirrored_flag,
	struct object_marker *markers,
	short maximum_marker_count);
void model_build_tangent_matrices(
	struct model *model);
void model_node_matrices_from_orientations(
	struct model const *model,
	struct real_matrix4x3 *node_matrices,
	struct real_orientation const *node_orientations,
	union real_point3d const *position,
	union real_vector3d const *forward,
	union real_vector3d const *up);
void render_model(
	long model_index,
	real level_of_detail_pixels,
	struct real_matrix4x3 const *node_matrices,
	byte const *region_permutation_indices,
	union real_rgb_color const *change_colors,
	real const *function_values,
	struct render_lighting const *lighting,
	union real_point3d const *centroid,
	real radius,
	long model_effect,
	long unique_identifier,
	short forced_shader_permutation_index,
	unsigned long flags);

/* ---------- globals */

/* ---------- public code */

#endif // __MODELS_H
