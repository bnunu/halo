/*
SOUND_CACHE.H

header included in hcex build.
*/

#ifndef __SOUND_CACHE_H
#define __SOUND_CACHE_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct sound_permutation;

/* ---------- prototypes/XBOX_SOUND_CACHE.C */

void _sound_cache_sound_request(
	struct sound_permutation *sound,
	boolean load,
	boolean reference,
	boolean block);

/* ---------- globals */

/* ---------- public code */

#endif // __SOUND_CACHE_H
