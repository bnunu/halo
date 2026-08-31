/*
HALOAUTOTEST.H
*/

#ifndef __HALOAUTOTEST_H
#define __HALOAUTOTEST_H
#pragma once

/* ---------- prototypes/HALOAUTOTEST.C */

void HATCleanup(void);
void HATReadMain(void);
void HATRawRead(struct gamepad_state *gamepad);
void HATRawLoopRead(struct gamepad_state *gamepad);
void HATRawWrite(struct gamepad_state *gamepad);
void HATInit(void);
void HATRun(struct gamepad_state *gamepad);

#endif // __HALOAUTOTEST_H
