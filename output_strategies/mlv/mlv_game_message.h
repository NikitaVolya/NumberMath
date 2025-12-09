/**
 * @file mlv_game_message.h
 * @brief Modal message dialogs for NumberMatch using MLV.
 *
 * This header provides structures, constants, and functions for creating
 * and displaying modal message boxes in the MLV game window. It supports
 * standard "OK" messages as well as "Yes/No" dialogs with a close button.
 *
 * The dialogs are centered in the game window and include preconfigured
 * buttons for user interaction. Blocking behavior ensures that user input
 * is captured before continuing game execution.
 *
 */

#ifndef _MLV_GAME_MESSAGE_H_
#define _MLV_GAME_MESSAGE_H_

#include<MLV/MLV_all.h>
#include<stdlib.h>
#include<stdio.h>
#include"mlv_game_setup.h"
#include"mlv_button.h"

#define GAME_MESSAGE_WIDTH (GAME_WINDOW_WIDTCH * 2 / 3)
#define GAME_MESSAGE_HEIGHT 120
#define GAME_MESSAGE_POS_X ((GAME_WINDOW_WIDTCH - GAME_MESSAGE_WIDTH) / 2)
#define GAME_MESSAGE_POS_Y (GAME_WINDOW_HEIGHT / 2 - GAME_MESSAGE_HEIGHT / 2)
#define GAME_MESSAGE_CLOSE_BTN_SIZE 22
#define GAME_MESSAGE_CLOSE_BTN_POS_X (GAME_MESSAGE_POS_X + GAME_MESSAGE_WIDTH - GAME_MESSAGE_CLOSE_BTN_SIZE)
#define GAME_MESSAGE_CLOSE_BTN_POS_Y (GAME_MESSAGE_POS_Y)

enum GAME_MESSAGE_TYPE {
    GAME_MESSAGE_TYPE_OK,
    GAME_MESSAGE_TYPE_YESNO
};
typedef enum GAME_MESSAGE_TYPE GAME_MESSAGE_TYPE;    

/**
 * @brief Result codes for game message dialogs.
 *
 * Used to indicate how the user closed or responded
 * to a modal message window.
 */
enum GAME_MESSAGE_RESULT {
    GAME_MESSAGE_RESULT_NULL = 0, /**< No result / uninitialized state. */
    GAME_MESSAGE_RESULT_NONE,     /**< Dialog closed without choosing an option. */
    GAME_MESSAGE_RESULT_OK,       /**< "OK" button was pressed. */
    GAME_MESSAGE_RESULT_YES,      /**< "Yes" option selected. */
    GAME_MESSAGE_RESULT_NO        /**< "No" option selected. */
};
typedef enum GAME_MESSAGE_RESULT GAME_MESSAGE_RESULT;

/**
 * @brief Create and return the standard "close" button used in game messages.
 *
 * Creates an MLV button containing the text "X", positioned at the
 * predefined close-button coordinates and with predefined size and colors.
 *
 * @return MLV_Button
 *         The initialized close button instance with position, size,
 *         text, and color styling applied.
 */
MLV_Button MLV_create_close_button();

/**
 * @brief Display a modal OK message box and wait for the user's response.
 *
 * This function renders a centered message box containing the given text,
 * an "Ok" button, and a close ("X") button.
 *
 * @param[in] text
 *      The message text displayed inside the dialog box. Must be a valid,
 *      null-terminated C-string.
 *
 * @note The function blocks execution until a result is chosen.
 */
void MLV_show_ok_game_message(const char *text);

/**
 * @brief Shows a Yes/No dialog box and waits for user input.
 *
 * Displays a modal message window with the given text, and "Yes", "No"
 * and "Close" buttons. Blocks until the user clicks one of them.
 *
 * @param[in] text Null-terminated message string to display.
 *
 * @return GAME_MESSAGE_RESULT
 *         - GAME_MESSAGE_RESULT_YES   — Yes  
 *         - GAME_MESSAGE_RESULT_NO   — No  
 *         - GAME_MESSAGE_RESULT_NONE — Closed
 */
GAME_MESSAGE_RESULT MLV_show_yesno_game_message(const char *text);

/**
 * @brief Displays a modal game message window of the given type.
 *
 * Shows a message box with the specified text. Depending on the
 * message type, an OK dialog or Yes/No dialog is displayed.  
 * Blocks until the user closes the dialog.
 *
 * @param[in] text Null-terminated message string to display.
 * @param[in] type Message box type (OK or YES/NO).
 *
 * @return GAME_MESSAGE_RESULT  
 *         - GAME_MESSAGE_RESULT_OK   — OK pressed / Yes selected  
 *         - GAME_MESSAGE_RESULT_NO   — No selected  
 *         - GAME_MESSAGE_RESULT_NONE — Closed without choice  
 *         - GAME_MESSAGE_RESULT_NULL — Unknown type
 */
GAME_MESSAGE_RESULT MLV_show_game_message(const char *text, GAME_MESSAGE_TYPE type);


#endif /* _MLV_GAME_MESSAGE_H_ */
