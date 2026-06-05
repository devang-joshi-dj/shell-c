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
 * Execute by -> gcc number_analyzer.c -o bin/number_analyzer -lm && ./bin/number_analyzer
 */
#include <stdio.h> // for printf, fgets functions
#include <stdlib.h> // for system, strtoul functions
#include <limits.h> // for CHAR_BIT, ULONG_MAX
#include <errno.h> // provides errno
#include <stddef.h> // for size_t
#include <time.h> // for clock function, CLOCKS_PER_SEC
#include <math.h> // for sqrt function
#include <string.h> // for strlen type
#include <stdbool.h> // for bool type

// display optimized for 64 character tables
// Note: extremely large values may not fit in all views
#define FORMAT_WIDTH 64

// maximum binary digits for unsigned long plus null terminator
#define BINARY_LEN ((sizeof(unsigned long int) * CHAR_BIT) + 1)
#define INPUT_BUFFER_SIZE 100

#define CORNER_CHAR_WIDTH 2
#define PADDING_CHAR_WIDTH 2
#define L_PADDING_CHAR_WIDTH 1

typedef struct {
    bool is_binary_palindrome;
    size_t total_bits_used;
    size_t ones_count;
    size_t zeros_count;
} BinaryInfo;

typedef struct {
    size_t done_process;
    size_t total_process;
} AnalysisProgress;

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

void number_to_binary(long unsigned int num, char *buffer, size_t size);
void get_visual_binary(const char *binary, char *buffer);
BinaryInfo get_binary_analysis(const char *binary);

size_t get_digit_count(unsigned long int num);
size_t get_digit_sum(unsigned long int num);
bool is_prime_number(unsigned long int num);
bool is_even(const unsigned long int num);
bool is_palindrome(unsigned long int num);
bool is_armstrong_number(unsigned long int num, const size_t digit_count);
bool is_perfect_number(const unsigned long int num);
bool is_harshad_number(const unsigned long int num, const size_t digit_sum);
int get_unit_digit(const unsigned long int num);

void draw_top_title_line(const int width);
void draw_bottom_title_line(const int width);
void draw_bottom_header_line(const int width);
void draw_top_line(const int width);
void draw_bottom_line(const int width);

void draw_title(const char *title, const int width);
void draw_header(const char *header, const int width);
void draw_error(const char *message, int width);
void draw_open_box_str(const char *label, const char *value, const int width);
void draw_open_box_ulong(const char *label, const unsigned long int value, const int width);
void draw_open_box_size_t(const char *label, const size_t value, const int width);
void draw_open_box_bool(const char *label, const bool value, const int width);
void draw_box_bottom(const int width);

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
                errno = 0; // global error indicator to store error information
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

/**
 * Function to get count of digits of the given number
 */
size_t get_digit_count(unsigned long int num) {
    size_t digit_count = 0;

    if (num == 0) digit_count++;

    while (num != 0) {
        digit_count++;
        num /= 10; // removes the least significant digit
    }

    return digit_count;
}

/**
 * Function to get sum of all the digits of the given number
 */
size_t get_digit_sum(unsigned long int num) {
    size_t sum = 0;

    while (num != 0) {
        int unit_digit = get_unit_digit(num);
        sum += unit_digit;
        num /= 10;
    }

    return sum;
}

/**
 * Function to check if the given number is prime or not
 */
bool is_prime_number(unsigned long int num) {
    int unit_digit = get_unit_digit(num);

    // checking if num is divisible of 2 but not 2, is divisible by 3 but not 3 or unit digit is 5 except number 5
    if (
        (num % 2 == 0 && num != 2 ) ||
        (unit_digit == 5 && num != 5) ||
        (num % 3 == 0 && num != 3)
    ) return false; // current_number is not prime

    if (num < 2) return false;
    else if (num == 2 || num == 3 || num == 5) return true;
    else if (num % 2 == 0) return false;
    else if (num % 3 == 0) return false;
    else if (num % 5 == 0) return false;
    else {
        // checking if the number is divisible by all odd numbers greater than 2 and less than its own square root
        unsigned long int num_root = (unsigned long int)sqrt(num); // casted into unsigned long as sqrt returns double
        for (unsigned long int i = 3; i <= num_root; i += 2) {
            if (!(num % i)) {
                return false; // current_number is not prime
            }
        }
    }

    return true;
}

/**
 * Function to check if the given number is even
 */
bool is_even(const unsigned long int num) {
    return num % 2 == 0;
}

/**
 * Function to check if the given number is a palindrome or not
 * Note: very large values may overflow during palindrome reversal
 */
bool is_palindrome(unsigned long int num) {
    const unsigned long int original_num = num;
    unsigned long int reverse_num = 0;

    while (num != 0) { // reversing the number through loop
        int unit_digit = get_unit_digit(num);
        reverse_num = (reverse_num * 10) + unit_digit;
        num /= 10;
    }

    return original_num == reverse_num;
}

/**
 * Function to check if the given number is an armstrong number or not
 * Note: Armstrong calculation may overflow for large inputs
 */
