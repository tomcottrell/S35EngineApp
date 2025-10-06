#ifndef SCREENINPUTSTESTPRESENTER_HPP
#define SCREENINPUTSTESTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenInputsTestView;

class ScreenInputsTestPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenInputsTestPresenter(ScreenInputsTestView& v);

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

    virtual ~ScreenInputsTestPresenter() {}

private:
    ScreenInputsTestPresenter();

    ScreenInputsTestView& view;
};

#endif // SCREENINPUTSTESTPRESENTER_HPP
