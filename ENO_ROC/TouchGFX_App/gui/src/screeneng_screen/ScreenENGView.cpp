#include <gui/screeneng_screen/ScreenENGView.hpp>

extern uint16_t frequency;

ScreenENGView::ScreenENGView()
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

void ScreenENGView::handleTickEvent()
{

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
}
