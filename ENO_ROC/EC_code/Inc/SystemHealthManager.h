////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: SystemHealthManager.h
//
// Description: System Health Monitor for STM32L4R7 with FreeRTOS (C Version)
//             Monitors RAM, Flash, Heap, CPU, Temperature, Power, and System metrics
//
// Author: Generated for Enovation Controls TouchGFX Project
//
// Copyright: Enovation Controls 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_HEALTH_MANAGER_H
#define SYSTEM_HEALTH_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    uint32_t totalRAM;
    uint32_t usedRAM;
    uint32_t freeRAM;
    uint8_t  ramPercentage;

    uint32_t totalFlash;
    uint32_t usedFlash;
    uint8_t  flashPercentage;

    uint32_t totalHeap;
    uint32_t usedHeap;
    uint32_t freeHeap;
    uint8_t  heapPercentage;

    uint32_t stackHighWaterMark;
    uint8_t  stackPercentage;
} MemoryInfo_t;

typedef struct {
    uint8_t  cpuUsage;
    uint32_t systemClockMHz;
    uint8_t  activeTaskCount;
    uint8_t  maxTaskCount;
    int16_t  temperature;
    uint16_t powerVoltage;  // in mV
    uint16_t powerCurrent;  // in mA
    uint8_t  powerStatus;   // 1 = OK, 0 = NOT OK
    uint32_t uptimeSeconds;
} SystemInfo_t;

typedef enum {
    STATUS_OK = 0,
    STATUS_WARNING = 1,
    STATUS_CRITICAL = 2
} HealthStatus_t;

typedef struct {
    MemoryInfo_t memInfo;
    SystemInfo_t sysInfo;

    uint32_t lastIdleTime;
    uint32_t lastTotalTime;
    TickType_t systemStartTick;
} SystemHealthManager_t;

// Global instance
extern SystemHealthManager_t g_systemHealth;

// Public functions
void SystemHealth_Init(void);
void SystemHealth_UpdateMetrics(void);

// Getters
const MemoryInfo_t* SystemHealth_GetMemoryInfo(void);
const SystemInfo_t* SystemHealth_GetSystemInfo(void);

// Status checkers
HealthStatus_t SystemHealth_GetRAMStatus(void);
HealthStatus_t SystemHealth_GetCPUStatus(void);
HealthStatus_t SystemHealth_GetTemperatureStatus(void);
HealthStatus_t SystemHealth_GetPowerStatus(void);
HealthStatus_t SystemHealth_GetSystemStatus(void);

// Individual metric updaters
void SystemHealth_UpdateMemoryMetrics(void);
void SystemHealth_UpdateCPUMetrics(void);
void SystemHealth_UpdateTemperature(void);
void SystemHealth_UpdatePowerMetrics(void);
void SystemHealth_UpdateTaskCount(void);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_HEALTH_MANAGER_H
