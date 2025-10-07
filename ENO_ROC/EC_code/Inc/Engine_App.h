#ifndef ENGINE_APP_H
#define ENGINE_APP_H

#include <stdint.h>
#include <stdbool.h>

// PDM States
typedef enum {
    STOPPED,
    STANDBY,
    CRANK,
	CRANK_REST,
	RUNNING,
	SPINDOWN
} engine_state_t;

typedef enum {
    START,
    STOP,
    EMERGENCY_STOP,
	NO_ACTION
} engine_action_t;

// External variables for UI access
extern engine_state_t engine_state;
extern engine_action_t engine_action;

// Function declarations
void engine_app(void);
void Stopped(void);
void Standby(void);
void Crank(void);
void Crank_Rest(void);
void Running(void);
void Spindown(void);
void Null(void);

#endif




