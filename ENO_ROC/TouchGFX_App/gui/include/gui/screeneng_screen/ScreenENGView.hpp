#ifndef SCREENENGVIEW_HPP
#define SCREENENGVIEW_HPP

#include <gui_generated/screeneng_screen/ScreenENGViewBase.hpp>
#include <gui/screeneng_screen/ScreenENGPresenter.hpp>

class ScreenENGView : public ScreenENGViewBase
{
public:
    ScreenENGView();
    virtual ~ScreenENGView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
};

#endif // SCREENENGVIEW_HPP
