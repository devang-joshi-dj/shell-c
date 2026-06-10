/**
 * number_analyzer.c
 *
 * Entry point for the Number Analyzer program.
 * Coordinates user input, number analysis modules, and terminal output.
 */
#include <stdio.h> // for snprintf function
#include <stdlib.h> // for system function
#include <time.h> // for CLOCKS_PER_SEC, clock function, clock_t
#include <limits.h> // for CHAR_BIT
#include <stddef.h> // for size_t type

#include "input.h" // for accept_unsigned_long function
#include "tui.h" // for TUI functions
#include "number_properties.h" // for is_number_property functions
#include "digits.h" // for digit functions
#include "binary.h" // for binary functions, BinaryInfo struct

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

// maximum binary digits for unsigned long plus null terminator
#define BINARY_LEN ((sizeof(unsigned long) * CHAR_BIT) + 1)
#define INPUT_BUFFER_SIZE 100

typedef struct {
	unsigned long number;
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

AnalysisResult analyze_number(const unsigned long num);
void draw_analysis(const unsigned long num);
void print_analysis(const AnalysisResult *analysis);

int main() {
	const unsigned long number = accept_unsigned_long("Reveal thy number for analysis", FORMAT_WIDTH);
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
 * Function to analyze number, show loader and calculate everything before printing
 */
AnalysisResult analyze_number(const unsigned long num) {
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
void draw_analysis(const unsigned long num) {
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
