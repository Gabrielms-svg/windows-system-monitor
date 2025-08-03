#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include "logger.h"  // make sure this is in the same folder

__declspec(dllexport) void show_cpu_memory_usage() {
    // --- MEMORY USAGE ---
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    DWORDLONG totalPhys = memInfo.ullTotalPhys;
    DWORDLONG physUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    DWORD memPercent = memInfo.dwMemoryLoad;

    printf("Memory Usage:\n");
    printf("-------------\n");
    printf("Used: %llu MB / %llu MB (%lu%%)\n\n",
           physUsed / (1024 * 1024), totalPhys / (1024 * 1024), memPercent);

    // ✅ Log the memory usage
    char message[256];
    sprintf(message, "Memory Usage: %llu MB / %llu MB (%lu%%)",
            physUsed / (1024 * 1024), totalPhys / (1024 * 1024), memPercent);
    log_message(message);

    // --- CPU USAGE ---
    static ULONGLONG lastIdleTime = 0, lastKernelTime = 0, lastUserTime = 0;

    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    ULONGLONG idle = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernel = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    ULONGLONG user = ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;

    ULONGLONG sys = (kernel + user);
    ULONGLONG sysDelta = sys - (lastKernelTime + lastUserTime);
    ULONGLONG idleDelta = idle - lastIdleTime;

    if (sysDelta > 0) {
        double cpuUsage = (1.0 - ((double)idleDelta / sysDelta)) * 100.0;
        printf("CPU Usage:\n");
        printf("----------\n");
        printf("CPU Load: %.2f%%\n", cpuUsage);

        // ✅ Log CPU usage
        sprintf(message, "CPU Usage: %.2f%%", cpuUsage);
        log_message(message);
    } else {
        printf("CPU Usage: Calculating...\n");
        log_message("CPU Usage: Calculating...");
    }

    // Save values for next run
    lastIdleTime = idle;
    lastKernelTime = kernel;
    lastUserTime = user;
}
