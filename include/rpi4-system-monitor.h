/*
 * Copyright (c) 2026. All rights reserved.
 *
 * QNX RPi4 System Monitor - Real-time CPU and Memory monitoring tool
 * 
 * This header defines the data structures and function declarations
 * for the RPi4 system monitor.
 */

#ifndef RPi4_SYSTEM_MONITOR_H
#define RPi4_SYSTEM_MONITOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CPUS 32

/**
 * CPU_Sample: Structure to hold CPU idle time samples
 * @wall_time: Monotonic time in nanoseconds
 * @idle_time: Array of idle times per CPU core in nanoseconds
 */
typedef struct {
    uint64_t wall_time;
    uint64_t idle_time[MAX_CPUS];
} CPU_Sample;

/**
 * Memory_Info: Structure to hold memory information
 * @total_ram: Total physical RAM in bytes
 * @free_ram: Free RAM in bytes
 * @used_ram: Used RAM in bytes (total - free)
 */
typedef struct {
    uint64_t total_ram;
    uint64_t free_ram;
    uint64_t used_ram;
} Memory_Info;

/**
 * get_nsecs: Get current monotonic time in nanoseconds
 * @return: Current time in nanoseconds
 */
uint64_t get_nsecs(void);

/**
 * get_total_ram: Calculate total physical RAM from system page
 * @return: Total RAM in bytes, 0 on error
 */
uint64_t get_total_ram(void);

/**
 * get_free_ram: Get available free RAM
 * @return: Free RAM in bytes
 */
uint64_t get_free_ram(void);

/**
 * get_memory_info: Retrieve current memory information
 * @total_ram: Total RAM to calculate usage from
 * @mem: Pointer to Memory_Info structure to fill
 */
void get_memory_info(uint64_t total_ram, Memory_Info *mem);

/**
 * get_cpu_sample: Get CPU idle time sample for all cores
 * @fd: File descriptor for /proc/1/as
 * @num_cpus: Number of CPU cores
 * @sample: Pointer to CPU_Sample structure to fill
 */
void get_cpu_sample(int fd, int num_cpus, CPU_Sample *sample);

#ifdef __cplusplus
}
#endif

#endif /* RPi4_SYSTEM_MONITOR_H */
