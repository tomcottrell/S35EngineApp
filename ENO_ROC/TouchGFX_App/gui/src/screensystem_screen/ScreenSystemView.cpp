////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: ScreenSystemView.cpp
//
// Description: TouchGFX System Health Monitor View Implementation (FIXED VERSION)
//             Real-time display of system metrics via text areas (C API Version)
//             Fixed multiple invalidation issues causing screen glitching
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gui/screensystem_screen/ScreenSystemView.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ScreenSystemView::ScreenSystemView() : updateCounter(0)
{
}

void ScreenSystemView::setupScreen()
{
    ScreenSystemViewBase::setupScreen();

    // Initialize system health manager and get current metrics
    SystemHealth_Init();
    SystemHealth_UpdateMetrics();
    updateAllMetrics();

    // REMOVED: Redundant invalidate() call that was causing conflicts
    // The updateAllMetrics() function now handles invalidation properly
}

void ScreenSystemView::tearDownScreen()
{
    ScreenSystemViewBase::tearDownScreen();
}

void ScreenSystemView::handleTickEvent()
{
    ScreenSystemViewBase::handleTickEvent();

    // Update every 60 ticks (approximately 1 second at 60 FPS)
    updateCounter++;
    if (updateCounter >= 60)
    {
        updateCounter = 0;
        SystemHealth_UpdateMetrics();
        updateAllMetrics();
    }
}

void ScreenSystemView::buttonHandler(uint8_t value)
{
    // Handle button presses for your system health screen
    switch(value)
    {
        case 0:
            // Button 0 pressed - refresh metrics immediately
            SystemHealth_UpdateMetrics();
            updateAllMetrics();
            break;
        case 1:
            // Button 1 pressed - could reset counters or navigate
            break;
        case 2:
            // Button 2 pressed - could show detailed view
            break;
        case 3:
            // Button 3 pressed - could go back to main menu
            break;
        default:
            break;
    }
}

void ScreenSystemView::updateAllMetrics()
{
    // Update all metric data in buffers first
    updateMemoryMetrics();
    updateSystemMetrics();
    updateStatusTexts();

    // FIXED: Single screen invalidation instead of multiple individual invalidations
    // This prevents the glitching/inverting behavior caused by competing invalidation calls
    this->invalidate();
}

void ScreenSystemView::updateMemoryMetrics()
{
    const MemoryInfo_t* memInfo = SystemHealth_GetMemoryInfo();

    // Update RAM Usage - Using correct buffer names from generated code
    formatMemoryText(textAreaRAMValueBuffer, TEXTAREARAMVALUE_SIZE,
                    memInfo->usedRAM, memInfo->totalRAM, memInfo->ramPercentage, true);
    // REMOVED: textAreaRAMValue.invalidate();

    // Update Flash Memory
    formatMemoryText(textAreaFlashValueBuffer, TEXTAREAFLASHVALUE_SIZE,
                    memInfo->usedFlash, memInfo->totalFlash, memInfo->flashPercentage, true);
    // REMOVED: textAreaFlashValue.invalidate();

    // Update Heap Usage
    formatMemoryText(textAreaHeapValueBuffer, TEXTAREAHEAPVALUE_SIZE,
                    memInfo->usedHeap, memInfo->totalHeap, memInfo->heapPercentage, false);
    // REMOVED: textAreaHeapValue.invalidate();

    // Update Stack Usage
    touchgfx::Unicode::snprintf(textAreaStackValueBuffer, TEXTAREASTACKVALUE_SIZE,
                               "HWM: %d words (%d%%)",
                               memInfo->stackHighWaterMark,
                               memInfo->stackPercentage);
    // REMOVED: textAreaStackValue.invalidate();
}

void ScreenSystemView::updateSystemMetrics()
{
    const SystemInfo_t* sysInfo = SystemHealth_GetSystemInfo();

    // Update CPU Usage - Using correct buffer name
    touchgfx::Unicode::snprintf(textAreaCPUValueBuffer, TEXTAREACPUVALUE_SIZE, "%d%%", sysInfo->cpuUsage);
    // REMOVED: textAreaCPUValue.invalidate();

    // Update System Information (this maps to the "System Info" area in your UI)
    touchgfx::Unicode::snprintf(textAreaSystemValueBuffer, TEXTAREASYSTEMVALUE_SIZE,
                               "%dMHz | Tasks: %d/%d",
                               sysInfo->systemClockMHz,
                               sysInfo->activeTaskCount,
                               sysInfo->maxTaskCount);
    // REMOVED: textAreaSystemValue.invalidate();
}

