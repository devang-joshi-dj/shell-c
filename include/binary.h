#ifndef BINARY
#define BINARY

#include <stddef.h> // for size_t type
#include <stdbool.h> // for bool type

typedef struct {
	bool is_binary_palindrome;
	size_t total_bits_used;
	size_t ones_count;
	size_t zeros_count;
} BinaryInfo;

void number_to_binary(long unsigned int num, char *buffer, size_t size);
void get_visual_binary(const char *binary, char *buffer);
BinaryInfo get_binary_analysis(const char *binary);
void number_to_hex(long unsigned int num, char *buffer, size_t size);
void number_to_octal(long unsigned int num, char *buffer, size_t size);
bool is_binary_pow_of_2(char *binary);
void display_bit_layout(char *binary);
unsigned long binary_to_decimal(char *binary);

#endif
