/*
OBJECT_LIGHTS.C

symbols in this file:
00128730 0030:
	_texture_cache_bitmap_load (0000)
00128760 0020:
	_code_00128760 (0000)
00128780 0040:
	_code_00128780 (0000)
001287C0 0060:
	_code_001287c0 (0000)
00128820 0100:
	_sample_lightmap (0000)
00128920 00f0:
	_sample_diffuse_texture (0000)
00128A10 0020:
	_code_00128a10 (0000)
00128A30 00c0:
	_lights_initialize (0000)
00128AF0 0010:
	_lights_dispose (0000)
00128B00 0030:
	_lights_initialize_for_new_map (0000)
00128B30 0020:
	_lights_dispose_from_old_map (0000)
00128B50 0010:
	_lights_enable (0000)
00128B60 0040:
	_light_delete (0000)
00128BA0 0060:
	_code_00128ba0 (0000)
00128C00 00d0:
	_object_get_self_illumination (0000)
00128CD0 0260:
	_light_particle (0000)
00128F30 0060:
	_light_disconnect_from_map (0000)
00128F90 00b0:
	_lights_disconnect_from_structure_bsp (0000)
00129040 0020:
	_light_attenuation (0000)
00129060 00a0:
	_code_00129060 (0000)
00129100 0020:
	_code_00129100 (0000)
00129120 0020:
	_code_00129120 (0000)
00129140 0040:
	_code_00129140 (0000)
00129180 0060:
	_code_00129180 (0000)
001291E0 0060:
	_code_001291e0 (0000)
00129240 0040:
	_code_00129240 (0000)
00129280 0110:
	_code_00129280 (0000)
00129390 00e0:
	_lights_queue_lens_flare (0000)
00129470 0230:
	_code_00129470 (0000)
001296A0 0400:
	_code_001296a0 (0000)
00129AA0 00f0:
	_code_00129aa0 (0000)
00129B90 00e0:
	_code_00129b90 (0000)
00129C70 01d0:
	_lights_render_diffuse (0000)
00129E40 0150:
	_lights_render_specular (0000)
00129F90 02d0:
	_lights_illumination_at_point (0000)
0012A260 0110:
	_lights_prepare_for_object_dynamic (0000)
0012A370 03b0:
	_lights_distant_lighting_at_point (0000)
0012A720 0280:
	_light_reconnect_to_map (0000)
0012A9A0 0060:
	_lights_reconnect_to_structure_bsp (0000)
0012AA00 00e0:
	_light_new (0000)
0012AAE0 00f0:
	_light_new_unattached (0000)
0012ABD0 0960:
	_lights_preprocess_scene (0000)
0012B530 0340:
	_lights_prepare_for_object_static (0000)
002896C0 00b0:
	_default_object_lighting (0000)
00289770 0021:
	??_C@_0CB@NDAECIIM@c?3?2halo?2source?2objects?2objects?4h@ (0000)
00289798 00b9:
	??_C@_0LJ@HECDDFBP@material?9?$DOlightmap_vertices?4type@ (0000)
00289854 0027:
	??_C@_0CH@GHJLLMCD@c?3?2halo?2SOURCE?2objects?2object_li@ (0000)
00289880 0095:
	??_C@_0JF@BFHNCHBA@material?9?$DOvertices?4type?$DN?$DN_raster@ (0000)
00289918 002c:
	??_C@_0CM@CCFOPHPG@couldn?8t?5allocate?5memory?5for?5obj@ (0000)
00289944 0014:
	??_C@_0BE@MMBBLMKK@lights_game_globals?$AA@ (0000)
00289958 000b:
	??_C@_0L@PNIBKHKO@light_data?$AA@ (0000)
00289964 000f:
	??_C@_0P@MLOLKFEJ@lights?5globals?$AA@ (0000)
00289974 0007:
	??_C@_06IDBEMGIK@lights?$AA@ (0000)
0028997C 003b:
	??_C@_0DL@OAOFOGFN@TEST_FLAG?$CIlight?9?$DOflags?0?5_point_l@ (0000)
002899B8 0023:
	??_C@_0CD@CEANBCDI@?$CBlights_globals?4marker_initializ@ (0000)
002899DC 0022:
	??_C@_0CC@DMOOLGHL@lights_globals?4marker_initialize@ (0000)
00289A00 0004:
	__real@3f34fdf4 (0000)
00289A04 003c:
	??_C@_0DM@NOKGCMAM@?$CBTEST_FLAG?$CIlight?9?$DOflags?0?5_point_@ (0000)
00289A40 002a:
	??_C@_0CK@JMHDKADE@lens_flare_parameters?4light_iden@ (0000)
00289A6C 0035:
	??_C@_0DF@MMLEIBDB@light?9?$DOcolor?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5ligh@ (0000)
00289AA4 0035:
	??_C@_0DF@KFPEGOCE@light?9?$DOcolor?4green?$DO?$DN0?40f?5?$CG?$CG?5ligh@ (0000)
00289ADC 0033:
	??_C@_0DD@NIPJINKE@light?9?$DOcolor?4red?5?$DO?$DN0?40f?5?$CG?$CG?5light@ (0000)
00289B10 0004:
	__real@bf3504f3 (0000)
0030B388 0605:
	_data_0030b388 (0000)
	_object_light_ambient_base (05f8)
	_object_light_ambient_scale (05fc)
	_object_light_secondary_scale (0600)
	_object_light_interpolate (0604)
00456E94 0004:
	_lights_game_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/light_definitions.h"
#include "objects/object_lights.h"
#include "objects/objects.h"
#include "render/render.h"
#include "render/render_debug.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "structures/cluster_partitions.h"
#include "structures/structure_bsp_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MAXIMUM_LIGHTS = 896,
};

enum
{
	LENS_FLARE_DEFINITION_TAG = 'lens',
};

enum
{
	_light_definition_dynamic_bit = 0,
	_light_definition_no_specular_bit,
	_light_definition_dont_light_own_object_bit,
	_light_definition_supersize_in_first_person_bit,
};

enum
{
	_point_light_dynamic_bit = 0,
	_point_light_connects_to_map_bit = 1,
	_point_light_connected_to_map_bit = 2,
};

/* ---------- macros */

