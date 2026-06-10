/**
 * input.c
 *
 * Operations related to input from user
 */
#include <stdio.h> // for fgets, printf, snprintf functions
#include <stdlib.h> // for strtoul functions
#include <string.h> // for strchr functions
#include <errno.h> // provides ERANGE, errno
#include <limits.h> // for ULONG_MAX
#include <stdbool.h> // for bool type
#include <stddef.h> // for NULL, size_t type

#include "tui.h" // for TUI functions

#define INPUT_BUFFER_SIZE 64

/**
 * Function to accept user input, validate it, and return it
 */
unsigned long accept_unsigned_long(const char *prompt, const int FORMAT_WIDTH) {
	char input[INPUT_BUFFER_SIZE+2]; // stores raw text entered by the user | +2 for \n & \0
	unsigned long value; // stores the converted number
	int is_value_allowed; // flag to control the loop

	do {
		printf("%s: ", prompt);
		is_value_allowed = true; // assuming success for user input

		/**
		 * fgets reads a line of stream from a text - safely, prevents buffer overflows and allows validation
		 * input - storage area for user
		 * sizeof(input) - limit on input characters
		 * stdin - standard input stream (keyboard)
		 * fgets returns input on success, NULL on failure
		 */
		if (fgets(input, sizeof(input), stdin) == NULL) {
			draw_error("Failed to read input", FORMAT_WIDTH);
			is_value_allowed = false;
		} else {
			if (strchr(input, '\n') == NULL) { // if user provided \n not found
				draw_error("Input exceeds maximum allowed length", FORMAT_WIDTH);
				is_value_allowed = false;

				int ch;
				while ((ch = getchar()) != '\n' && ch != EOF);
			} else {
				char *ptr = input;

				while (*ptr == ' ' || *ptr == '\t') ptr++; // skip leading spaces and tabs

				if (*ptr == '\n' || *ptr == '\0') { // checking empty input
					draw_error("Please enter a number", FORMAT_WIDTH);
					is_value_allowed = false; // resetting the value to repeat
				} else if (*ptr == '-') { // checking negative number
					draw_error("Please provide a positive number", FORMAT_WIDTH);
					is_value_allowed = false;
				} else {
					/**
					 * global error indicator to store error information
					 * reset before strtoul so overflow can be detected reliably
					 */
					errno = 0;

					char *endptr; // to be used by strtoul

					/**
					 * strtoul converts a string to unsigned long -
					 * along with overflow detection error detection, leftover character detection and base selection
					 * input - the string to be converted
					 * &endptr - pointer to point to the first character that was not converted
					 * 10 - to interprete input as base decimal
					 */
					value = strtoul(ptr, &endptr, 10);

					if (ptr == endptr) { // checking if no digits found
						draw_error("Please provide a valid number", FORMAT_WIDTH);
						is_value_allowed = false;
					} else if (errno == ERANGE) { // checking if strtoul report an overflow (number too large for unsigned long)
						const size_t error_len = 128; // Least length FORMAT WIDTH (64) for message and extra length for ULONG_MAX
						char error_msg[error_len];

						/**
						 * formatting ULONG_MAX into error message
						 * snprintf prevents writing beyond the buffer size
						*/
						snprintf(error_msg, error_len, "Please provide a number less than %lu", ULONG_MAX);

						draw_error(error_msg, FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else  if (*endptr == '.') { // check for float characters
						draw_error("Decimal numbers are not allowed", FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
						draw_error("Please provide a valid number", FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					}
				}
			}
		}
	} while (!is_value_allowed);

	return value;
}
