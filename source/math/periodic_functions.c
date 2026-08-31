/*
PERIODIC_FUNCTIONS.C

symbols in this file:
000F9D90 0070:
	_periodic_functions_dispose (0000)
000F9E00 0130:
	_periodic_function_evaluate (0000)
000F9F30 0120:
	_transition_function_evaluate (0000)
000FA050 0100:
	_code_000fa050 (0000)
000FA150 0130:
	_code_000fa150 (0000)
000FA280 02b0:
	_code_000fa280 (0000)
000FA530 00c0:
	_periodic_functions_initialize (0000)
0027AC7C 000a:
	??_C@_09PFNFPFNK@very?5late?$AA@ (0000)
0027AC88 0005:
	??_C@_04KNPHFNKG@late?$AA@ (0000)
0027AC90 000b:
	??_C@_0L@JFLLBJBL@very?5early?$AA@ (0000)
0027AC9C 0006:
	??_C@_05MMFAAGKE@early?$AA@ (0000)
0027ACA4 0006:
	??_C@_05GILPFPMI@spark?$AA@ (0000)
0027ACAC 0007:
	??_C@_06EJKHKJBM@wander?$AA@ (0000)
0027ACB4 0007:
	??_C@_06NJLEAOMD@jitter?$AA@ (0000)
0027ACBC 0018:
	??_C@_0BI@LODCFJBP@slide?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027ACD4 0006:
	??_C@_05IODDPPOI@slide?$AA@ (0000)
0027ACDC 0020:
	??_C@_0CA@POBCGHPD@diagonal?5wave?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027ACFC 000e:
	??_C@_0O@JLGDFEJA@diagonal?5wave?$AA@ (0000)
0027AD0C 0019:
	??_C@_0BJ@LIPGHGNF@cosine?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027AD28 0007:
	??_C@_06IDPAJACI@cosine?$AA@ (0000)
0027AD30 0005:
	??_C@_04DDFHDMOL@zero?$AA@ (0000)
0027AD38 0004:
	??_C@_03OHDNLOHO@one?$AA@ (0000)
0027AD3C 0029:
	??_C@_0CJ@OKNGCK@c?3?2halo?2SOURCE?2math?2periodic_fun@ (0000)
0027AD68 0004:
	__real@41cccccd (0000)
0027AD6C 003f:
	??_C@_0DP@JEKDFHEF@function_type?$DO?$DN0?5?$CG?$CG?5function_typ@ (0000)
0027ADAC 0004:
	__real@447fc000 (0000)
0027ADB0 0041:
	??_C@_0EB@CENGDIOD@function_type?$DO?$DN0?5?$CG?$CG?5function_typ@ (0000)
0027ADF4 0004:
	__real@3cce16a7 (0000)
0027ADF8 0004:
	__real@3d002d4f (0000)
0027ADFC 0004:
	__real@3d377812 (0000)
0027AE00 0008:
	__real@4000000000000000 (0000)
0027AE08 0008:
	__real@3fd0000000000000 (0000)
0027AE10 0004:
	__real@3a802008 (0000)
0027AE14 0004:
	__real@422fede0 (0000)
0027AE18 0004:
	__real@41c90fdb (0000)
0027AE1C 0004:
	__real@3f65c8fa (0000)
0027AE20 0004:
	__real@41e00000 (0000)
0027AE24 0004:
	__real@3cdfffff (0000)
0027AE28 001d:
	??_C@_0BN@IGAJMLPO@?$CBfunction_tables_initialized?$AA@ (0000)
0030791C 0060:
	_data_0030791c (0000)
	_global_periodic_functions_enum (0030)
	_global_transition_functions_enum (0054)
004561BC 004c:
	_bss_004561bc (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "periodic_functions.h"

/* ---------- constants */