#define light_get(index) \
	((struct light_datum *)datum_get(light_data, (index)))

#define light_definition_get(index) \
	((struct light_definition *)tag_get(LIGHT_DEFINITION_TAG, (index)))

/* ---------- structures */

struct lights_game_globals
{
	boolean render_lights;
	byte reserved01[3];
};

struct environment_vertex_compressed
{
	real_point3d position;
	unsigned long normal;
	unsigned long binormal;
	unsigned long tangent;
	real_point2d texcoord;
};

struct environment_lightmap_vertex_compressed
{
	unsigned long normal;
	unsigned long texcoord;
};

struct structure_surface
{
	word vertex_indices[3];
};

struct light_definition
{
	long flags;
	real radius;
	real radius_modifier_lower;
	real radius_modifier_upper;
	real falloff_angle;
	real cutoff_angle;
	real lens_flare_only_radius;
	real cosine_falloff_angle;
	real cosine_cutoff_angle;
	real radius_scale;
	real sine_cutoff_angle;
	byte reserved2C[0x80];
	struct tag_reference lens_flare;
};

struct light_datum
{
	struct datum_header header;
	unsigned short flags;
	long definition_index;
	long rasterizer_light_index;
	long marker;
	long cluster_reference;
	real_rgb_color color;
	byte reserved20[0xC];
	long object_index;
	real_point3d position;
	real_vector3d direction;
	real_vector3d up;
	real radius;
	long creation_time;
	short object_attachment_index;
	short object_function_index;
	union
	{
		short object_change_color_index;
		struct
		{
			real_point3d position;
			real_vector3d direction;
		} node;
	};
	real scale;
};

