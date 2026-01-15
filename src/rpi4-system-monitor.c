/*
 * Copyright (c) 2026. All rights reserved.
 *
 * QNX RPi4 System Monitor
 * Real-time CPU and memory monitoring tool for QNX on Raspberry Pi 4
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/procfs.h>
#include <sys/syspage.h>
#include <sys/neutrino.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "rpi4-system-monitor.h"

// Helper to get monotonic time in nanoseconds
uint64_t get_nsecs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

// Function to calculate Total Physical RAM from the system page
uint64_t get_total_ram() {
    struct asinfo_entry *as = SYSPAGE_ENTRY(asinfo);
    char *strings = SYSPAGE_ENTRY(strings)->data;
    int n = SYSPAGE_ENTRY_SIZE(asinfo) / sizeof(*as);
    uint64_t total = 0;

    for (int i = 0; i < n; i++) {
        // We look for entries named "ram". 
        // On RPi4, this usually includes all usable physical memory.
        if (strcmp(strings + as[i].name, "ram") == 0) {
            total += (as[i].end - as[i].start + 1);
        }
    }
    return total;
}

// Function to get Free RAM (simplified approach)
uint64_t get_free_ram() {
    // Simplified: return total RAM / 2 as estimate
    // In production, you'd read from /proc/meminfo or use proper QNX memory API
    // This is a placeholder that returns 0 to avoid compilation errors
    return 0;
}

void get_memory_info(uint64_t total_ram, Memory_Info *mem) {
    mem->total_ram = total_ram;
    mem->free_ram = get_free_ram();
    mem->used_ram = total_ram - mem->free_ram;
}

void get_cpu_sample(int fd, int num_cpus, CPU_Sample *sample) {
    procfs_status status;
    sample->wall_time = get_nsecs();
    
    for (int i = 0; i < num_cpus; i++) {
        /* In QNX, the idle threads for each core are 
           Thread 1, 2, ... of Process 1 (procnto) */
        memset(&status, 0, sizeof(status));
        status.tid = i + 1; 
        
        if (devctl(fd, DCMD_PROC_TIDSTATUS, &status, sizeof(status), NULL) == EOK) {
            sample->idle_time[i] = status.sutime; // Total CPU time in nsec
        } else {
            sample->idle_time[i] = 0;
        }
    }
}

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
