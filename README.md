# shell-c

A collection of terminal-based C programs built while learning the C programming language, terminal user interfaces, problem solving, and low-level programming concepts.

## Programs

### Prime Generator
Prints all prime numbers up to a user-provided number.

### Number Analyzer
Analyzes a number and displays mathematical, logical and binary properties of the number provided from user

### Calculator (WIP)
Interactive calculator to perform mathematical operations through a menu-driven interface

---

## Project Structure
```text
shell-c/
├─ Makefile
├─ compile_flags.txt
├─ README.md
├─ include/     # Public header files
	└─ foo.h
├─ src/         # Reusable implementation files
	└─ foo.c
├─ programs/    # Program entry points (contains main())
	└─ bar.c
├─ bin/         # Compiled executables
	└─ .gitkeep
```

---

## Building

Build all programs:
```bash
make
```

Build a specific program:
```bash
make prime_generator
make number_analyzer
```

Run a program:
```bash
make run-pg
make run-na
```

Remove compiled executables:
```bash
make clean
```

---

## Development Notes

### Makefile
The project uses a Makefile to simplify compilation and execution of programs.

### compile_flags.txt
The `compile_flags.txt` file provides compiler flags for editor tooling such as clangd and Zed.

This allows the editor to:
- Locate project headers in `include/`
- Use the correct C language standard
- Display compiler warnings while editing

The file mirrors the flags used by the Makefile to keep editor diagnostics consistent with project builds.

## Goals

This repository is primarily a learning project focused on:
- C programming fundamentals
- Modular code organization
- Header and source file separation
- Terminal user interface development
- Number theory and mathematical algorithms
