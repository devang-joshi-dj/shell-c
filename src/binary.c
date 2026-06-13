/**
 * binary.c
 *
 * Binary representation and bit analysis utilities.
 */
#include <stdio.h> // for printf functions
#include <string.h> // for strlen function
#include <stddef.h> // for size_t type
#include <stdbool.h> // for bool type
#include "binary.h"


/**
 * Function to write binary value of the given number in the buffer
 * int num - number to be converted to binary
 * char* buffer - pointer to character array to write into
 * size_t - maximum safe size of buffer to be written into
 */
void number_to_binary(long unsigned int num, char *buffer, size_t size) {
	if (size == 0) return;
	if (num == 0) {
		if (size > 1) {
			buffer[0] = '0';
			buffer[1] = '\0';
		}
	} else {
		size_t write_index = 0;

		// loop until num is 0 or buffer is written upto (size - 1) characters
		// 1 bit reserved for null terminator
		while (num != 0 && write_index < size - 1) {
			buffer[write_index] = (num % 2) + '0'; // converting integer digit to ASCII character digit
			num /= 2; // repeatedly dividing by 2 extracts binary digits from least significant bit
			write_index++;
		}
		buffer[write_index] = '\0';

		// write_index has become now the length of buffer
		// reversing string to form correct binary code
		for (size_t i = 0; i < (write_index / 2); i++) {
			char temp = buffer[i];
			buffer[i] = buffer[write_index-i-1];
			buffer[write_index-i-1] = temp;
		}
	}
}

/**
 * Function to write visualised binary of the given binary value in the buffer
 * char *binary - binary number to be visualised
 * char *buffer - pointer to character array to write into
 */
void get_visual_binary(const char *binary, char *buffer) {
	size_t i;

	for (i = 0; binary[i] != '\0'; i++) {
		// strcat(buffer, binary[i] == '1' ? "█" : "░");
		// buffer[i] = binary[i] == '1' ? '#' : '_';
		buffer[i] = binary[i] == '1' ? '|' : '-';
	}
	buffer[i] = '\0';
}

/**
 * Function to get total bits, ones count, zeros count and to check if the given binary is palindrome or not
 */
BinaryInfo get_binary_analysis(const char *binary) {
	BinaryInfo binary_info = {0}; // initialize all fields to zero to ensure all members have known default values

	binary_info.is_binary_palindrome = true; // assuming binary is palindrome
	binary_info.total_bits_used = strlen(binary); // total bits are equal to length of the binary string value
	binary_info.ones_count = 0;
	binary_info.zeros_count = 0;

	for (size_t i = 0; binary[i] != '\0'; i++) {
		// counting the ones and zeros
		if (binary[i] == '1') binary_info.ones_count++;
		else binary_info.zeros_count++;

		// traversing upto half the length of binary string until it is checked that binary is not palindrome
		if (i < binary_info.total_bits_used / 2 && binary_info.is_binary_palindrome) {
			// checking if i[th] values from start and end of binary string is not equal and if true, binary is not palindrome
			if (binary[i] != binary[binary_info.total_bits_used - 1 - i]) binary_info.is_binary_palindrome = false;
		}
	}

	return binary_info;
}

/**
 * Function to write hex value of the given number in the buffer
 * int num - number to be converted to hex
 * char* buffer - pointer to character array to write into
 * size_t - maximum safe size of buffer to be written into
 */
void number_to_hex(long unsigned int num, char *buffer, size_t size) {
	if (size == 0) return;
	if (num == 0) {
		if (size > 1) {
			buffer[0] = '0';
			buffer[1] = '\0';
		}
	} else {
		size_t write_index = 0;
		const int UNALLOWED_VALUES_FROM = 10;

		// loop until num is 0 or buffer is written upto (size - 1) characters
		// 1 bit reserved for null terminator
		while (num != 0 && write_index < size - 1) {
			int remainder = (num % 16);
			buffer[write_index] = remainder + (remainder >= UNALLOWED_VALUES_FROM ? ('A' - UNALLOWED_VALUES_FROM) : '0'); // converting integer digit to ASCII character digit
			num /= 16; // repeatedly dividing by 2 extracts hex digits from least significant bit
			write_index++;
		}
		buffer[write_index] = 'x';
		buffer[write_index+1] = '0';
		buffer[write_index+2] = '\0';

		// write_index has become now the length of buffer
		// reversing string to form correct hex code
		for (size_t i = 0; i < ((write_index+2) / 2); i++) {
			char temp = buffer[i];
			buffer[i] = buffer[(write_index+2)-i-1];
			buffer[(write_index+2)-i-1] = temp;
		}
	}
}

/**
 * Function to write octal value of the given number in the buffer
 * int num - number to be converted to octal
 * char* buffer - pointer to character array to write into
 * size_t - maximum safe size of buffer to be written into
 */
void number_to_octal(long unsigned int num, char *buffer, size_t size) {
	if (size == 0) return;
	if (num == 0) {
		if (size > 1) {
			buffer[0] = '0';
			buffer[1] = '\0';
		}
	} else {
		size_t write_index = 0;

		// loop until num is 0 or buffer is written upto (size - 1) characters
		// 1 bit reserved for null terminator
		while (num != 0 && write_index < size - 1) {
			buffer[write_index] = (num % 8) + '0'; // converting integer digit to ASCII character digit
			num /= 8; // repeatedly dividing by 2 extracts octal digits from least significant bit
			write_index++;
		}
		buffer[write_index] = '0';
		buffer[write_index+1] = '\0';

		// write_index has become now the length of buffer
		// reversing string to form correct octal code
		for (size_t i = 0; i < ((write_index+1) / 2); i++) {
			char temp = buffer[i];
			buffer[i] = buffer[(write_index+1)-i-1];
			buffer[(write_index+1)-i-1] = temp;
		}
	}
}

/**
 * Function to check if provided binary value is power of 2
 */
bool is_binary_pow_of_2(char *binary) {
	size_t binary_len = strlen(binary);

	if (binary_len > 1 && binary[0] == '1') {
		for (size_t i = 1; i < binary_len; i++) {
			if (binary[i] == '1') return false;
		}
	} else return false;

	return true;
}

/**
 * Function to draw a table for representing positions of each bit of binary
 */
void display_bit_layout(char *binary) {
	size_t binary_len = strlen(binary);
	size_t cols = 8;
	size_t rows = (binary_len + (cols-1)) / cols; // +(cols-1) to ciel the binary_len to be divided by cols

	for (size_t i = 0; i < rows; i++) {
		size_t loop_until = binary_len - (i * cols);

		if (loop_until > cols) loop_until = cols;

		printf("Bit   : ");
		for (size_t j = 0; j < loop_until; j++) printf("%zu ", binary_len - (i * cols) - j - 1);
		printf("\n");

		printf("Value : ");
		for (size_t j = 0; j < loop_until; j++) {
			size_t bit_val = binary_len - (i * cols) - j - 1;

			printf(
			 	bit_val > 99 ? " %c  " : bit_val > 9 ? " %c " : "%c ",
				binary[(i * cols) + j]
			);
		}
		printf("\n\n");
	}

	/*for (size_t i = 0; i < (binary_len*4+1); i++) printf("─");
	printf("\n│");
	for (size_t i = 0; i < binary_len; i++) printf(" %zu │", binary_len - i - 1);
	printf("\n│");
	for (size_t i = 0; i < binary_len; i++) printf(" %c │", binary[i]);
	printf("\n");
	for (size_t i = 0; i < (binary_len*4+1); i++) printf("─");*/
}