struct rasterizer_lens_flare_submit_parameters
{
	struct lens_flare_definition *definition;
	real_point3d position;
	unsigned long compressed_direction;
	unsigned long compressed_up;
	unsigned long compressed_light_color;
	short light_identifier;
	short light_index;
	short lens_flare_index;
	byte compressed_window_index;
	byte compressed_light_scale;
	long internal_occlusion_pixels;
};

struct lights_globals
{
	boolean marker_initialized;
	byte pad1[3];
	long marker;
	short scene_point_light_count;
	short padA;
	long scene_point_lights[128];
	struct rasterizer_lens_flare_submit_parameters queued_lens_flares[8];
	short queued_lens_flare_count;
	short pad34E;
};

typedef char verify_lights_game_globals_size[
	sizeof(struct lights_game_globals) == 0x4 ? 1 : -1];
typedef char verify_light_datum_flags_offset[
	offsetof(struct light_datum, flags) == 0x2 ? 1 : -1];
typedef char verify_light_datum_cluster_reference_offset[
	offsetof(struct light_datum, cluster_reference) == 0x10 ? 1 : -1];
typedef char verify_environment_vertex_compressed_size[
	sizeof(struct environment_vertex_compressed) == 0x20 ? 1 : -1];
typedef char verify_environment_lightmap_vertex_compressed_size[
	sizeof(struct environment_lightmap_vertex_compressed) == 0x8 ? 1 : -1];
typedef char verify_structure_material_vertices_offset[
	offsetof(struct structure_material, vertices) == 0xB0 ? 1 : -1];
typedef char verify_structure_material_compressed_vertex_data_offset[
	offsetof(struct structure_material, compressed_vertex_data) == 0xEC ? 1 : -1];
typedef char verify_light_definition_lens_flare_offset[
	offsetof(struct light_definition, lens_flare) == 0xAC ? 1 : -1];
typedef char verify_light_datum_size[
	sizeof(struct light_datum) == 0x7C ? 1 : -1];
typedef char verify_lights_globals_size[
	sizeof(struct lights_globals) == 0x350 ? 1 : -1];

/* ---------- prototypes */

struct bitmap_data;

boolean game_engine_allow_dynamic_lighting(
	void);
pixel32 bitmap_2d_get_pixel(
	struct bitmap_data *bitmap,
	real_point2d const *texcoord,
	real mipmap);
byte compress_real_to_int8(
	real value);
unsigned long compress_real_vector3d_to_int32_clamp(
	real_vector3d const *vector);
void environment_lightmap_vertex_compressed_get_texcoord(
	struct environment_lightmap_vertex_compressed const *vertex,
	real_point2d *texcoord);
void environment_vertex_compressed_get_texcoord(
	struct environment_vertex_compressed const *vertex,
	real_point2d *texcoord);
real real_rgb_color_brightness(
	real_rgb_color const *color);
void pixel32_to_real_rgb_color(
	pixel32 pixel,
	real_rgb_color *color);
pixel32 real_a_rgb_color_to_pixel32(
	real alpha,
	real_rgb_color const *color);
void profile_texture_start(
	void);
void profile_texture_end(
	void);
void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load);

/* ---------- globals */

extern boolean debug_lights;
extern struct data_array *light_data;
extern struct cluster_partition light_cluster_partition;
extern struct lights_game_globals *lights_game_globals;
static struct lights_globals lights_globals;

/* ---------- public code */

void *texture_cache_bitmap_load(
	struct bitmap_data *bitmap)
{
	void *hardware_format;

	profile_texture_start();
	hardware_format = _texture_cache_bitmap_get_hardware_format(
		bitmap,
		TRUE,
		TRUE);
	profile_texture_end();

	return hardware_format;
}

void lights_initialize(
	void)
{
	light_data = game_state_data_new(
		"lights",
		MAXIMUM_LIGHTS,
		sizeof(struct light_datum));
	lights_game_globals = game_state_malloc(
		"lights globals",
		NULL,
		sizeof(struct lights_game_globals));
	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0xC2,
		light_data);
	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0xC3,
		lights_game_globals);
	lights_game_globals->render_lights = TRUE;
	if (light_data)
	{
		cluster_partition_new(&light_cluster_partition, "light");
	}
	else
	{
		error(_error_silent, "couldn't allocate memory for object lights.");
	}

	return;
}