enum
{
	_periodic_function_one = 0,
	_periodic_function_zero,
	_periodic_function_cosine,
	_periodic_function_cosine_variable_period,
	_periodic_function_diagonal_wave,
	_periodic_function_diagonal_wave_variable_period,
	_periodic_function_slide,
	_periodic_function_slide_variable_period,
	_periodic_function_noise,
	_periodic_function_jitter,
	_periodic_function_wander,
	_periodic_function_spark,
	NUMBER_OF_PERIODIC_FUNCTIONS,
};

enum
{
	PERIODIC_FUNCTION_TABLE_SIZE = 1024,
	PERIODIC_FUNCTION_TABLE_MASK = PERIODIC_FUNCTION_TABLE_SIZE-1,
};

/* ---------- macros */

/* ---------- structures */

struct tag_enum_definition
{
	long count;
	char **names;
	void *unused;
};

struct periodic_functions_globals
{
	boolean function_tables_initialized;
	byte pad[3];
	byte *transition_function_tables[NUMBER_OF_TRANSITION_FUNCTIONS];
	byte *periodic_function_tables[NUMBER_OF_PERIODIC_FUNCTIONS];
};

/* ---------- prototypes */

static void code_000fa050(real *values);
static void code_000fa150(byte *table, short function_type);
void code_000fa280(short function_type, byte *table);

__inline long fast_ftol(
	real d)
{
	long result;

	__asm
	{
		fld d
		fistp result
	}

	return result;
}

/* ---------- globals */

char *data_0030791c[NUMBER_OF_PERIODIC_FUNCTIONS] =
{
	"one",
	"zero",
	"cosine",
	"cosine (variable period)",
	"diagonal wave",
	"diagonal wave (variable period)",
	"slide",
	"slide (variable period)",
	"noise",
	"jitter",
	"wander",
	"spark",
};

struct tag_enum_definition global_periodic_functions_enum =
{
	NUMBER_OF_PERIODIC_FUNCTIONS,
	data_0030791c,
	NULL,
};

static char *transition_function_names[NUMBER_OF_TRANSITION_FUNCTIONS] =
{
	"linear",
	"early",
	"very early",
	"late",
	"very late",
	"cosine",
};

struct tag_enum_definition global_transition_functions_enum =
{
	NUMBER_OF_TRANSITION_FUNCTIONS,
	transition_function_names,
	NULL,
};

struct periodic_functions_globals bss_004561bc = { 0 };

/* ---------- public code */

void periodic_functions_dispose(
	void)
{
	short function_index;

	if (bss_004561bc.function_tables_initialized)
	{
		for (function_index = 0; function_index < NUMBER_OF_PERIODIC_FUNCTIONS; function_index++)
		{
			match_free(
				"c:\\halo\\SOURCE\\math\\periodic_functions.c",
				122,
				bss_004561bc.periodic_function_tables[function_index]);
		}

		for (function_index = 0; function_index < NUMBER_OF_TRANSITION_FUNCTIONS; function_index++)
		{
			match_free(
				"c:\\halo\\SOURCE\\math\\periodic_functions.c",
				132,
				bss_004561bc.transition_function_tables[function_index]);
		}

		bss_004561bc.function_tables_initialized = FALSE;
	}

	return;
}

real periodic_function_evaluate(
	short function_type,
	real time)
{
	long index;
	byte *table;
	real fraction;
	real first_value;
	real second_value;
	real result;

	if (function_type == _periodic_function_one)
		return 1.0f;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		157,
		function_type>=0 && function_type<NUMBER_OF_PERIODIC_FUNCTIONS);

	if (bss_004561bc.function_tables_initialized)
	{
		time *= 25.6f;
		fraction = (real)fmod((double)time, 1.0);
		index = fast_ftol(time-fraction);
		table = bss_004561bc.periodic_function_tables[function_type];
		index &= PERIODIC_FUNCTION_TABLE_MASK;
		first_value = table[index] * (1.0f/255.0f);
		index = (index+1)&PERIODIC_FUNCTION_TABLE_MASK;
		second_value = table[index] * (1.0f/255.0f);

		if (((1 << function_type)&0xC0) != 0)
		{
			if (first_value > 0.75f && second_value < 0.25f)
				second_value += 1.0f;

			result = (1.0f-fraction)*first_value + second_value*fraction;
			if (result > 1.0f)
				return result-1.0f;

			return result;
		}

		return (1.0f-fraction)*first_value + second_value*fraction;
	}

	return 0.0f;
}

