#ifndef __GAME_ALLEGIANCE_H
#define __GAME_ALLEGIANCE_H
#pragma once

boolean game_team_is_enemy(
	short our_team_index,
	short other_team_index);
void game_allegiance_dispose(
	void);
void game_allegiance_dispose_from_old_map(
	void);

#endif // __GAME_ALLEGIANCE_H
