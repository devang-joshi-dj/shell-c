/** TODO: Put necessary comments
 * The program provides mathematical, logical and binary details about a number provided from user
 * Its provides the following
 *  - Binary representation
 *  - Digit counter
 *  - Digit sum calculation
 *  - Prime number detection
 *  - Even/Odd detection
 *  - Palindrome detection
 *  - Armstrong number detection
 *  - Perfect number detection
 *  - Harshad number detection
 *  - Binary palindrome detection
 *  - Binary digit count (total, 1s, 0s)
 *  - Magic number score
 * Execute by -> gcc number_analyzer.c -o bin/number_analyzer -lm && ./bin/number_analyzer
 */
#include <stddef.h> // for size_t
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system, abs functions
#include <math.h> // for sqrt function
#include <string.h> // for strlen type

#define FORMAT_WIDTH 60

typedef struct {
    int is_binary_palindrome;
    int total_bits_used;
    int ones_count;
    int zeros_count;
} BinaryInfo;

int main();
unsigned long int accept_number(const char *prompt);
void get_binary_value(long unsigned int num, char *buffer, size_t size);
void get_visual_binary(const char *binary, char *buffer);
BinaryInfo get_binary_analysis(const char *binary);

int get_digit_count(unsigned long int num);
int get_int_digit_sum(unsigned long int num);
int is_prime_number(unsigned long int num);
int check_odd_even(unsigned long int num);
int is_palindrome(unsigned long int num);

void draw_top_title_line(int width);
void draw_bottom_title_line(int width);
void draw_bottom_header_line(int width);
void draw_top_line(int width);
void draw_bottom_line(int width);

void draw_title(char *title, int width);
void draw_header(char *header, int width);
void draw_error(char *header, int width);
void draw_open_box_str(char *label, char *value, int width);
void draw_open_box_int(char *label, unsigned long int value, int width);
void draw_box_bottom(int width);
int intlen(unsigned long int num);

