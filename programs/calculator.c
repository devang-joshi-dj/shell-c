/**
 * calculator.c
 *
 * Entry point for the Calculator program.
 * Provides an interactive menu-driven environment for
 * performing calculations, analyzing results, and
 * managing calculation history.
 */
#include <stdio.h> // for printf
#include <stdlib.h> // for EXIT_SUCCESS, NULL, free, malloc
#include <math.h> // for cbrtl, fmodl, isfinite, powl, sqrtl
#include <errno.h> // for EDOM, errno
#include <stdbool.h> // for bool type
#include <stddef.h> // for size_t type
#include <time.h> // NULL, ctime, difftime, time, time_t

#include "input.h" // for input functions
#include "tui.h" // for TUI functions
#include "system.h" // for system functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 14
#define CONTINUOUS_MENU_ITEMS 10
#define INITIAL_HISTORY_CAPACITY 4
#define FILE_PATH "./bin/calculator_history.txt"

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
	OP_CBRT,
	OP_CONTINUOUS_CALC
} Operation;

typedef struct {
	Operation operation;
	long double operand1;
	long double operand2;

	bool has_second_operand;

	long double result;
	time_t timestamp;
} HistoryEntry;

void perform_operations();
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

void continuous_calculation();
long double continuous_addition(long double num);
long double continuous_subtraction(long double num);
long double continuous_multiplication(long double num);
long double continuous_division(long double num);
long double continuous_modulus(long double num);
long double continuous_power(long double num);
long double continuous_square(long double num);
long double continuous_square_root(long double num);
long double continuous_cube(long double num);
long double continuous_cube_root(long double num);

void display_result(const long double result);
void handle_post_operation(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
);
void add_to_history(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
);
void init_history();
void destroy_history();
void view_history();
void clear_history();
void save_history_to_file();

static HistoryEntry *history = NULL;
static size_t history_capacity = 0;
static size_t history_size = 0;
static size_t calculations_done = 0;
static time_t calc_start_time;

static const char *const MENU[] = { // main menu
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
	"Continuous Calculation Mode",
	"View History",
	"Clear History",
	"Save History to file",
};

static const char *const CONTINUOUS_MENU[] = { // continuous calculation mode menu
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
	"Modulus",
	"Power Raise to",
	"Square",
	"Square Root",
	"Cube",
	"Cube Root",
};

static const char *const OPERATIONS[] = {"+", "-", "*", "/", "%", "^", "²", "√", "³", "∛"};

int main() {
	calc_start_time = time(NULL);
	clear_screen();

	draw_title("CALCULATOR", FORMAT_WIDTH);
	perform_operations();

	return EXIT_SUCCESS;
}

/**
 * Function to keep accepting input for performing calculations until user exits
 */
void perform_operations() {
	init_history();

	while (true) {
		show_operations_menu(MENU, MENU_ITEMS, FORMAT_WIDTH);
		const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);

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
			case 11: continuous_calculation(); break;
			case 12: view_history(); break;
			case 13: clear_history(); break;
			case 14: save_history_to_file(); break;
			case 0:
				destroy_history();
				printf("Active Session Time    : %.0f seconds \n", difftime(time(NULL), calc_start_time));
				printf("Total Calculations Done: %zu\n", calculations_done);
				exit_program("Thank you for using Calculator.\n");
				break;
		}

		if (selected_option != 11) wait_for_enter(); // ignore for continuous calculation
	}
}

void addition() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	const long double result = num1 + num2;

	handle_post_operation(OP_ADD, num1, num2, true, result);
}

void subtraction() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	const long double result = num1 - num2;

	handle_post_operation(OP_SUBTRACT, num1, num2, true, result);
}

void multiplication() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	const long double result = num1 * num2;

	handle_post_operation(OP_MULTIPLY, num1, num2, true, result);
}

void division() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot divide by zero", FORMAT_WIDTH);
		return;
	}

	const long double result = num1 / num2;

	handle_post_operation(OP_DIVIDE, num1, num2, true, result);
}

