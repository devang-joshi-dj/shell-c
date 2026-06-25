/**
 * tui.c
 *
 * Provides system level utility functions.
 */

#include <stdio.h> // for printf, fflush functions
#include <stdlib.h> // for EXIT_SUCCESS, exit functions

/**
 * Function to clear terminal using ANSI escape sequences
 * \033[2J - clears the entire screen
 * \033[H - move cursor to top-left corner
 */
void clear_screen() {
	printf("\033[2J\033[H");
	fflush(stdout);
}

/**
 * Function to exit the program with a message
 */
void exit_program(const char *message) {
	printf("%s", message);
	exit(EXIT_SUCCESS);
}
