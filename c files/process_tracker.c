#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include "logger.h"

__declspec(dllexport) void show_processes() {
    DWORD processes[1024], cbNeeded, processCount;

    if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        log_message("Failed to enumerate processes.");
        return;
    }

    processCount = cbNeeded / sizeof(DWORD);

    for (DWORD i = 0; i < processCount; i++) {
        DWORD pid = processes[i];
        if (pid == 0) continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (hProcess) {
            char processName[MAX_PATH] = "<unknown>";

            // Get image file name (ANSI version)
            if (GetProcessImageFileNameA(hProcess, processName, MAX_PATH)) {
                printf("PID: %5lu | Name: %s\n", pid, processName);

                char log_entry[256];
                sprintf(log_entry, "PID: %lu | Process Name: %s", pid, processName);
                log_message(log_entry);
            } else {
                printf("PID: %5lu | Name: <unknown>\n", pid);

                char log_entry[100];
                sprintf(log_entry, "Warning: Could not get name for PID %lu", pid);
                log_message(log_entry);
            }

            CloseHandle(hProcess);
        }
    }
}