void modulus() {
	const long double num1 = accept_long_double("Enter first number ", FORMAT_WIDTH);
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot perform modulus by zero", FORMAT_WIDTH);
		return;
	}
	const long double result = fmodl(num1, num2);

	handle_post_operation(OP_MODULUS, num1, num2, true, result);
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

	handle_post_operation(OP_POWER, base, exponent, true, result);
}

void square()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = num * num;

	handle_post_operation(OP_SQUARE, num, 0.0L, false, result);
}

void square_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);

	if (num < 0.0L) {
		draw_error("Cannot calculate the square root of a negative number", FORMAT_WIDTH);
		return;
	}

	const long double result = sqrtl(num);

	handle_post_operation(OP_SQRT, num, 0.0L, false, result);
}

void cube()  {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = num * num * num;

	handle_post_operation(OP_CUBE, num, 0.0L, false, result);
}

void cube_root() {
	const long double num = accept_long_double("Enter number", FORMAT_WIDTH);
	const long double result = cbrtl(num);

	handle_post_operation(OP_CBRT, num, 0.0L, false, result);
}

void continuous_calculation() {
	long double calculating_num = accept_long_double("Enter initial number", FORMAT_WIDTH);
	const long double num = calculating_num;

	bool is_continuous_calculation_active = true;
	while (is_continuous_calculation_active) {
		show_operations_menu(CONTINUOUS_MENU, CONTINUOUS_MENU_ITEMS, FORMAT_WIDTH);
		printf("Current Value = %Lg\n\n", calculating_num);
		const int selected_option = accept_menu_option("Choice", CONTINUOUS_MENU_ITEMS, FORMAT_WIDTH);

		switch (selected_option) {
			case 1: calculating_num = continuous_addition(calculating_num); break;
			case 2: calculating_num = continuous_subtraction(calculating_num); break;
			case 3: calculating_num = continuous_multiplication(calculating_num); break;
			case 4: calculating_num = continuous_division(calculating_num); break;
			case 5: calculating_num = continuous_modulus(calculating_num); break;
			case 6: calculating_num = continuous_power(calculating_num); break;
			case 7: calculating_num = continuous_square(calculating_num); break;
			case 8: calculating_num = continuous_square_root(calculating_num); break;
			case 9: calculating_num = continuous_cube(calculating_num); break;
			case 10: calculating_num = continuous_cube_root(calculating_num); break;
			case 0:
				is_continuous_calculation_active = false;
				printf("Exited continuous calculation mode\n");
				break;
		}
		printf("\nResult = %Lg\n", calculating_num);

		wait_for_enter();
	}
	handle_post_operation(OP_CONTINUOUS_CALC, num, 0, false, calculating_num);
}


long double continuous_addition(long double num) {
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	return num + num2;
}

long double continuous_subtraction(long double num) {
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	return num - num2;
}

long double continuous_multiplication(long double num) {
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);
	return num * num2;
}

long double continuous_division(long double num) {
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot divide by zero", FORMAT_WIDTH);
		return num;
	}

	return num / num2;
}

long double continuous_modulus(long double num) {
	const long double num2 = accept_long_double("Enter second number", FORMAT_WIDTH);

	if (num2 == 0.0L) {
		draw_error("Cannot perform modulus by zero", FORMAT_WIDTH);
		return num;
	}

	return fmodl(num, num2);
}

long double continuous_power(long double num) {
	const long double exponent = accept_long_double("Enter exponent", FORMAT_WIDTH);

	errno = 0;

	const long double result = powl(num, exponent);

	if (errno == EDOM) {
		draw_error("Invalid power operation", FORMAT_WIDTH);
		return num;
	}

	return result;
}

long double continuous_square(long double num) {
	return num * num;
}

long double continuous_square_root(long double num) {
	if (num < 0.0L) {
		draw_error("Cannot calculate the square root of a negative number", FORMAT_WIDTH);
		return num;
	}

	return sqrtl(num);
}

long double continuous_cube(long double num) {
	return num * num * num;
}

