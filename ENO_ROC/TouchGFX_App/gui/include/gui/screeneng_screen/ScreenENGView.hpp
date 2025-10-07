#ifndef SCREENENGVIEW_HPP
#define SCREENENGVIEW_HPP

#include <gui_generated/screeneng_screen/ScreenENGViewBase.hpp>
#include <gui/screeneng_screen/ScreenENGPresenter.hpp>
#include "button_handler.hpp"

class ScreenENGView : public ScreenENGViewBase
{
public:
    ScreenENGView();
    virtual ~ScreenENGView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
};

#endif // SCREENENGVIEW_HPP
