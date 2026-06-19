/**
 * binary_explorer.c
 *
 * Entry point for the Binary Explorer program.
 * Provides an interactive environment for exploring
 * binary representation and bit-level operations.
 */
#include <stdio.h> // for fflush, printf functions
#include <stdlib.h> // for EXIT_SUCCESS, abs, exit functions
#include <stdbool.h> // for bool type
#include <limits.h> // for CHAR_BIT
#include <stddef.h> // for size_t type
#include <string.h> // for strcpy, strlen functions

#include "input.h" // for input functions
#include "binary.h" // for binary functions, BinaryInfo struct
#include "tui.h" // for TUI functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 9
#define ULONG_BITS (sizeof(unsigned long) * CHAR_BIT)
#define BINARY_LEN (ULONG_BITS + 1) // +1 for \0
#define HEX_LEN (ULONG_BITS / 4 + 3) // +3 for \0, 0, x
#define OCTAL_LEN ((ULONG_BITS + 2) / 3 + 2) // +2 for \0, 0

typedef struct {
	unsigned long value;
	char binary[BINARY_LEN];
	char hex[HEX_LEN];
	char octal[OCTAL_LEN];
} NumberSnapshot;

typedef enum {
	BIT_CLEAR,
	BIT_SET,
	BIT_TOGGLE
} BitOperation;

typedef enum {
	SHIFT_LEFT,
	SHIFT_RIGHT
} ShiftDirection;

void clear_screen();
void show_operations_menu();
void perform_operations(const unsigned long num);

void display_current_num_binary(NumberSnapshot *original);
void show_basic_num_info(NumberSnapshot *original);
void show_num_info(NumberSnapshot *original);
void modify_bit(NumberSnapshot *original, BitOperation operation);
void check_bit(NumberSnapshot *original);
void shift_bits(NumberSnapshot *original, ShiftDirection direction);
void compare_with_2nd_number(NumberSnapshot *original);
void exit_program();

int main() {
	clear_screen();

	const unsigned long number = accept_unsigned_long("Reveal thy number for analysis", FORMAT_WIDTH);

	perform_operations(number);

	return 0;
}

/**
 * Function to clear terminal using ANSI escape sequences
 */
void clear_screen() {
	/**
	 * \033[2J - clears the entire screen
	 * \033[H - move cursor to top-left corner
	 */
	printf("\033[2J\033[H");
	fflush(stdout);
}


/**
 * Function to show operations menu to user and accept input, validate it, return it
 */
void show_operations_menu() {
	char menu[MENU_ITEMS][MENU_ITEMS*4] = {
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

	draw_double_line_separator(FORMAT_WIDTH);

	printf("\n");
	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, menu[i]);
	}
	printf("0. Exit\n\n");

}

/**
 * Function to keep accepting input for performing operation on given number until user exits
 */
void perform_operations(const unsigned long num) {
	bool accepting_operation = true;
	// bool perform_new_operation = false;


	NumberSnapshot original = {.value = num};
	number_to_binary(num, original.binary, sizeof(original.binary));
	number_to_hex(num, original.hex, sizeof(original.hex));
	number_to_octal(num, original.octal, sizeof(original.octal));

	draw_title("BINARY EXPLORER", FORMAT_WIDTH);
	show_basic_num_info(&original);

	while (accepting_operation) {
		show_operations_menu();
		// const int selected_option = accept_menu_option("Please choose from the menu above to select an operation", MENU_ITEMS, FORMAT_WIDTH);
		const int selected_option = accept_menu_option("Choice", MENU_ITEMS, FORMAT_WIDTH);

		draw_single_line_separator(FORMAT_WIDTH);
		printf("\n");

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
				main();
				break;
			case 0:
				exit_program();
				break;
		}

		wait_for_enter();
	}
}

void display_current_num_binary(NumberSnapshot *original) {
	printf("Current Number : ");
	printf("%lu = %s\n\n", original->value, original->binary);
}

/**
 * Function to show basic information of the number at the start of the program
 */
void show_basic_num_info(NumberSnapshot *original) {
	printf("Current Number\n\n");

	printf("Decimal : %lu\n", original->value);
	printf("Binary  : %s\n", original->binary);
	printf("Hex     : %s\n", original->hex);
	printf("Octal   : %s\n", original->octal);
}

/**
 * Function to show detailed information of the number
 */
void show_num_info(NumberSnapshot *original) {
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
void modify_bit(NumberSnapshot *original, BitOperation operation) {
	display_current_num_binary(original);

	const int selected_bit = accept_menu_option(
		operation == BIT_TOGGLE ?
			"Enter bit position to toggle" :
			operation == BIT_SET ?
				"Enter bit position to set" :
				"Enter bit position to clear",
		strlen(original->binary) - 1,
		FORMAT_WIDTH
	);

	char new_binary[BINARY_LEN];
	strcpy(new_binary, original->binary);

	if (operation == BIT_TOGGLE) {
		new_binary[strlen(original->binary) - selected_bit - 1] =
			new_binary[strlen(original->binary) - selected_bit - 1] == '1' ? '0' : '1';
	} else {
		new_binary[strlen(original->binary) - selected_bit - 1] = operation + '0';
	}

	unsigned long new_value = binary_to_decimal(new_binary);


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
void check_bit(NumberSnapshot *original) {
	display_current_num_binary(original);

	const int selected_bit = accept_menu_option("Enter bit position to inspect", strlen(original->binary) - 1, FORMAT_WIDTH);

	printf("\nBit Position : %d\n\n", selected_bit);

	printf("Result : %s\n", original->binary[selected_bit] == '1' ? "SET" : "CLEAR");
}

/**
 * Function to shift the bits of the given number by user provided amount
 */
void shift_bits(NumberSnapshot *original, ShiftDirection direction) {
	display_current_num_binary(original);

	const unsigned long shift_amount = accept_unsigned_long("Enter shift amount", FORMAT_WIDTH);

	const int new_value = direction == SHIFT_LEFT ?
		original->value << shift_amount :
		original->value >> shift_amount;

	char new_binary[BINARY_LEN];
	number_to_binary(new_value, new_binary, sizeof(new_binary));

	int binary_len_diff = strlen(original->binary) - strlen(new_binary);
	char prepend_bits[BINARY_LEN];

	if (binary_len_diff) {
		int write_index;
		for (write_index = 0; write_index < abs(binary_len_diff); write_index++) {
			prepend_bits[write_index] = '0';
		}
		prepend_bits[write_index] = '\0';
	}

	printf("\nOperation: SHIFT %s %lu\n\n", direction == SHIFT_LEFT ? "LEFT" : "RIGHT" , shift_amount);

	printf("Before : %s%s\n", binary_len_diff < 0 ? prepend_bits : "", original->binary);
	printf("After  : %s%s\n\n", binary_len_diff > 0 ? prepend_bits : "", new_binary);

	printf("Decimal Result : %d\n", new_value);
}

void compare_with_2nd_number(NumberSnapshot *original) {

}

void exit_program() {
	printf("Thank you for using Binary Explorer.\n");
	printf("Goodbye!\n");
	exit(EXIT_SUCCESS);
}
