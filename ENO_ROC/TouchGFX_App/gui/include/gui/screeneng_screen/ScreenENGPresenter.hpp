#ifndef SCREENENGPRESENTER_HPP
#define SCREENENGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenENGView;

class ScreenENGPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenENGPresenter(ScreenENGView& v);

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

    virtual ~ScreenENGPresenter() {}

private:
    ScreenENGPresenter();

    ScreenENGView& view;
};

#endif // SCREENENGPRESENTER_HPP
