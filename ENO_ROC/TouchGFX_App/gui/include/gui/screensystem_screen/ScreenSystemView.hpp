////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: ScreenSystemView.hpp
//
// Description: TouchGFX System Health Monitor View Header
//             Text-based system health display for STM32L4R7 (C API Version)
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SCREENSYSTEMVIEW_HPP
#define SCREENSYSTEMVIEW_HPP

#include <gui_generated/screensystem_screen/ScreenSystemViewBase.hpp>
#include <gui/screensystem_screen/ScreenSystemPresenter.hpp>

extern "C" {
    #include "../../../../EC_code/Inc/SystemHealthManager.h"
}

class ScreenSystemView : public ScreenSystemViewBase
{
public:
    ScreenSystemView();
    virtual ~ScreenSystemView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    // Button handler for interactions
    virtual void buttonHandler(uint8_t value);

    // Update methods for UI refresh
    void updateMemoryMetrics();
    void updateSystemMetrics();
    void updateStatusTexts();
    void updateAllMetrics();

protected:
    uint16_t updateCounter;

private:
    // Helper methods for status display
    touchgfx::colortype getStatusColor(HealthStatus_t status);
    const char* getStatusString(HealthStatus_t status);
    void formatMemoryText(touchgfx::Unicode::UnicodeChar* buffer, uint16_t bufferSize,
                         uint32_t used, uint32_t total, uint8_t percentage, bool inKB = true);
    void formatUptimeText(touchgfx::Unicode::UnicodeChar* buffer, uint16_t bufferSize, uint32_t seconds);
};

#endif // SCREENSYSTEMVIEW_HPP