boolean code_00128a10(
	void)
{
	if (lights_game_globals->render_lights && game_engine_allow_dynamic_lighting())
		return TRUE;

	return FALSE;
}

void lights_dispose(
	void)
{
	cluster_partition_delete(&light_cluster_partition);

	return;
}

void lights_initialize_for_new_map(
	void)
{
	data_make_valid(light_data);
	lights_game_globals->render_lights = TRUE;
	cluster_partition_make_valid(&light_cluster_partition);

	return;
}

void lights_dispose_from_old_map(
	void)
{
	data_make_invalid(light_data);
	cluster_partition_make_invalid(&light_cluster_partition);

	return;
}

boolean lights_enable(
	boolean enable)
{
	lights_game_globals->render_lights = enable;

	return enable;
}

long light_new(
	long definition_index,
	long object_index,
	short object_attachment_index,
	short object_function_index,
	short object_change_color_index)
{
	struct light_definition *definition = light_definition_get(definition_index);
	long light_index = NONE;

	if (TEST_FLAG(definition->flags, _light_definition_dynamic_bit)
		|| definition->lens_flare.index != NONE)
	{
		light_index = datum_new(light_data);
		if (light_index != NONE)
		{
			struct light_datum *light = light_get(light_index);

			light->definition_index = definition_index;
			light->object_index = object_index;
			light->object_attachment_index = object_attachment_index;
			light->object_function_index = object_function_index;
			light->object_change_color_index = object_change_color_index;
			light->flags = 0;
			SET_FLAG(light->flags, _point_light_dynamic_bit,
				TEST_FLAG(definition->flags, _light_definition_dynamic_bit));
			SET_FLAG(light->flags, _point_light_connects_to_map_bit,
				TEST_FLAG(light->flags, _point_light_dynamic_bit)
				|| definition->lens_flare.index != NONE);
			light->cluster_reference = NONE;
			light->creation_time = NONE;
			light_reconnect_to_map(light_index);
			light->marker = lights_globals.marker - 1;
		}
	}

	return light_index;
}

long light_new_unattached(
	long definition_index,
	long object_index,
	short object_node_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real scale)
{
	long light_index = datum_new(light_data);

	if (light_index != NONE)
	{
		struct light_datum *light = light_get(light_index);
		struct light_definition *definition = light_definition_get(definition_index);

		light->flags = 0;
		light->creation_time = game_time_get();
		light->definition_index = definition_index;
		light->object_index = object_index;
		light->scale = scale;
		light->cluster_reference = NONE;
		SET_FLAG(light->flags, _point_light_dynamic_bit, TRUE);
		SET_FLAG(light->flags, _point_light_connects_to_map_bit, TRUE);
		if (object_index == NONE)
		{
			light->position = *position;
			light->direction = *direction;
		}
		else
		{
			light->object_attachment_index = object_node_index;
			light->node.position = *position;
			light->node.direction = *direction;
		}
		light_reconnect_to_map(light_index);
		light->marker = lights_globals.marker - 1;
	}

	return light_index;
}

void light_delete(
	long light_index)
{
	struct light_datum *light = datum_get(light_data, light_index);

	cluster_partition_disconnect(
		&light_cluster_partition,
		light_index,
		&light->cluster_reference);
	datum_delete(light_data, light_index);

	return;
}

