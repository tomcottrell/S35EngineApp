////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: SystemHealthManager.cpp
//
// Description: System Health Monitor Implementation
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SystemHealthManager.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: SystemHealthManager.c
//
// Description: System Health Monitor Implementation (Pure C Version)
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SystemHealthManager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdlib.h>

// Thresholds
#define RAM_WARNING_THRESHOLD     80
#define RAM_CRITICAL_THRESHOLD    95
#define CPU_WARNING_THRESHOLD     80
#define CPU_CRITICAL_THRESHOLD    95
#define TEMP_WARNING_THRESHOLD    70
#define TEMP_CRITICAL_THRESHOLD   85

// Global instance - NOTE: This is the corrected declaration
SystemHealthManager_t g_systemHealth;

// External linker symbols
extern uint32_t _estack;
extern uint32_t _Min_Stack_Size;
extern uint32_t _end;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Private function prototypes
static uint32_t GetUsedRAM(void);
static uint32_t GetUsedFlash(void);
static uint32_t GetUsedHeap(void);
static uint8_t CalculateCPUUsage(void);
static int16_t ReadTemperature(void);
static void ReadPowerMetrics(void);
static uint8_t CalculatePercentage(uint32_t used, uint32_t total);

void SystemHealth_Init(void)
{
    memset(&g_systemHealth, 0, sizeof(SystemHealthManager_t));

    // Set static values based on STM32L4R7 specifications
    g_systemHealth.memInfo.totalRAM = 640 * 1024;
    g_systemHealth.memInfo.totalFlash = 2048 * 1024;
    g_systemHealth.memInfo.totalHeap = configTOTAL_HEAP_SIZE;

    g_systemHealth.sysInfo.systemClockMHz = 80;
    g_systemHealth.sysInfo.maxTaskCount = configMAX_PRIORITIES;
    g_systemHealth.sysInfo.powerStatus = 1; // Power OK at startup

    g_systemHealth.lastIdleTime = 0;
    g_systemHealth.lastTotalTime = 0;
    g_systemHealth.systemStartTick = xTaskGetTickCount();
}

void SystemHealth_UpdateMetrics(void)
{
    SystemHealth_UpdateMemoryMetrics();
    SystemHealth_UpdateCPUMetrics();
    SystemHealth_UpdateTemperature();
    SystemHealth_UpdatePowerMetrics();
    SystemHealth_UpdateTaskCount();

    // Update uptime
    TickType_t currentTick = xTaskGetTickCount();
    g_systemHealth.sysInfo.uptimeSeconds = (currentTick - g_systemHealth.systemStartTick) / configTICK_RATE_HZ;
}

void SystemHealth_UpdateMemoryMetrics(void)
{
    g_systemHealth.memInfo.usedRAM = GetUsedRAM();
    g_systemHealth.memInfo.freeRAM = g_systemHealth.memInfo.totalRAM - g_systemHealth.memInfo.usedRAM;
    g_systemHealth.memInfo.ramPercentage = CalculatePercentage(g_systemHealth.memInfo.usedRAM, g_systemHealth.memInfo.totalRAM);

    g_systemHealth.memInfo.usedFlash = GetUsedFlash();
    g_systemHealth.memInfo.flashPercentage = CalculatePercentage(g_systemHealth.memInfo.usedFlash, g_systemHealth.memInfo.totalFlash);

    g_systemHealth.memInfo.usedHeap = GetUsedHeap();
    g_systemHealth.memInfo.freeHeap = g_systemHealth.memInfo.totalHeap - g_systemHealth.memInfo.usedHeap;
    g_systemHealth.memInfo.heapPercentage = CalculatePercentage(g_systemHealth.memInfo.usedHeap, g_systemHealth.memInfo.totalHeap);

    g_systemHealth.memInfo.stackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    g_systemHealth.memInfo.stackPercentage = 100 - ((g_systemHealth.memInfo.stackHighWaterMark * 100) / 512);
    if (g_systemHealth.memInfo.stackPercentage > 100)
        g_systemHealth.memInfo.stackPercentage = 100;
}

void SystemHealth_UpdateCPUMetrics(void)
{
    g_systemHealth.sysInfo.cpuUsage = CalculateCPUUsage();
}

void SystemHealth_UpdateTemperature(void)
{
    g_systemHealth.sysInfo.temperature = ReadTemperature();
}

void SystemHealth_UpdatePowerMetrics(void)
{
    ReadPowerMetrics();
}

void SystemHealth_UpdateTaskCount(void)
{
    g_systemHealth.sysInfo.activeTaskCount = uxTaskGetNumberOfTasks();
}

