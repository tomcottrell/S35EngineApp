#include "PDM.h"
#include "IO_Mapping.h"
#include "Engine_App.h"
#include "main.h"
#include "frequency.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint16_t Crank_Disconnect = 700;
uint32_t slush_start_time = 0;

engine_state_t engine_state = STOPPED;
engine_action_t engine_action = NO_ACTION;


void engine_app()
{
	switch(engine_state){
	case STOPPED:
		Stopped();
	break;
	case STANDBY:

	break;
	case CRANK:
		Crank();
	break;
	case CRANK_REST:

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
}
// ============================================================================
// Engine Stopped State Function
// ============================================================================
void Stopped()
{
	if((frequency != 0) && (frequency != 0xFFFF))
	{
		engine_state = CRANK;
		slush_start_time = HAL_GetTick();
	}
}
// ============================================================================
// Engine Crank State Function
// ============================================================================
void Crank()
{

	if((frequency < Crank_Disconnect) || (frequency == 0xFFFF))
	{
		slush_start_time = HAL_GetTick();
	if(frequency < 1 || (frequency == 0xFFFF))
		{
		engine_state = SPINDOWN;
		}
	}
	else
	{
		if((HAL_GetTick() - slush_start_time) >= 1000)  // 1s elapsed
		{
			engine_state = RUNNING;
		    slush_start_time = HAL_GetTick();  // Restart
		}
		//slush updates automatically
	}

}
// ============================================================================
// Engine Running State Function
// ============================================================================
void Running()
{
	if((frequency > Crank_Disconnect) || (frequency == 0xFFFF))
	{
		slush_start_time = HAL_GetTick();
	}
	else
	{
		if((HAL_GetTick() - slush_start_time) >= 2000)  // 2s elapsed
		{
			engine_state = SPINDOWN;
		    slush_start_time = HAL_GetTick();  // Restart
		}
	}
}
// ============================================================================
// Engine Spindown State Function
// ============================================================================
void Spindown()
{
	if(frequency > 0 || (frequency != 0xFFFF))
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

