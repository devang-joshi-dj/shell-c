/**
 * The program provides mathematical, logical and binary details about a number provided from user
 * Its provides the following
 *  - Binary notation
 *  - Digit counter
 *  - Digit sum calculation
 *  - Prime number detection
 *  - Even/Odd detection
 *  - Palindrome detection
 *  - Armstrong number detection - number is equal to sum of its digits raised to the power number of digits
 *  - Perfect number detection - number is equal to sum of all proper positive divisors of the number
 *  - Harshad number detection - number is evenly divisible by sum of its digits
 *  - Binary palindrome detection
 *  - Binary digit count (total, 1s, 0s)
 */
#include <stdio.h> // for fgets, printf, snprintf functions
#include <stdlib.h> // for strtoul, system functions
#include <time.h> // for CLOCKS_PER_SEC, clock function, clock_t
#include <errno.h> // provides ERANGE, errno
#include <limits.h> // for CHAR_BIT, ULONG_MAX
#include <stdbool.h> // for bool type
#include <stddef.h> // for NULL, size_t type

#include "tui.h" // for TUI functions
#include "number_properties.h" // for is_number_property functions
#include "digits.h" // for digit functions
#include "binary.h" // for binary functions, BinaryInfo struct

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

// maximum binary digits for unsigned long plus null terminator
#define BINARY_LEN ((sizeof(unsigned long int) * CHAR_BIT) + 1)
#define INPUT_BUFFER_SIZE 100

typedef struct {
	unsigned long int number;
	size_t digit_count;
	size_t digit_sum;
	bool is_prime;
	bool is_even;
	bool is_palindrome;
	bool is_armstrong;
	bool is_perfect;
	bool is_harshad;
	char binary[BINARY_LEN];
	char visual_binary[BINARY_LEN];
	BinaryInfo binary_info;
} AnalysisResult;

unsigned long int accept_number(const char *prompt);
AnalysisResult analyze_number(const unsigned long int num);
void draw_analysis(const unsigned long int num);
void print_analysis(const AnalysisResult *analysis);

int main() {
	const unsigned long int number = accept_number("Reveal thy number for analysis");
	clock_t start_time = clock(); // storing time value after taking user input

	draw_analysis(number);
	AnalysisResult analysis = analyze_number(number);

	system("clear"); // for clearing terminal in Linux/macOS

	print_analysis(&analysis);

	clock_t end_time = clock(); // storing time value after program execution
	double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // elapsed clock ticks converted into seconds

	char end_title[FORMAT_WIDTH];
	snprintf(end_title, FORMAT_WIDTH, "ANALYSIS COMPLETED SUCCESSFULLY IN %lg SECONDS", time_taken);

	draw_title(end_title, FORMAT_WIDTH);

	return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
unsigned long int accept_number(const char *prompt) {
	char input[INPUT_BUFFER_SIZE]; // stores raw text entered by the user
	unsigned long int value; // stores the converted number
	int is_value_allowed; // flag to control the loop

	do {
		printf("%s: ", prompt);
		is_value_allowed = 1; // assuming success for user input

		/**
		 * fgets reads a line of stream from a text - safely, prevents buffer overflows and allows validation
		 * input - storage area for user
		 * sizeof(input) - limit on input characters
		 * stdin - standard input stream (keyboard)
		 * fgets returns input on success, NULL on failure
		 */
		if (fgets(input, sizeof(input), stdin) == NULL) {
			draw_error("ERROR: Failed to read input", FORMAT_WIDTH);
			is_value_allowed = 0;
		} else {
			char *ptr = input;

			while (*ptr == ' ' || *ptr == '\t') ptr++; // skip leading spaces and tabs

			if (*ptr == '\n' || *ptr == '\0') { // checking empty input
				draw_error("ERROR: Please enter a number", FORMAT_WIDTH);
				is_value_allowed = 0; // resetting the value to repeat
			} else if (*ptr == '-') { // checking negative number
				draw_error("ERROR: Please provide a positive number", FORMAT_WIDTH);
				is_value_allowed = 0;
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
					draw_error("ERROR: Please provide a valid number", FORMAT_WIDTH);
					is_value_allowed = 0;
				} else if (errno == ERANGE) { // checking if strtoul report an overflow (number too large for unsigned long)
					const size_t error_len = 128; // Least length FORMAT WIDTH (64) for message and extra length for ULONG_MAX
					char error_msg[error_len];

					/**
					 * formatting ULONG_MAX into error message
					 * snprintf prevents writing beyond the buffer size
					*/
					snprintf(error_msg, error_len, "ERROR: Please provide a number less than %lu", ULONG_MAX);

					draw_error(error_msg, FORMAT_WIDTH);
					is_value_allowed = 0; // resetting the value to repeat
				} else  if (*endptr == '.') { // check for float characters
					draw_error("ERROR: Decimal numbers are not allowed", FORMAT_WIDTH);
					is_value_allowed = 0; // resetting the value to repeat
				} else if (*endptr != '\n' && *endptr != '\0') { // check for invalid characters
					draw_error("ERROR: Please provide a valid number", FORMAT_WIDTH);
					is_value_allowed = 0; // resetting the value to repeat
				}
			}
		}
	} while (!is_value_allowed);

	return value;
}

