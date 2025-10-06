////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: ScreenSystemPresenter.cpp
//
// Description: TouchGFX System Health Monitor Presenter Implementation
//             MVP Pattern Presenter for System Health Screen (C API Version)
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gui/screensystem_screen/ScreenSystemView.hpp>
#include <gui/screensystem_screen/ScreenSystemPresenter.hpp>

ScreenSystemPresenter::ScreenSystemPresenter(ScreenSystemView& v)
    : view(v)
{
}

void ScreenSystemPresenter::activate()
{
    // Called when the screen becomes active
    // The view handles its own metric updates via handleTickEvent()
    // You could trigger an immediate update here if needed
}

void ScreenSystemPresenter::deactivate()
{
    // Called when the screen becomes inactive
    // System health manager continues running in background
}

void ScreenSystemPresenter::notifySystemHealthUpdate()
{
    // Called by Model if implementing push-based updates
    // Notify the view to update its display
    view.updateAllMetrics();
}
