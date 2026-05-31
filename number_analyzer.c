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
#include <limits.h> // for CHAR_BIT, ULONG_MAX
#include <errno.h> // provides errno
#include <stddef.h> // for size_t
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system, strtoul functions
#include <time.h> // for clock function, CLOCKS_PER_SEC
#include <math.h> // for sqrt function
#include <string.h> // for strlen type

// display optimized for 64 character tables
// extremely large values may not fit in all views
#define FORMAT_WIDTH 64
#define CORNER_CHAR_WIDTH 2
#define PADDING_CHAR_WIDTH 2
#define L_PADDING_CHAR_WIDTH 1
#define BINARY_LEN (sizeof(unsigned long int) * CHAR_BIT) + 1 // largest number available for the machine in bits + 1 bit reserved for null terminator

typedef struct {
    int is_binary_palindrome;
    int total_bits_used;
    int ones_count;
    int zeros_count;
} BinaryInfo;

unsigned long int accept_number(const char *prompt);

void get_binary_value(long unsigned int num, char *buffer, size_t size);
void get_visual_binary(const char *binary, char *buffer);
BinaryInfo get_binary_analysis(const char *binary);

int get_digit_count(unsigned long int num);
int get_digit_sum(unsigned long int num);
int is_prime_number(unsigned long int num);
int is_even(const unsigned long int num);
int is_palindrome(unsigned long int num);
int is_armstrong_number(unsigned long int num, const int digit_count);
int is_perfect_number(const unsigned long int num);
int is_harshad_number(const unsigned long int num, const int digit_sum);
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
void draw_open_box_int(const char *label, const unsigned long int value, const int width);
void draw_box_bottom(const int width);

