/*
PHYSICS.C

symbols in this file:
0013FF60 0080:
	_pin_fraction (0000)
0013FFE0 00b0:
	_physics_get_features_in_sphere (0000)
00140090 0070:
	_code_00140090 (0000)
00140100 0110:
	_render_debug_physics (0000)
00140210 0100:
	_physics_instance_new (0000)
00140310 00a0:
	_physics_test_point (0000)
001403B0 0120:
	_physics_test_vector (0000)
001404D0 0150:
	_code_001404d0 (0000)
00140620 0100:
	_code_00140620 (0000)
00140720 0b80:
	_physics_compute_new (0000)
001412A0 0470:
	_code_001412a0 (0000)
00141710 0490:
	_code_00141710 (0000)
00141BA0 0180:
	_code_00141ba0 (0000)
00141D20 01b0:
	_code_00141d20 (0000)
00141ED0 07c0:
	_physics_update_new (0000)
00142690 1430:
	_code_00142690 (0000)
00143AC0 0260:
	_physics_update (0000)
0028BC20 008d:
	??_C@_0IN@ODNFDOFI@mass_point?9?$DOground_material_type@ (0000)
0028BCB0 0021:
	??_C@_0CB@OHGIJBLL@c?3?2halo?2SOURCE?2physics?2physics?4c@ (0000)
0028BCD4 0004:
	__real@45ea6000 (0000)
0028BCD8 0057:
	??_C@_0FH@DDOACPI@unit_definition?9?$DOunit?4blip_type?$DO@ (0000)
0028BD30 0015:
	??_C@_0BF@GMDJIBPD@model_instance_valid?$AA@ (0000)
0028BD48 0010:
	??_C@_0BA@LNKGLGKJ@rotated_forward?$AA@ (0000)
0028BD58 000b:
	??_C@_0L@GBBDLPMF@rotated_up?$AA@ (0000)
0028BD64 000f:
	??_C@_0P@OGJNHPDG@up?$CB?$DNrotated_up?$AA@ (0000)
0028BD74 0019:
	??_C@_0BJ@GEGCJHGH@forward?$CB?$DNrotated_forward?$AA@ (0000)
0028BD90 0004:
	__real@364c6ce0 (0000)
0028BD94 0004:
	__real@34a5b36e (0000)
0028BD98 0004:
	__real@3b33abb1 (0000)
0028BDA0 0008:
	__real@3f80000000000000 (0000)
0028BDA8 0012:
	??_C@_0BC@MDPNAJDL@?$CGangular_velocity?$AA@ (0000)
0028BDBC 0016:
	??_C@_0BG@NPNNHCFI@?$CGangular_acceleration?$AA@ (0000)
0028BDD4 0011:
	??_C@_0BB@IHCLBKLO@?$CGlinear_velocity?$AA@ (0000)
0028BDE8 0015:
	??_C@_0BF@LNOJMOGI@?$CGlinear_acceleration?$AA@ (0000)
0028BE00 001d:
	??_C@_0BN@FOPAIIBD@instance?9?$DOphysics?9?$DOmass?$DO0?40f?$AA@ (0000)
0028BE20 0018:
	??_C@_0BI@HOJEHDKE@?$CGobject?9?$DOobject?4forward?$AA@ (0000)
0028BE38 0013:
	??_C@_0BD@JHPMDDBM@?$CGobject?9?$DOobject?4up?$AA@ (0000)
0028BE4C 001c:
	??_C@_0BM@EKBNDJOG@?$CGtranslational_acceleration?$AA@ (0000)
0028BE68 000d:
	??_C@_0N@EMPCNLII@magic_torque?$AA@ (0000)
0028BE78 000c:
	??_C@_0M@LNGNIFCJ@magic_force?$AA@ (0000)
0030CEB4 002c:
	_global_gravity (0000)
	_global_water_density (0004)
	_global_air_density (0008)
	_global_physics_collision_depth (000c)
	_depths_of_hell (0010)
0045E01A 0001:
	_debug_physics_disable_penetration_freeze (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "physics.h"

#include "collision_features.h"
#include "objects/object_definitions.h"
#include "objects/objects.h"
#include "render/render_debug.h"
#include "units/vehicles.h"
#include "physics_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* NOTE: January's physics.c sees the whole vehicle runtime datum.  In this
   tree units/vehicles.h forward-declares struct vehicle_runtime_datum only
   and units/vehicles.c keeps the layout private, so the collision force and
   torque accumulators the physics update consumes are declared here.  These
   two definitions belong in units/vehicles.h; note also that the region at
   0x460..0x477 is two real_vector3d, not the real_quaternion + real_point2d
   that units/vehicles.c currently guesses. */

