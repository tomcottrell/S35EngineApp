#include <gui/screentesthome_screen/ScreenTestHomeView.hpp>

ScreenTestHomeView::ScreenTestHomeView() :
buttonHandlerObj(10, static_cast<Button*>(&button0),
					 static_cast<Button*>(&button1),
					 static_cast<Button*>(&button2),
					 static_cast<Button*>(&button3)) {
}

extern char firmware_version[];
#define VERSION_STRING_LENGTH		24			//Copied from main.h

void ScreenTestHomeView::setupScreen()
{
    ScreenTestHomeViewBase::setupScreen();

    Unicode::strncpy(textVersionBuffer, firmware_version, VERSION_STRING_LENGTH);
}

void ScreenTestHomeView::tearDownScreen()
{
    ScreenTestHomeViewBase::tearDownScreen();
}

/*
 * Virtual Action Handlers
 */
void ScreenTestHomeView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
      {
    	  case 0:
    		  //Button 0: Goto CAN test screen
    		  application().gotoScreenCANtestScreenNoTransition();
    		  break;
    	  case 1:
    		  //Button 1: Goto outputs test screen
    		  application().gotoScreenOutputsTestScreenNoTransition();
    		  break;
    	  case 2:
    		  //Button 2: Goto inputs test screen
    		  application().gotoScreenInputsTestScreenNoTransition();
    		  break;
    	  case 3:
    		  //Button 3: Goto LCD test screen
    		  application().gotoScreenLCDtestScreenNoTransition();
    		  break;
      }

  this->invalidate();
}

void ScreenTestHomeView::handleTickEvent()
{

	this->invalidate();
}
