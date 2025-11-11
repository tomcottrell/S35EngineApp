#include "PDM.h"
#include "IO_Mapping.h"
#include "Engine_App.h"
#include "main.h"
#include "frequency.h"
#include "Fault_Handling.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint16_t Crank_Disconnect = 1000;
uint16_t Crank_Time = 10000; //10 seconds
uint16_t Crank_Speed_Signal_Wait_Time = 5000; //5 seconds
uint16_t Crank_Rest_Time = 5000; // 5 seconds
uint8_t  Crank_Attempts = 0;
uint8_t  Set_Crank_Attempts = 3;
uint32_t slush_start_time = 0;
const char* engine_error ="NO FAULTS";

engine_state_t engine_state = STOPPED;
engine_action_t engine_action = NO_ACTION;

bool CRANK;
bool FUEL;

void engine_app()
{
	switch(engine_state){
	case INITIALISATION :
		engine_state = STOPPED;
		engine_action = NO_ACTION;
	break;
	case STOPPED:
		Stopped();
	break;
	case STANDBY:

	break;
	case CRANKING:
		Crank();
	break;
	case CRANK_REST:
		Crank_Rest();
	break;
	case RUNNING:
		Running();
	break;
	case SPINDOWN:
		Spindown();
	break;
	// Add the default case if no option is set
	}
	//add stop checks function
	Fault_Handling();
	Stop_Check();
}
// ============================================================================
// Engine Stopped State Function
// ============================================================================
void Stopped()
{
	if(frequency != 0)
	{
		engine_state = SPINDOWN;
		slush_start_time = HAL_GetTick();
	}
	else
	{
		if(engine_action == START)
		{
			engine_state = CRANKING;
			Crank_Attempts = 0;
			slush_start_time = HAL_GetTick();
		}
		engine_action = NO_ACTION;
	}
}
// ============================================================================
// Engine Crank State Function
// ============================================================================
void Crank()
{
	CRANK = 1;
	FUEL = 1;
	if(frequency > Crank_Disconnect)
	{
		CRANK = 0; //CRANK
		engine_action = NO_ACTION;
		engine_state = RUNNING;
	}
	else
	{
		if((HAL_GetTick() - slush_start_time) < Crank_Time)
		{

		}
		else
		{
			CRANK = 0; //CRANK
			FUEL = 0; //FUEL
			Crank_Attempts = Crank_Attempts + 1;
			engine_state = CRANK_REST;
			slush_start_time = HAL_GetTick();
		}
	}
}
// ============================================================================
// Engine Crank Rest State Function
// ============================================================================
void Crank_Rest()
{
	if((HAL_GetTick() - slush_start_time) < Crank_Rest_Time)
	{

	}
	else
	{
		if(frequency < 1)
		{
			if(Crank_Attempts >= Set_Crank_Attempts)
			{
				engine_state = SPINDOWN;
				slush_start_time = HAL_GetTick();
			}
			else
			{
				engine_state = CRANKING;
				slush_start_time = HAL_GetTick();
			}
		}
		else
		{
			engine_action = STOP;
			Crank_Attempts = 0;
		}
	}
}
// ============================================================================
// Engine Running State Function
// ============================================================================
void Running()
{
	if(engine_action == STOP)
	{
		engine_action = NO_ACTION;
		engine_state = SPINDOWN;
		slush_start_time = HAL_GetTick();
	}
}
// ============================================================================
// Engine Spindown State Function
// ============================================================================
void Spindown()
{
	CRANK = 0; //CRANK
	FUEL = 0; //FUEL
	if(frequency > 0)
	{
		slush_start_time = HAL_GetTick();
	}
	else
	{
		if((HAL_GetTick() - slush_start_time) >= 3000)  // 3s elapsed
		{
			engine_state = STOPPED;
		    slush_start_time = HAL_GetTick();  // Restart
		}
	}
}

void Standby()
{

}

void Stop_Check()
{
	//if(frequency > 2000)
	//{
	//	engine_action = STOP;
	//	engine_error = "OVERSPEED";
	//
	//}
	//else if(frequency < Crank_Disconnect)
	//{
	//	engine_action = STOP;
	//	engine_error = "UNDERSPEED";
	//}
	//else
	//{
	//	engine_error = "NO FAULT";
	//}
	if((engine_action == STOP) && (engine_state >= CRANKING && engine_state <= RUNNING))
	{
		engine_state = SPINDOWN;
		slush_start_time = HAL_GetTick();
	}
	engine_action = NO_ACTION;
}
