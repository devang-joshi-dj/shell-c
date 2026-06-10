# shell-c

A collection of terminal-based C programs built while learning the C programming language, terminal user interfaces, problem solving, and low-level programming concepts.

## Programs

### Prime Generator
Generates and displays all prime numbers up to a user-specified number.

#### Features

- Validates user input before processing
- Generates prime numbers up to a given number
- Displays the total number of primes found
- Displays all prime numbers generated

#### Concepts Practiced

- Arrays
- Input validation
- Loop optimization
- Function decomposition
- Prime number algorithms
- Time complexity considerations


### Number Analyzer
Analyzes a user-provided number and displays its mathematical, logical and binary properties.

#### Features
- Calculates digit count
- Calculates digit sum
- Detects prime numbers
- Detects even and odd numbers
- Detects palindrome
- Detects armstrong numbers
- Detects perfect numbers
- Detects harshad numbers
- Displays binary notation
- Detects binary palindrome
- Calculates binary digit counts (total, 1s, 0s)

#### Concepts Practiced

- Module architecture (`digits`, `binary`, `number_properties`, `tui`)
- Header and source separation
- Reusable modules
- Makefiles
- Input validation and error handling
- Terminal-based user interfaces
- Number property algorithms
- Binary representation


### Binary Explorer (WIP)
Interactive program for visualizing and manipulating the binary representation of a user-provided number.

#### Features
- Showing number information
	- Signed Decimal representation
	- Binary representation
	- Hexadecimal representation
	- Octal representation
	- Number of set bits
	- Number of clear bits
	- Binary palindrome detection
	- Bit position table
- Setting bit
- Clearing bit
- Toggling bit
- Checking bit
- Shifting bits
- Counting total, set, unset bits,
- Checking if the number is a power of two
- Comparing 2 binary numbers and showing their following results
	- Addition
	- Subtraction
	- AND
	- OR
	- XOR

### Calculator (WIP)
Interactive calculator to perform mathematical operations through a menu-driven interface.

---

## Project Structure
```text
shell-c/
├─ Makefile
├─ compile_flags.txt
├─ README.md
├─ include/     # Public header files
│	└─ foo.h
├─ src/         # Reusable implementation files
│	└─ foo.c
├─ programs/    # Program entry points (contains main())
│	└─ bar.c
├─ bin/         # Compiled executables
│	└─ .gitkeep
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
