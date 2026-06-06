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

		// loop until num is 0 or buffer is written upto size - 1 characters
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
