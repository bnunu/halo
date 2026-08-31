/*
PHYSICS_VARIABLES.C

symbols in this file:
00143D20 0070:
	_physics_variable_position_update (0000)
00143D90 00f0:
	_physics_variable_speed_update (0000)
00143E80 0080:
	_physics_variable_speed_update_seek (0000)
00143F00 0040:
	_physics_variable_update (0000)
00143F40 0060:
	_code_00143f40 (0000)
00143FA0 0080:
	_physics_variable_position_update_seek (0000)
00144020 0090:
	_physics_variable_update_seek (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"
#include "physics/physics_variables.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static real physics_variable_position_get_seek_direction(
	real position,
	struct physics_variable_position const *definition,
	boolean cyclical_position,
	real desired_position);

/* ---------- globals */

/* ---------- public code */

void
physics_variable_speed_update(
	real *speed,
	struct physics_variable_speed const *definition,
	real magnitude)
{
	real absolute_magnitude = (real)fabs(magnitude);
	real acceleration = absolute_magnitude * definition->acceleration;
	real deceleration = absolute_magnitude * definition->deceleration;

	if (magnitude > 0.f)
	{
		if (*speed <= -deceleration)
			*speed += deceleration;
		else if (*speed >= 0.f)
			*speed += acceleration;
		else
			*speed = (*speed / deceleration + 1.f) * acceleration;

		*speed = MIN(*speed, absolute_magnitude * definition->positive_scale);

		return;
	}

	if (magnitude < 0.f)
	{
		if (*speed >= deceleration)
			*speed -= deceleration;
		else if (*speed <= 0.f)
			*speed -= acceleration;
		else
			*speed = (*speed / deceleration - 1.f) * acceleration;

		*speed = MAX(-absolute_magnitude * definition->negative_scale, *speed);
	}

	return;
}

boolean
physics_variable_speed_update_seek(
	real *speed,
	struct physics_variable_speed const *definition,
	real desired_speed,
	real magnitude)
{
	boolean result = FALSE;

	if (*speed > desired_speed)
	{
		physics_variable_speed_update(speed, definition, -magnitude);
		if (*speed <= desired_speed)
		{
			*speed = desired_speed;
			result = TRUE;
		}
	}
	else if (*speed < desired_speed)
	{
		physics_variable_speed_update(speed, definition, magnitude);
		if (*speed >= desired_speed)
		{
			*speed = desired_speed;
			result = TRUE;
		}
	}
	else
	{
		result = TRUE;
	}

	return result;
}

void
physics_variable_position_update(
	real *position,
	struct physics_variable_position const *definition,
	boolean cyclical_position,
	real speed)
{
	*position += speed;

	if (*position < definition->minimum)
	{
		if (cyclical_position)
			*position += definition->maximum - definition->minimum;
		else
			*position = definition->minimum;

		return;
	}

	if (*position > definition->maximum)
	{
		if (cyclical_position)
			*position -= definition->maximum - definition->minimum;
		else
			*position = definition->maximum;
	}

	return;
}

boolean
physics_variable_position_update_seek(
	real *position,
	struct physics_variable_position const *definition,
	boolean cyclical_position,
	real desired_position,
	real speed)
{
	real direction = physics_variable_position_get_seek_direction(*position, definition, cyclical_position, desired_position);

	if (direction != 0.f)
	{
		physics_variable_position_update(position, definition, cyclical_position, direction * speed);
		if (physics_variable_position_get_seek_direction(*position, definition, cyclical_position, desired_position) == direction)
			return FALSE;
	}

	*position = desired_position;

	return TRUE;
}

void
physics_variable_update(
	real *position,
	real *speed,
	struct physics_variable const *definition,
	boolean cyclical_position,
	real magnitude)
{
	physics_variable_speed_update(speed, &definition->speed, magnitude);
	physics_variable_position_update(position, &definition->position, cyclical_position, *speed);

	return;
}

boolean
physics_variable_update_seek(
	real *position,
	real *speed,
	struct physics_variable const *definition,
	boolean cyclical_position,
	real desired_position,
	real magnitude)
{
	struct physics_variable const *parameters = definition;
	real direction = physics_variable_position_get_seek_direction(*position, &parameters->position, cyclical_position, desired_position);

	if (direction != 0.f)
	{
		physics_variable_speed_update(speed, &parameters->speed, direction * magnitude);
		physics_variable_position_update(position, &parameters->position, cyclical_position, *speed);
		if (physics_variable_position_get_seek_direction(*position, &parameters->position, cyclical_position, desired_position) == direction)
			return FALSE;
	}

	*position = desired_position;
	*speed = 0.f;

	return TRUE;
}

/* ---------- private code */

static real
physics_variable_position_get_seek_direction(
	real position,
	struct physics_variable_position const *definition,
	boolean cyclical_position,
	real desired_position)
{
	real direction = desired_position - position;

	if (direction != 0.f)
	{
		if (cyclical_position && fabs(direction) > (definition->maximum - definition->minimum) * 0.5f)
			direction = -direction;

		direction = direction > 0.f ? 1.f : -1.f;
	}

	return direction;
}
