/**
 * binary_explorer.c
 *
 * Entry point for the Binary Explorer program.
 * Provides an interactive environment for exploring
 * binary representation and bit-level operations.
 */
#include <stdio.h> // for printf functions
#include <limits.h> // for CHAR_BIT

#include "input.h" // for accept_unsigned_long function

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 10

int show_operations_menu();

int main() {
	const unsigned long number = accept_unsigned_long("Reveal thy number for analysis");
	show_operations_menu();
	return 0;
}

/**
 * Function to show operations menu to user and accept input, validate it, return it
 */
int show_operations_menu() {
	// int value;
	char menu[MENU_ITEMS][MENU_ITEMS*3] = {
		"Show Number Information",
		"Set a Bit",
		"Clear a Bit",
		"Toggle a Bit",
		"Check a Bit",
		"Shift Left",
		"Shift Right",
		"Compare With Another Number",
		"Enter New Number",
		"Exit",
	};

	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, menu[i]);
	}
	printf("%s\n", "Please choose from the menu above to select an operation:");

	// return value;
}
