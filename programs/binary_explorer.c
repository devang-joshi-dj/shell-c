/**
 * binary_explorer.c
 *
 * Entry point for the Binary Explorer program.
 * Provides an interactive environment for exploring
 * binary representation and bit-level operations.
 *
 * An interactive C program for visualizing binary numbers and performing
 * common bitwise operations such as
 *  - show number information such as decimal, binary, hexadecimal, octal representation
 *  - setting bit
 *  - clearing bit
 *  - toggling bit
 *  - checking bit
 *  - shifting bits
 *  - comparing 2 binary numbers
 */
#include <stdio.h> // for printf functions

#define MENU_ITEMS 10

int show_operations_menu();

int main() {
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

	printf("%s\n", "Please choose from the menu below to select an operation:");
	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, menu[i]);
	}

	// return value;
}