bool is_armstrong_number(unsigned long int num, const size_t digit_count) {
    const unsigned long int original_num = num;
    unsigned long int sum = 0;

    if (num == 0) return true;

    while (num != 0) { // reversing the number through loop
        int unit_digit = get_unit_digit(num);
        sum += (unsigned long int)pow(unit_digit, digit_count); // casted into unsigned long as pow returns double
        num /= 10;
    }

    // returning if number is equal to sum of its digits raised to the power number of digits
    return original_num == sum;
}

/**
 * Function to check if the given number is a perfect number or not
 */
bool is_perfect_number(const unsigned long int num) {
    unsigned long int sum = 1; // 1 is always a proper divisor

    if (num <= 1) return false; // 1 is not a perfect number

    unsigned long int traverse_upto = (unsigned long int)sqrt(num);

    for (unsigned long int i = 2; i <= traverse_upto; i++) {
        if (num % i == 0) { // checking for proper divisors of the number
            sum += i;

            unsigned long int pair = num / i;

            if (pair != i) {
                sum += pair;
            }
        }
    }

    return num == sum;// returning number is equal to sum of all proper positive divisors of the number
}

/**
 * Function to check if the given number is a harshad number or not
 */
bool is_harshad_number(const unsigned long int num, const size_t digit_sum) {
    if (!num) return false;
    return num % digit_sum == 0; // returning if number is evenly divisible by sum of its digits
}

/**
 * Function to return unit digit of given number
 */
int get_unit_digit(const unsigned long int num) {
    return num % 10;
}

/**
 * Function to draw top line for title with symbols for the given width
 */
void draw_top_title_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("╔");
    if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("═");
    printf("╗\n");
}

/**
 * Function to draw bottom line for title with symbols for the given width
 */
void draw_bottom_title_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("╚");
    if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("═");
    printf("╝\n");
}

/**
 * Function to draw bottom line for header with symbols for the given width
 */
void draw_bottom_header_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("├");
    if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
    printf("┤\n");
}

/**
 * Function to draw top line with symbols for the given width
 */
void draw_top_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("┌");
    if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
    printf("┐\n");
}

/**
 * Function to draw bottom line with symbols for the given width
 */
void draw_bottom_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("└");
    if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
    printf("┘\n");
}

/**
 * Function to draw a box with center aligned given title for the given width of the box
 */
void draw_title(const char *title, const int width) {
    int total_empty_space = width - (int)strlen(title) - CORNER_CHAR_WIDTH;
    int left_padding = total_empty_space / 2;
    int right_padding = total_empty_space - left_padding;

    draw_top_title_line(width);
    printf("║");
    for (int i = 0; i < left_padding; i++) printf(" ");
    printf("%s", title);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf("║\n");
    draw_bottom_title_line(width);
    printf("\n");
}

/**
 * Function to draw a box with left aligned given header for the given width of the box
 */
void draw_header(const char *header, const int width) {
    int right_padding =
        width
        - (int)strlen(header)
        - CORNER_CHAR_WIDTH
        - L_PADDING_CHAR_WIDTH;

    draw_top_line(width);
    printf("│ ");
    printf("%s", header);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf("│\n");
    draw_bottom_header_line(width);
}

/**
 * Function to draw a box with left aligned given error for the given width of the box
 */
void draw_error(const char *message, int width) {
    int message_len = (int)strlen(message);

    if (width < (message_len + CORNER_CHAR_WIDTH + PADDING_CHAR_WIDTH))
        width = message_len + CORNER_CHAR_WIDTH + PADDING_CHAR_WIDTH; // increase width of the box if given width is smaller

    int right_padding = width - message_len - CORNER_CHAR_WIDTH - PADDING_CHAR_WIDTH;

    draw_top_line(width);
    printf("│ ");
    printf("%s", message);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf(" │\n");
    draw_box_bottom(width);
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and string value for the given width of the box
 */
void draw_open_box_str(const char *label, const char *value, const int width) {
    int right_padding =
        width
        - (int)strlen(label)
        - (int)strlen(value)
        - CORNER_CHAR_WIDTH
        - L_PADDING_CHAR_WIDTH;

    printf("│ ");
    printf("%s%s", label, value);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf("│\n");
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and unsigned long int value for the given width of the box
 */
void draw_open_box_ulong(const char *label, const unsigned long int value, const int width) {
    int right_padding =
        width
        - (int)strlen(label)
        - (int)get_digit_count(value)
        - CORNER_CHAR_WIDTH
        - L_PADDING_CHAR_WIDTH;

    printf("│ ");
    printf("%s%lu", label, value);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf("│\n");
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and size_t value for the given width of the box
 */
void draw_open_box_size_t(const char *label, const size_t value, const int width) {
    int right_padding =
        width
        - (int)strlen(label)
        - (int)get_digit_count((unsigned long int)value)
        - CORNER_CHAR_WIDTH
        - L_PADDING_CHAR_WIDTH;

    printf("│ ");
    printf("%s%zu", label, value);
    if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
    printf("│\n");
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and boolean value for the given width of the box
 */
void draw_open_box_bool(const char *label, const bool value, const int width) {
    const char *bool_str = value ? "YES" : "NO";
    draw_open_box_str(label, bool_str, width);
}

/**
 * Function to draw an a line to close the box
 */
void draw_box_bottom(const int width) {
    draw_bottom_line(width);
    printf("\n");
}
