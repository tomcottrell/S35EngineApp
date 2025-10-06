#include <gui/screeninputstest_screen/ScreenInputsTestView.hpp>

extern uint8_t analog_config[2];
extern float SupplyVoltage;
extern float analogInput1value;
extern float analogInput2value;
extern uint16_t frequency;


ScreenInputsTestView::ScreenInputsTestView() :
	buttonHandlerObj(10, static_cast<Button*>(&button0),
	                     static_cast<Button*>(&button1),
	                     static_cast<Button*>(&button2),
	                     static_cast<Button*>(&button3)) {
}

void ScreenInputsTestView::setupScreen()
{
    ScreenInputsTestViewBase::setupScreen();
}

void ScreenInputsTestView::tearDownScreen()
{
    ScreenInputsTestViewBase::tearDownScreen();
}

/*
 * Virtual Action Handlers
 */
void ScreenInputsTestView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
    {
  	  case 0:
  		  //button 0: Exit
  		  application().gotoScreenTestHomeScreenNoTransition();
      	  break;
      case 1:
      	  //button 1: Analog input1 config change
    	  if(++analog_config[0] > 3)
    		  analog_config[0] = 0;
      	  break;
      case 2:
      	  //button 2: Analog input2 config change
    	  if(++analog_config[1] > 3)
    		  analog_config[1] = 0;
      	  break;
      case 3:
      	  //button 3
      	  break;
      default:
      	  break;
    }
  this->invalidate();

}

void ScreenInputsTestView::handleTickEvent()
{
	static uint8_t loop_count;

	//Input 1 config message
	switch(analog_config[0])
	{
		case 0:
			//Not used
			Unicode::strncpy(textConfig1Buffer, "Not used", 10);
			break;
		case 1:
			//Resistive
			Unicode::strncpy(textConfig1Buffer, "Resistance", 12);
			break;
		case 2:
			//Voltage
			Unicode::strncpy(textConfig1Buffer, "0-5V", 6);
			break;
		case 3:
			//4-20mA
			Unicode::strncpy(textConfig1Buffer, "4-20mA", 8);
			break;
	}

	//Input 2 config message
	switch(analog_config[1])
	{
		case 0:
			//Not used
			Unicode::strncpy(textConfig2Buffer, "Not used", 10);
			break;
		case 1:
			//Resistive
			Unicode::strncpy(textConfig2Buffer, "Resistance", 12);
			break;
		case 2:
			//Voltage
			Unicode::strncpy(textConfig2Buffer, "0-5V", 6);
			break;
		case 3:
			//4-20mA
			Unicode::strncpy(textConfig2Buffer, "4-20mA", 8);
			break;
	}

	//Analogue input 1 value
	switch(analog_config[0])
	{
		case 0:
			//Not used
			Unicode::snprintf(textValue1Buffer,TEXTVALUE1_SIZE, "----");
			break;
		case 1:
			//Resistive: No decimal places
			Unicode::snprintfFloat(textValue1Buffer,TEXTVALUE1_SIZE, "%6.0fR", analogInput1value);
			break;
		case 2:
			//Voltage: 2 decimal places
			Unicode::snprintfFloat(textValue1Buffer,TEXTVALUE1_SIZE, "%6.2fV", analogInput1value);
			break;
		case 3:
			//4-20mA current: 2 decimal places
			Unicode::snprintfFloat(textValue1Buffer,TEXTVALUE1_SIZE, "%6.2fmA", analogInput1value * 1000);
			break;
	}

	//Analogue input 2
	switch(analog_config[1])
	{
		case 0:
			//Not used
			Unicode::snprintf(textValue2Buffer,TEXTVALUE2_SIZE, "----");
			break;
		case 1:
			//Resistive: No decimal places
			Unicode::snprintfFloat(textValue2Buffer,TEXTVALUE2_SIZE, "%6.0fR", analogInput2value);
			break;
		case 2:
			//Voltage: 2 decimal places
			Unicode::snprintfFloat(textValue2Buffer,TEXTVALUE2_SIZE, "%6.2fV", analogInput2value);
			break;
		case 3:
			//4-20mA current: 2 decimal places
			Unicode::snprintfFloat(textValue2Buffer,TEXTVALUE2_SIZE, "%6.2fmA", analogInput2value * 1000);
			break;
	}


	//Supply voltage
	Unicode::snprintfFloat(textSupplyVoltageBuffer,TEXTSUPPLYVOLTAGE_SIZE, "%5.1f", SupplyVoltage);


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


	//if (buttonHandlerObj.handleTickEvent()) {
	if(++loop_count > 19)
	{
		loop_count = 0;
		this->invalidate();
	}

}
