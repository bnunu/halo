/*
HUD_NAV_POINTS.H

header included in hcex build.
*/

#ifndef __HUD_NAV_POINTS_H
#define __HUD_NAV_POINTS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/HUD_NAV_POINTS.C */

void hud_initialize_nav_points(
	void);
void hud_initialize_nav_points_for_new_map(
	void);
void hud_dispose_nav_points_from_old_map(
	void);
void hud_dispose_nav_points(
	void);
void hud_activate_nav_point_with_game_engine_flag(
	short nav_index,
	long player_index,
	short flag_index,
	real vertical_offset);
void hud_activate_nav_point_with_flag(
	short nav_index,
	long player_index,
	short flag_index,
	real vertical_offset);
void hud_activate_nav_point_with_object(
	short nav_index,
	long player_index,
	long object_index,
	real vertical_offset);
void hud_activate_team_nav_point_with_game_engine_flag(
	short nav_index,
	short team_index,
	short flag_index,
	real vertical_offset);
void hud_activate_team_nav_point_with_flag(
	short nav_index,
	short team_index,
	short flag_index,
	real vertical_offset);
void hud_activate_team_nav_point_with_object(
	short nav_index,
	short team_index,
	long object_index,
	real vertical_offset);
void hud_activate_global_nav_point_with_game_engine_flag(
	short nav_index,
	short flag_index,
	real vertical_offset);
void hud_deactivate_nav_point_with_game_engine_flag(
	long player_unit_index,
	short flag_index);
void hud_deactivate_nav_point_with_flag(
	long player_unit_index,
	short flag_index);
void hud_deactivate_nav_point_with_object(
	long player_unit_index,
	long object_index);
void hud_deactivate_team_nav_point_with_flag(
	short team_index,
	short flag_index);
void hud_deactivate_team_nav_point_with_object(
	short team_index,
	long object_index);
void hud_unit_activate_nav_point_with_flag(
	short nav_index,
	long unit_index,
	short flag_index,
	real vertical_offset);
void hud_unit_activate_nav_point_with_object(
	short nav_index,
	long unit_index,
	long object_index,
	real vertical_offset);
void hud_unit_deactivate_nav_point_with_flag(
	long unit_index,
	short flag_index);
void hud_unit_deactivate_nav_point_with_object(
	long unit_index,
	long object_index);
short hud_get_nav_point_render_type(
	short local_player_index,
	real_point3d const *head,
	real_point3d const *position,
	long reference_object_index);
void hud_render_nav_points(
	short local_player_index);
void hud_update_nav_point_local_player(
	short local_player_index);
void hud_update_nav_points(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __HUD_NAV_POINTS_H
