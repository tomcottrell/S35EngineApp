#ifndef SCREENINPUTSTESTVIEW_HPP
#define SCREENINPUTSTESTVIEW_HPP

#include <gui_generated/screeninputstest_screen/ScreenInputsTestViewBase.hpp>
#include <gui/screeninputstest_screen/ScreenInputsTestPresenter.hpp>
#include "button_handler.hpp"

class ScreenInputsTestView : public ScreenInputsTestViewBase
{
public:
    ScreenInputsTestView();
    virtual ~ScreenInputsTestView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
};

#endif // SCREENINPUTSTESTVIEW_HPP
