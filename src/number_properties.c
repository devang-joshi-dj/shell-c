#include <stdbool.h> // for bool type
#include <math.h> // for pow, sqrt function
#include "number_properties.h"

#include "digits.h" // for get_unit_digit function

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