struct _vehicle_datum
{
	word flags;
	short unknown426;
	byte unknown428;
	byte unknown429;
	byte unknown42a;
	byte unknown42b;
	real unknown42c;
	real unknown430;
	real unknown434;
	real unknown438;
	real unknown43c;
	real unknown440;
	real unknown444;
	real unknown448;
	byte unknown44c[8];
	real_point3d hover_position;
	real_vector3d collision_force;
	real_vector3d collision_torque;
	long unknown478;
};

struct vehicle_runtime_datum
{
	long definition_index;
	struct _object_datum object;
	struct _unit_datum unit;
	struct _vehicle_datum vehicle;
};

struct physics_test_vector_result
{
	real t;
	real_plane3d plane;
};

/* ---------- prototypes */

void render_debug_vectors(
	boolean immediate,
	const real_point3d *point,
	const real_vector3d *forward,
	const real_vector3d *up,
	real size);

/* ---------- globals */

real global_gravity = 0.0035651792f;
real global_water_density = 1.0f;
real global_air_density = 0.0011f;
real global_physics_collision_depth = 0.2f;
long depths_of_hell = 0;

/* ---------- public code */

real pin_fraction(
	real value,
	real begin,
	real end)
{
	if (begin < end)
	{
		if (value <= begin)
			return 0.0f;
		else if (value >= end)
			return 1.0f;
		else
			return (value - begin) / (end - begin);
	}
	else
	{
		if (value <= end)
			return 1.0f;
		else if (value >= begin)
			return 0.0f;
		else
			return (begin - value) / (begin - end);
	}
}


boolean physics_get_features_in_sphere(
	struct physics_instance const *instance,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	struct collision_feature_list *features)
{
	short mass_point_index;
	real scaled_radius;

	for (mass_point_index = 0;
		mass_point_index < instance->physics->mass_points.count;
		mass_point_index++)
	{
		struct mass_point_definition const *mass_point = TAG_BLOCK_GET_ELEMENT(
			&instance->physics->mass_points,
			mass_point_index,
			struct mass_point_definition);
		real_point3d point;

		matrix4x3_transform_point(&instance->world_matrix, &mass_point->position, &point);

		scaled_radius = mass_point->radius * instance->world_matrix.scale;

		collision_features_from_point(
			&point,
			height,
			scaled_radius + width,
			instance->object_index,
			NONE,
			0,
			NONE,
			NONE,
			features);
	}

	return features->count[_collision_feature_sphere] ||
		features->count[_collision_feature_cylinder] ||
		features->count[_collision_feature_prism];
}

