/**
 * The program accepts highest value from user
 * and generates prime numbers from 2 to upto the provided number
 * Logic for efficiency to check a number for bieng a prime number
 *  - Divide the number with all the numbers equal to and smaller than itself upto 1 and only 2 divisions should be possible
 *  - OR
 *  - Numbers ending in 0, 2, 4, 6, or 8 are even and never prime (except 2).
 *  - Numbers ending in 5 are divisible by 5 and never prime (except 5).
 *  - If the sum of the digits is divisible by 3, the number is not prime.
 *  - Divide the number by all prime numbers (e.g., 7, 11, 13) that are less than sqrt of number.
 *  - If none of these divisions result in a whole number, the number is prime.
 * Execute by -> gcc prime_generator.c -o bin/prime_generator -lm && ./bin/prime_generator
 */
#include <stdio.h> // for printf, scanf, getchar functions
#include <math.h> // for sqrt function
#include <time.h> // for clock function, CLOCKS_PER_SEC
#include <stdlib.h> // for system function

int main();
int accept_prime_num_upto(char *prompt);
int is_prime_num (int current_number, int primes[], int total_prime);
int getIntDigitSum(int num);
int getUnitDigit(int num);
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

	printf("%s", "Calculating...");
	fflush(stdout); // to clear the output buffer - for executing above printf before moving forward

	// pushing first prime number starting from
   	primes[total_prime] = PRIME_NUM_FROM;
   	total_prime += 1;

	// parsing even numbers from (PRIME_NUM_FROM+1) to user's given number and calculating prime numbers
	for (int current_number=PRIME_NUM_FROM+1;current_number<=prime_num_upto;current_number += 2) {
		if (is_prime_num(current_number, primes, total_prime)) {
			primes[total_prime] = current_number;
			total_prime = total_prime + 1;
		}
	}

	system("clear"); // for clearing terminal in Linux/macOS

	// prints all generated prime numbers
	print_primes(primes, total_prime);

	end = clock(); // storing time value after program execution
	time_taken = ((double)(end-start)) / CLOCKS_PER_SEC; // elapsed clock ticks converted into seconds

	// prints total prime numbers and the range being calculated from
	printf("\nTotal Prime Numbers found: %d\n", total_prime);
	printf("From %d to %ld\n", PRIME_NUM_FROM, prime_num_upto);
	printf("Execution time: %f seconds \n", time_taken);

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
int is_prime_num (int current_number, int primes[], int total_prime) {
    int flag = 1;
    int unit_digit = getUnitDigit(current_number);

    // checking if unit digit is 5 (except number 5) or sum of digits is divisible by 3 but not 3
    if (
        (unit_digit == 5 && current_number != 5) ||
        (getIntDigitSum(current_number)%3 == 0 && current_number != 3)
    ) {
        flag = 0;
    } else {
        // checking if the number is divisible by all the prime numbers less than its own square root
        int current_num_root = sqrt(current_number);
        for (int i = 0; i < total_prime; i++) {
            if (primes[i] <= current_num_root) {
                if (!(current_number % primes[i])) {
                    flag = 0;
                    break; // breaking the loop if confirmed that current_number is not prime
                }
            } else {
                break; // breaking the loop if confirmed that we have parsed founded primes upto square root of current_number
            }
        }
    }
    return flag;
}

/**
 * Function to get sum of all the digits of given number
 */
int getIntDigitSum(int num) {
    int sum = 0;

    do {
        int unit_digit = getUnitDigit(num);
        sum += unit_digit;
        num /= 10;
    } while (num > 0);

    return sum;
}

/**
 * Function to return unit digit of given number
 */
int getUnitDigit(int num) {
    return num%10;
}

/**
 * Function to parse through given array of primes and print all the items
 */
void print_primes(int primes[], int total_prime) {
    for (int i=0;i<total_prime;i++) {
        printf("%d\t", primes[i]);
        if ((i+1)%15==0) printf("\n"); // to jump on next line after 15 tab included entries in single line
    }
}
