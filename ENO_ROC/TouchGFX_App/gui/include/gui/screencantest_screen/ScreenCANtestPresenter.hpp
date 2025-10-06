#ifndef SCREENCANTESTPRESENTER_HPP
#define SCREENCANTESTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenCANtestView;

class ScreenCANtestPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenCANtestPresenter(ScreenCANtestView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenCANtestPresenter() {}

private:
    ScreenCANtestPresenter();

    ScreenCANtestView& view;
};

#endif // SCREENCANTESTPRESENTER_HPP
