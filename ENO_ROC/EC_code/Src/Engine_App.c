#include "PDM.h"
#include "IO_Mapping.h"
#include "Engine_App.h"
#include "main.h"
#include "frequency.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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

	break;
	case CRANK_REST:

	break;
	case RUNNING:
	Running();
	break;
	case SPINDOWN:

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
	if((frequency > 600) && (frequency != 0xFFFF))
	{
		engine_state = RUNNING;
	}
}
// ============================================================================
// Engine Crank State Function
// ============================================================================
void Crank()
{

}
// ============================================================================
// Engine Running State Function
// ============================================================================
void Running()
{
	if((frequency < 600) || (frequency == 0xFFFF))
	{
		engine_state = STOPPED;
	}
}
// ============================================================================
// Engine Stopped State Function
// ============================================================================