int main() {
    const int BINARY_LEN = 33; // 1 bit reserved for null terminator

    char binary[BINARY_LEN]; // upto 4294967295
    char visual_binary[BINARY_LEN];
    BinaryInfo binary_info;

    unsigned long int number = accept_number("Reveal thy number for analysis");

    system("clear"); // for clearing terminal in Linux/macOS

    get_binary_value(number, binary, sizeof(binary));
    get_visual_binary(binary, visual_binary);
    binary_info = get_binary_analysis(binary);

    draw_title("NUMBER ANALYZER", FORMAT_WIDTH);

    draw_header("INPUT DETAILS", FORMAT_WIDTH);
    draw_open_box_int("Entered Number        : ", number, FORMAT_WIDTH);
    draw_open_box_str("Binary Notation : ", binary, FORMAT_WIDTH);
    draw_open_box_int("Digit Count           : ", get_digit_count(number), FORMAT_WIDTH);
    draw_open_box_int("Digit Sum             : ", get_int_digit_sum(number), FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_header("MATHEMATICAL ANALYSIS", FORMAT_WIDTH);
    draw_open_box_str("Prime Number          : ", is_prime_number(number) ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_str("Even / Odd            : ", check_odd_even(number) ? "EVEN" : "ODD", FORMAT_WIDTH);
    draw_open_box_str("Palindrome            : ", is_palindrome(number) ? "YES" : "NO", FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_header("BINARY ANALYSIS", FORMAT_WIDTH);
    draw_open_box_str("Binary                : ", binary, FORMAT_WIDTH);
    draw_open_box_str("Visual                : ", visual_binary, FORMAT_WIDTH);
    draw_open_box_str("Palindrome            : ", binary_info.is_binary_palindrome ? "YES" : "NO", FORMAT_WIDTH);
    draw_open_box_int("Total Bits Used       : ", binary_info.total_bits_used, FORMAT_WIDTH);
    draw_open_box_int("Ones Count            : ", binary_info.ones_count, FORMAT_WIDTH);
    draw_open_box_int("Zeros Count           : ", binary_info.zeros_count, FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_title("ANALYSIS COMPLETED SUCCESSFULLY", FORMAT_WIDTH);

    return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
unsigned long int accept_number(const char *prompt) {
    const unsigned long int MAX_VAL = 4294967295;
	unsigned long int value;
	int is_value_allowed = 0;

	do {
		printf("%s: ", prompt);
		is_value_allowed = 1;

		// checking if the input is valid integer
		if (scanf("%ld", &value) != 1) {
			draw_error("ERROR: Please provide a valid number", FORMAT_WIDTH);
			while (getchar() != '\n'); // clearing the buffer
			is_value_allowed = 0; // resetting the value to repeat
		}

		if (value > MAX_VAL) {
		    char error_msg[FORMAT_WIDTH];
    		// formatting MAX_VAL into error message
			snprintf(error_msg, FORMAT_WIDTH, "ERROR: Please provide a positive number less than %ld", MAX_VAL);

		    draw_error(error_msg, FORMAT_WIDTH);
    		is_value_allowed = 0; // resetting the value to repeat
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
    int write_index = 0;

    if (size == 0) return;
    if (num == 0) {
        if (size > 1) {
            buffer[0] = '0';
            buffer[1] = '\0'; // null terminator
        }
    } else {
        // loop until num is 0 or buffer is written upto size - 1 characters
        // 1 bit reserved for null terminator
        while (num != 0 && write_index < size - 1) {
            buffer[write_index] = (num % 2) + '0'; // converting integer digit to ASCII character digit
            num /= 2;
            write_index++; // incrementing write_index
        }
        buffer[write_index] = '\0'; // null terminator

        // write_index has become now the length of buffer
        // reversing string to form correct binary code
        for (int i = 0; i < (write_index / 2); i++) {
            char temp;
            temp = buffer[i];
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
    for (size_t i = 0; binary[i] != '\0'; i++) {
        // strcat(buffer, binary[i] == '1' ? "█" : "░");
        // buffer[i] = binary[i] == '1' ? '#' : '_';
        buffer[i] = binary[i] == '1' ? '|' : '-';
    }
}

/**
 * Function to get total bits, ones count, zeros count and to check if the given binary is palindrome or not
 */
BinaryInfo get_binary_analysis(const char *binary) {
    BinaryInfo binary_info;

    binary_info.is_binary_palindrome = 1; // assuming binary is palindrome
    binary_info.total_bits_used = strlen(binary); // total bits are equal to length of the binary string value
    binary_info.ones_count = 0;
    binary_info.zeros_count = 0;

    for (int i = 0; binary[i] != '\0'; i++) {
        // counting the ones and zeros
        if (binary[i] == '1') binary_info.ones_count++;
        else binary_info.zeros_count++;

        // traversing upto half the length of binary string until it is checked that binary is not palindrome
        if ((i <=binary_info.total_bits_used / 2) && binary_info.is_binary_palindrome) {
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

    while (num != 0) {
        digit_count++;
        num /= 10;
    }

    return digit_count;
}

/**
 * Function to get sum of all the digits of the given number
 */
int get_int_digit_sum(unsigned long int num) {
    int sum = 0;

    while (num != 0) {
        int unit_digit = num % 10;
        sum += unit_digit;
        num /= 10;
    }

    return abs(sum);
}

/**
 * Function to check if the given number is prime or not
 */
int is_prime_number(unsigned long int num) {
    int flag = 1;
    int unit_digit = num % 10;

    // checking if num is divisible of 2 but not 2, is divisible by 3 but not 3 or unit digit is 5 except number 5
    if (
        (num % 2 == 0 && num != 2 ) ||
        (unit_digit == 5 && num != 5) ||
        (num % 3 == 0 && num != 3)
    ) flag = 0;
    else {
        if (num < 2) flag = 0;
        else {
            // checking if the number is divisible by all the prime numbers less than its own square root
            int num_root = sqrt(num);
            for (int i = 3; i <= num_root; i += 2) {
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
 * Function to check if the given number is even or odd
 */
int check_odd_even(unsigned long int num) {
    return num % 2 == 0;
}

/**
 * Function to check if the given number is a palindrome or not
 */
int is_palindrome(unsigned long int num) {
    unsigned long int original_num = num;
    unsigned long int reverse_num = 0;

    while (num != 0) { // reversing the number through loop
        int unit_digit = num % 10;
        reverse_num = (reverse_num * 10) + unit_digit;
        num /= 10;
    }

    return original_num == reverse_num;
}

/**
 * Function to draw top line for title with symbols for the given width
 */
void draw_top_title_line(int width) {
    int actual_width = width-2;
    printf("╔"); // taking up 1 width
    for (int i = 0; i < actual_width; i++) printf("═"); // taking up 2 less than width
    printf("╗\n"); // taking up 1 width
}

/**
 * Function to draw bottom line for title with symbols for the given width
 */
void draw_bottom_title_line(int width) {
    int actual_width = width-2;
    printf("╚"); // taking up 1 width
    for (int i = 0; i < actual_width; i++) printf("═"); // taking up 2 less than width
    printf("╝\n"); // taking up 1 width
}

/**
 * Function to draw bottom line for header with symbols for the given width
 */
void draw_bottom_header_line(int width) {
    int actual_width = width-2;
    printf("├"); // taking up 1 width
    for (int i = 0; i < actual_width; i++) printf("─"); // taking up 2 less than width
    printf("┤\n"); // taking up 1 width
}

/**
 * Function to draw top line with symbols for the given width
 */
void draw_top_line(int width) {
    int actual_width = width-2;
    printf("┌"); // taking up 1 width
    for (int i = 0; i < actual_width; i++) printf("─"); // taking up 2 less than width
    printf("┐\n"); // taking up 1 width
}

/**
 * Function to draw bottom line with symbols for the given width
 */
void draw_bottom_line(int width) {
    int actual_width = width-2;
    printf("└"); // taking up 1 width
    for (int i = 0; i < actual_width; i++) printf("─"); // taking up 2 less than width
    printf("┘\n"); // taking up 1 width
}

/**
 * Function to draw a box with center aligned given title for the given width of the box
 */
void draw_title(char *title, int width) {
    int total_empty_space = width - strlen(title) - 2;
    int one_side_spacing = total_empty_space / 2;
    int other_side_spacing = total_empty_space - one_side_spacing;
    draw_top_title_line(width);
    printf("║"); // taking up 1 width
    for (int i = 0; i < one_side_spacing; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < other_side_spacing; i++) printf(" ");
    printf("║\n"); // taking up 1 width
    draw_bottom_title_line(width);
    printf("\n");
}

/**
 * Function to draw a box with left aligned given header for the given width of the box
 */
void draw_header(char *header, int width) {
    int right_side_spacing = width - 3 - strlen(header);
    draw_top_line(width);
    printf("│ "); // taking up 1 width
    printf("%s", header);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n"); // taking up 1 width
    draw_bottom_header_line(width);
}

/**
 * Function to draw a box with left aligned given error for the given width of the box
 */
void draw_error(char *header, int width) {
    int right_side_spacing = width - 3 - strlen(header);
    draw_top_line(width);
    printf("│ "); // taking up 1 width
    printf("%s", header);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n"); // taking up 1 width
    draw_box_bottom(width);
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and string value for the given width of the box
 */
void draw_open_box_str(char *label, char *value, int width) {
    int right_side_spacing = width - 3 - strlen(label) - strlen(value);
    printf("│ "); // taking up 1 width
    printf("%s%s", label, value);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n"); // taking up 1 width
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and integer value for the given width of the box
 */
void draw_open_box_int(char *label, unsigned long int value, int width) {
    int right_side_spacing = width - 3 - strlen(label) - intlen(value);
    printf("│ "); // taking up 1 width
    printf("%s%lu", label, value);
    for (int i = 0; i < right_side_spacing; i++) printf(" ");
    printf("│\n"); // taking up 1 width
}

/**
 * Function to draw an a line to close the box
 */
void draw_box_bottom(int width) {
    draw_bottom_line(width);
    printf("\n");
}

/**
 * Function to get count of digits of the given number
 */
int intlen(unsigned long int num) {
    int int_char_count = 0;

    if (num <= 0) int_char_count++;

    while (num != 0) {
        int_char_count++;
        num /= 10;
    }

    return int_char_count;
}
