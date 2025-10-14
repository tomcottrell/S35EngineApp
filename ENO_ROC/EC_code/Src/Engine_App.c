#include "PDM.h"
#include "IO_Mapping.h"
#include "Engine_App.h"
#include "main.h"
#include "frequency.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint16_t Crank_Disconnect = 1000;
uint16_t Crank_Time = 10000; //10 seconds
uint16_t Crank_Speed_Signal_Wait_Time = 5000; //5 seconds
uint16_t Crank_Rest_Time = 5000; // 5 seconds
uint8_t  Crank_Attempts = 0;
uint8_t  Set_Crank_Attempts = 1;
uint32_t slush_start_time = 0;

engine_state_t engine_state = STOPPED;
engine_action_t engine_action = NO_ACTION;

extern bool Output1_control;
extern bool Output2_control;

void engine_app()
{
	switch(engine_state){
	case INITIALISATION :
		engine_state = STOPPED;
	break;
	case STOPPED:
		Stopped();
	break;
	case STANDBY:

	break;
	case CRANK:
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
			engine_state = CRANK;
			Crank_Attempts = 0;
			slush_start_time = HAL_GetTick();
		}
		engine_action = NO_ACTION;
	}
// Demo for if Crank Was from key switch and not a display
//	if(frequency != 0)
//	{
//		engine_state = CRANK;
//		slush_start_time = HAL_GetTick();
//	}
}
// ============================================================================
// Engine Crank State Function
// ============================================================================
void Crank()
{
	Output1_control = 1; //CRANK
	Output2_control = 1; //FUEL
	// Can add speed signal count as well
	if(frequency > Crank_Disconnect)
	{
		Output1_control = 0; //CRANK
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
			Output1_control = 0; //CRANK
			Output2_control = 0; //FUEL
			Crank_Attempts = Crank_Attempts + 1;
			engine_state = CRANK_REST;
			slush_start_time = HAL_GetTick();
		}
	}
// Demo for if Crank Was from key switch and not a display
//	if(frequency < Crank_Disconnect)
//	{
//		slush_start_time = HAL_GetTick();
//	if(frequency < 1)
//		{
//		engine_state = SPINDOWN;
//		}
//	}
//	else
//	{
//		if((HAL_GetTick() - slush_start_time) >= 1000)  // 1s elapsed
//		{
//			engine_state = RUNNING;
//		    slush_start_time = HAL_GetTick();  // Restart
//		}
//		//slush updates automatically
//	}

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
				engine_state = CRANK;
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
//	if(frequency > Crank_Disconnect)
//	{
//		slush_start_time = HAL_GetTick();
//	}
//	else
//	{
//		if((HAL_GetTick() - slush_start_time) >= 2000)  // 2s elapsed
//		{
//			engine_state = SPINDOWN;
//		    slush_start_time = HAL_GetTick();  // Restart
//		}
//	}
}
// ============================================================================
// Engine Spindown State Function
// ============================================================================
void Spindown()
{
	Output1_control = 0; //CRANK
	Output2_control = 0; //FUEL
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
	if((engine_action == STOP) && (engine_state >= CRANK && engine_state <= RUNNING))
	{
		engine_state = SPINDOWN;
		slush_start_time = HAL_GetTick();
	}
	engine_action = NO_ACTION;
}
