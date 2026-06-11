#ifndef INPUT
#define INPUT

unsigned long accept_unsigned_long(const char *prompt, const int FORMAT_WIDTH);
unsigned int accept_menu_option(
	const char *prompt,
	const unsigned int max_options,
	const int FORMAT_WIDTH
);

#endif
