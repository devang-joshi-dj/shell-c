/**
 * The program to perform mathematical operations through interactive menu
 * Menu includes operations -
 *  Addition, Subtraction, Multiplication, Division,
 *  Power, Square, Square Root, Cube, Cube Root, Modulus,
 *  View History, Clear History, Save History to file, Exit
 *  Continuous Calculation, Expression Calculation (in near future)
 * When user chooses an operation,
 *  if applicable,
 *      it asks number of numbers to input for that particular operation,
 *      it asks base and power to input for that particular operation,
 *  and then input the desired numbers
 * Divide by zero handling
 * Even/Odd, Prime Number, Palindrome detector of result
 * History of every calculation is recorded and can be cleared
 * Exit with exit message of calculations done in the exited session of the program
 * Execute by -> gcc calculator.c -o bin/calculator && ./bin/calculator
 */
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system function

#define MENU_ITEMS 15

int main();
void show_welcome_message();
int show_operations_menu();

int main() {
    system("clear"); // for clearing terminal in Linux/macOS
    show_welcome_message();
    show_operations_menu();

    return 0;
}

/**
 * Function to show welcome message to user
 */
void show_welcome_message() {
    printf("Welcome to Calculator. Thank You for opening it. It tells me that you already very smart person ;)\n");
}

/**
 * Function to show operations menu to user and accept input, validate it, return it
 */
int show_operations_menu() {
	int value;
	char menu[MENU_ITEMS][MENU_ITEMS*4] = {
	    "Please choose from the menu below to select an operation:",
		"Addition",
		"Subtraction",
		"Multiplication",
		"Division",
		"Power",
		"Square",
		"Square Root",
		"Cube",
		"Cube Root",
		"Modulus",
		"View History",
		"Clear History",
		"Save History to file",
		"Exit"
	};

	for (int i=0;i<MENU_ITEMS;i++) {
	    if (!i) printf("%s\n", menu[0]);
	    printf("%d. %s\n", i, menu[i]);
	}

	return value;
}
