/**
 * input.c
 *
 * Operations related to input from user
 */
#include <stdio.h> // for fgets, printf, snprintf functions
#include <stdlib.h> // for strtoul, strtold functions
#include <string.h> // for strchr functions
#include <errno.h> // provides ERANGE, errno
#include <limits.h> // for ULONG_MAX
#include <stdbool.h> // for bool type
#include <stddef.h> // for NULL, size_t type
#include <float.h> // for LDBL_MAX, LDBL_MIN

#include "tui.h" // for TUI functions
#include "input.h"

/**
 * Function to accept user input as unsigned long int, validate it, and return it
 */
unsigned long accept_unsigned_long(const char *prompt, const int FORMAT_WIDTH) {
	char input[INPUT_BUFFER_SIZE]; // stores raw text entered by the user
	unsigned long value; // stores the converted number
	bool is_value_allowed; // flag to control the loop

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
					 * along with overflow error detection, leftover character detection and base selection
					 * input - the string to be converted
					 * &endptr - pointer to point to the first character that was not converted
					 * 10 - to interpret input as base decimal
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
						snprintf(error_msg, error_len, "Please provide a number between 0 and %lu", ULONG_MAX);

						draw_error(error_msg, FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else {
						// skip any spaces or tabs remaining after the converted number
						while (*endptr == ' ' || *endptr == '\t') endptr++;

						if (*endptr == '.') { // check for float characters
							draw_error("Decimal numbers are not allowed", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
							draw_error("Please provide a valid number", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						}
					}
				}
			}
		}
	} while (!is_value_allowed);

	return value;
}

/**
 * Function to accept user input as unsigned int, validate it, and return it
 */
unsigned int accept_unsigned_int(const char *prompt, const int FORMAT_WIDTH) {
	char input[INPUT_BUFFER_SIZE]; // stores raw text entered by the user
	unsigned int value; // stores the converted number
	bool is_value_allowed; // flag to control the loop

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
					 * strtoul converts a string to unsigned long as temporary conversion result
					 * along with overflow error detection, leftover character detection and base selection
					 * input - the string to be converted
					 * &endptr - pointer to point to the first character that was not converted
					 * 10 - to interpret input as base decimal
					 */
					unsigned long temp = strtoul(ptr, &endptr, 10);

					if (ptr == endptr) { // checking if no digits found
						draw_error("Please provide a valid number", FORMAT_WIDTH);
						is_value_allowed = false;
					} else if (errno == ERANGE || temp > UINT_MAX) { // checking if value exceeds the range of unsigned int
						const size_t error_len = 128; // Least length FORMAT WIDTH (64) for message and extra length for UINT_MAX
						char error_msg[error_len];

						/**
						 * formatting UINT_MAX into error message
						 * snprintf prevents writing beyond the buffer size
						*/
						snprintf(error_msg, error_len, "Please provide a number between 0 and %u", UINT_MAX);

						draw_error(error_msg, FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else {
						// skip any spaces or tabs remaining after the converted number
						while (*endptr == ' ' || *endptr == '\t') endptr++;

						if (*endptr == '.') { // check for float characters
							draw_error("Decimal numbers are not allowed", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
							draw_error("Please provide a valid number", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else {
							value = (unsigned int)temp;
						}
					}
				}
			}
		}
	} while (!is_value_allowed);

	return value;
}

/**
 * Function to accept user input as long double, validate it, and return it
 */
long double accept_long_double(const char *prompt, const int FORMAT_WIDTH) {
	char input[INPUT_BUFFER_SIZE]; // stores raw text entered by the user
	long double value; // stores the converted long double number
	bool is_value_allowed; // flag to control the loop

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
				} else {
					/**
					 * global error indicator to store error information
					 * reset before strtold so overflow can be detected reliably
					 */
					errno = 0;

					char *endptr; // to be used by strtold

					/**
					 * strtold converts a string to long double
					 * along with overflow error detection, leftover character detection and base selection
					 * input - the string to be converted
					 * &endptr - pointer to point to the first character that was not converted
					 */
					value = strtold(ptr, &endptr);

					if (ptr == endptr) { // checking if no digits found
						draw_error("Please provide a valid number", FORMAT_WIDTH);
						is_value_allowed = false;
					} else if (errno == ERANGE) { // checking if value exceeds the range of long double
						const size_t error_len = 128; // Least length FORMAT WIDTH (64) for message and extra length for LDBL_MAX
						char error_msg[error_len];

						/**
						 * formatting LDBL_MAX into error message
						 * snprintf prevents writing beyond the buffer size
						*/
						snprintf(error_msg, error_len, "Please provide a number between %.3Le and %.3Le", -LDBL_MAX, LDBL_MAX);

						draw_error(error_msg, FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else {
						// skip any spaces or tabs remaining after the converted number
						while (*endptr == ' ' || *endptr == '\t') endptr++;

						if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
							draw_error("Please provide a valid number", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						}
					}
				}
			}
		}
	} while (!is_value_allowed);

	return value;
}

/**
 * Function to accept user input, validate it, and return it for user menu option
 */
unsigned int accept_menu_option(const char *prompt, const size_t max_options, const int FORMAT_WIDTH) {
	char input[INPUT_BUFFER_SIZE]; // stores raw text entered by the user
	unsigned int value; // stores the validated menu option
	bool is_value_allowed; // flag to control the loop

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
					 * strtoul converts a string to unsigned long as temporary conversion result
					 * along with overflow error detection, leftover character detection and base selection
					 * input - the string to be converted
					 * &endptr - pointer to point to the first character that was not converted
					 * 10 - to interpret input as base decimal
					 */
					unsigned long temp = strtoul(ptr, &endptr, 10);

					if (ptr == endptr) { // checking if no digits found
						draw_error("Please provide a valid number", FORMAT_WIDTH);
						is_value_allowed = false;
					} else if (errno == ERANGE || temp > UINT_MAX) { // checking if value exceeds the range of unsigned int
						// draw_error("Value exceeds supported range", FORMAT_WIDTH);
						draw_range_error("Invalid value", 0, max_options, FORMAT_WIDTH);
						is_value_allowed = false; // resetting the value to repeat
					} else {
						// skip any spaces or tabs remaining after the converted number
						while (*endptr == ' ' || *endptr == '\t') endptr++;

						if (*endptr == '.') { // check for float characters
							draw_error("Decimal numbers are not allowed", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
							draw_error("Please provide a valid number", FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else if (temp > max_options) {
							draw_range_error("Invalid value", 0, max_options, FORMAT_WIDTH);
							is_value_allowed = false; // resetting the value to repeat
						} else {
							value = (unsigned int)temp;
						}
					}
				}
			}
		}
	} while (!is_value_allowed);

	draw_single_line_separator(FORMAT_WIDTH);
	printf("\n");

	return value;
}

/**
 * Waits for the user to press Enter before continuing.
 *
 * Discards all characters from the current input line until
 * a newline character or EOF is encountered
 */
void wait_for_enter(void) {
	int ch;

	printf("\nPress ENTER to continue...");

	while((ch = getchar()) != '\n' && ch != EOF);
}
