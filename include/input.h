#ifndef INPUT
#define INPUT

#include <stddef.h> // for size_t type

#define INPUT_BUFFER_SIZE 64 + 2 // +2 for \n & \0

unsigned long accept_unsigned_long(const char *prompt, const int FORMAT_WIDTH);
unsigned int accept_unsigned_int(const char *prompt, const int FORMAT_WIDTH);
long double accept_long_double(const char *prompt, const int FORMAT_WIDTH);
unsigned int accept_menu_option(
	const char *prompt,
	const size_t max_options,
	const int FORMAT_WIDTH
);
void wait_for_enter(void);

#endif