real transition_function_evaluate(
	short function_type,
	real value)
{
	long index;
	byte *table;
	real scaled;
	real fraction;
	real first_value;
	real second_value;

	if (value < 0.0f)
		value = 0.0f;
	else if (value > 1.0f)
		value = 1.0f;

	if (function_type == _transition_function_linear)
		return value;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		216,
		function_type>=0 && function_type<NUMBER_OF_TRANSITION_FUNCTIONS);

	if (bss_004561bc.function_tables_initialized)
	{
		table = bss_004561bc.transition_function_tables[function_type];
		scaled = value*(real)(PERIODIC_FUNCTION_TABLE_SIZE-1);
		fraction = (real)fmod((double)scaled, 1.0);
		index = fast_ftol(scaled-0.5f);
		if ((short)index == PERIODIC_FUNCTION_TABLE_SIZE-1)
			return table[PERIODIC_FUNCTION_TABLE_SIZE-1] * (1.0f/255.0f);

		first_value = table[(short)index] * (1.0f/255.0f);
		second_value = table[(short)index+1] * (1.0f/255.0f);

		return first_value*(1.0f-fraction) + second_value*fraction;
	}

	return 0.0f;
}

/* ---------- private code */

static void code_000fa050(
	real *values)
{
	long index;
	long count;
	real sum = 0.0f;
	real x;
	real value;
	real amplitudes[3];
	real cosine0;
	real cosine1;
	real cosine2;
	real *destination;

	index = 0;
	destination = values;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		*destination = sum;
		amplitudes[0] = real_seed_random(get_global_random_seed_address());
		amplitudes[1] = real_seed_random(get_global_random_seed_address());
		amplitudes[2] = real_seed_random(get_global_random_seed_address());
		x = (real)index;
		value = (real_seed_random(get_global_random_seed_address())+1.0f)*0.25f;
		cosine0 = (real)cos(x*0.044792242f);
		value = (cosine0+1.0f)*amplitudes[2] + value;
		cosine1 = (real)cos(x*0.03129321f);
		value = (cosine1+1.0f)*amplitudes[1] + value;
		cosine2 = (real)cos(x*0.025157286f);
		value = (cosine2+1.0f)*amplitudes[0] + value;
		sum += value;
		destination++;
		index++;
	}

	destination = values;
	for (index = 0; index < PERIODIC_FUNCTION_TABLE_SIZE; index++)
		*destination++ *= 1.0f/sum;

	return;
}

static void code_000fa150(
	byte *table,
	short function_type)
{
	long index;
	long count;
	real value;
	real result;

	index = 0;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		value = index * (1.0f/(PERIODIC_FUNCTION_TABLE_SIZE-1));
		switch (function_type)
		{
		case _transition_function_linear:
			result = value;
			break;
		case _transition_function_early:
			result = (real)pow((double)value, 0.5);
			break;
		case _transition_function_very_early:
			result = (real)pow((double)value, 0.25);
			break;
		case _transition_function_late:
			result = (real)pow((double)value, 2.0);
			break;
		case _transition_function_very_late:
			result = (real)pow((double)value, 4.0);
			break;
		case _transition_function_cosine:
			result = ((real)sin(value*3.1415927f-1.5707964f)+1.0f)*0.5f;
			break;
		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\math\\periodic_functions.c", 411, TRUE);
			system_exit(-1);
			break;
		}

		table[index] = (byte)PIN((long)(result*255.0f), 0, 255);
		index++;
	}

	return;
}

