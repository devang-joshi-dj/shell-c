/**
 * binary_explorer.c
 *
 * Entry point for the Binary Explorer program.
 * Provides an interactive environment for exploring
 * binary representation and bit-level operations.
 */
#include <stdio.h> // for printf functions
#include <stdlib.h> // for EXIT_SUCCESS, abs, labs functions
#include <stdbool.h> // for bool type
#include <limits.h> // for CHAR_BIT
#include <stddef.h> // for size_t type
#include <string.h> // for strcpy, strlen functions

#include "digits.h" // for digit functions
#include "input.h" // for input functions
#include "binary.h" // for binary functions, BinaryInfo struct
#include "tui.h" // for TUI functions
#include "system.h" // for system functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 9
#define ULONG_BITS (sizeof(unsigned long) * CHAR_BIT)
#define BINARY_LEN (ULONG_BITS + 1) // +1 for \0
#define HEX_LEN (ULONG_BITS / 4 + 3) // +3 for \0, 0, x
#define OCTAL_LEN ((ULONG_BITS + 2) / 3 + 2) // +2 for \0, 0

typedef enum {
	BIT_CLEAR,
	BIT_SET,
	BIT_TOGGLE
} BitOperation;

typedef enum {
	SHIFT_LEFT,
	SHIFT_RIGHT
} ShiftDirection;

typedef struct {
	unsigned long value;
	size_t binary_len;
	char binary[BINARY_LEN];
	char hex[HEX_LEN];
	char octal[OCTAL_LEN];
} NumberSnapshot;

void run_binary_explorer();
void perform_operations(const unsigned long num);

void display_current_num_binary(const NumberSnapshot *original);
void show_basic_num_info(const NumberSnapshot *original);
void show_num_info(const NumberSnapshot *original);
void modify_bit(const NumberSnapshot *original, const BitOperation operation);
void check_bit(const NumberSnapshot *original);
void shift_bits(const NumberSnapshot *original, ShiftDirection direction);
void compare_with_2nd_number(const NumberSnapshot *original);
void display_binary_operation(const char *operation, const char *top, const char *bottom, const char *result);
void set_prepend_bits(int difference, char *buffer);

static const char *MENU[] = {
	"Show Number Information",
	"Set a Bit",
	"Clear a Bit",
	"Toggle a Bit",
	"Check a Bit",
	"Shift Left",
	"Shift Right",
	"Compare With Another Number",
	"Enter New Number",
};

int main() {
	run_binary_explorer();
	return EXIT_SUCCESS;
}

void run_binary_explorer() {
	clear_screen();

	const unsigned long number = accept_unsigned_long("Reveal thy number for analysis", FORMAT_WIDTH);

	perform_operations(number);
}

/**
 * Function to keep accepting input for performing operation on given number until user exits
 */
void perform_operations(const unsigned long num) {
	bool accepting_operation = true;

	NumberSnapshot original = {.value = num};
	number_to_binary(num, original.binary, sizeof(original.binary));
	number_to_hex(num, original.hex, sizeof(original.hex));
	number_to_octal(num, original.octal, sizeof(original.octal));
	original.binary_len = strlen(original.binary);

	draw_title("BINARY EXPLORER", FORMAT_WIDTH);
	show_basic_num_info(&original);

	while (accepting_operation) {
		show_operations_menu(MENU, MENU_ITEMS, FORMAT_WIDTH);
		const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);

		switch (selected_option) {
			case 1:
				show_num_info(&original);
				break;
			case 2:
				modify_bit(&original, BIT_SET);
				break;
			case 3:
				modify_bit(&original, BIT_CLEAR);
				break;
			case 4:
				modify_bit(&original, BIT_TOGGLE);
				break;
			case 5:
				check_bit(&original);
				break;
			case 6:
				shift_bits(&original, SHIFT_LEFT);
				break;
			case 7:
				shift_bits(&original, SHIFT_RIGHT);
				break;
			case 8:
				compare_with_2nd_number(&original);
				break;
			case 9:
				accepting_operation = false;
				run_binary_explorer();
				break;
			case 0:
				exit_program("Thank you for using Binary Explorer.\n");
				break;
		}

		wait_for_enter();
	}
}

void display_current_num_binary(const NumberSnapshot *original) {
	printf("Current Number : ");
	printf("%lu = %s\n\n", original->value, original->binary);
}

