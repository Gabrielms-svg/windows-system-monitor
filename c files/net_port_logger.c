#define _WIN32_WINNT 0x0600  // Ensure Vista or newer

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#include <winerror.h>   // <-- Add this line
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

__declspec(dllexport) void show_network_ports() {
    PMIB_TCPTABLE_OWNER_PID tcpTable;
    DWORD size = 0;

    // Ask size first
    DWORD result = GetExtendedTcpTable(NULL, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    tcpTable = (PMIB_TCPTABLE_OWNER_PID)malloc(size);
    if (tcpTable == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    result = GetExtendedTcpTable(tcpTable, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    if (result != NO_ERROR) {
        printf("Failed to retrieve TCP table. Error code: %lu\n", result);
        free(tcpTable);
        return;
    }

    printf("Active TCP Ports:\n");
    printf("-----------------\n");

    for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
        DWORD port = ntohs((u_short)tcpTable->table[i].dwLocalPort);
        DWORD pid = tcpTable->table[i].dwOwningPid;
        printf("Port: %5u  |  PID: %5u\n", port, pid);
    }
	for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
        char log_entry[128];
        sprintf(log_entry, "Port: %u | PID: %u",
                ntohs((u_short)tcpTable->table[i].dwLocalPort),
                tcpTable->table[i].dwOwningPid);
	

        log_message(log_entry);
  // ✅ use logger
    }
    free(tcpTable);
}
