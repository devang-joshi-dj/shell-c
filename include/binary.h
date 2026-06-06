#ifndef BINARY
#define BINARY

#include <stddef.h> // for size_t type

typedef struct {
	bool is_binary_palindrome;
	size_t total_bits_used;
	size_t ones_count;
	size_t zeros_count;
} BinaryInfo;

void number_to_binary(long unsigned int num, char *buffer, size_t size);
void get_visual_binary(const char *binary, char *buffer);
BinaryInfo get_binary_analysis(const char *binary);

#endif