/**
 * Function to show basic information of the number at the start of the program
 */
void show_basic_num_info(const NumberSnapshot *original) {
	printf("Current Number\n\n");

	printf("Decimal : %lu\n", original->value);
	printf("Binary  : %s\n", original->binary);
	printf("Hex     : %s\n", original->hex);
	printf("Octal   : %s\n", original->octal);
}

/**
 * Function to show detailed information of the number
 */
void show_num_info(const NumberSnapshot *original) {
	BinaryInfo binary_info = get_binary_analysis(original->binary);
	printf("Decimal : %lu\n", original->value);
	printf("Binary  : %s\n", original->binary);
	printf("Hex     : %s\n", original->hex);
	printf("Octal   : %s\n\n", original->octal);

	printf("Set Bits     : %zu\n", binary_info.ones_count);
	printf("Clear Bits   : %zu\n", binary_info.zeros_count);
	printf("Total Bits   : %zu\n\n", binary_info.total_bits_used);

	printf("Power Of Two : %s\n", is_binary_pow_of_2(original->binary) ? "YES" : "NO");
	printf("Palindrome   : %s\n", binary_info.is_binary_palindrome ? "YES" : "NO");
	printf("Parity       : %s\n\n", binary_info.ones_count % 2 ? "ODD" : "EVEN");

	printf("Bit Layout\n\n");
	display_bit_layout(original->binary);
}

/**
 * Function to modify a binary number's bit and display newly generated decimal after binary bit modification
 */
void modify_bit(const NumberSnapshot *original, const BitOperation operation) {
	display_current_num_binary(original);

	const int selected_bit = accept_menu_option(
		operation == BIT_TOGGLE ?
			"Enter bit position to toggle" :
			operation == BIT_SET ?
				"Enter bit position to set" :
				"Enter bit position to clear",
		original->binary_len - 1,
		FORMAT_WIDTH
	);

	char new_binary[BINARY_LEN];
	strcpy(new_binary, original->binary);

	if (operation == BIT_TOGGLE) {
		new_binary[original->binary_len - selected_bit - 1] =
			new_binary[original->binary_len - selected_bit - 1] == '1' ? '0' : '1';
	} else {
		new_binary[original->binary_len - selected_bit - 1] = operation + '0';
	}

	const unsigned long new_value = binary_to_decimal(new_binary);


	printf(
		"\nOperation: %s BIT %d\n\n",
		operation == BIT_TOGGLE ? "TOGGLE" : operation == BIT_SET ? "SET" : "CLEAR",
		selected_bit
	);

	printf("Before : %s\n", original->binary);
	printf("After  : %s\n\n", new_binary);

	printf("Decimal Result : %lu\n", new_value);

}

/**
 * Function to inspect the binary bit and display if selected bit is set or clear
 */
void check_bit(const NumberSnapshot *original) {
	display_current_num_binary(original);

	const int selected_bit = accept_menu_option("Enter bit position to inspect", original->binary_len - 1, FORMAT_WIDTH);

	printf("\nBit Position : %d\n\n", selected_bit);

	printf("Result : %s\n", original->binary[original->binary_len - selected_bit - 1] == '1' ? "SET" : "CLEAR");
}

/**
 * Function to shift the bits of the given number by user provided amount
 */
void shift_bits(const NumberSnapshot *original, ShiftDirection direction) {
	display_current_num_binary(original);

	// Note: For extremely large shift amount, the unexpected answer could be displayed
	const unsigned int shift_amount = accept_unsigned_int("Enter shift amount", FORMAT_WIDTH);

	if (shift_amount >= ULONG_BITS) {
		draw_error("Shift amount must be less than the total number of bits.", FORMAT_WIDTH);
		return;
	}

	const unsigned long new_value = direction == SHIFT_LEFT ?
		original->value << shift_amount :
		original->value >> shift_amount;

	char new_binary[BINARY_LEN];
	number_to_binary(new_value, new_binary, sizeof(new_binary));

	char before_prepend_bits[BINARY_LEN] = "";
	char after_prepend_bits[BINARY_LEN] = "";

	set_prepend_bits(strlen(new_binary) - original->binary_len, before_prepend_bits);
	set_prepend_bits(original->binary_len - strlen(new_binary), after_prepend_bits);

	printf("\nOperation: SHIFT %s %u\n\n", direction == SHIFT_LEFT ? "LEFT" : "RIGHT" , shift_amount);

	printf("Before : %s%s\n", before_prepend_bits, original->binary);
	printf("After  : %s%s\n\n", after_prepend_bits, new_binary);

	printf("Decimal Result : %lu\n", new_value);
}

