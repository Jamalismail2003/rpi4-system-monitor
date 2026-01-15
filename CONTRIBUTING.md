# Contributing to QNX RPi4 System Monitor

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Welcome diverse perspectives
- Report issues responsibly

## How to Contribute

### Reporting Bugs

1. Check existing issues to avoid duplicates
2. Create a new issue with a clear description
3. Include:
   - QNX version
   - Raspberry Pi model
   - Steps to reproduce
   - Expected vs actual behavior
   - Error messages or logs

### Suggesting Enhancements

1. Check the existing feature list in README.md
2. Open an issue describing:
   - The feature or improvement
   - Use cases and benefits
   - Potential implementation approach

### Submitting Code

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes
4. Test thoroughly on QNX
5. Follow code style conventions (see below)
6. Commit with clear messages: `git commit -m "Add descriptive message"`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Create a Pull Request with detailed description

## Code Style

- Use 4-space indentation
- Follow POSIX C89/C99 standards
- Add comments for complex logic
- Keep functions focused and modular
- Use meaningful variable names
- Maximum line length: 100 characters (preferred)

## Coding Standards

### Header Comments
```c
/*
 * Brief description of what the function does
 * 
 * @param1: Description of parameter
 * @param2: Description of parameter
 * @return: Description of return value
 */
```

### Variable Naming
```c
uint64_t sample_count;      // Good
uint64_t sc;                // Avoid
struct CPU_Sample cpu_sample;  // Good
CPU_SAMPLE *pCpuSample;     // Avoid Hungarian notation
```

### Error Handling
```c
if (condition == -1) {
    perror("descriptive error message");
    return EXIT_FAILURE;
}
```

## Testing

- Test on actual Raspberry Pi 4 hardware when possible
- Test with different QNX versions (7.0+)
- Test with different CPU core counts
- Ensure no memory leaks (use valgrind if available)
- Test as both root and non-root user

## Documentation

- Update README.md for user-facing changes
- Add inline code comments for complex sections
- Document API changes in code comments
- Update CHANGELOG if project has one

## Build & Testing Checklist

Before submitting a PR, verify:
- [ ] Code compiles without warnings: `make clean && make`
- [ ] Cross-compilation works: `make TARGET=aarch64`
- [ ] No compiler warnings or errors
- [ ] Manual testing on target hardware completed
- [ ] Code follows style guidelines
- [ ] Comments are clear and helpful
- [ ] No unnecessary dependencies added

## Commit Messages

Write clear, concise commit messages:

```
Add CPU load calculation per core

- Implement idle time sampling
- Calculate load percentage
- Update display format

Fixes #123
```

Bad example:
```
fix stuff
updated code
```

## Pull Request Process

1. Update documentation as needed
2. Follow the PR template provided
3. Link related issues: "Fixes #123"
4. Be responsive to review feedback
5. Keep commits atomic and well-organized

## Questions or Need Help?

- Open a discussion issue
- Check existing documentation
- Review similar code sections

## Recognition

Contributors will be acknowledged in:
- README.md contributors section
- GitHub contributors page
- Release notes

Thank you for contributing to QNX RPi4 System Monitor!
