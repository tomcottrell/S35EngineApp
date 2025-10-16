#include <gui/screeneng_screen/ScreenENGView.hpp>
#include "Engine_App.h"
#include "Fault_Handling.h"

extern uint16_t frequency;

ScreenENGView::ScreenENGView():
		buttonHandlerObj(10, 	static_cast<Button*>(&button0),
								static_cast<Button*>(&button1),
								static_cast<Button*>(&button2),
								static_cast<Button*>(&button3))
{

}

void ScreenENGView::setupScreen()
{
    ScreenENGViewBase::setupScreen();
}

void ScreenENGView::tearDownScreen()
{
    ScreenENGViewBase::tearDownScreen();
}

void ScreenENGView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
    {
  	  case 0:
  		  //button 0: Exit
      	  break;
      case 1:
      	  //button 1:
      	  break;
      case 2:
      	  //button 2:
      	  break;
      case 3:
      	  //button 3
      	  break;
      default:
      	  break;
    }
  this->invalidate();

}

void ScreenENGView::handleTickEvent()
{
	static uint8_t loop_count;

	Unicode::strncpy(textActionBuffer, engine_action_to_string(), TEXTACTION_SIZE);
	Unicode::strncpy(textStateBuffer, engine_state_to_string(), TEXTSTATE_SIZE);

	//Frequency
	if(frequency != 0xFFFF)
	{
		//frequency valid: display
		Unicode::snprintf(textFrequencyBuffer,TEXTFREQUENCY_SIZE, "%d", frequency);
	}
	else{
		//Frequency invalid
		Unicode::snprintf(textFrequencyBuffer,TEXTFREQUENCY_SIZE, "----");
	}

	Unicode::snprintf(textFuelBuffer,TEXTFUEL_SIZE, "%d", FUEL);
	Unicode::snprintf(textCrankBuffer,TEXTCRANK_SIZE, "%d", CRANK);
	Unicode::snprintf(textCrankAttemptsBuffer,TEXTCRANKATTEMPTS_SIZE, "%d", Crank_Attempts);
	//Unicode::strncpy(textErrorBuffer, engine_error, TEXTERROR_SIZE);
	Unicode::snprintf(textFaultBuffer,TEXTFAULT_SIZE, "%d", fault_array[0].error_code);
	Unicode::snprintf(textLampBuffer,TEXTLAMP_SIZE, "%d", fault_array[0].lamp);
	Unicode::snprintf(textLampBuffer,TEXTLAMP_SIZE, "%d", fault_count);


	if(++loop_count > 19)
		{
			loop_count = 0;
			this->invalidate();
		}
}


const char* ScreenENGView::engine_state_to_string()
{
    switch(engine_state)
    {
        case STOPPED: return "STOPPED";
        case STANDBY: return "STANDBY";
        case CRANKING: return "CRANKING";
        case CRANK_REST: return "CRANK_REST";
        case RUNNING: return "RUNNING";
        case SPINDOWN: return "SPINDOWN";
        default: return "UNKNOWN";
    }
}

const char* ScreenENGView::engine_action_to_string()
{
    switch(engine_action)
    {
        case START: return "START";
        case STOP: return "STOP";
        case EMERGENCY_STOP: return "EMERGENCY_STOP";
        case NO_ACTION: return "NO_ACTION";
        default: return "UNKNOWN";
    }
}

void ScreenENGView::Start()
{
	engine_action = START;
}

void ScreenENGView::Stop()
{
	engine_action = STOP;
}
