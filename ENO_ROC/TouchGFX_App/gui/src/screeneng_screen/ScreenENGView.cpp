#include <gui/screeneng_screen/ScreenENGView.hpp>
#include "Engine_App.h"

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

	Unicode::snprintf(textActionBuffer, TEXTACTION_SIZE, "%d", engine_action);
	Unicode::snprintf(textStateBuffer, TEXTSTATE_SIZE, "%d",engine_state);

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


	if(++loop_count > 19)
		{
			loop_count = 0;
			this->invalidate();
		}
}
