/**
 * The program provide mathematical, logical and binary details about a number inputted from user
 * Its provides the following
 *  - Palindrome detection
 *  - Binary representation
 *  - Binary palindrome detection
 *  - Digit counter
 *  - Digit sum calculation
 *  - Armstrong number detection
 *  - Perfect number detection
 *  - Harshad number detection
 *  - Prime number detection
 *  - Even/Odd detection
 *  - Magic number score
 * Execute by -> gcc number_analyzer.c -o bin/number_analyzer && ./bin/number_analyzer
 */
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system, abs functions
#include <string.h> // for strlen type

int main();
int accept_number(char *prompt);
char *get_binary_value(int num);
int get_digit_count(int num);
int get_int_digit_sum(int num);

void drawLine(int width);
void draw_title(char *title, int width);
void draw_header(char *header, int width);
void draw_open_box_str(char *label, char *value, int width);
void draw_open_box_int(char *label, int value, int width);
void draw_box_bottom(int width);
int intlen(int num);

int main() {
    const int FORMAT_WIDTH = 40;
    int number = accept_number("Reveal thy number for analysis");

    system("clear"); // for clearing terminal in Linux/macOS

    draw_title("NUMBER ANALYZER", FORMAT_WIDTH);

    draw_header("INPUT DETAILS", FORMAT_WIDTH);
    draw_open_box_int("Entered Number:        ", number, FORMAT_WIDTH);
    draw_open_box_str("Binary Representation: ", get_binary_value(number), FORMAT_WIDTH);
    draw_open_box_int("Digit Count:           ", get_digit_count(number), FORMAT_WIDTH);
    draw_open_box_int("Digit Sum:             ", get_int_digit_sum(number), FORMAT_WIDTH);
    draw_box_bottom(FORMAT_WIDTH);

    draw_title("ANALYSIS COMPLETED SUCCESSFULLY", FORMAT_WIDTH);

    return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
int accept_number(char *prompt) {
	int value;
	int is_value_allowed = 0;

	do {
		printf("%s: ", prompt);
		is_value_allowed = 1;

		// checking if the input is valid integer
		if (scanf("%d", &value) != 1) {
			printf("***Error - Please input a valid number***\n");
			while (getchar() != '\n'); // clearing the buffer
			is_value_allowed = 0; // resetting the value to repeat
		}
	} while (!is_value_allowed);

	return value;
}

/**
 * Function to get binary value of the given number
 */
char *get_binary_value(int num) {
    /*static char binary[8] = ""; // added static keyword to save the binary in memory even after function ends
    char temp[2];

    while (num != 0) {
        sprintf(temp, "%d", num%2);
        strcat(binary, temp);
        num /= 2;
    }

    return binary;*/
    return "1010101";
}

/**
 * Function to get count of digits of the given number
 */
int get_digit_count(int num) {
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
int get_int_digit_sum(int num) {
    int sum = 0;

    while (num != 0) {
        int unit_digit = num%10;
        sum += unit_digit;
        num /= 10;
    }

    return abs(sum);
}

/**
 * Function to draw line with symbols for the given width
 */
void drawLine(int width) {
    int actual_width = width-2;
    printf("+"); // taking up 1 width
    for (int i=0;i<actual_width;i++) printf("-"); // taking up 2 less than width
    printf("+\n"); // taking up 1 width
}

/**
 * Function to draw a box with center aligned given title for the given width of the box
 */
void draw_title(char *title, int width) {
    int total_empty_space = width - strlen(title) - 2;
    int one_side_spacing = total_empty_space / 2;
    int other_side_spacing = total_empty_space - one_side_spacing;
    drawLine(width);
    printf("|"); // taking up 1 width
    for (int i=0;i<one_side_spacing;i++) printf(" ");
    printf("%s", title);
    for (int i=0;i<other_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
    drawLine(width);
    printf("\n");
}

/**
 * Function to draw a box with left aligned given header for the given width of the box
 */
void draw_header(char *header, int width) {
    int right_side_spacing = width - 2 - strlen(header);
    drawLine(width);
    printf("|"); // taking up 1 width
    printf("%s", header);
    for (int i=0;i<right_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
    drawLine(width);
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and string value for the given width of the box
 */
void draw_open_box_str(char *label, char *value, int width) {
    int right_side_spacing = width - 2 - strlen(label) - strlen(value);
    printf("|"); // taking up 1 width
    printf("%s%s", label, value);
    for (int i=0;i<right_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and integer value for the given width of the box
 */
void draw_open_box_int(char *label, int value, int width) {
    int right_side_spacing = width - 2 - strlen(label) - intlen(value);
    printf("|"); // taking up 1 width
    printf("%s%i", label, value);
    for (int i=0;i<right_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
}

/**
 * Function to draw an a line to close the box
 */
void draw_box_bottom(int width) {
    drawLine(width);
    printf("\n");
}

/**
 * Function to get count of digits of the given number
 */
int intlen(int num) {
    int int_char_count = 0;

    if (num <= 0) int_char_count++;

    while (num != 0) {
        int_char_count++;
        num /= 10;
    }

    return int_char_count;
}