void lights_queue_lens_flare(
	long lens_flare_definition_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real_vector3d const *up,
	real_rgb_color const *color,
	real scale)
{
	if (lights_globals.queued_lens_flare_count < MAXIMUM_QUEUED_LENS_FLARES
		&& (color->red != 0.0f || color->green != 0.0f || color->blue != 0.0f))
	{
		struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters =
			&lights_globals.queued_lens_flares[lights_globals.queued_lens_flare_count];

		lens_flare_parameters->compressed_light_color = real_a_rgb_color_to_pixel32(1.0f, color);
		lens_flare_parameters->compressed_light_scale = compress_real_to_int8(scale);
		lens_flare_parameters->definition = tag_get(LENS_FLARE_DEFINITION_TAG, lens_flare_definition_index);
		lens_flare_parameters->position = *position;
		lens_flare_parameters->compressed_direction = compress_real_vector3d_to_int32_clamp(direction);
		lens_flare_parameters->compressed_up = compress_real_vector3d_to_int32_clamp(up);
		lens_flare_parameters->compressed_window_index = (byte)render.window_index;
		lens_flare_parameters->light_index = NONE;
		lens_flare_parameters->light_identifier = NONE;
		lens_flare_parameters->lens_flare_index = lights_globals.queued_lens_flare_count;
		lights_globals.queued_lens_flare_count++;
	}

	return;
}

void light_disconnect_from_map(
	long light_index)
{
	struct light_datum *light = datum_get(light_data, light_index);

	if (TEST_FLAG(light->flags, _point_light_connects_to_map_bit))
	{
		match_assert(
			"c:\\halo\\SOURCE\\objects\\object_lights.c",
			0x4D0,
			TEST_FLAG(light->flags, _point_light_connected_to_map_bit));
		cluster_partition_disconnect(
			&light_cluster_partition,
			light_index,
			&light->cluster_reference);
		SET_FLAG(light->flags, _point_light_connected_to_map_bit, FALSE);
	}

	return;
}

void lights_disconnect_from_structure_bsp(
	void)
{
	long light_index;

	for (light_index = data_next_index(light_data, NONE);
		light_index != NONE;
		light_index = data_next_index(light_data, light_index))
	{
		struct light_datum *light = datum_get(light_data, light_index);

		if (TEST_FLAG(light->flags, _point_light_connected_to_map_bit))
		{
			light_disconnect_from_map(light_index);
			SET_FLAG(light->flags, _point_light_connected_to_map_bit, TRUE);
		}
	}

	return;
}

void lights_reconnect_to_structure_bsp(
	void)
{
	long light_index;

	for (light_index = data_next_index(light_data, NONE);
		light_index != NONE;
		light_index = data_next_index(light_data, light_index))
	{
		struct light_datum *light = datum_get(light_data, light_index);

		if (TEST_FLAG(light->flags, _point_light_connected_to_map_bit))
		{
			SET_FLAG(light->flags, _point_light_connected_to_map_bit, FALSE);
			light_reconnect_to_map(light_index);
		}
	}

	return;
}

void light_marker_begin(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x664,
		!lights_globals.marker_initialized);
	lights_globals.marker++;
	lights_globals.marker_initialized = TRUE;

	return;
}

boolean light_mark(
	long light_index)
{
	struct light_datum *light = light_get(light_index);

	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x67F,
		lights_globals.marker_initialized);
	if (light->marker != lights_globals.marker)
	{
		light->marker = lights_globals.marker;
		return TRUE;
	}

	return FALSE;
}

boolean code_00129180(
	long light_index)
{
	struct light_datum *light = light_get(light_index);

	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x66F,
		lights_globals.marker_initialized);

	if (light->marker != lights_globals.marker)
		return TRUE;

	return FALSE;
}

void light_marker_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x68E,
		lights_globals.marker_initialized);
	lights_globals.marker_initialized = FALSE;

	return;
}

/* ---------- private code */