void render_debug_physics(
	struct physics_instance *instance)
{
	struct object_datum *object = object_get(instance->object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	real_point3d center_of_mass;
	short mass_point_index;

	matrix4x3_transform_point(&instance->world_matrix, &instance->physics->center_of_mass, &center_of_mass);

	render_debug_vectors(
		TRUE,
		&center_of_mass,
		&instance->world_matrix.forward,
		&instance->world_matrix.up,
		definition->object.bounding_radius);

	for (mass_point_index = 0;
		mass_point_index < instance->physics->mass_points.count;
		mass_point_index++)
	{
		struct mass_point_definition const *mass_point = TAG_BLOCK_GET_ELEMENT(
			&instance->physics->mass_points,
			mass_point_index,
			struct mass_point_definition);
		real_point3d position;
		real_vector3d forward;
		real_vector3d up;

		matrix4x3_transform_point(&instance->world_matrix, &mass_point->position, &position);
		matrix4x3_transform_normal(&instance->world_matrix, &mass_point->forward, &forward);
		matrix4x3_transform_normal(&instance->world_matrix, &mass_point->up, &up);

		render_debug_sphere(TRUE, &position, mass_point->radius, global_real_argb_white);
		render_debug_vectors(TRUE, &position, &forward, &up, mass_point->radius*0.5f);
	}
}

boolean physics_instance_new(
	struct physics_instance *instance,
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	real_point3d center_of_mass;

	if (definition->object.physics.index != NONE)
	{
		instance->object_index = object_index;
		instance->physics = physics_definition_get(definition->object.physics.index);

		instance->world_matrix.scale = 1.0f;
		object_get_origin(object_index, &instance->world_matrix.position);
		object_get_orientation(object_index, &instance->world_matrix.forward, &instance->world_matrix.up);
		cross_product3d(&instance->world_matrix.up, &instance->world_matrix.forward, &instance->world_matrix.left);

		set_real_point3d(&center_of_mass,
			-instance->physics->center_of_mass.x,
			-instance->physics->center_of_mass.y,
			-instance->physics->center_of_mass.z);
		matrix4x3_transform_point(&instance->world_matrix, &center_of_mass, &center_of_mass);
		instance->world_matrix.position = center_of_mass;

		return TRUE;
	}

	return FALSE;
}

boolean physics_test_point(
	struct physics_instance const *instance,
	real_point3d const *point)
{
	real_point3d local_point;
	struct physics_definition const *physics;
	short mass_point_index;

	matrix4x3_inverse_transform_point(&instance->world_matrix, point, &local_point);
	physics = instance->physics;

	for (mass_point_index = 0;
		mass_point_index < physics->mass_points.count;
		mass_point_index++)
	{
		struct mass_point_definition const *mass_point = TAG_BLOCK_GET_ELEMENT(
			&physics->mass_points,
			mass_point_index,
			struct mass_point_definition);

		if (point_in_sphere(&local_point, &mass_point->position, mass_point->radius))
			return TRUE;

		physics = instance->physics;
	}

	return FALSE;
}

boolean physics_test_vector(
	struct physics_instance const *instance,
	real_point3d const *point,
	real_vector3d const *vector,
	struct physics_test_vector_result *result)
{
	boolean hit = FALSE;
	real_point3d local_point;
	real_vector3d local_vector;
	real_vector3d normal;
	short mass_point_index;

	result->t = REAL_MAX;

	matrix4x3_inverse_transform_point(&instance->world_matrix, point, &local_point);
	matrix4x3_inverse_transform_vector(&instance->world_matrix, vector, &local_vector);

	for (mass_point_index = 0;
		mass_point_index < instance->physics->mass_points.count;
		mass_point_index++)
	{
		struct mass_point_definition const *mass_point = TAG_BLOCK_GET_ELEMENT(
			&instance->physics->mass_points,
			mass_point_index,
			struct mass_point_definition);
		real t;

		if (sphere_test_vector3d(
				&mass_point->position,
				mass_point->radius,
				&local_point,
				&local_vector,
				&t,
				&normal) &&
			result->t > t)
		{
			real_point3d intersection;

			result->t = t;
			plane3d_from_point_and_normal(
				&result->plane,
				point_from_line3d(&local_point, &local_vector, t, &intersection),
				&normal);
			hit = TRUE;
		}
	}

	if (hit)
		matrix4x3_transform_plane(&instance->world_matrix, &result->plane, &result->plane);