long double continuous_cube_root(long double num) {
	return cbrtl(num);
}

/**
 * Function to check and display the result
 */
void display_result(const long double result) {
	printf("\nResult = %Lg\n", result);
}

/**
 * Function to display the result, increment calculations done and adding operation to the history
 */
void handle_post_operation(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
) {
	if (!isfinite(result)) {
		draw_error("Calculation result is outside the supported range", FORMAT_WIDTH);
		return;
	}

	calculations_done++;
	if (operation != OP_CONTINUOUS_CALC) display_result(result);
	add_to_history(operation, num1, num2, has_second_operand, result);
}

/**
 * Function for handling addition of operation done to the history
 */
void add_to_history(
	Operation operation,
	long double num1,
	long double num2,
	bool has_second_operand,
	long double result
) {
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

/**
 * Function to initialise history
 */
void init_history() {
	history_capacity = INITIAL_HISTORY_CAPACITY;
	history_size = 0;

	history = malloc(history_capacity * sizeof(HistoryEntry));

	if (history == NULL) {
		exit_program("Failed to allocate history");
	}
}

/**
 * Function for destroying history
 */
void destroy_history() {
	free(history);
	history = NULL;
	history_capacity = 0;
	history_size = 0;
}

/**
 * Function to view history
 */
void view_history() {
	if (!history_size) {
		printf("History is empty.\n");
		return;
	}
	for (size_t i = 0; i < history_size; i++) {
		if (history[i].has_second_operand) {
			printf(
				"%zu │ %Lg %s %Lg = %Lg │ %s",
				i+1,
				history[i].operand1,
				OPERATIONS[history[i].operation],
				history[i].operand2,
				history[i].result,
				ctime(&history[i].timestamp)
			);
		} else {
			if (history[i].operation == OP_SQRT || history[i].operation == OP_CBRT)
				printf(
					"%zu │ %s%Lg = %Lg │ %s",
					i+1,
					OPERATIONS[history[i].operation],
					history[i].operand1,
					history[i].result,
					ctime(&history[i].timestamp)
				);
			else
				printf(
					"%zu │ %Lg%s = %Lg │ %s",
					i+1,
					history[i].operand1,
					history[i].operation == OP_CONTINUOUS_CALC ? "-> Continuous Operation" : OPERATIONS[history[i].operation],
					history[i].result,
					ctime(&history[i].timestamp)
				);
		}
	}
}

/**
 * Function to clear history
 */
void clear_history() {
	history_size = 0;
	printf("History is cleared.\n");
}

/**
 * Function to write history to given FILE_PATH if history exists
 */
void save_history_to_file() {
	if (!history_size) {
		printf("History is empty.\n");
		return;
	}

	FILE *history_file = fopen(FILE_PATH, "w");

	if (history_file == NULL) {
		draw_error("File cannot be created", FORMAT_WIDTH);
		return;
	}

	for (size_t i = 0; i < history_size; i++) {
		if (history[i].has_second_operand) {
			fprintf(
				history_file,
				"%zu │ %Lg %s %Lg = %Lg │ %s",
				i+1,
				history[i].operand1,
				OPERATIONS[history[i].operation],
				history[i].operand2,
				history[i].result,
				ctime(&history[i].timestamp)
			);
		} else {
			if (history[i].operation == OP_SQRT || history[i].operation == OP_CBRT)
				fprintf(
					history_file,
					"%zu │ %s%Lg = %Lg │ %s",
					i+1,
					OPERATIONS[history[i].operation],
					history[i].operand1,
					history[i].result,
					ctime(&history[i].timestamp)
				);
			else
				fprintf(
					history_file,
					"%zu │ %Lg%s = %Lg │ %s",
					i+1,
					history[i].operand1,
					history[i].operation == OP_CONTINUOUS_CALC ? "-> Continuous Operation" : OPERATIONS[history[i].operation],
					history[i].result,
					ctime(&history[i].timestamp)
				);
		}
	}

	printf("File is written successfully in %s.\n", FILE_PATH);
	fclose(history_file);
}
