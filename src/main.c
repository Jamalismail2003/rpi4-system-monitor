/*
 * Copyright (c) 2026. All rights reserved.
 *
 * QNX RPi4 System Monitor - Main Program
 * Real-time CPU and memory monitoring tool for QNX on Raspberry Pi 4
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syspage.h>
#include <time.h>

#include "rpi4-system-monitor.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    // Open the address space of the process manager (procnto)
    int fd = open("/proc/1/as", O_RDONLY);
    if (fd == -1) {
        perror("Could not open /proc/1/as (run as root)");
        fprintf(stderr, "This program requires root privileges to monitor CPU usage.\n");
        return 1;
    }

    // Get the number of CPUs from the system page
    int num_cpus = _syspage_ptr->num_cpu;
    
    // Get total RAM
    uint64_t total_ram = get_total_ram();
    if (total_ram == 0) {
        fprintf(stderr, "Error: Could not determine total RAM.\n");
        close(fd);
        return 1;
    }

    CPU_Sample cpu_prev, cpu_curr;
    Memory_Info mem_info;

    printf("=== QNX RPi4 System Monitor ===\n");
    printf("CPUs: %d | Total RAM: %.2f MB\n", num_cpus, (double)total_ram / (1024 * 1024));
    printf("Press Ctrl+C to stop.\n\n");

    get_cpu_sample(fd, num_cpus, &cpu_prev);

    while (1) {
        sleep(1);
        get_cpu_sample(fd, num_cpus, &cpu_curr);
        get_memory_info(total_ram, &mem_info);

        uint64_t wall_diff = cpu_curr.wall_time - cpu_prev.wall_time;
        double total_cpu_load = 0;

        printf("\r");
        
        // Display CPU loads
        printf("CPU: ");
        for (int i = 0; i < num_cpus; i++) {
            uint64_t idle_diff = cpu_curr.idle_time[i] - cpu_prev.idle_time[i];
            
            // Usage = 100% - (Idle Time / Wall Time)
            double load = 100.0 - ((double)idle_diff / wall_diff * 100.0);
            if (load < 0) load = 0;
            if (load > 100) load = 100;

            printf("[%d]:%.0f%% ", i, load);
            total_cpu_load += load;
        }
        
        printf("| Avg:%.0f%% ", total_cpu_load / num_cpus);
        
        // Display Memory info
        printf("| RAM: Total:%.0fMB   ",
               (double)total_ram / (1024 * 1024));
        
        fflush(stdout);

        cpu_prev = cpu_curr;
    }

    close(fd);
    return 0;
}
