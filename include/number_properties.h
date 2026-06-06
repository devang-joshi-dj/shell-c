#ifndef NUMBER_PROPERTIES
#define NUMBER_PROPERTIES

#include <stddef.h> // for size_t type
#include <stdbool.h> // for bool type

bool is_prime_number(unsigned long int num);
bool is_even(const unsigned long int num);
bool is_palindrome(unsigned long int num);
bool is_armstrong_number(unsigned long int num, const size_t digit_count);
bool is_perfect_number(const unsigned long int num);
bool is_harshad_number(const unsigned long int num, const size_t digit_sum);

#endif