const MemoryInfo_t* SystemHealth_GetMemoryInfo(void)
{
    return &g_systemHealth.memInfo;
}

const SystemInfo_t* SystemHealth_GetSystemInfo(void)
{
    return &g_systemHealth.sysInfo;
}

HealthStatus_t SystemHealth_GetRAMStatus(void)
{
    if (g_systemHealth.memInfo.ramPercentage >= RAM_CRITICAL_THRESHOLD) return STATUS_CRITICAL;
    if (g_systemHealth.memInfo.ramPercentage >= RAM_WARNING_THRESHOLD) return STATUS_WARNING;
    return STATUS_OK;
}

HealthStatus_t SystemHealth_GetCPUStatus(void)
{
    if (g_systemHealth.sysInfo.cpuUsage >= CPU_CRITICAL_THRESHOLD) return STATUS_CRITICAL;
    if (g_systemHealth.sysInfo.cpuUsage >= CPU_WARNING_THRESHOLD) return STATUS_WARNING;
    return STATUS_OK;
}

HealthStatus_t SystemHealth_GetTemperatureStatus(void)
{
    if (g_systemHealth.sysInfo.temperature >= TEMP_CRITICAL_THRESHOLD) return STATUS_CRITICAL;
    if (g_systemHealth.sysInfo.temperature >= TEMP_WARNING_THRESHOLD) return STATUS_WARNING;
    return STATUS_OK;
}

HealthStatus_t SystemHealth_GetPowerStatus(void)
{
    if (!g_systemHealth.sysInfo.powerStatus || g_systemHealth.sysInfo.powerVoltage < 3000) return STATUS_CRITICAL;
    if (g_systemHealth.sysInfo.powerVoltage < 3200) return STATUS_WARNING;
    return STATUS_OK;
}

HealthStatus_t SystemHealth_GetSystemStatus(void)
{
    HealthStatus_t worstStatus = STATUS_OK;
    HealthStatus_t statuses[4] = {
        SystemHealth_GetRAMStatus(),
        SystemHealth_GetCPUStatus(),
        SystemHealth_GetTemperatureStatus(),
        SystemHealth_GetPowerStatus()
    };

    for (int i = 0; i < 4; i++) {
        if (statuses[i] > worstStatus) worstStatus = statuses[i];
    }
    return worstStatus;
}

// Private function implementations
static uint32_t GetUsedRAM(void)
{
    uint32_t dataSize = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint32_t bssSize = (uint32_t)&_ebss - (uint32_t)&_sbss;
    size_t heapUsed = g_systemHealth.memInfo.totalHeap - xPortGetFreeHeapSize();
    uint32_t estimatedStackUsage = g_systemHealth.sysInfo.activeTaskCount * 512 * 4;

    return dataSize + bssSize + heapUsed + estimatedStackUsage;
}

static uint32_t GetUsedFlash(void)
{
    uint32_t programSize = (uint32_t)&_end - 0x08000000;
    return programSize;
}

static uint32_t GetUsedHeap(void)
{
    return g_systemHealth.memInfo.totalHeap - xPortGetFreeHeapSize();
}

static uint8_t CalculateCPUUsage(void)
{
    // Simple simulation for now
    static uint8_t simCPU = 25;
    simCPU += (rand() % 11) - 5;
    if (simCPU < 5) simCPU = 5;
    if (simCPU > 95) simCPU = 95;
    return simCPU;
}

static int16_t ReadTemperature(void)
{
    static int16_t simTemp = 45;

    if (g_systemHealth.sysInfo.cpuUsage > 80) simTemp += 1;
    else if (g_systemHealth.sysInfo.cpuUsage < 20) simTemp -= 1;

    simTemp += (rand() % 3) - 1;

    if (simTemp < 25) simTemp = 25;
    if (simTemp > 85) simTemp = 85;

    return simTemp;
}

static void ReadPowerMetrics(void)
{
    g_systemHealth.sysInfo.powerVoltage = 3300 + (rand() % 100) - 50;
    g_systemHealth.sysInfo.powerCurrent = 100 + (g_systemHealth.sysInfo.cpuUsage * 2) + (rand() % 20);
    g_systemHealth.sysInfo.powerStatus = (g_systemHealth.sysInfo.powerVoltage >= 3000) ? 1 : 0;
}

static uint8_t CalculatePercentage(uint32_t used, uint32_t total)
{
    if (total == 0) return 0;
    uint32_t percentage = (used * 100) / total;
    return percentage > 100 ? 100 : (uint8_t)percentage;
}
