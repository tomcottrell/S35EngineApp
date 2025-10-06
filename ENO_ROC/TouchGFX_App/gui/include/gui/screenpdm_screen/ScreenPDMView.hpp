#ifndef SCREENPDMVIEW_HPP
#define SCREENPDMVIEW_HPP

#include <gui_generated/screenpdm_screen/ScreenPDMViewBase.hpp>
#include <gui/screenpdm_screen/ScreenPDMPresenter.hpp>

// Add C interface to access PDM data
extern "C" {
    #include "../../../../EC_code/Inc/PDM.h"
}

class ScreenPDMView : public ScreenPDMViewBase
{
public:
    ScreenPDMView();
    virtual ~ScreenPDMView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Add this for periodic updates
    virtual void handleTickEvent();
    void updateD03();
    void updateD07();
    void updateD011();

protected:
    uint16_t updateCounter;  // For controlling update rate

    void updatePDMStatus();
    void updateDigitalInputs();
    void updateDigitalOutputs();
    void updateAnalogInputs();
};

#endif // SCREENPDMVIEW_HPP
