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
