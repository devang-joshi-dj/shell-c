/**
 * The program accepts highest value from user
 * and generates prime numbers from 2 to upto the provided number
 * Execute by -> gcc prime_generator.c -o bin/prime_generator && ./bin/prime_generator
 */
#include <stdio.h> // for printf, scanf, getchar functions
#include <stdlib.h> // for system function
#include <time.h> // for clock function, CLOCKS_PER_SEC

int main();
int accept_prime_num_upto(const char *prompt);
int is_prime_num (int current_number);
void print_primes(int primes[], int total_prime);

int main() {
    clock_t start, end; // special data type to store time values
    double time_taken;

    // taking user input for maximum number upto which program will run to find prime values
	long int prime_num_upto = accept_prime_num_upto("Please enter the highest number upto which you want to find prime numbers");

	start = clock(); // storing time value after taking user input

	const int PRIME_NUM_FROM = 2;
	int total_prime = 0;
	int prime_arr_size = prime_num_upto/4+1;
	int primes[prime_arr_size] = {};

	system("clear"); // for clearing terminal in Linux/macOS

	// parsing from PRIME_NUM_FROM to user's given number and calculating prime numbers
	for (int current_number=PRIME_NUM_FROM;current_number<=prime_num_upto;current_number++) {
		if (is_prime_num(current_number)) {
			primes[total_prime] = current_number;
			total_prime = total_prime + 1;
		}
	}

	// prints all generated prime numbers
	print_primes(primes, total_prime);

	end = clock(); // storing time value after program execution
	time_taken = ((double)(end-start)) / CLOCKS_PER_SEC; // elapsed clock ticks converted into seconds

	// prints total prime numbers and the range being calculated from
	printf("\nTotal Prime Numbers found - %d\n", total_prime);
	printf("From %d to %ld\n", PRIME_NUM_FROM, prime_num_upto);
	printf("Execution time: %f seconds \n", time_taken);

	return 0;
}

/**
 * Function to accept user input, validate it, and return it
 */
int accept_prime_num_upto(const char *prompt) {
	int value;

	do {
		printf("%s: ", prompt);

		// checking if the input is valid integer
		if (scanf("%d", &value) != 1) {
			printf("***Error - Please input a valid number***\n");
			while (getchar() != '\n'); // clearing the buffer
			value = 0; // resetting the value to repeat
			continue;
		}

		// checking if the value meets the range condition
		if (value <= 1) printf("***Error - Please print value greater or equal than 2***\n");
	} while (value < 2);

	return value;
}

/**
 * Function to check if number is prime
 */
int is_prime_num (int current_number) {
    int flag = 1;
    for (int i = (current_number/2);i>1;i--) {
        if (!(current_number%i)) flag = 0;
    }
    return flag;
}

/**
 * Function to parse through given array of primes and print all the items
 */
void print_primes(int primes[], int total_prime) {
    for (int i=0;i<total_prime;i++) {
        printf("%d\t", primes[i]);
        if ((i+1)%20==0) printf("\n"); // to jump on next line after 20 tab included entries in single line
    }
}