void code_000fa280(
	short function_type,
	byte *table)
{
	long index;
	long count;
	real random_values[PERIODIC_FUNCTION_TABLE_SIZE];
	real values[PERIODIC_FUNCTION_TABLE_SIZE];
	real minimum = 3.402823466e+38f;
	real maximum = -3.402823466e+38f;
	real x;
	real random_x;
	real result;
	real range;
	real *value;

	code_000fa050(random_values);
	index = 0;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		x = index*0.027343748f;
		random_x = random_values[index]*28.0f;
		switch (function_type)
		{
		case _periodic_function_one:
			result = 1.0f;
			break;
		case _periodic_function_zero:
			result = 0.0f;
			break;
		case _periodic_function_cosine:
			result = (real)cos(x*6.2831855f);
			break;
		case _periodic_function_cosine_variable_period:
			result = (real)cos(random_x*6.2831855f);
			break;
		case _periodic_function_slide:
			result = (real)fmod((double)x, 1.0);
			break;
		case _periodic_function_slide_variable_period:
			result = (real)fmod((double)random_x, 1.0);
			break;
		case _periodic_function_diagonal_wave:
			result = (real)fmod((double)x, 1.0);
			if (result < 0.5f)
				result *= 2.0f;
			else
				result = 1.0f-(result-0.5f)*2.0f;
			break;
		case _periodic_function_diagonal_wave_variable_period:
			result = (real)fmod((double)random_x, 1.0);
			if (result < 0.5f)
				result *= 2.0f;
			else
				result = 1.0f-(result-0.5f)*2.0f;
			break;
		case _periodic_function_noise:
			result = real_seed_random(get_global_random_seed_address());
			break;
		case _periodic_function_jitter:
		case _periodic_function_wander:
			result = ((real)cos(x*0.89759791f)*(real)cos(x*25.132742f) +
				(real)cos(x*43.9823f)*(real)sin(x*1.5707964f))*0.5f +
				(real)sin(x*3.1415927f)*(real)cos(x*6.2831855f);
			break;
		case _periodic_function_spark:
			result = (real)fmod((double)random_x, 1.0);
			result *= result;
			break;
		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\math\\periodic_functions.c", 499, TRUE);
			system_exit(-1);
			break;
		}

		if (result > maximum)
			maximum = result;
		if (result < minimum)
			minimum = result;
		values[index] = result;
		index++;
	}

	range = ((1 << function_type)&0xC0) != 0
		? 0.0f
		: maximum-minimum;
	value = values;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		if (range != 0.0f)
			result = (*value-minimum)/range;
		else
			result = *value;
		*table++ = (byte)PIN((long)(result*255.0f), 0, 255);
		value++;
	}

	return;
}

#define function_tables_initialized bss_004561bc.function_tables_initialized

void periodic_functions_initialize(
	void)
{
	short function_index;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		67,
		!function_tables_initialized);
	function_tables_initialized = TRUE;
	*get_global_random_seed_address() = 0x20F3F660;

	for (function_index = 0; function_index < NUMBER_OF_PERIODIC_FUNCTIONS; function_index++)
	{
		bss_004561bc.periodic_function_tables[function_index] = match_malloc(
			"c:\\halo\\SOURCE\\math\\periodic_functions.c",
			78,
			PERIODIC_FUNCTION_TABLE_SIZE);
		if (bss_004561bc.periodic_function_tables[function_index])
		{
			code_000fa280(
				function_index,
				bss_004561bc.periodic_function_tables[function_index]);
		}
		else
		{
			function_tables_initialized = FALSE;
		}
	}

	for (function_index = 0; function_index < NUMBER_OF_TRANSITION_FUNCTIONS; function_index++)
	{
		bss_004561bc.transition_function_tables[function_index] = match_malloc(
			"c:\\halo\\SOURCE\\math\\periodic_functions.c",
			96,
			PERIODIC_FUNCTION_TABLE_SIZE);
		if (bss_004561bc.transition_function_tables[function_index])
		{
			code_000fa150(
				bss_004561bc.transition_function_tables[function_index],
				function_index);
		}
		else
		{
			function_tables_initialized = FALSE;
		}
	}

	return;
}

#undef function_tables_initialized
