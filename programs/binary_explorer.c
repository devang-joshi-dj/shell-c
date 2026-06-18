/**
 * binary_explorer.c
 *
 * Entry point for the Binary Explorer program.
 * Provides an interactive environment for exploring
 * binary representation and bit-level operations.
 */
#include <stdio.h> // for printf functions
#include <stdbool.h> // for bool type
#include <limits.h> // for CHAR_BIT
#include <string.h> // for strlen functions

#include "input.h" // for accept_unsigned_long function
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

void clear_screen();
void show_operations_menu();
bool perform_operations(const unsigned long num);

void show_basic_num_info(NumberSnapshot *original);
void show_num_info(NumberSnapshot *original);
void modify_bit(NumberSnapshot *original, BitOperation operation);
void check_bit(NumberSnapshot *original);
void left_shift(NumberSnapshot *original);
void right_shift(NumberSnapshot *original);
void compare_with_2nd_number(NumberSnapshot *original);

int main() {
	const unsigned long number = accept_unsigned_long("Reveal thy number for analysis", FORMAT_WIDTH);

	bool perform_new_operation = perform_operations(number);

	if (perform_new_operation && number) {
		clear_screen();
		main();
	}

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
bool perform_operations(const unsigned long num) {
	bool accepting_operation = true;
	bool perform_new_operation = false;


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
				left_shift(&original);
				break;
			case 7:
				right_shift(&original);
				break;
			case 8:
				compare_with_2nd_number(&original);
				break;
			case 9:
				perform_new_operation = true;
				accepting_operation = false;
				break;
			case 0:
				accepting_operation = false;
				break;
		}
	}

	return perform_new_operation;
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
	printf("Current Number : ");
	printf("%lu = %s\n\n", original->value, original->binary);

	const int selected_option = accept_menu_option("Enter bit position to set", strlen(original->binary)-1, FORMAT_WIDTH);

	char new_binary[BINARY_LEN];
	strcpy(new_binary, original->binary);

	if (operation == BIT_TOGGLE) {
		new_binary[strlen(original->binary) - selected_option - 1] =
			new_binary[strlen(original->binary) - selected_option - 1] == '1' ? '0' : '1';
	} else {
		new_binary[strlen(original->binary) - selected_option - 1] = operation + '0';
	}

	unsigned long new_value = binary_to_decimal(new_binary);


	printf(
		"\nOperation: %s %i\n\n",
		operation == BIT_TOGGLE ? "TOGGLE BIT" : operation == BIT_SET ? "SET BIT" : "CLEAR BIT",
		selected_option
	);

	printf("Before : %s\n", original->binary);
	printf("After  : %s\n\n", new_binary);

	printf("Decimal Result : %lu\n", new_value);

}
void check_bit(NumberSnapshot *original) {

}
void left_shift(NumberSnapshot *original) {

}
void right_shift(NumberSnapshot *original) {

}
void compare_with_2nd_number(NumberSnapshot *original) {

}
