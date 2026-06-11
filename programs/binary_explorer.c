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

#include "input.h" // for accept_unsigned_long function
#include "binary.h" // for binary functions, BinaryInfo struct
#include "tui.h" // for TUI functions

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

#define MENU_ITEMS 10
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

void clear_screen();
void show_operations_menu();
bool perform_operations(const unsigned long num);

void show_basic_num_info(NumberSnapshot *original);
void show_num_info(NumberSnapshot *original);
void set_bit(NumberSnapshot *original);
void clear_bit(NumberSnapshot *original);
void toggle_bit(NumberSnapshot *original);
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

	printf("\n");
	for (int i = 0; i < MENU_ITEMS; i++) {
		printf("%d. %s\n", i+1, menu[i]);
	}

}

bool perform_operations(const unsigned long num) {
	bool accepting_operation = true;
	bool perform_new_operation = false;


	NumberSnapshot original = {.value = num};
	number_to_binary(num, original.binary, sizeof(original.binary));
	number_to_hex(num, original.hex, sizeof(original.hex));
	number_to_octal(num, original.octal, sizeof(original.octal));

	draw_title("BINARY EXPLORER", FORMAT_WIDTH);
	show_basic_num_info(&original);

	draw_double_line_separator(FORMAT_WIDTH);

	while (accepting_operation) {
		show_operations_menu();
		const int selected_option = accept_menu_option("Please choose from the menu above to select an operation", MENU_ITEMS, FORMAT_WIDTH);
		switch (selected_option) {
			case 1:
				show_num_info(&original);
				break;
			case 2:
				set_bit(&original);
				break;
			case 3:
				clear_bit(&original);
				break;
			case 4:
				toggle_bit(&original);
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
			case 10:
				accepting_operation = false;
				break;
		}
	}

	return perform_new_operation;
}

void show_basic_num_info(NumberSnapshot *original) {
	printf("Current Number\n\n");

	printf("Decimal : %lu\n", original->value);
	printf("Binary  : %s\n", original->binary);
	printf("Hex     : %s\n", original->hex);
	printf("Octal   : %s\n", original->octal);
}
void show_num_info(NumberSnapshot *original) {
	printf("Decimal : %lu\n", original->value);
	printf("Binary  : %s\n", original->binary);
	printf("Hex     : %s\n", original->hex);
	printf("Octal   : %s\n\n", original->octal);

	printf("Set Bits  :");
	printf("Clear Bits  :");
	printf("Total Bits  :");

}
void set_bit(NumberSnapshot *original) {

}
void clear_bit(NumberSnapshot *original) {

}
void toggle_bit(NumberSnapshot *original) {

}
void check_bit(NumberSnapshot *original) {

}
void left_shift(NumberSnapshot *original) {

}
void right_shift(NumberSnapshot *original) {

}
void compare_with_2nd_number(NumberSnapshot *original) {

}
