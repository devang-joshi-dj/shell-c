/**
 * prime_generator.c
 *
 * Entry point for the Prime Generator program.
 * Handles user interaction and prime number generations.
 */
#include <stdio.h> // for fflush, getchar, printf, scanf functions
#include <stdlib.h> // for system function
#include <time.h> // for CLOCKS_PER_SEC, clock function, clock_t
#include <math.h> // for sqrt function

#define PRIMES_PER_LINE 15

int accept_prime_num_upto(char *prompt);
int is_prime_num (int current_number, int primes[], int total_prime);
void print_primes(int primes[], int total_prime, int first_prime, int last_prime);

int main() {
	clock_t start_calc, end_calc, end_execution; // declaring with special data type to store time values
	double calc_time_taken, execution_time_taken;

	// taking user input for maximum number upto which program will run to find prime values
	int prime_num_upto = accept_prime_num_upto("Please enter the highest number upto which you want to find prime numbers");

	start_calc = clock(); // storing time value after taking user input

	const int PRIME_NUM_FROM = 2;
	int total_prime = 0;
	int prime_arr_size = (prime_num_upto / 4) + 1;
	int primes[prime_arr_size] = {};

	printf("%s", "Calculating...");
	fflush(stdout); // to clear the output buffer - for executing above printf before moving forward

	// pushing first prime number starting from
   	primes[total_prime] = PRIME_NUM_FROM;
   	total_prime += 1;

	// parsing even numbers from (PRIME_NUM_FROM+1) to user's given number and calculating prime numbers
	for (int current_number = PRIME_NUM_FROM+1; current_number <= prime_num_upto; current_number += 2) {
		if (is_prime_num(current_number, primes, total_prime)) {
			primes[total_prime] = current_number;
			total_prime = total_prime + 1;
		}
	}

	end_calc = clock(); // storing time value after program calculation

	system("clear"); // for clearing terminal in Linux/macOS

	print_primes(primes, total_prime, PRIME_NUM_FROM, prime_num_upto); // prints all generated prime numbers

	end_execution = clock(); // storing time value after program execution

	calc_time_taken = ((double)(end_calc - start_calc)) / CLOCKS_PER_SEC; // elapsed clock ticks converted into seconds
	execution_time_taken = ((double)(end_execution - start_calc)) / CLOCKS_PER_SEC; // elapsed clock ticks converted into seconds

	// prints calculation time and total run time of the program
	printf("Calculation Time: \t%f seconds \n", calc_time_taken);
	printf("Execution Time:   \t%f seconds \n", execution_time_taken);

	return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
int accept_prime_num_upto(char *prompt) {
	int value;

	do {
		printf("%s: ", prompt);

		// checking if the input is valid integer
		if (scanf("%d", &value) != 1) {
			printf("> ERROR: Please input a valid number\n");
			while (getchar() != '\n'); // clearing the buffer
			value = 0; // resetting the value to repeat
			continue;
		}

		// checking if the value meets the range condition
		if (value <= 1) printf("> ERROR: Please print value greater or equal than 2\n");
	} while (value < 2);

	return value;
}

/**
 * Function to check if number is prime
 */
int is_prime_num (int current_number, int primes[], int total_prime) {
	int flag = 1;
	int unit_digit = current_number % 10;

	// checking if unit digit is 5 (except number 5) or sum of digits is divisible by 3 but not 3
	if (
		(unit_digit == 5 && current_number != 5) ||
		(current_number % 3 == 0 && current_number != 3)
	) flag = 0;
	else {
		// checking if the number is divisible by known prime numbers upto square root of current_number
		int current_num_root = sqrt(current_number);
		for (int i = 0; i < total_prime; i++) {
			if (primes[i] <= current_num_root) {
				if (!(current_number % primes[i])) {
					flag = 0;
					break; // breaking the loop if confirmed that current_number is not prime
				}
			} else {
				break; // breaking the loop if confirmed that we have parsed known primes upto square root of current_number
			}
		}
	}
	return flag;
}

/**
 * Function to parse through given array of primes and print all the items
 */
void print_primes(int primes[], int total_prime, int first_prime, int last_prime) {
	for (int i = 0; i < total_prime; i++) {
		printf("%d\t", primes[i]);
		if ((i + 1) % PRIMES_PER_LINE == 0) printf("\n"); // to jump on next line after PRIMES_PER_LINE tab included entries in single line
	}

	// prints total prime numbers and the range being calculated from
	printf("\nTotal Prime Numbers found: %d\n", total_prime);
	printf("From %d to %d\n", first_prime, last_prime);
	printf("------------------------------------------\n");
}
