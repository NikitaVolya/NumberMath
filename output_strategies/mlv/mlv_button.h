/**
 * @file mlv_button.h
 * @brief Provides functions to create, draw, and manage buttons using MLV.
 *
 * This header defines the MLV_Button structure and functions to:
 * - Create buttons with or without custom fonts.
 * - Detect mouse interaction with buttons.
 * - Draw buttons on the screen with proper colors and highlights.
 * - Free button-related resources.
 *
 * @note Default font path for buttons is "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf".
 */


#ifndef _MLV_BUTTON_H_
#define _MLV_BUTTON_H_

#define MAX_BUTTON_TEXT_SIZE 255

#include<MLV/MLV_all.h>
#include<string.h>
#include"../../game_objects/vector2i.h"

/**
 * @struct MLV_Button
 * @brief Represents a clickable button in the UI.
 *
 * @var MLV_Button::text
 * Text displayed on the button.
 *
 * @var MLV_Button::pos
 * Top-left position of the button (vector2i).
 *
 * @var MLV_Button::size
 * Width and height of the button (vector2i).
 *
 * @var MLV_Button::font
 * Pointer to the font used for the button text.
 *
 * @var MLV_Button::fill_color
 * Button background color.
 *
 * @var MLV_Button::text_color
 * Color of the button text.
 *
 * @var MLV_Button::highlight_color
 * Background color when the button is hovered by the mouse.
 */
typedef struct {
    char text[MAX_BUTTON_TEXT_SIZE];
    MLV_Font* font;
    vector2i pos, size;
    MLV_Color fill_color, text_color, highlight_color;
} MLV_Button;

/**
 * @brief Creates a button with a specific font.
 *
 * @param text Text to display on the button.
 * @param font Path to the font file.
 * @param font_size Size of the font.
 * @param pos Top-left position of the button.
 * @param size Width and height of the button.
 * @param fill_color Background color of the button.
 * @param text_color Text color.
 * @param highlight_color Background color when hovered.
 * @return MLV_Button The initialized button structure.
 */
MLV_Button MLV_create_button_with_font(const char *text, const char *font, int font_size,
                                       vector2i position, vector2i size,
                                       MLV_Color fill_color, MLV_Color text_color, MLV_Color highlight_color);

/**
 * @brief Creates a button with the default font.
 *
 * @param text Text to display on the button.
 * @param pos Top-left position of the button.
 * @param size Width and height of the button.
 * @param fill_color Background color of the button.
 * @param text_color Text color.
 * @param highlight_color Background color when hovered.
 * @return MLV_Button The initialized button structure.
 */
MLV_Button MLV_create_button(const char *text,
                             vector2i pos, vector2i size,
                             MLV_Color fill_color, MLV_Color text_color,
                             MLV_Color highlight_color);

/**
 * @brief Creates a basic button with default colors and font.
 *
 * @param text Text to display on the button.
 * @param pos Top-left position of the button.
 * @param size Width and height of the button.
 * @return MLV_Button The initialized button structure with default styling.
 */
MLV_Button MLV_create_base_button(const char *text,
                                  vector2i pos, vector2i size);

/**
 * @brief Checks if the mouse pointer is currently over a button.
 *
 * @param button Pointer to the button structure.
 * @param mouse_p Pointer to the current mouse position (vector2i).
 * @return int 1 if the mouse is on the button, 0 otherwise.
 */
int MLV_mouse_is_on_button(MLV_Button *button, vector2i *mouse_p);

/**
 * @brief Draws a button on the screen, highlighting it if the mouse is over it.
 *
 * @param button Pointer to the button structure.
 * @param mouse_p Pointer to the current mouse position (vector2i).
 */
void MLV_draw_button(MLV_Button *button, vector2i *mouse_p);

/**
 * @brief Frees resources allocated for the button (e.g., the font).
 *
 * @param button Pointer to the button structure.
 */
void MLV_free_button(MLV_Button *button);

#endif /* _MLV_BUTTON_H */