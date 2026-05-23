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
#include <stdlib.h> // for system function
#include <string.h> // for strlen type

int main();
int accept_number(char *prompt);

void drawLine(int width);
void drawTitle(char *title, int width);
void drawHeader(char *header, int width);
void drawOpenBox(char *label, char *value, int width);
void drawBoxBottom(int width);

int main() {
    const int FORMAT_WIDTH = 40;
    int number = accept_number("Reveal thy number for analysis");

    system("clear"); // for clearing terminal in Linux/macOS

    drawTitle("NUMBER ANALYZER", FORMAT_WIDTH);

    drawHeader("INPUT DETAILS", FORMAT_WIDTH);
    drawOpenBox("Entered Number: ", "Num", FORMAT_WIDTH);
    drawOpenBox("Data 2: ", "Value", FORMAT_WIDTH);
    drawOpenBox("Data 3: ", "Value", FORMAT_WIDTH);
    drawOpenBox("Data 4: ", "Value", FORMAT_WIDTH);
    drawBoxBottom(FORMAT_WIDTH);

    drawTitle("ANALYSIS COMPLETED SUCCESSFULLY", FORMAT_WIDTH);

    return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
int accept_number(char *prompt) {
	int value;

	do {
		printf("%s: ", prompt);

		// checking if the input is valid integer
		if (scanf("%d", &value) != 1) {
			printf("***Error - Please input a valid number***\n");
			while (getchar() != '\n'); // clearing the buffer
			value = 0; // resetting the value to repeat
		}
	} while (value < 2);

	return value;
}

void drawLine(int width) {
    int actual_width = width-2;
    printf("+"); // taking up 1 width
    for (int i=0;i<actual_width;i++) printf("-"); // taking up 2 less than width
    printf("+\n"); // taking up 1 width
}
void drawTitle(char *title, int width) {
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
void drawHeader(char *header, int width) {
    int right_side_spacing = width - 2 - strlen(header);
    drawLine(width);
    printf("|"); // taking up 1 width
    printf("%s", header);
    for (int i=0;i<right_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
    drawLine(width);
}
void drawOpenBox(char *label, char *value, int width) {
    int right_side_spacing = width - 2 - strlen(label) - strlen(value);
    printf("|"); // taking up 1 width
    printf("%s%s", label, value);
    for (int i=0;i<right_side_spacing;i++) printf(" ");
    printf("|\n"); // taking up 1 width
}
void drawBoxBottom(int width) {
    drawLine(width);
    printf("\n");
}
