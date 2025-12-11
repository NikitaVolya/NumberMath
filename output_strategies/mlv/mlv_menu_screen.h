#ifndef MLV_MENU_SCREEN_H
#define MLV_MENU_SCREEN_H

/************************************************************
 *                     MENU SCREEN MODULE
 * ----------------------------------------------------------
 * This module handles:
 *   • The intro animation sequence
 *   • Four dynamic background modes
 *   • The main interactive menu (Play / Load / Tutorial / Quit)
 *   • A background mode toggle button ("BG")
 *   • A multi-page animated tutorial viewer
 *
 * Only the high-level menu entry point is public.
 * All helpers inside mlv_menu_screen.c remain static.
 ************************************************************/

#include <MLV/MLV_all.h>
#include "../../game_objects/vector2i.h"
#include "../../game.h"
#include "mlv_game_setup.h"
#include "../../game_config.h"   /* required for struct game_config */


/************************************************************
 *               PUBLIC ENUM: MENU RESULT
 *
 * Note:
 *   Your current mlv_menu_screen.c does NOT return a MENU_RESULT.
 *   The menu directly calls:
 *
 *       start_game(config);
 *       load_game(config);
 *       show_tutorial_screen();
 *
 *   …and only exits the loop for Quit.
 *
 * The enum is kept here exactly as you provided it
 * in case other modules depend on it.
 ************************************************************/

typedef enum {
    MENU_PLAY       = 1,
    MENU_SCOREBOARD = 2,
    MENU_TUTORIAL   = 3,
    MENU_QUIT       = 4
} MENU_RESULT;


/************************************************************
 *                 PUBLIC FUNCTION: MENU
 ************************************************************/

/**
 * @brief Displays the NumberMatch animated main menu.
 *
 * This function:
 *   - Runs the intro animation.
 *   - Draws the main menu buttons.
 *   - Handles mouse and keyboard input.
 *   - Shows the tutorial screen.
 *   - Allows background mode switching.
 *
 * Internally triggers:
 *   start_game(config)
 *   load_game(config)
 *   show_tutorial_screen()
 *
 * @param config  Pointer to the game's configuration struct.
 */
void mlv_show_menu(struct game_config *config);


#endif /* MLV_MENU_SCREEN_H */
