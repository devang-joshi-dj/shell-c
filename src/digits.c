#include <stddef.h> // for size_t type
#include "digits.h"

/**
 * Function to get count of digits of the given number
 */
size_t get_digit_count(unsigned long int num) {
	size_t digit_count = 0;

	if (num == 0) digit_count++;

	while (num != 0) {
		digit_count++;
		num /= 10; // removes the least significant digit
	}

	return digit_count;
}

/**
 * Function to get sum of all the digits of the given number
 */
size_t get_digit_sum(unsigned long int num) {
	size_t sum = 0;

	while (num != 0) {
		int unit_digit = get_unit_digit(num);
		sum += unit_digit;
		num /= 10;
	}

	return sum;
}


/**
 * Function to return unit digit of given number
 */
int get_unit_digit(const unsigned long int num) {
	return num % 10;
}
