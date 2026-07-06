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
#include <stdio.h> // for printf
#include <stdlib.h> // for EXIT_SUCCESS, NULL, free, malloc
#include <math.h> // for cbrtl, fmodl, isfinite, powl, sqrtl
#include <errno.h> // for EDOM, errno
#include <stddef.h> // for size_t type
#include <time.h> // NULL, time, time_t

#include "input.h" // for input functions
#include "tui.h" // for TUI functions
#include "system.h" // for system functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 13
#define INITIAL_HISTORY_CAPACITY 4

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
	time_t timestamp;
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
void init_history();
void add_to_history(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
);
void view_history();
void clear_history();
void save_history();

HistoryEntry *history = NULL;
size_t history_capacity = 0;
size_t history_size = 0;

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

const char *OPERATIONS[] = {"+", "-", "*", "/", "%", "^", "^2", "^½", "^3", "^⅓"};

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
	init_history();

	while (true) {
		show_operations_menu();
		const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);
		draw_single_line_separator(FORMAT_WIDTH);
		printf("\n");

		switch (selected_option) {
			case 1: addition(); break;
			case 2: subtraction(); break;
			case 3: multiplication(); break;
			case 4: division(); break;
			case 5: modulus(); break;
			case 6: power(); break;
			case 7: square(); break;
			case 8: square_root(); break;
			case 9: cube(); break;
			case 10: cube_root(); break;
			case 11: view_history(); break;
			case 12: clear_history(); break;
			case 13: save_history(); break;
			case 0:
				clear_history();
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
	const long double result = num1 + num2;

	add_to_history(OP_ADD, num1, num2, true, result);
}

void subtraction() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	const long double result = num1 - num2;

	add_to_history(OP_SUBTRACT, num1, num2, true, result);
}

void multiplication() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	const long double result = num1 * num2;

	add_to_history(OP_MULTIPLY, num1, num2, true, result);
}

void division() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot divide by zero", FORMAT_WIDTH);
		return;
	}

	const long double result = num1 / num2;

	add_to_history(OP_DIVIDE, num1, num2, true, result);
}

void modulus() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot perform modulus by zero", FORMAT_WIDTH);
		return;
	}
	const long double result = fmodl(num1, num2);

	add_to_history(OP_MODULUS, num1, num2, true, result);
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

	add_to_history(OP_POWER, base, exponent, true, result);
}

void square()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = num * num;

	add_to_history(OP_SQUARE, num, 0, false, result);
}

void square_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	if (num < 0.0L) {
		draw_error("Cannot calculate the square root of a negative number", FORMAT_WIDTH);
		return;
	}

	const long double result = sqrtl(num);

	add_to_history(OP_SQRT, num, 0, false, result);
}

void cube()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = num * num * num;

	add_to_history(OP_CUBE, num, 0, false, result);
}

void cube_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = cbrtl(num);

	add_to_history(OP_CBRT, num, 0, false, result);
}

void display_result(long double result) {
	if (!isfinite(result)) {
		draw_error("Calculation result is outside the supported range", FORMAT_WIDTH);
		return;
	}

	printf("\nResult = %Lg\n", result);
}

void init_history() {
	history_capacity = INITIAL_HISTORY_CAPACITY;
	history_size = 0;

	history = malloc(history_capacity * sizeof(HistoryEntry));

	if (history == NULL) {
		exit_program("Failed to allocate history");
	}
}

void add_to_history(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
) {
	display_result(result);

	if (history_size == history_capacity) {
		size_t new_capacity = history_capacity * 2;

		HistoryEntry *temp = realloc(history, new_capacity * sizeof(HistoryEntry));

		if (temp == NULL) {
			exit_program("Failed to allocate more space to history");
		}
		history = temp;
		history_capacity = new_capacity;
	}

	history[history_size++] = (HistoryEntry){
		.operation = operation,
		.operand1 = num1,
		.operand2 = num2,
		.has_second_operand = has_second_operand,
		.result = result,
		.timestamp = time(NULL)
	};
}

void view_history();

void clear_history() {
	free(history);

	history = NULL;
	history_capacity = 0;
	history_size = 0;
}

void save_history();
