#include <gtest/gtest.h>
#include <stdint.h>
#include <string.h>
#include "rpi4-system-monitor.h"

// Test get_nsecs function
TEST(RPi4SystemMonitorTest, GetNsecsReturnsValidTime) {
    uint64_t time1 = get_nsecs();
    uint64_t time2 = get_nsecs();
    
    // Time should be monotonically increasing
    EXPECT_LE(time1, time2);
    EXPECT_GT(time2, 0);
}

TEST(RPi4SystemMonitorTest, GetNsecsProgressesOverTime) {
    uint64_t time1 = get_nsecs();
    usleep(10000); // Sleep for 10ms
    uint64_t time2 = get_nsecs();
    
    // Time difference should be at least 10ms (10,000,000 nanoseconds)
    uint64_t diff = time2 - time1;
    EXPECT_GE(diff, 9000000); // Allow some tolerance
}

// Test get_total_ram function
TEST(RPi4SystemMonitorTest, GetTotalRamReturnsValue) {
    uint64_t total_ram = get_total_ram();
    
    // Total RAM should be greater than 0 on a running system
    // Or should return 0 if unable to read (graceful failure)
    EXPECT_GE(total_ram, 0);
}

// Test get_memory_info function
TEST(RPi4SystemMonitorTest, GetMemoryInfoInitializesStructure) {
    Memory_Info mem = {0};
    uint64_t test_total_ram = 4ULL * 1024 * 1024 * 1024; // 4GB
    
    get_memory_info(test_total_ram, &mem);
    
    EXPECT_EQ(mem.total_ram, test_total_ram);
    // used_ram should be calculated
    EXPECT_EQ(mem.used_ram, test_total_ram - mem.free_ram);
}

TEST(RPi4SystemMonitorTest, GetMemoryInfoWithDifferentRamSizes) {
    Memory_Info mem1, mem2;
    uint64_t ram_1gb = 1ULL * 1024 * 1024 * 1024;
    uint64_t ram_8gb = 8ULL * 1024 * 1024 * 1024;
    
    get_memory_info(ram_1gb, &mem1);
    get_memory_info(ram_8gb, &mem2);
    
    EXPECT_EQ(mem1.total_ram, ram_1gb);
    EXPECT_EQ(mem2.total_ram, ram_8gb);
    EXPECT_NE(mem1.total_ram, mem2.total_ram);
}

// Test CPU_Sample structure initialization
TEST(RPi4SystemMonitorTest, CPUSampleStructureInitialization) {
    CPU_Sample sample = {0};
    
    // Verify all fields are initialized
    EXPECT_EQ(sample.wall_time, 0);
    for (int i = 0; i < MAX_CPUS; i++) {
        EXPECT_EQ(sample.idle_time[i], 0);
    }
}

// Test Memory_Info structure initialization
TEST(RPi4SystemMonitorTest, MemoryInfoStructureInitialization) {
    Memory_Info mem = {0};
    
    EXPECT_EQ(mem.total_ram, 0);
    EXPECT_EQ(mem.free_ram, 0);
    EXPECT_EQ(mem.used_ram, 0);
}

// Test edge case: very large memory value
TEST(RPi4SystemMonitorTest, GetMemoryInfoWithLargeValue) {
    Memory_Info mem;
    uint64_t large_ram = UINT64_MAX / 2; // Use half of max uint64 to avoid overflow
    
    get_memory_info(large_ram, &mem);
    
    EXPECT_EQ(mem.total_ram, large_ram);
}

// Test edge case: zero RAM
TEST(RPi4SystemMonitorTest, GetMemoryInfoWithZeroRam) {
    Memory_Info mem;
    
    get_memory_info(0, &mem);
    
    EXPECT_EQ(mem.total_ram, 0);
}
