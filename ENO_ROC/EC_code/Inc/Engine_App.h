#ifndef ENGINE_APP_H
#define ENGINE_APP_H

#include <stdint.h>
#include <stdbool.h>


// PDM States
typedef enum {
	INITIALISATION, //0
	STOPPED, 		//1
    STANDBY,		//2
    CRANKING,		//3
	CRANK_REST,		//4
	RUNNING,		//5
	SPINDOWN		//6
} engine_state_t;

typedef enum {
    START,			//0
    STOP,			//1
    EMERGENCY_STOP,	//2
	NO_ACTION		//3
} engine_action_t;

// External variables for UI access
extern engine_state_t engine_state;
extern engine_action_t engine_action;
extern uint8_t  Crank_Attempts;
extern bool CRANK;
extern bool FUEL;
extern bool LOW_OIL_PRESSURE;
extern bool HIGH_ENG_TEMP;
extern bool EXTERNAL_SHUTDOWN;
extern const char* engine_error;

// Function declarations
void engine_app(void);
void Stopped(void);
void Standby(void);
void Crank(void);
void Crank_Rest(void);
void Running(void);
void Spindown(void);
void Stop_Check(void);
void Null(void);

#endif




