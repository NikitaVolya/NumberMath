/**
 * @file custom_output.h
 * @brief Utility functions for console output and cursor management.
 *
 * This module provides helper functions for:
 * - Repeating text printing
 * - Controlling terminal input mode
 * - Getting cursor position
 * - Printing text at a specific console position with borders
 */


#ifndef _CUSTOM_OUTPUT_H
#define _CUSTOM_OUTPUT_H

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include "../../game_objects/vector2i.h"

#define BORDER_SYMBL "#"
#define BORDER_SIZE 2

/**
 * @brief Moves the console cursor to a specific position.
 *
 * @param pos A vector2i structure specifying the target position:
 *            - pos.x : column number (horizontal position, starting from 1)
 *            - pos.y : row number (vertical position, starting from 1)
 *
 * @details
 * This macro uses ANSI escape sequences to move the cursor
 * to the given (row, column) position in the terminal.
 * It does not perform bounds checking, so the caller should
 * ensure that pos.x and pos.y are within the terminal size.
 *
 * @note Coordinates are 1-based (top-left is 1,1).
 */
#define gotopos(pos) printf("\033[%d;%dH", (pos.y), (pos.x))

/**
 * @brief Prints a string multiple times consecutively.
 * @param text String to print.
 * @param n Number of repetitions.
 */
void repeat_print(const char *text, int n);

/**
 * @brief Sets the terminal to raw input mode (non-canonical, no echo).
 * @param old_tio Pointer to a termios structure to save current terminal settings.
 */
void set_input_mode(struct termios *old_tio);

/**
 * @brief Reads the current cursor position from the terminal.
 * @param row Pointer to an integer where the row will be stored.
 * @param col Pointer to an integer where the column will be stored.
 *
 * @details The function queries the terminal with ANSI escape sequences
 *          and parses the response to get the current cursor location.
 */
void get_cursor_position(int *row, int *col);

/**
 * @brief Prints a text block at a specified position with a surrounding border.
 * @param text Text to print.
 * @param pos Top-left position (vector2i) in the console where the text starts.
 *
 * @details The function:
 * - Saves the current cursor position.
 * - Calculates the maximum line width.
 * - Prints a border around the text.
 * - Prints the text inside the border.
 * - Restores the cursor to its original position.
 */
void print_over(const char *text, vector2i pos);

#endif /* _CUSTOM_OUTPUT_H */