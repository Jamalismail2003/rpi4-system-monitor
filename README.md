# QNX RPi4 System Monitor

A real-time system monitoring tool for QNX Neutrino running on Raspberry Pi 4. Displays CPU utilization per core and total system memory information.

## Features

- **Real-time CPU Monitoring**: Display per-core CPU load and average load
- **Memory Information**: Show total physical RAM
- **Live Updates**: Updates every second with smooth refresh
- **Multi-core Support**: Automatically detects and monitors all CPU cores (up to 32)
- **Lightweight**: Minimal resource overhead
- **QNX Native**: Uses QNX system APIs and device control operations

## Requirements

- QNX Neutrino 7.0 or later
- Raspberry Pi 4 with appropriate QNX BSP
- Root privileges (required to read `/proc/1/as`)
- Standard C library with POSIX support

## Building

### Prerequisites
Ensure you have the QNX toolchain installed and the cross-compiler available.

### Build Commands

**For Raspberry Pi 4 (aarch64):**
```bash
make clean
make TARGET=aarch64
```

**Using QCC directly:**
```bash
qcc -V gcc_ntoaarch64le src/rpi4-system-monitor.c -I./include -o rpi4-system-monitor
```

**Native compilation (if on QNX):**
```bash
make
```

### Output

```
=== QNX RPi4 System Monitor ===
CPUs: 4 | Total RAM: 3840.00 MB
Press Ctrl+C to stop.

CPU: [0]:45% [1]:32% [2]:28% [3]:35% | Avg:35% | RAM: Total:3840MB
```

## Usage

```bash
sudo ./rpi4-system-monitor
```

**Note:** Root privileges are required because the program reads CPU statistics from `/proc/1/as`.

## Project Structure

```
rpi4-system-monitor/
├── src/
│   └── rpi4-system-monitor.c    # Main source file
├── include/
│   └── rpi4-system-monitor.h    # Header file with data structures
├── Makefile                      # Build configuration
├── README.md                     # This file
├── LICENSE                       # License information
└── .gitignore                    # Git ignore patterns
```

## How It Works

### CPU Monitoring
1. Opens `/proc/1/as` (process manager address space)
2. Uses `devctl()` with `DCMD_PROC_TIDSTATUS` to get idle thread statistics
3. Calculates CPU load as: `100% - (Idle_Time_Delta / Wall_Time_Delta)`
4. Updates display every second

### Memory Information
- Parses system page entries to find total RAM
- Shows total physical memory available to the system

## Implementation Details

### Data Structures

**CPU_Sample**: Stores CPU idle time samples
```c
typedef struct {
    uint64_t wall_time;        // Monotonic time in nanoseconds
    uint64_t idle_time[32];    // Per-core idle times
} CPU_Sample;
```

**Memory_Info**: Stores memory statistics
```c
typedef struct {
    uint64_t total_ram;        // Total physical RAM
    uint64_t free_ram;         // Available free RAM
    uint64_t used_ram;         // Used RAM
} Memory_Info;
```

### Key Functions

- `get_nsecs()`: Get current monotonic time in nanoseconds
- `get_total_ram()`: Calculate total physical RAM from system page
- `get_cpu_sample()`: Get CPU idle time for all cores
- `get_memory_info()`: Retrieve current memory information

## Limitations

- Requires root privileges to access CPU statistics
- Free RAM calculation currently returns 0 (can be enhanced with QNX-specific memory APIs)
- Supports up to 32 CPU cores (MAX_CPUS constant)
- CPU load calculations may have rounding in display

## Future Enhancements

- [ ] Implement proper free memory calculation using QNX typed memory API
- [ ] Add process-level CPU/memory monitoring
- [ ] Implement history/trending data
- [ ] Add configurable update intervals
- [ ] Create web dashboard interface
- [ ] Add syslog integration for alerts

## License

[Specify your license here - MIT, Apache 2.0, etc.]

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Support

For issues, questions, or suggestions, please open an issue in the GitHub repository.

## Author

Created for QNX system monitoring on Raspberry Pi 4.

## References

- [QNX Neutrino Documentation](https://www.qnx.com/developers/docs/)
- [Raspberry Pi 4 BSP Documentation](https://www.qnx.com/developers/docs/7.0.0/com.qnx.doc.bsp_raspberrypi4/topic/about.html)
- POSIX real-time API specifications

---

**Note:** This tool is optimized for Raspberry Pi 4 running QNX Neutrino. Compatibility with other platforms may vary.
