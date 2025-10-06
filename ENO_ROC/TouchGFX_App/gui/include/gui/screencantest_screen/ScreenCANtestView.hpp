#ifndef SCREENCANTESTVIEW_HPP
#define SCREENCANTESTVIEW_HPP

#include <gui_generated/screencantest_screen/ScreenCANtestViewBase.hpp>
#include <gui/screencantest_screen/ScreenCANtestPresenter.hpp>
#include "button_handler.hpp"

class ScreenCANtestView : public ScreenCANtestViewBase
{
public:
    ScreenCANtestView();
    virtual ~ScreenCANtestView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
};

#endif // SCREENCANTESTVIEW_HPP