void sample_lightmap(
	struct structure_material *material,
	struct bitmap_data *bitmap,
	struct structure_surface const *surface,
	real u,
	real v,
	real_rgb_color *color)
{
	real_point2d texcoords[3];
	real_point2d texcoord;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x8F,
		material->lightmap_vertices.type==_rasterizer_vertex_type_environment_lightmap_uncompressed
			|| material->lightmap_vertices.type==_rasterizer_vertex_type_environment_lightmap_compressed);
	environment_lightmap_vertex_compressed_get_texcoord(
		(struct environment_lightmap_vertex_compressed *)
			((struct environment_vertex_compressed *)material->compressed_vertex_data.address
			+ material->vertices.count) + surface->vertex_indices[0],
		&texcoords[0]);
	environment_lightmap_vertex_compressed_get_texcoord(
		(struct environment_lightmap_vertex_compressed *)
			((struct environment_vertex_compressed *)material->compressed_vertex_data.address
			+ material->vertices.count) + surface->vertex_indices[1],
		&texcoords[1]);
	environment_lightmap_vertex_compressed_get_texcoord(
		(struct environment_lightmap_vertex_compressed *)
			((struct environment_vertex_compressed *)material->compressed_vertex_data.address
			+ material->vertices.count) + surface->vertex_indices[2],
		&texcoords[2]);
	texcoord.x = (texcoords[1].x - texcoords[0].x) * u + (texcoords[2].x - texcoords[0].x) * v + texcoords[0].x;
	texcoord.y = (texcoords[2].y - texcoords[0].y) * v + (texcoords[1].y - texcoords[0].y) * u + texcoords[0].y;
	pixel32_to_real_rgb_color(bitmap_2d_get_pixel(bitmap, &texcoord, 1.0f), color);

	return;
}

void sample_diffuse_texture(
	struct structure_material *material,
	struct bitmap_data *bitmap,
	struct structure_surface const *surface,
	real u,
	real v,
	real_rgb_color *color)
{
	real_point2d texcoords[3];
	real_point2d texcoord;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0xA7,
		material->vertices.type==_rasterizer_vertex_type_environment_uncompressed
			|| material->vertices.type==_rasterizer_vertex_type_environment_compressed);
	environment_vertex_compressed_get_texcoord(
		(struct environment_vertex_compressed *)material->compressed_vertex_data.address
		+ surface->vertex_indices[0],
		&texcoords[0]);
	environment_vertex_compressed_get_texcoord(
		(struct environment_vertex_compressed *)material->compressed_vertex_data.address
		+ surface->vertex_indices[1],
		&texcoords[1]);
	environment_vertex_compressed_get_texcoord(
		(struct environment_vertex_compressed *)material->compressed_vertex_data.address
		+ surface->vertex_indices[2],
		&texcoords[2]);
	texcoord.x = (texcoords[1].x - texcoords[0].x) * u + (texcoords[2].x - texcoords[0].x) * v + texcoords[0].x;
	texcoord.y = (texcoords[2].y - texcoords[0].y) * v + (texcoords[1].y - texcoords[0].y) * u + texcoords[0].y;
	pixel32_to_real_rgb_color(bitmap_2d_get_pixel(bitmap, &texcoord, 0.3f), color);

	return;
}

real light_attenuation(
	real radius,
	real distance)
{
	return 1.0f - (distance * distance) / (radius * radius);
}

long code_00129100(
	long *reference_index,
	short cluster_index)
{
	return cluster_partition_get_first_datum(
		&light_cluster_partition,
		reference_index,
		cluster_index);
}

long code_00129120(
	long *reference_index)
{
	return cluster_partition_get_next_datum(
		&light_cluster_partition,
		reference_index);
}

real code_00128760(
	real a,
	real b,
	real c,
	real u,
	real v)
{
	return (b - a) * u + (c - a) * v + a;
}

void code_00129b90(
	long light_index,
	real_point3d *position,
	real *radius)
{
	struct light_datum *light = light_get(light_index);
	struct light_definition *definition = light_definition_get(light->definition_index);
	real light_radius = definition->radius_modifier_upper * definition->radius;

	if (!TEST_FLAG(definition->flags, _light_definition_no_specular_bit))
	{
		light_radius *= definition->radius_scale;
	}

	if (light_radius < definition->lens_flare_only_radius)
	{
		*position = light->position;
		*radius = definition->lens_flare_only_radius;
	}
	else if (definition->cutoff_angle < _pi / 2)
	{
		if (definition->cutoff_angle < _pi / 4)
		{
			light_radius /= definition->cosine_cutoff_angle;
			*radius = light_radius;
			position->x = light_radius * light->direction.i + light->position.x;
			position->y = light_radius * light->direction.j + light->position.y;
			position->z = light_radius * light->direction.k + light->position.z;
		}
		else
		{
			*radius = light_radius * definition->sine_cutoff_angle;
			light_radius *= definition->cosine_cutoff_angle;
			position->x = light_radius * light->direction.i + light->position.x;
			position->y = light_radius * light->direction.j + light->position.y;
			position->z = light_radius * light->direction.k + light->position.z;
		}
	}
	else
	{
		*position = light->position;
		*radius = light_radius;
	}

	return;
}

