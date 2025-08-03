#include <windows.h>
#include <stdio.h>

typedef void (*ShowProcessesFunc)();
typedef void (*ShowCPUMemoryFunc)();
typedef void (*ShowNetworkPortsFunc)();

int main() {
    // Load process_tracker.dll
    HINSTANCE hProc = LoadLibrary("process_tracker.dll");
    if (!hProc) {
        printf("Failed to load process_tracker.dll\n");
        return 1;
    }
    ShowProcessesFunc show_processes = (ShowProcessesFunc)GetProcAddress(hProc, "show_processes");

    // Load cpu_memory.dll
    HINSTANCE hCpuMem = LoadLibrary("cpu_memory.dll");
    if (!hCpuMem) {
        printf("Failed to load cpu_memory.dll\n");
        return 1;
    }
    ShowCPUMemoryFunc show_cpu_mem = (ShowCPUMemoryFunc)GetProcAddress(hCpuMem, "show_cpu_memory_usage");

    // Load net_port_logger.dll
    HINSTANCE hNetPort = LoadLibrary("net_port_logger.dll");
    if (!hNetPort) {
        printf("Failed to load net_port_logger.dll\n");
        return 1;
    }
    ShowNetworkPortsFunc show_ports = (ShowNetworkPortsFunc)GetProcAddress(hNetPort, "show_network_ports");

    // Run each function
    printf("\n== SYSTEM MONITOR ==\n\n");

    if (show_processes) {
        printf("[1] Running Processes:\n");
        show_processes();
        printf("\n");
    }

    if (show_cpu_mem) {
        printf("[2] CPU and Memory Usage:\n");
        show_cpu_mem();
        printf("\n");
    }

    if (show_ports) {
        printf("[3] Network Ports:\n");
        show_ports();
        printf("\n");
    }

    // Clean up
    FreeLibrary(hProc);
    FreeLibrary(hCpuMem);
    FreeLibrary(hNetPort);

    return 0;
}
