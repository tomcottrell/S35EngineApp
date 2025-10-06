#ifndef SCREENTESTHOMEVIEW_HPP
#define SCREENTESTHOMEVIEW_HPP

#include <gui_generated/screentesthome_screen/ScreenTestHomeViewBase.hpp>
#include <gui/screentesthome_screen/ScreenTestHomePresenter.hpp>
#include "button_handler.hpp"

class ScreenTestHomeView : public ScreenTestHomeViewBase
{
public:
    ScreenTestHomeView();
    virtual ~ScreenTestHomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
};

#endif // SCREENTESTHOMEVIEW_HPP
