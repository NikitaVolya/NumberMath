
#include <MLV/MLV_all.h>
#include"../output_config.h"
#include"../../game_objects/vector2i.h"
#include"../../game_objects/outils.h"
#include"mlv_custom_text_animation.h"

/*  
    Displays the current game score on the screen using a styled text box.

    input:
        score - integer score value to be converted and drawn  

    output:
        Draws the score visually on the game window using MLV  
*/
void display_game_score(int score);

/*  
    Determines the visual style (font and background colors) of a game field cell  
    based on its state flags.

    input:
        cell            – pointer to the field_cell structure describing cell state  
        font_color      – output pointer where the chosen font color will be stored  
        background_color– output pointer where the chosen background color will be stored  
    output:
        Updates font_color and background_color with the final style values  
*/
void select_cell_style(field_cell *cell, MLV_Color *font_color, MLV_Color *background_color);

/*  
    Renders the entire game grid onto the MLV window.

    input:
        field – pointer to the game_field structure containing all cells  
        shift – vertical offset applied to the grid for scrolling animations  

    output:
        Renders the full grid and its decorations to the screen  
*/
void display_game_grid(game_field *field, int shift);

/*  
    Draws a circular button with a highlight and a text label.

    input:
        x, y        – center position of the button  
        radius      – button radius  
        fill_color  – main fill color of the circle  
        text        – label drawn above the highlight  

    output:
        Renders a stylized circular button to the screen  
*/
void draw_circle_button(int x, int y, int radius, MLV_Color fill_color, char *text);

/*  
    Displays the “expand field” action button and draws its current state.

    input:
        config  – pointer to game configuration containing the field  
        pos     – current mouse position (vector2i)

    output:
        Renders the expand button on the screen with appropriate color  
        and current remaining expansions indicator  
*/
void display_expand_button(struct game_config *config, vector2i pos);

/*  
    Displays the “help / hint” button and updates its visual state.

    input:
        config  – pointer to the game_config structure containing current field data  
        pos     – current mouse cursor position

    output:
        Renders the help button with appropriate color and indicator  
*/
void display_help_button(struct game_config *config, vector2i pos);

/*  
    Displays the current game stage in the top-left corner of the window.

    input:
        config – pointer to the game_config structure containing the current field state

    output:
        Renders the stage indicator (e.g., "stage: 12")
*/
void display_stage(struct game_config *config);

/*  
    Renders the entire game screen using the MLV graphics library.

    input:
        config – pointer to the game_config structure containing all game state data

    output:
        Fully updated game screen for the current frame
*/
void display_mlv_game_screen(struct game_config *config);

/*  
    Creates an animated score message that floats upward on the screen.

    input:
        x, y    - starting coordinates of the message (vector2i)
        score   - integer score value to display

    output:
        No return value. The animation is added to the MLV text animation system.
*/
void show_score_message(int x, int y, int score);

/*
    Handles user input for the MLV game interface, including mouse clicks, motion, and dragging.

    input:
        config - pointer to the game_config structure containing the current field, cursor, shift, and other settings

    behavior:
        - Updates the global mouse position (mouse_p) on motion events.
        - Computes the cursor position on the game grid and sets config->cursor_p if inside bounds.
        - Detects single clicks on:
            * Game grid -> triggers user_game_select() and shows a score message if a match occurs.
            * Expand button -> calls expand_game_field().
            * Help button -> calls show_game_hints().
        - Supports mouse dragging to scroll the game field if it exceeds the visible grid area.
        - Resets field shift to zero if the grid is smaller than the visible area.
        - Uses static variables to track previous mouse button state and last mouse Y position for detecting single clicks and drag.

    output:
        No return value. Updates config and triggers visual feedback through MLV text animations and field updates.
*/
void user_mlv_game_input(struct game_config* config);