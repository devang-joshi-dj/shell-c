# Compiler to use
CC = gcc

# Preprocessor flags (header search paths)
CPPFLAGS = -Iinclude

# Warning flags
CFLAGS = -std=c23 -Wall -Wextra

# Combined compiler flags (preprocessor + warnings)
COMMONFLAGS = $(CPPFLAGS) $(CFLAGS)

# Directory for reusable source code
SRC = src

# Directory for entry-point programs (each contains main())
PROG = programs

# Output directory for compiled binaries
BIN = bin

# Executable names managed by this Makefile
PROGRAM_NAMES = prime_generator number_analyzer

# Full paths to compiled binaries
PROGRAMS = $(addprefix $(BIN)/,$(PROGRAM_NAMES))

# Linker flags (libraries used at final linking stage)
LDFLAGS = -lm

# Phony targets (do not represent files; always executed)
.PHONY: all clean run-na run-pg

# Default target: build all programs
all: $(PROGRAM_NAMES)

number_analyzer:
	$(CC) $(PROG)/number_analyzer.c $(SRC)/tui.c $(COMMONFLAGS) -o $(BIN)/number_analyzer $(LDFLAGS)

prime_generator:
	$(CC) $(PROG)/prime_generator.c $(COMMONFLAGS) -o $(BIN)/prime_generator $(LDFLAGS)

run-na: number_analyzer
	./$(BIN)/number_analyzer

run-pg: prime_generator
	./$(BIN)/prime_generator

# Remove compiled binaries
clean:
	rm -f $(PROGRAMS)
