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
 * Execute by -> gcc binary_convertor.c -o bin/binary_convertor && ./bin/binary_convertor
 */
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system function

int main();
int accept_number(char *prompt);

int main() {
    system("clear"); // for clearing terminal in Linux/macOS

    int number = accept_number("Reveal thy number for analysis");

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
