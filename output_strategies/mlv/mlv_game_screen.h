/**
 * @file mlv_game_screen.h
 * @brief Rendering and input handling for NumberMatch using the MLV library.
 *
 * This module provides functions to display the game state, buttons, score,
 * stage, and handle user input via mouse interactions in the MLV window.
 * It integrates with the custom text animation system for visual feedback.
 *
 */


#include <MLV/MLV_all.h>
#include"../output_config.h"
#include"../../game_objects/vector2i.h"
#include"../../game_objects/outils.h"
#include"mlv_button.h"
#include"mlv_custom_text_animation.h"

/**
 * @brief Displays the current score in a styled text box.
 * @param[in] score Current integer score to display.
 */
void display_game_score(int score);

/**
 * @brief Determines visual style for a cell based on its state.
 * @param[in] cell Pointer to the field_cell structure.
 * @param[out] font_color Output pointer to store the chosen font color.
 * @param[out] background_color Output pointer to store the chosen background color.
 */
void select_cell_style(field_cell *cell, MLV_Color *font_color, MLV_Color *background_color);


/**
 * @brief Renders the full game grid with its current state.
 * @param[in] field Pointer to the game_field structure.
 * @param[in] shift Vertical offset for scrolling animations.
 */
void display_game_grid(game_field *field, int shift);

/**
 * @brief Draws a circular button with highlight and text label.
 * @param[in] x X-coordinate of the button center.
 * @param[in] y Y-coordinate of the button center.
 * @param[in] radius Radius of the circular button.
 * @param[in] fill_color Main fill color of the button.
 * @param[in] text Label to display on the button.
 */
void draw_circle_button(int x, int y, int radius, MLV_Color fill_color, char *text);

/**
 * @brief Displays the "expand field" button and updates its visual state.
 * @param[in] config Pointer to game configuration structure.
 * @param[in] pos Current mouse position.
 */
void display_expand_button(struct game_config *config, vector2i pos);

/**
 * @brief Displays the "help/hint" button and updates its visual state.
 * @param[in] config Pointer to game configuration structure.
 * @param[in] pos Current mouse position.
 */
void display_help_button(struct game_config *config, vector2i pos);

/**
 * @brief Displays the current game stage in the top-left corner.
 * @param[in] config Pointer to game configuration structure.
 */
void display_stage(struct game_config *config);

/**
 * @brief Renders the entire game screen using MLV.
 * @param[in] config Pointer to game configuration structure.
 */
void display_mlv_game_screen(struct game_config *config);

/**
 * @brief Creates an animated score message that floats upward.
 * @param[in] x Starting X-coordinate.
 * @param[in] y Starting Y-coordinate.
 * @param[in] score Score value to display.
 */
void show_score_message(int x, int y, int score);

/**
 * @brief Handles user input for the MLV game interface.
 *
 * @param[in, out] config Pointer to the game_config structure.
 *
 * This function updates the mouse position, cursor location, detects clicks
 * on the game grid, expand button, and help button, and handles dragging
 * for scrolling the field. It also triggers score animations and field updates.
 */
void user_mlv_game_input(struct game_config* config);