////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: ScreenSystemPresenter.hpp
//
// Description: TouchGFX System Health Monitor Presenter Header
//             MVP Pattern Presenter for System Health Screen (C API Version)
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SCREENSYSTEMPRESENTER_HPP
#define SCREENSYSTEMPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenSystemView;

class ScreenSystemPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenSystemPresenter(ScreenSystemView& v);

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

    virtual ~ScreenSystemPresenter() {}

    // Model update notifications
    virtual void notifySystemHealthUpdate();

private:
    ScreenSystemPresenter();

    ScreenSystemView& view;
};

#endif // SCREENSYSTEMPRESENTER_HPP
