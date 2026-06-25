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

#include "input.h" // for input functions
#include "tui.h" // for TUI functions
#include "system.h" // for system functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 13

void show_welcome_message();
void perform_operations();
void show_operations_menu();

int main() {
	clear_screen();

	show_welcome_message();
	perform_operations();

	return EXIT_SUCCESS;
}

/**
 * Function to show welcome message to user
 */
void show_welcome_message() {
	draw_title("CALCULATOR", FORMAT_WIDTH);
	printf("Welcome to Calculator. Thank You for opening it. It tells me that you already very smart person ;)\n");
}

void perform_operations() {
	show_operations_menu();
	const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);
	draw_single_line_separator(FORMAT_WIDTH);
	printf("\n");

	switch (selected_option) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 0:
			exit_program("Thank you for using Calculator.\n");
			break;
	}

	wait_for_enter();
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