void ScreenSystemView::updateStatusTexts()
{
    // Update status text areas with colored status strings
    textAreaRAMStatus.setColor(getStatusColor(SystemHealth_GetRAMStatus()));
    touchgfx::Unicode::strncpy(textAreaRAMStatusBuffer, getStatusString(SystemHealth_GetRAMStatus()), TEXTAREARAMSTATUS_SIZE-1);
    // REMOVED: textAreaRAMStatus.invalidate();

    textAreaCPUStatus.setColor(getStatusColor(SystemHealth_GetCPUStatus()));
    touchgfx::Unicode::strncpy(textAreaCPUStatusBuffer, getStatusString(SystemHealth_GetCPUStatus()), TEXTAREACPUSTATUS_SIZE-1);
    // REMOVED: textAreaCPUStatus.invalidate();

    textAreaFlashStatus.setColor(getStatusColor(SystemHealth_GetRAMStatus())); // Assuming flash uses RAM status for now
    touchgfx::Unicode::strncpy(textAreaFlashStatusBuffer, getStatusString(SystemHealth_GetRAMStatus()), TEXTAREAFLASHSTATUS_SIZE-1);
    // REMOVED: textAreaFlashStatus.invalidate();

    textAreaHeapStatus.setColor(getStatusColor(SystemHealth_GetRAMStatus())); // Assuming heap uses RAM status for now
    touchgfx::Unicode::strncpy(textAreaHeapStatusBuffer, getStatusString(SystemHealth_GetRAMStatus()), TEXTAREAHEAPSTATUS_SIZE-1);
    // REMOVED: textAreaHeapStatus.invalidate();

    textAreaStackStatus.setColor(getStatusColor(SystemHealth_GetRAMStatus())); // Assuming stack uses RAM status for now
    touchgfx::Unicode::strncpy(textAreaStackStatusBuffer, getStatusString(SystemHealth_GetRAMStatus()), TEXTAREASTACKSTATUS_SIZE-1);
    // REMOVED: textAreaStackStatus.invalidate();

    textAreaSystemStatus.setColor(getStatusColor(SystemHealth_GetSystemStatus()));
    touchgfx::Unicode::strncpy(textAreaSystemStatusBuffer, getStatusString(SystemHealth_GetSystemStatus()), TEXTAREASYSTEMSTATUS_SIZE-1);
    // REMOVED: textAreaSystemStatus.invalidate();
}

const char* ScreenSystemView::getStatusString(HealthStatus_t status)
{
    switch(status)
    {
        case STATUS_OK:
            return "OK";
        case STATUS_WARNING:
            return "WARNING";
        case STATUS_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

touchgfx::colortype ScreenSystemView::getStatusColor(HealthStatus_t status)
{
    switch(status)
    {
        case STATUS_OK:
            return touchgfx::Color::getColorFromRGB(0, 255, 0);    // Green
        case STATUS_WARNING:
            return touchgfx::Color::getColorFromRGB(255, 255, 0);  // Yellow
        case STATUS_CRITICAL:
            return touchgfx::Color::getColorFromRGB(255, 0, 0);    // Red
        default:
            return touchgfx::Color::getColorFromRGB(128, 128, 128); // Gray
    }
}

void ScreenSystemView::formatMemoryText(touchgfx::Unicode::UnicodeChar* buffer, uint16_t bufferSize,
                                       uint32_t used, uint32_t total, uint8_t percentage, bool showBytes)
{
    if (showBytes)
    {
        // Show memory in KB or MB format
        if (total > 1024 * 1024)
        {
            // Show in MB
            touchgfx::Unicode::snprintf(buffer, bufferSize,
                                       "%d/%dMB (%d%%)",
                                       used / (1024 * 1024),
                                       total / (1024 * 1024),
                                       percentage);
        }
        else
        {
            // Show in KB
            touchgfx::Unicode::snprintf(buffer, bufferSize,
                                       "%d/%dKB (%d%%)",
                                       used / 1024,
                                       total / 1024,
                                       percentage);
        }
    }
    else
    {
        // Show raw values (for heap, etc.)
        touchgfx::Unicode::snprintf(buffer, bufferSize,
                                   "%d/%d (%d%%)",
                                   used, total, percentage);
    }
}
