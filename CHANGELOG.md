# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-01-14

### Added
- Initial release
- Real-time CPU monitoring per core and average
- Memory information display
- Support for Raspberry Pi 4 running QNX Neutrino
- Makefile for cross-compilation
- Comprehensive documentation (README, CONTRIBUTING)
- Header file with data structures and function declarations
- Support for up to 32 CPU cores
- Live display updates every second
- Root privilege checking and error handling

### Features
- Per-core CPU load calculation using idle time statistics
- Total physical RAM detection from system page
- Multi-core CPU support with automatic core detection
- Lightweight and responsive monitoring
- Uses native QNX APIs for system data access

### Documentation
- README.md with usage instructions
- CONTRIBUTING.md for developer guidelines
- MIT License
- .gitignore for repository management
- Inline code documentation

## [Unreleased]

### Planned
- [ ] Free memory calculation using QNX typed memory API
- [ ] Process-level monitoring capabilities
- [ ] Historical data and trending
- [ ] Configurable update intervals
- [ ] Network interface statistics
- [ ] Disk usage monitoring
- [ ] Temperature monitoring (if available)
- [ ] Web dashboard interface
- [ ] JSON output format option
- [ ] System alerts/notifications

---

## Version History

### Version 1.0.0
- **Release Date:** 2026-01-14
- **Status:** Initial Release
- **Supported Platforms:** 
  - QNX Neutrino 7.0+
  - Raspberry Pi 4 (aarch64)
- **Tested With:**
  - QNX 7.1.0
  - QCC toolchain

---

## Notes

### Breaking Changes
None for v1.0.0

### Known Issues
- Free RAM calculation not fully implemented (returns 0)
- May require updates for non-RPi4 hardware
- CPU load calculations rounded to nearest percent in display

### Migration Guide
N/A for initial release

### Deprecations
None

---

For details on future versions, see the [GitHub Issues](https://github.com/yourname/rpi4-system-monitor/issues) and [Project Board](https://github.com/yourname/rpi4-system-monitor/projects).

---

## How to Release

1. Update version numbers in documentation
2. Update CHANGELOG.md
3. Create a git tag: `git tag -a v1.0.0 -m "Release v1.0.0"`
4. Push tag: `git push origin v1.0.0`
5. Create release notes on GitHub

---

**Maintainer:** [Your Name]  
**License:** MIT  
**Last Updated:** 2026-01-14