int main() {
    unsigned long int number = accept_number("Reveal thy number for analysis");
    clock_t start_time = clock(); // storing time value after taking user input

    system("clear"); // for clearing terminal in Linux/macOS

    int digit_count = get_digit_count(number);
    int digit_sum = get_digit_sum(number);

    char binary[BINARY_LEN];
    char visual_binary[BINARY_LEN];
    get_binary_value(number, binary, sizeof(binary));
    get_visual_binary(binary, visual_binary);
    BinaryInfo binary_info = get_binary_analysis(binary);

    draw_title("NUMBER ANALYZER", FORMAT_WIDTH);

    draw_header("INPUT DETAILS", FORMAT_WIDTH);
    draw_open_box_int("Entered Number        : ", number, FORMAT_WIDTH);
    draw_open_box_str("Binary Notation       : ", binary, FORMAT_WIDTH);
    draw_open_box_int("Digit Count           : ", digit_count, FORMAT_WIDTH);
    draw_open_box_int("Digit Sum             : ", digit_sum, FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_header("MATHEMATICAL ANALYSIS", FORMAT_WIDTH);
    draw_open_box_str("Prime Number          : ", is_prime_number(number) ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_str("Even / Odd            : ", is_even(number) ? "EVEN" : "ODD", FORMAT_WIDTH);
    draw_open_box_str("Palindrome            : ", is_palindrome(number) ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_str("Armstrong Number      : ", is_armstrong_number(number, digit_count) ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_str("Perfect Number        : ", is_perfect_number(number) ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_str("Harshad Number        : ", is_harshad_number(number, digit_sum) ? "YES" : "NO", FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_header("BINARY ANALYSIS", FORMAT_WIDTH);
    draw_open_box_str("Binary                : ", binary, FORMAT_WIDTH);
    draw_open_box_str("Visual                : ", visual_binary, FORMAT_WIDTH);
    draw_open_box_str("Palindrome            : ", binary_info.is_binary_palindrome ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_int("Total Bits Used       : ", binary_info.total_bits_used, FORMAT_WIDTH);
    draw_open_box_int("Ones Count            : ", binary_info.ones_count, FORMAT_WIDTH);
    draw_open_box_int("Zeros Count           : ", binary_info.zeros_count, FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

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
    char input[100]; // stores raw text entered by the user
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
		 * fgets returns &input[0] (pointer of first address of array of characters) if successful
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
    			 * strtoul converts of string to unsigned long -
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
     			int error_len = FORMAT_WIDTH + get_digit_count(ULONG_MAX);
                    char error_msg[error_len];

                    /**
                    * formatting ULONG_MAX into error message
                    * snprintf prevents writing beyond the buffer size
                    */
              		snprintf(error_msg, error_len, "ERROR: Please provide a number less than %lu", ULONG_MAX);

                    draw_error(error_msg, FORMAT_WIDTH);
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
 * Function to write binary value of the given number in the buffer
 * int num - number to be converted to binary
 * char* buffer - pointer to character array to write into
 * size_t - maximum safe size of buffer to be written into
 */
void get_binary_value(long unsigned int num, char *buffer, size_t size) {
    if (size == 0) return;
    if (num == 0) {
        if (size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0';
        }
    } else {
        int write_index = 0;

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
        for (int i = 0; i < (write_index / 2); i++) {
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
    BinaryInfo binary_info;

    binary_info.is_binary_palindrome = 1; // assuming binary is palindrome
    binary_info.total_bits_used = (int)strlen(binary); // total bits are equal to length of the binary string value
    binary_info.ones_count = 0;
    binary_info.zeros_count = 0;

    for (size_t i = 0; binary[i] != '\0'; i++) {
        // counting the ones and zeros
        if (binary[i] == '1') binary_info.ones_count++;
        else binary_info.zeros_count++;

        // traversing upto half the length of binary string until it is checked that binary is not palindrome
        if (i < binary_info.total_bits_used / 2 && binary_info.is_binary_palindrome) {
            // checking if i[th] values from start and end of binary string is not equal and if true, binary is not palindrome
            if (binary[i] != binary[binary_info.total_bits_used - 1 - i]) binary_info.is_binary_palindrome = 0;
        }
    }

    return binary_info;
}

/**
 * Function to get count of digits of the given number
 */
int get_digit_count(unsigned long int num) {
    int digit_count = 0;

    if (num == 0) digit_count++;

    while (num != 0) {
        digit_count++;
        num /= 10;
    }

    return digit_count;
}

/**
 * Function to get sum of all the digits of the given number
 */
int get_digit_sum(unsigned long int num) {
    int sum = 0;

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
int is_prime_number(unsigned long int num) {
    int flag = 1;
    int unit_digit = get_unit_digit(num);

    // checking if num is divisible of 2 but not 2, is divisible by 3 but not 3 or unit digit is 5 except number 5
    if (
        (num % 2 == 0 && num != 2 ) ||
        (unit_digit == 5 && num != 5) ||
        (num % 3 == 0 && num != 3)
    ) flag = 0;
    else {
        if (num < 2) flag = 0;
        else {
            // checking if the number is divisible by all odd numbers greater than 2 and less than its own square root
            unsigned long int num_root = (unsigned long int)sqrt(num); // casted into int as sqrt returns double
            for (unsigned long int i = 3; i <= num_root; i += 2) {
                if (!(num % i)) {
                    flag = 0;
                    break; // breaking the loop if confirmed that current_number is not prime
                }
            }
        }
    }

    return flag;
}

/**
 * Function to check if the given number is even
 */
int is_even(const unsigned long int num) {
    return num % 2 == 0;
}

/**
 * Function to check if the given number is a palindrome or not
 */
int is_palindrome(unsigned long int num) {
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
 */
int is_armstrong_number(unsigned long int num, const int digit_count) {
    const unsigned long int original_num = num;
    int sum = 0;

    while (num != 0) { // reversing the number through loop
        int unit_digit = get_unit_digit(num);
        sum += (unsigned long int)pow(unit_digit, digit_count); // casted into int as pow returns double
        num /= 10;
    }

    // returning if number is equal to sum of its digits raised to the power number of digits
    return original_num == sum;
}

/**
 * Function to check if the given number is a perfect number or not
 */
int is_perfect_number(const unsigned long int num) {
    int sum = 0;
    unsigned long int traverse_upto = num / 2;

    for (int i = 1; i <= traverse_upto; i++) {
        if (num % i == 0) { // checking for proper divisors of the number
            sum += i;
        }
    }

    return num == sum;// returning number is equal to sum of all proper positive divisors of the number
}

/**
 * Function to check if the given number is a harshad number or not
 */
int is_harshad_number(const unsigned long int num, const int digit_sum) {
    if (!num) return 0;
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
    for (int i = 0; i < actual_width; i++) printf("═");
    printf("╗\n");
}

/**
 * Function to draw bottom line for title with symbols for the given width
 */
void draw_bottom_title_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("╚");
    for (int i = 0; i < actual_width; i++) printf("═");
    printf("╝\n");
}

/**
 * Function to draw bottom line for header with symbols for the given width
 */
void draw_bottom_header_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("├");
    for (int i = 0; i < actual_width; i++) printf("─");
    printf("┤\n");
}

/**
 * Function to draw top line with symbols for the given width
 */
void draw_top_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("┌");
    for (int i = 0; i < actual_width; i++) printf("─");
    printf("┐\n");
}

/**
 * Function to draw bottom line with symbols for the given width
 */
void draw_bottom_line(const int width) {
    int actual_width = width - CORNER_CHAR_WIDTH;

    printf("└");
    for (int i = 0; i < actual_width; i++) printf("─");
    printf("┘\n");
}

/**
 * Function to draw a box with center aligned given title for the given width of the box
 */
void draw_title(const char *title, const int width) {
    int total_empty_space = width - (int)strlen(title) - CORNER_CHAR_WIDTH;
    int one_side_spacing = total_empty_space / 2;
    int other_side_spacing = total_empty_space - one_side_spacing;

    draw_top_title_line(width);
    printf("║");
    for (int i = 0; i < one_side_spacing; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < other_side_spacing; i++) printf(" ");
    printf("║\n");
    draw_bottom_title_line(width);
    printf("\n");
}

/**
 * Function to draw a box with left aligned given header for the given width of the box
 */
void draw_header(const char *header, const int width) {
    int right_side_spacing = width - (int)strlen(header) - CORNER_CHAR_WIDTH - L_PADDING_CHAR_WIDTH;

    draw_top_line(width);
    printf("│ ");
    printf("%s", header);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
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

    int right_side_spacing = width - message_len - CORNER_CHAR_WIDTH - PADDING_CHAR_WIDTH;

    draw_top_line(width);
    printf("│ ");
    printf("%s", message);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf(" │\n");
    draw_box_bottom(width);
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and string value for the given width of the box
 */
void draw_open_box_str(const char *label, const char *value, const int width) {
    int right_side_spacing = width - (int)strlen(label) - (int)strlen(value) - CORNER_CHAR_WIDTH - L_PADDING_CHAR_WIDTH;

    printf("│ ");
    printf("%s%s", label, value);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n");
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and integer value for the given width of the box
 */
void draw_open_box_int(const char *label, const unsigned long int value, const int width) {
    int right_side_spacing = width - (int)strlen(label) - get_digit_count(value) - CORNER_CHAR_WIDTH - L_PADDING_CHAR_WIDTH;

    printf("│ ");
    printf("%s%lu", label, value);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n");
}

/**
 * Function to draw an a line to close the box
 */
void draw_box_bottom(const int width) {
    draw_bottom_line(width);
    printf("\n");
}
