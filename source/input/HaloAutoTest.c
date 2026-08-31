/*
HALOAUTOTEST.C
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "HaloAutoTest.h"
#include "integer_math.h"
#include "input.h"

/* ---------- constants */

enum
{
	_HAT_mode_write = 3,
	_HAT_mode_read,
	_HAT_mode_loop
};

/* ---------- globals */

static struct
{
	HANDLE file;
	unsigned long mode;
} HAT_globals;

/* ---------- public code */

void HATCleanup(
	void)
{
	if (HAT_globals.file)
	{
		CloseHandle(HAT_globals.file);
	}

	return;
}

void HATReadMain(
	void)
{
	if (GetFileAttributesA("d:\\write.xts")!=-1)
	{
		HAT_globals.mode = _HAT_mode_write;
	}
	else if (GetFileAttributesA("d:\\read.xts")!=-1)
	{
		HAT_globals.mode = _HAT_mode_read;
	}
	else if (GetFileAttributesA("d:\\loop.xts")!=-1)
	{
		HAT_globals.mode = _HAT_mode_loop;
	}

	return;
}

void HATRawRead(
	struct gamepad_state *gamepad)
{
	DWORD number_of_bytes_read = 0;
	ReadFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_read, 0);

	return;
}

void HATRawLoopRead(
	struct gamepad_state *gamepad)
{
	DWORD number_of_bytes_read = 0;
	ReadFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_read, 0);

	if (number_of_bytes_read==0)
	{
		SetFilePointer(HAT_globals.file, 0, 0, 0);
		ReadFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_read, 0);
	}

	return;
}

void HATRawWrite(
	struct gamepad_state *gamepad)
{
	DWORD number_of_bytes_written = 0;
	WriteFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_written, 0);

	return;
}

void HATInit(
	void)
{
	HATReadMain();

	if (HAT_globals.mode==_HAT_mode_write)
	{
		HAT_globals.file = CreateFileA(
			"d:\\state.data",
			GENERIC_WRITE,
			0,
			0,
			CREATE_ALWAYS,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);
	}
	else if (HAT_globals.mode==_HAT_mode_read||HAT_globals.mode==_HAT_mode_loop)
	{
		HAT_globals.file = CreateFileA(
			"d:\\state.data",
			GENERIC_READ,
			0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);
	}

	return;
}

void HATRun(
	struct gamepad_state *gamepad)
{
	switch (HAT_globals.mode)
	{
	case _HAT_mode_write:
	{
		DWORD number_of_bytes_written = 0;
		WriteFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_written, 0);
		break;
	}
	case _HAT_mode_read:
	{
		DWORD number_of_bytes_read = 0;
		ReadFile(HAT_globals.file, gamepad, sizeof(struct gamepad_state), &number_of_bytes_read, 0);
		break;
	}
	case _HAT_mode_loop:
	{
		HATRawLoopRead(gamepad);
		break;
	}
	default:
		break;
	}

	return;
}