/**
 * Function to analyze number, show loader and calculate everything before printing
 */
AnalysisResult analyze_number(const unsigned long int num) {
	AnalysisResult result = {0}; // initialize all fields to zero to ensure all members have known default values

	result.number = num;
	result.digit_count = get_digit_count(num);
	result.digit_sum = get_digit_sum(num);
	result.is_prime = is_prime_number(num);
	result.is_even = is_even(num);
	result.is_palindrome = is_palindrome(num);
	result.is_armstrong = is_armstrong_number(num, result.digit_count);
	result.is_perfect = is_perfect_number(num);
	result.is_harshad = is_harshad_number(num, result.digit_sum);

	number_to_binary(num, result.binary, sizeof(result.binary));
	get_visual_binary(result.binary, result.visual_binary);
	result.binary_info = get_binary_analysis(result.binary);

	return result;
}

/**
 * Function to draw loading screen
 */
void draw_analysis(const unsigned long int num) {
	system("clear"); // for clearing terminal in Linux/macOS

	draw_header("ANALYSIS PROGRESS", FORMAT_WIDTH);

	if (num >= 100000000000000000UL) draw_open_box_str( "Large Number Detected. ", "Analysis may take a while.", FORMAT_WIDTH);

	draw_open_box_str("Analyzing...", "", FORMAT_WIDTH);
	draw_box_bottom(FORMAT_WIDTH);
	// fflush(stdout); // to clear the output buffer - for executing above printf before moving forward
}

/**
 * Function to print the analysis of the number provided by user
 */
void print_analysis(const AnalysisResult *analysis) {
	draw_title("NUMBER ANALYZER v1.0", FORMAT_WIDTH);

	draw_header("INPUT DETAILS", FORMAT_WIDTH);
	draw_open_box_ulong("Entered Number        : ", analysis->number, FORMAT_WIDTH);
	draw_open_box_str("Binary Notation       : ", analysis->binary, FORMAT_WIDTH);
	draw_open_box_size_t("Digit Count           : ", analysis->digit_count, FORMAT_WIDTH);
	draw_open_box_size_t("Digit Sum             : ", analysis->digit_sum, FORMAT_WIDTH);
	draw_box_bottom(FORMAT_WIDTH);

	draw_header("MATHEMATICAL ANALYSIS", FORMAT_WIDTH);
	draw_open_box_bool("Prime Number          : ", analysis->is_prime, FORMAT_WIDTH);
	draw_open_box_str("Even / Odd            : ", analysis->is_even ? "EVEN" : "ODD", FORMAT_WIDTH);
	draw_open_box_bool("Palindrome            : ", analysis->is_palindrome, FORMAT_WIDTH);
	draw_open_box_bool("Armstrong Number      : ", analysis->is_armstrong, FORMAT_WIDTH);
	draw_open_box_bool("Perfect Number        : ", analysis->is_perfect, FORMAT_WIDTH);
	draw_open_box_bool("Harshad Number        : ", analysis->is_harshad, FORMAT_WIDTH);
	draw_box_bottom(FORMAT_WIDTH);

	draw_header("BINARY ANALYSIS", FORMAT_WIDTH);
	draw_open_box_str("Binary                : ", analysis->binary, FORMAT_WIDTH);
	draw_open_box_str("Visual                : ", analysis->visual_binary, FORMAT_WIDTH);
	draw_open_box_bool("Palindrome            : ", analysis->binary_info.is_binary_palindrome, FORMAT_WIDTH);
	draw_open_box_size_t("Total Bits Used       : ", analysis->binary_info.total_bits_used, FORMAT_WIDTH);
	draw_open_box_size_t("Ones Count            : ", analysis->binary_info.ones_count, FORMAT_WIDTH);
	draw_open_box_size_t("Zeros Count           : ", analysis->binary_info.zeros_count, FORMAT_WIDTH);
	draw_box_bottom(FORMAT_WIDTH);
}
