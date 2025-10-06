#include <gui/screencantest_screen/ScreenCANtestView.hpp>


extern uint16_t j1939EngineSpeed;
extern uint16_t lastj1939EngineSpeed;

extern uint16_t j1939EngineOilPressure;
extern uint16_t lastj1939EngineOilPressure;

extern int16_t j1939EngineCoolantTemp;
extern int16_t lastj1939EngineCoolantTemp;

extern uint16_t j1939EngineLoad;
extern uint16_t lastj1939EngineLoad;

extern uint32_t can_tx_count;
extern uint32_t can_rx_count;


ScreenCANtestView::ScreenCANtestView() :
	buttonHandlerObj(10, static_cast<Button*>(&button0),
						 static_cast<Button*>(&button1),
						 static_cast<Button*>(&button2),
						 static_cast<Button*>(&button3)) {
}

void ScreenCANtestView::setupScreen()
{
    ScreenCANtestViewBase::setupScreen();
}

void ScreenCANtestView::tearDownScreen()
{
    ScreenCANtestViewBase::tearDownScreen();
}

/*
 * Virtual Action Handlers
 */
void ScreenCANtestView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
  	  {
      case 0:
    	  //button 0: Exit to home screen
    	  application().gotoScreenTestHomeScreenNoTransition();
    	  break;
      case 1:
    	  //button 1
    	  application().gotoScreenSystemScreenNoTransition();
    	  break;
      case 2:
    	  //button 2
    	  break;
      case 3:
    	  //button 3
    	  break;
      default:
    	  break;
    }

  this->invalidate();
}

void ScreenCANtestView::handleTickEvent()
{
	//Engine speed
		if(j1939EngineSpeed != 0xFFFF)
		{
			Unicode::snprintf(textEngSpeedBuffer,TEXTENGSPEED_SIZE, "%d", j1939EngineSpeed);
			imageProgressEngineSpeed.setValue(j1939EngineSpeed);
		}
		else
		{
			Unicode::snprintf(textEngSpeedBuffer,TEXTENGSPEED_SIZE, "----");
			imageProgressEngineSpeed.setValue(0);
		}


	//Engine Oil Pressure
		if(j1939EngineOilPressure != 0xFFFF)
		{
			Unicode::snprintf(textEngineOilPressureBuffer,TEXTENGINEOILPRESSURE_SIZE, "%d", j1939EngineOilPressure);
			imageProgressEngineOilPressure.setValue(j1939EngineOilPressure);
		}
		else
		{
			Unicode::snprintf(textEngineOilPressureBuffer,TEXTENGINEOILPRESSURE_SIZE, "----");
			imageProgressEngineOilPressure.setValue(0);
		}


	//Coolant temperature
		if(j1939EngineCoolantTemp != 0xFFFF)
		{
			Unicode::snprintf(textCoolantTempBuffer,TEXTCOOLANTTEMP_SIZE, "%d", j1939EngineCoolantTemp);
			imageProgressCoolantTemp.setValue(j1939EngineCoolantTemp + 40);
		}
		else
		{
			Unicode::snprintf(textCoolantTempBuffer,TEXTCOOLANTTEMP_SIZE, "----");
			imageProgressCoolantTemp.setValue(0);
		}

	//Engine load
		if(j1939EngineLoad != 0xFFFF)
		{
			Unicode::snprintf(textEngineLoadBuffer,TEXTENGINELOAD_SIZE, "%d", j1939EngineLoad);
			imageProgressEngineLoad.setValue(j1939EngineLoad);
		}
		else
		{
			Unicode::snprintf(textEngineLoadBuffer,TEXTENGINELOAD_SIZE, "----");
			imageProgressEngineLoad.setValue(0);
		}

	//CAN counters
	Unicode::snprintf(textRXcountBuffer,TEXTRXCOUNT_SIZE, "%d", can_rx_count);
	Unicode::snprintf(textTXcountBuffer,TEXTTXCOUNT_SIZE, "%d", can_tx_count);
	Unicode::snprintf(textTotalCountBuffer,TEXTTOTALCOUNT_SIZE, "%d", can_rx_count + can_tx_count);

	this->invalidate();
}
