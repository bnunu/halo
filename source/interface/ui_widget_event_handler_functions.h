/*
UI_WIDGET_EVENT_HANDLER_FUNCTIONS.H

header included in hcex build.
*/

#ifndef __UI_WIDGET_EVENT_HANDLER_FUNCTIONS_H
#define __UI_WIDGET_EVENT_HANDLER_FUNCTIONS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/UI_WIDGET_EVENT_HANDLER_FUNCTIONS.C */

boolean ui_widget_event_handler_function_invoke(
	struct widget_instance *widget,
	struct event_record *event,
	unsigned short function_index,
	boolean *widget_deleted);
void reset_last_player1_profile_index(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __UI_WIDGET_EVENT_HANDLER_FUNCTIONS_H
