#ifndef MLV_MENU_SCREEN_H
#define MLV_MENU_SCREEN_H

#include <MLV/MLV_all.h>
#include "../../game_objects/vector2i.h"
#include "../../game.h"
#include "mlv_game_setup.h"

typedef enum {
    MENU_PLAY = 1,
    MENU_SCOREBOARD = 2,
    MENU_TUTORIAL = 3,
    MENU_QUIT = 4
} MENU_RESULT;

/**
 * @brief Displays the tutorial screen with multiple pages.
 *
 * This function handles rendering and user interaction for the tutorial.
 * Users can navigate between pages using keyboard arrows (Left/Right),
 * mouse clicks on arrows, or exit with Enter/Escape keys.
 * Each page is displayed with text content and page number indicator.
 *
 * @note
 * - Uses global `tutorial_pages` array for content.
 * - Arrow positions are fixed near the bottom of the window.
 * - Assumes `GAME_WINDOW_WIDTH` and `GAME_WINDOW_HEIGHT` define the window size.
 */
void show_tutorial_screen();

/**
 * @brief Displays the main menu of the NumberMatch game.
 *
 * This function creates a menu window, handles rendering of menu options,
 * and processes user input via mouse interaction. Menu options include:
 * "New Game", "Load Game", "Tutorial", and "Quit".
 *
 * @param config Pointer to the current game configuration structure,
 *               used for starting or loading games.
 */
void mlv_show_menu(struct game_config *config);

#endif