	return hit;
}


/* NOT YET EXACT: 1168/1168 bytes, 335 of 355 instruction slots identical.
   The only divergence is which physics_instance parameter the register
   allocator keeps in ESI across the two object_get_and_verify_type calls:
   January keeps instance_b (reloading instance_a from [ebp+8]), we keep
   instance_a.  Every source permutation tried - declaration order, splitting
   the initialisers into statements, commuting the mass product, caching
   either physics pointer in a local, const on either parameter - is byte
   inert.  Do not re-sweep those. */
boolean code_00141710(
	struct physics_instance const *instance_a,
	struct physics_instance const *instance_b)
{
	boolean collided = FALSE;
	struct vehicle_runtime_datum *object_a = vehicle_runtime_get(instance_a->object_index);
	struct vehicle_runtime_datum *object_b = vehicle_runtime_get(instance_b->object_index);
	real mass = square_root(instance_a->physics->mass * instance_b->physics->mass);
	real_vector3d total_force_a;
	real_vector3d total_force_b;
	real_vector3d total_torque_a;
	real_vector3d total_torque_b;
	short a_index;
	short b_index;

	set_real_vector3d(&total_force_a, 0.0f, 0.0f, 0.0f);
	set_real_vector3d(&total_force_b, 0.0f, 0.0f, 0.0f);
	set_real_vector3d(&total_torque_a, 0.0f, 0.0f, 0.0f);
	set_real_vector3d(&total_torque_b, 0.0f, 0.0f, 0.0f);

	for (a_index = 0;
		a_index < instance_a->physics->mass_points.count;
		a_index++)
	{
		struct mass_point_definition const *mass_point_a = TAG_BLOCK_GET_ELEMENT(
			&instance_a->physics->mass_points,
			a_index,
			struct mass_point_definition);
		real_point3d point_a;

		matrix4x3_transform_point(&instance_a->world_matrix, &mass_point_a->position, &point_a);

		for (b_index = 0;
			b_index < instance_b->physics->mass_points.count;
			b_index++)
		{
			struct mass_point_definition const *mass_point_b = TAG_BLOCK_GET_ELEMENT(
				&instance_b->physics->mass_points,
				b_index,
				struct mass_point_definition);
			real radius = mass_point_b->radius + mass_point_a->radius;
			real_point3d point_b;
			real_vector3d direction;
			real distance;

			matrix4x3_transform_point(&instance_b->world_matrix, &mass_point_b->position, &point_b);

			distance = normalize3d(vector_from_points3d(&point_a, &point_b, &direction));

			if (distance < radius && distance > 0.0f)
			{
				real depth = (radius - distance)*0.5f;
				real magnitude = global_gravity/global_physics_collision_depth*depth*mass*2.0f;
				real_vector3d force_a;
				real_vector3d force_b;
				real_vector3d torque_a;
				real_vector3d torque_b;
				real_point3d contact_point;
				real_vector3d offset_a;
				real_vector3d offset_b;

				scale_vector3d(&direction, -magnitude, &force_a);
				scale_vector3d(&direction, magnitude, &force_b);

				point_from_line3d(&point_a, &direction, mass_point_a->radius - depth, &contact_point);

				vector_from_points3d(&object_a->object.position, &contact_point, &offset_a);
				vector_from_points3d(&object_b->object.position, &contact_point, &offset_b);

				cross_product3d(&offset_a, &force_a, &torque_a);
				cross_product3d(&offset_b, &force_b, &torque_b);

				collided = TRUE;

				add_vectors3d(&force_a, &total_force_a, &total_force_a);
				add_vectors3d(&force_b, &total_force_b, &total_force_b);
				add_vectors3d(&torque_a, &total_torque_a, &total_torque_a);
				add_vectors3d(&torque_b, &total_torque_b, &total_torque_b);
			}
		}
	}

	if (collided)
	{
		add_vectors3d(&total_force_a, &object_a->vehicle.collision_force, &object_a->vehicle.collision_force);
		add_vectors3d(&total_torque_a, &object_a->vehicle.collision_torque, &object_a->vehicle.collision_torque);
		SET_FLAG(object_a->object.flags, _object_at_rest_bit, FALSE);

		if (!(instance_b->physics->radius > 0.0f))
		{
			add_vectors3d(&total_force_b, &object_b->vehicle.collision_force, &object_b->vehicle.collision_force);
			add_vectors3d(&total_torque_b, &object_b->vehicle.collision_torque, &object_b->vehicle.collision_torque);
			SET_FLAG(object_b->object.flags, _object_at_rest_bit, FALSE);
		}
	}

	return collided;
}

/* ---------- private code */
