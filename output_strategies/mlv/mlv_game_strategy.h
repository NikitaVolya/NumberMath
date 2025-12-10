/**
 * @file mlv_game_strategy.h
 * @brief Central include header for all MLV-based UI modules of the game.
 *
 * - Game window and setup utilities (`mlv_game_setup.h`)
 * - Custom animated text rendering (`mlv_custom_text_animation.h`)
 * - Main game screen drawing and interactions (`mlv_game_screen.h`)
 * - Menu rendering and navigation (`mlv_menu_screen.h`)
 * - Standard modal message dialogs (`mlv_game_message.h`)
 * - End-game message and summary dialogs (`mlv_end_game_message.h`)
 *
 * @note  
 * This header should be included only in modules that require access to the
 * full UI system. For lightweight components, consider including only the
 * specific modules needed to avoid unnecessary dependencies.
 */

#include"mlv_game_setup.h"
#include"mlv_custom_text_animation.h"
#include"mlv_game_screen.h"
#include"mlv_menu_screen.h"
#include"mlv_game_message.h"
#include"mlv_end_game_message.h"
