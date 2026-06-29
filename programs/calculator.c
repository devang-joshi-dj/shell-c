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
#include <stdlib.h> // for EXIT_SUCCESS
#include <math.h> // for cbrtl, fmodl, powl, sqrtl
#include <errno.h> // for EDOM, errno

#include "input.h" // for input functions
#include "tui.h" // for TUI functions
#include "system.h" // for system functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 13

typedef enum {
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_MODULUS,
	OP_POWER,
	OP_SQUARE,
	OP_SQRT,
	OP_CUBE,
	OP_CBRT
} Operation;

typedef struct {
	Operation operation;
	long double operand1;
	long double operand2;

	bool has_second_operand;

	long double result;
	size_t timestamp;
} HistoryEntry;

void show_welcome_message();
void perform_operations();
void show_operations_menu();
void addition();
void subtraction();
void multiplication();
void division();
void modulus();
void power();
void square();
void square_root();
void cube();
void cube_root();
void display_result(long double result);

// HistoryEntry *history = malloc(4 * sizeof(HistoryEntry));

const char MENU[MENU_ITEMS][MENU_ITEMS*2] = {
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
	printf("Welcome to Calculator. Thank You for opening it. It tells me that you already very smart person ;)");
}

void perform_operations() {
	while (true) {
		show_operations_menu();
		const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);
		draw_single_line_separator(FORMAT_WIDTH);
		printf("\n");

		switch (selected_option) {
			case 1:
				addition();
				break;
			case 2:
				subtraction();
				break;
			case 3:
				multiplication();
				break;
			case 4:
				division();
				break;
			case 5:
				modulus();
				break;
			case 6:
				power();
				break;
			case 7:
				square();
				break;
			case 8:
				square_root();
				break;
			case 9:
				cube();
				break;
			case 10:
				cube_root();
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
}

/**
 * Function to show operations menu to user and accept input, validate it, return it
 */
void show_operations_menu() {
	draw_double_line_separator(FORMAT_WIDTH);

	printf("\n");
	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, MENU[i]);
	}
	printf("0. Exit\n\n");
}

void addition() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	display_result(num1 + num2);
	// add_to_history(num, result);
}

void subtraction() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	display_result(num1 - num2);
}

void multiplication() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	display_result(num1 * num2);
}

void division() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot divide by zero", FORMAT_WIDTH);
		return;
	}

	display_result(num1 / num2);
}

void modulus() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot perform modulus by zero", FORMAT_WIDTH);
		return;
	}

	display_result(fmodl(num1, num2));
}

void power()  {
	const long double base = accept_long_double("Enter base    ", FORMAT_WIDTH);
	const long double exponent = accept_long_double("Enter exponent", FORMAT_WIDTH);

	errno = 0;

	const long double result = powl(base, exponent);

	if (errno == EDOM) {
		draw_error("Invalid power operation", FORMAT_WIDTH);
		return;
	}

	display_result(result);
}

void square()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	display_result(num * num);
}

void square_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	if (num < 0.0L) {
		draw_error("Cannot calculate the square root of a negative number", FORMAT_WIDTH);
		return;
	}

	display_result(sqrtl(num));
}

void cube()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	display_result(num * num * num);
}

void cube_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	display_result(cbrtl(num));
}


void display_result(long double result) {
	if (!isfinite(result)) {
		draw_error("Calculation result is outside the supported range", FORMAT_WIDTH);
		return;
	}

	printf("\nResult = %Lg\n", result);
}
