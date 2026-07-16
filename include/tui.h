#ifndef TUI_H
#define TUI_H

#include <stddef.h> // for size_t type
#include <stdbool.h> // for bool type

void show_operations_menu(const char *MENU[], const int MENU_ITEMS, const int FORMAT_WIDTH);
void draw_single_line_separator(const int width);
void draw_double_line_separator(const int width);
void draw_title(const char *title, const int width);
void draw_header(const char *header, const int width);
void draw_error(const char *message, int width);
void draw_range_error(const char *message, int min_range, int max_range, int width);
void draw_open_box_str(
	const char *label,
	const char *value,
	const int width
);

void draw_open_box_ulong(
	const char *label,
	const unsigned long value,
	const int width
);

void draw_open_box_size_t(
	const char *label,
	const size_t value,
	const int width
);

void draw_open_box_bool(
	const char *label,
	const bool value,
	const int width
);

void draw_box_bottom(const int width);

#endif