/**
 * Function to compare user provided number with an additional number and display results of multiple operations
 */
void compare_with_2nd_number(const NumberSnapshot *original) {
	printf("Current Number : %lu\n", original->value);

	const unsigned long new_value = accept_unsigned_long("Enter second number", FORMAT_WIDTH);

	const unsigned long add_value = original->value + new_value;
	/**
	 * Cast operands to long before subtraction to allow signed results.
	 * Note: This is only reliable when both values fit within LONG_MAX.
	 */
	const long diff_value = (long)original->value - (long)new_value;
	const unsigned long and_value = original->value & new_value;
	const unsigned long or_value = original->value | new_value;
	const unsigned long xor_value = original->value ^ new_value;

	char new_binary[BINARY_LEN], add_binary[BINARY_LEN], diff_binary[BINARY_LEN],
	and_binary[BINARY_LEN], or_binary[BINARY_LEN], xor_binary[BINARY_LEN];

	number_to_binary(new_value, new_binary, sizeof(new_binary));
	number_to_binary(add_value, add_binary, sizeof(add_binary));
	number_to_binary(labs(diff_value), diff_binary, sizeof(diff_binary));
	number_to_binary(and_value, and_binary, sizeof(and_binary));
	number_to_binary(or_value, or_binary, sizeof(or_binary));
	number_to_binary(xor_value, xor_binary, sizeof(xor_binary));

	display_binary_operation("Addition", original->binary, new_binary, add_binary);
	printf("Result: %lu\n", add_value);
	draw_single_line_separator(FORMAT_WIDTH);

	if (diff_value < 0) {
		display_binary_operation("Subtraction", original->binary, new_binary, "");
		printf("Result: %ld\n\n", diff_value);
		printf("Magnitude: %s\n", diff_binary);
	} else {
		display_binary_operation("Subtraction", original->binary, new_binary, diff_binary);
		printf("Result: %lu\n", diff_value);
	}
	draw_single_line_separator(FORMAT_WIDTH);

	display_binary_operation("AND", original->binary, new_binary, and_binary);
	printf("Result: %lu\n", and_value);
	draw_single_line_separator(FORMAT_WIDTH);

	display_binary_operation("OR", original->binary, new_binary, or_binary);
	printf("Result: %lu\n", or_value);
	draw_single_line_separator(FORMAT_WIDTH);

	display_binary_operation("XOR", original->binary, new_binary, xor_binary);
	printf("Result: %lu\n", xor_value);
	draw_single_line_separator(FORMAT_WIDTH);
}

/**
 * Function to display an operation of two binaries and their result
 */
void display_binary_operation(const char *operation, const char *top, const char *bottom, const char *result) {
	int top_len = (int)strlen(top);
	int bottom_len = (int)strlen(bottom);
	int result_len = (int)strlen(result);
	int max_len = max_of_three(top_len, bottom_len, result_len);

	char top_prepend_bits[BINARY_LEN] = "";
	char bottom_prepend_bits[BINARY_LEN] = "";
	char result_prepend_bits[BINARY_LEN] = "";

	set_prepend_bits(max_len - top_len, top_prepend_bits);
	set_prepend_bits(max_len - bottom_len, bottom_prepend_bits);
	set_prepend_bits(max_len - result_len, result_prepend_bits);

	printf("\n%s\n\n", operation);

	if (top_len) printf("%s%s\n", top_prepend_bits, top);
	if (bottom_len) printf("%s%s", bottom_prepend_bits, bottom);
	draw_single_line_separator(max_len);
	if (result_len) printf("%s%s\n\n", result_prepend_bits, result);
}

/**
 * Function to prepend bits to the buffer based on the difference
 */
void set_prepend_bits(int difference, char *buffer) {
	if (!difference) return;

	if (difference > 0) {
		int write_index;
		for (write_index = 0; write_index < abs(difference); write_index++) {
			buffer[write_index] = '0';
		}
		buffer[write_index] = '\0';
	}
}
