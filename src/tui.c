#include <stdio.h> // for printf, snprintf functions
#include <string.h> // for strlen function
#include "tui.h"

#define CORNER_CHAR_WIDTH 2
#define PADDING_CHAR_WIDTH 2
#define L_PADDING_CHAR_WIDTH 1

// static functions are private to this file, not global
// for internal use only
static void draw_top_title_line(const int width);
static void draw_bottom_title_line(const int width);
static void draw_bottom_header_line(const int width);
static void draw_top_line(const int width);
static void draw_bottom_line(const int width);

/**
 * Function to draw top line for title with symbols for the given width
 */
void draw_top_title_line(const int width) {
	int actual_width = width - CORNER_CHAR_WIDTH;

	printf("╔");
	if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("═");
	printf("╗\n");
}

/**
 * Function to draw bottom line for title with symbols for the given width
 */
void draw_bottom_title_line(const int width) {
	int actual_width = width - CORNER_CHAR_WIDTH;

	printf("╚");
	if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("═");
	printf("╝\n");
}

/**
 * Function to draw bottom line for header with symbols for the given width
 */
void draw_bottom_header_line(const int width) {
	int actual_width = width - CORNER_CHAR_WIDTH;

	printf("├");
	if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
	printf("┤\n");
}

/**
 * Function to draw top line with symbols for the given width
 */
void draw_top_line(const int width) {
	int actual_width = width - CORNER_CHAR_WIDTH;

	printf("┌");
	if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
	printf("┐\n");
}

/**
 * Function to draw bottom line with symbols for the given width
 */
void draw_bottom_line(const int width) {
	int actual_width = width - CORNER_CHAR_WIDTH;

	printf("└");
	if (actual_width > 0) for (int i = 0; i < actual_width; i++) printf("─");
	printf("┘\n");
}

/**
 * Function to draw a box with center aligned given title for the given width of the box
 */
void draw_title(const char *title, const int width) {
	int total_empty_space = width - (int)strlen(title) - CORNER_CHAR_WIDTH;
	int left_padding = total_empty_space / 2;
	int right_padding = total_empty_space - left_padding;

	draw_top_title_line(width);
	printf("║");
	for (int i = 0; i < left_padding; i++) printf(" ");
	printf("%s", title);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf("║\n");
	draw_bottom_title_line(width);
	printf("\n");
}

/**
 * Function to draw a box with left aligned given header for the given width of the box
 */
void draw_header(const char *header, const int width) {
	int right_padding =
		width
		- (int)strlen(header)
		- CORNER_CHAR_WIDTH
		- L_PADDING_CHAR_WIDTH;

	draw_top_line(width);
	printf("│ ");
	printf("%s", header);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf("│\n");
	draw_bottom_header_line(width);
}

/**
 * Function to draw a box with left aligned given error for the given width of the box
 */
void draw_error(const char *message, int width) {
	int message_len = (int)strlen(message);

	if (width < (message_len + CORNER_CHAR_WIDTH + PADDING_CHAR_WIDTH))
		width = message_len + CORNER_CHAR_WIDTH + PADDING_CHAR_WIDTH; // increase width of the box if given width is smaller

	int right_padding = width - message_len - CORNER_CHAR_WIDTH - PADDING_CHAR_WIDTH;

	draw_top_line(width);
	printf("│ ");
	printf("%s", message);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf(" │\n");
	draw_box_bottom(width);
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and string value for the given width of the box
 */
void draw_open_box_str(const char *label, const char *value, const int width) {
	int right_padding =
		width
		- (int)strlen(label)
		- (int)strlen(value)
		- CORNER_CHAR_WIDTH
		- L_PADDING_CHAR_WIDTH;

	printf("│ ");
	printf("%s%s", label, value);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf("│\n");
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and unsigned long int value for the given width of the box
 */
void draw_open_box_ulong(const char *label, const unsigned long int value, const int width) {
	char value_str[32];
	snprintf(value_str, sizeof(value_str), "%lu", value);

	draw_open_box_str(label, value_str, width);
	/*int right_padding =
		width
		- (int)strlen(label)
		- (int)strlen(value_str)
		- CORNER_CHAR_WIDTH
		- L_PADDING_CHAR_WIDTH;

	printf("│ ");
	printf("%s%lu", label, value);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf("│\n");*/
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and size_t value for the given width of the box
 */
void draw_open_box_size_t(const char *label, const size_t value, const int width) {
	char value_str[32];
	snprintf(value_str, sizeof(value_str), "%zu", value);

	draw_open_box_str(label, value_str, width);
	/*int right_padding =
		width
		- (int)strlen(label)
		- (int)strlen(value_str)
		- CORNER_CHAR_WIDTH
		- L_PADDING_CHAR_WIDTH;

	printf("│ ");
	printf("%s%zu", label, value);
	if (right_padding > 0) for (int i = 0; i < right_padding; i++) printf(" ");
	printf("│\n");*/
}

/**
 * Function to draw an open box with no up or bottom lines to show given label and boolean value for the given width of the box
 */
void draw_open_box_bool(const char *label, const bool value, const int width) {
	draw_open_box_str(label, value ? "YES" : "NO", width);
}

/**
 * Function to draw an a line to close the box
 */
void draw_box_bottom(const int width) {
	draw_bottom_line(width);
	printf("\n");
}
