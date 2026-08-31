/*
RECORDED_ANIMATION_INITIALIZE.C

symbols in this file:
00081D80 0070:
	_recorded_animation_byteswap_unit_control (0000)
00081DF0 0090:
	_recorded_animation_initialize_unit_control (0000)
00081E80 0070:
	_recorded_animation_write_unit_control (0000)
00258B44 000e:
	??_C@_0O@FLAIBBAO@real_vector3d?$AA@ (0000)
00258B54 000e:
	??_C@_0O@FKMKHLDJ@real_vector2d?$AA@ (0000)
002DCE10 0110:
	_data_002dce10 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cutscene/recorded_animation_initialize.h"
#include "memory/byte_swapping.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct unit_control_data_entry
{
	struct byte_swap_definition *bs_def;
	long size;
	long offset;
};

/* ---------- prototypes */

extern struct byte_swap_definition byte_bs_definition;
extern struct byte_swap_definition word_bs_definition;
extern struct byte_swap_definition long_bs_definition;

/* ---------- globals */

/* January anchors this whole .data run on its first object, so this one stays externally
   visible; the rest of the run is file static as in the original. */
byte_swap_code real_vector2d_bs_codes[] = { _4byte, _4byte };

static struct byte_swap_definition real_vector2d_bs_definition =
	{ "real_vector2d", sizeof(real_vector2d), real_vector2d_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

static byte_swap_code real_vector3d_bs_codes[] = { _4byte, _4byte, _4byte };

static struct byte_swap_definition real_vector3d_bs_definition =
	{ "real_vector3d", sizeof(real_vector3d), real_vector3d_bs_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE };

/* the recording stream carries only a 2d throttle; playback zeroes throttle.k */
static struct unit_control_data_entry unit_control_v1_map[] =
{
	{ &byte_bs_definition, sizeof(byte), offsetof(struct unit_control_data, animation_state) },
	{ &byte_bs_definition, sizeof(byte), offsetof(struct unit_control_data, aiming_speed) },
	{ &word_bs_definition, sizeof(short), offsetof(struct unit_control_data, control_flags) },
	{ &word_bs_definition, sizeof(short), offsetof(struct unit_control_data, weapon_index) },
	{ &word_bs_definition, sizeof(short), NONE },
	{ &real_vector2d_bs_definition, sizeof(real_vector2d), offsetof(struct unit_control_data, throttle) },
	{ &real_vector3d_bs_definition, sizeof(real_vector3d), offsetof(struct unit_control_data, facing_vector) },
	{ &real_vector3d_bs_definition, sizeof(real_vector3d), offsetof(struct unit_control_data, aiming_vector) },
	{ &real_vector3d_bs_definition, sizeof(real_vector3d), offsetof(struct unit_control_data, looking_vector) },
	{ NULL, NONE, NONE },
};

static struct unit_control_data_entry unit_control_v2_map[] =
{
	{ &long_bs_definition, sizeof(long), offsetof(struct unit_control_data, primary_trigger) },
	{ NULL, NONE, NONE },
};

static struct unit_control_data_entry unit_control_v3_map[] =
{
	{ &word_bs_definition, sizeof(short), offsetof(struct unit_control_data, grenade_index) },
	{ NULL, NONE, NONE },
};

static struct unit_control_data_entry unit_control_v4_map[] =
{
	{ &word_bs_definition, sizeof(short), offsetof(struct unit_control_data, zoom_level) },
	{ NULL, NONE, NONE },
};

static struct unit_control_data_entry *unit_control_data_map[] =
{
	unit_control_v1_map,
	unit_control_v2_map,
	unit_control_v3_map,
	unit_control_v4_map,
};

/* ---------- public code */

void recorded_animation_byteswap_unit_control(byte **playback_stream, byte unit_version)
{
	short version_index;

	for (version_index = 0; version_index < MAX(unit_version, 1); version_index++)
	{
		struct unit_control_data_entry *entry = unit_control_data_map[version_index];

		while (entry->size != NONE)
		{
			byte_swap_data(entry->bs_def, *playback_stream, 1);
			*playback_stream += entry->size;
			entry++;
		}
	}
}

void recorded_animation_initialize_unit_control(
	struct unit_control_data *control,
	byte **playback_stream,
	byte unit_version)
{
	short version_index;

	csmemset(control, 0, sizeof(*control));
	control->zoom_level = NONE;

	for (version_index = 0; version_index < MAX(unit_version, 1); version_index++)
	{
		struct unit_control_data_entry *entry = unit_control_data_map[version_index];

		while (entry->size != NONE)
		{
			if (entry->offset != NONE)
				csmemcpy((byte *)control + entry->offset, *playback_stream, entry->size);

			*playback_stream += entry->size;
			entry++;
		}
	}
}

void recorded_animation_write_unit_control(
	struct unit_control_data const *control,
	byte **playback_stream,
	byte unit_version)
{
	short version_index;

	for (version_index = 0; version_index < MAX(unit_version, 1); version_index++)
	{
		struct unit_control_data_entry *entry = unit_control_data_map[version_index];

		while (entry->size != NONE)
		{
			csmemcpy(*playback_stream, (byte const *)control + entry->offset, entry->size);
			*playback_stream += entry->size;
			entry++;
		}
	}
}

/* ---------- private code */
