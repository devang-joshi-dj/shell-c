/**
 * calculator.c
 *
 * Entry point for the Calculator program.
 * Provides an interactive menu-driven environment for
 * performing calculations, analyzing results, and
 * managing calculation history.
 *
 * The program to perform mathematical operations through interactive menu
 * Menu includes operations -
 *  Addition, Subtraction, Multiplication, Division, Modulus,
 *  Power, Square, Square Root, Cube, Cube Root,
 *  View History, Clear History, Save History to file, Exit
 *  Continuous Calculation, Expression Calculation
 * When user chooses an operation,
 *  if applicable,
 *      it asks number of numbers to input for that particular operation,
 *      it asks base and power to input for that particular operation,
 *  and then input the desired numbers
 * Divide by zero handling
 * History of every calculation with timestamp is recorded and can be cleared
 * Exit with exit message of active session time, calculations done in the exited session of the program
 */
#include <stdio.h> // for printf functions
#include <stdlib.h> // for system function

#include "tui.h" // for TUI functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 13

void show_welcome_message();
void show_operations_menu();

int main() {
	system("clear"); // for clearing terminal in Linux/macOS
	show_welcome_message();
	show_operations_menu();

	return 0;
}

/**
 * Function to show welcome message to user
 */
void show_welcome_message() {
	printf("Welcome to Calculator. Thank You for opening it. It tells me that you already very smart person ;)\n");
}

/**
 * Function to show operations menu to user and accept input, validate it, return it
 */
void show_operations_menu() {
	char menu[MENU_ITEMS][MENU_ITEMS*2] = {
		"Addition",
		"Subtraction",
		"Multiplication",
		"Division",
		"Modulus",
		"Power",
		"Square",
		"Square Root",
		"Cube",
		"Cube Root",
		"View History",
		"Clear History",
		"Save History to file",
	};

	draw_double_line_separator(FORMAT_WIDTH);

	printf("\n");
	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, menu[i]);
	}
	printf("0. Exit\n\n");
}