void light_reconnect_to_map(
	long light_index)
{
	struct light_datum *light = light_get(light_index);
	struct light_definition *definition = light_definition_get(light->definition_index);
	struct object_marker markers[1];
	struct location location;
	real_point3d position;
	real radius;

	if (light->creation_time == NONE)
	{
		object_get_marker_by_name(
			light->object_index,
			object_get_attachment_marker_name(light->object_index, light->object_attachment_index),
			markers,
			1);
		light->position = markers[0].matrix.position;
		light->direction = markers[0].matrix.forward;
		light->up = markers[0].matrix.up;
	}
	else if (object_try_and_get(light->object_index))
	{
		real_matrix4x3 *node_matrix = object_get_node_matrix(light->object_index, light->object_attachment_index);

		matrix4x3_transform_point(node_matrix, &light->node.position, &light->position);
		matrix4x3_transform_normal(node_matrix, &light->node.direction, &light->direction);
		perpendicular3d(&light->direction, &light->up);
		normalize3d(&light->up);
	}

	if (TEST_FLAG(light->flags, _point_light_connects_to_map_bit))
	{
		struct light_datum *point_light = light_get(light_index);
		struct light_definition *point_light_definition = light_definition_get(point_light->definition_index);
		real light_radius = point_light_definition->radius_modifier_upper * point_light_definition->radius;

		if (!TEST_FLAG(point_light_definition->flags, _light_definition_no_specular_bit))
		{
			light_radius *= point_light_definition->radius_scale;
		}

		if (light_radius < point_light_definition->lens_flare_only_radius)
		{
			position = point_light->position;
			radius = point_light_definition->lens_flare_only_radius;
		}
		else if (point_light_definition->cutoff_angle < _pi / 2)
		{
			if (point_light_definition->cutoff_angle < _pi / 4)
			{
				radius = light_radius / point_light_definition->cosine_cutoff_angle;
				position.x = radius * point_light->direction.i + point_light->position.x;
				position.y = radius * point_light->direction.j + point_light->position.y;
				position.z = radius * point_light->direction.k + point_light->position.z;
			}
			else
			{
				radius = light_radius * point_light_definition->sine_cutoff_angle;
				light_radius *= point_light_definition->cosine_cutoff_angle;
				position.x = light_radius * point_light->direction.i + point_light->position.x;
				position.y = light_radius * point_light->direction.j + point_light->position.y;
				position.z = light_radius * point_light->direction.k + point_light->position.z;
			}
		}
		else
		{
			position = point_light->position;
			radius = light_radius;
		}
		match_assert(
			"c:\\halo\\SOURCE\\objects\\object_lights.c",
			0x4F9,
			!TEST_FLAG(light->flags, _point_light_connected_to_map_bit));
		if (light->object_index != NONE && object_try_and_get(light->object_index))
		{
			object_get_location(light->object_index, &location);
		}
		else
		{
			scenario_location_from_point(&location, &position);
		}
		cluster_partition_reconnect(
			&light_cluster_partition,
			light_index,
			&light->cluster_reference,
			&position,
			radius,
			&location);
		SET_FLAG(light->flags, _point_light_connected_to_map_bit, TRUE);
	}

	return;
}

void code_00129280(
	long light_index)
{
	if (debug_lights)
	{
		struct light_datum *light = light_get(light_index);
		struct light_definition *definition = light_definition_get(light->definition_index);
		real light_radius = definition->radius_modifier_upper * definition->radius;
		real_argb_color color = *global_real_argb_orange;

		render_debug_sphere(TRUE, &light->position, definition->lens_flare_only_radius, global_real_argb_white);
		render_debug_sphere(TRUE, &light->position, light->radius, &color);
		color.red *= 0.8f;
		color.green *= 0.8f;
		color.blue *= 0.8f;
		if (!TEST_FLAG(definition->flags, _light_definition_no_specular_bit))
		{
			light_radius *= definition->radius_scale;
			render_debug_sphere(TRUE, &light->position, definition->radius_scale * light->radius, &color);
		}
		color.red *= 0.8f;
		color.green *= 0.8f;
		color.blue *= 0.8f;
		render_debug_sphere(TRUE, &light->position, light_radius, &color);
	}

	return;
}

void code_00129470(
	long object_index,
	short cluster_index,
	real_point3d const *center,
	real radius,
	long *light_indices,
	real *light_intensities,
	real *light_attenuations,
	short *light_count,
	short maximum_light_count)
{
	long light_index;
	long reference_index;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\object_lights.c",
		0x544,
		lights_globals.marker_initialized);
	for (light_index = cluster_partition_get_first_datum(&light_cluster_partition, &reference_index, cluster_index);
		light_index != NONE;
		light_index = cluster_partition_get_next_datum(&light_cluster_partition, &reference_index))
	{
		if (code_00129180(light_index))
		{
			struct light_datum *light = light_get(light_index);

			if (light->rasterizer_light_index != NONE
				&& (light->object_index != object_index
					|| !TEST_FLAG(light_definition_get(light->definition_index)->flags,
						_light_definition_dont_light_own_object_bit)))
			{
				real_point3d const *light_position = &light->position;
				real distance = distance3d(light_position, center);

				if (distance < radius + light->radius)
				{
					real attenuation = light_attenuation(light->radius, distance);
					real intensity = real_rgb_color_brightness(&light->color) * attenuation;
					short index;

					if (*light_count < maximum_light_count)
					{
						index = (*light_count)++;
					}
					else
					{
						real minimum_intensity = REAL_MAX;
						short dimmest_index = NONE;

						for (index = 0; index < *light_count; index++)
						{
							if (minimum_intensity > light_intensities[index])
							{
								minimum_intensity = light_intensities[index];
								dimmest_index = index;
							}
						}

						if (minimum_intensity < intensity)
						{
							index = dimmest_index;
						}
					}

					if (index < maximum_light_count)
					{
						light_indices[index] = light_index;
						light_intensities[index] = intensity;
						light_attenuations[index] = attenuation;
					}
				}
			}
			light_mark(light_index);
		}
	}

	return;
}

void lights_prepare_for_object_dynamic(
	long object_index,
	struct render_lighting *lighting)
{
	struct object_cluster_iterator iterator;
	real_point3d center;
	real radius;
	real light_intensities[MAXIMUM_RENDERED_POINT_LIGHTS];
	real light_attenuations[MAXIMUM_RENDERED_POINT_LIGHTS];
	short cluster_index;
	short light_index;

	object_get_bounding_sphere(object_index, &center, &radius);
	lighting->point_light_count = 0;
	light_marker_begin();
	for (cluster_index = object_get_first_cluster(&iterator, object_index);
		cluster_index != NONE;
		cluster_index = object_get_next_cluster(&iterator, object_index))
	{
		code_00129470(
			object_index,
			cluster_index,
			&center,
			radius,
			lighting->point_light_indices,
			light_intensities,
			light_attenuations,
			&lighting->point_light_count,
			MAXIMUM_RENDERED_POINT_LIGHTS);
	}
	light_marker_end();
	for (light_index = 0; light_index < lighting->point_light_count; light_index++)
	{
		lighting->point_light_indices[light_index] =
			light_get(lighting->point_light_indices[light_index])->rasterizer_light_index;
	}

	return;
}
