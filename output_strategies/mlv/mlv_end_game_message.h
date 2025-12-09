
#include<MLV/MLV_all.h>
#include"mlv_button.h"
#include"../output_config.h"
#include"../../game_objects/outils.h"

#define END_GAME_MESSAGE_WIDTH (GAME_WINDOW_WIDTCH * 2 / 3)
#define END_GAME_MESSAGE_HEIGHT 120
#define END_GAME_MESSAGE_POS_X ((GAME_WINDOW_WIDTCH - GAME_MESSAGE_WIDTH) / 2)
#define END_GAME_MESSAGE_POS_Y (GAME_WINDOW_HEIGHT / 2 - GAME_MESSAGE_HEIGHT / 2)
#define END_GAME_MESSAGE_CLOSE_BTN_SIZE 22
#define END_GAME_MESSAGE_CLOSE_BTN_POS_X (GAME_MESSAGE_POS_X + GAME_MESSAGE_WIDTH - GAME_MESSAGE_CLOSE_BTN_SIZE)
#define END_GAME_MESSAGE_CLOSE_BTN_POS_Y (GAME_MESSAGE_POS_Y)

/**
 * @brief Creates a standard close ("X") button for message dialogs.
 *
 * Constructs an MLV_Button positioned in the upper-right corner
 * of the message box, using predefined size and colors.
 *
 * @return MLV_Button  
 *         A fully initialized close button with text "X",
 *         fixed position, size, and color style.
 */
MLV_Button MLV_create_close_button();

/**
 * @brief Displays a modal “OK” message box and waits for user interaction.
 *
 * This function creates and renders a blocking message dialog with the
 * specified text and two buttons: an "OK" button and a close ("X") button.
 * It continuously handles mouse input, updates button states, and redraws
 * the interface each frame until the user clicks one of the buttons.
 *
 * @param text  
 *        Null-terminated string displayed inside the message box.
 *        The text is centered both horizontally and vertically.
 *
 * @note  
 * This function blocks execution until the user presses either the "OK"
 * button or the close button.  
 * It updates the window every frame using MLV_delay_according_to_frame_rate()
 * and MLV_actualise_window().
 *
 * @see MLV_create_base_button
 * @see MLV_create_close_button
 * @see MLV_draw_button
 * @see MLV_mouse_is_on_button
 */
void mlv_end_game_message(struct game_config *config);

/**
 * @brief Displays a modal “Yes/No” message box and waits for user selection.
 *
 * This function creates and renders a blocking message dialog with the
 * specified text and three buttons: "Yes", "No", and a close ("X") button.
 * It continuously handles mouse input, updates button states, and redraws
 * the interface each frame until the user clicks one of the buttons.
 *
 * @param text  
 *        Null-terminated string displayed inside the message box.
 *        The text is centered both horizontally and vertically.
 *
 * @return GAME_MESSAGE_RESULT  
 *         Returns one of the following based on user interaction:
 *         - GAME_MESSAGE_RESULT_YES   : User clicked "Yes".
 *         - GAME_MESSAGE_RESULT_NO    : User clicked "No".
 *         - GAME_MESSAGE_RESULT_NONE  : User clicked the close ("X") button.
 *
 * @note  
 * This function blocks execution until the user makes a selection.  
 * It updates the window every frame using MLV_delay_according_to_frame_rate()
 * and MLV_actualise_window().
 *
 * @see MLV_create_base_button
 * @see MLV_create_close_button
 * @see MLV_draw_button
 * @see MLV_mouse_is_on_button
 */
GAME_MESSAGE_RESULT MLV_show_yesno_game_message(const char *text);

/**
 * @brief Displays a game message box of the specified type.
 *
 * Depending on the provided type, this function shows either an "Ok" message
 * or a "Yes/No" message dialog. It delegates rendering and input handling
 * to the appropriate specialized function.
 *
 * @param text  
 *        Null-terminated string to display inside the message box.
 *
 * @param type  
 *        The type of message to display. Can be:
 *        - GAME_MESSAGE_TYPE_OK    : Shows a simple "Ok" dialog.
 *        - GAME_MESSAGE_TYPE_YESNO : Shows a "Yes/No" dialog.
 *
 * @return GAME_MESSAGE_RESULT  
 *         The result of the user's interaction:
 *         - GAME_MESSAGE_RESULT_OK    : "Ok" clicked or "Yes" clicked.
 *         - GAME_MESSAGE_RESULT_YES   : "Yes" clicked (only for Yes/No type).
 *         - GAME_MESSAGE_RESULT_NO    : "No" clicked  (only for Yes/No type).
 *         - GAME_MESSAGE_RESULT_NONE  : Close button clicked.
 *         - GAME_MESSAGE_RESULT_NULL  : Invalid message type.
 *
 * @note  
 * This function is a wrapper that calls the specialized message box
 * functions for the chosen type. Execution blocks until the user
 * responds to the dialog.
 *
 * @see MLV_show_ok_game_message
 * @see MLV_show_yesno_game_message
 */
GAME_MESSAGE_RESULT MLV_show_game_message(const char *text, GAME_MESSAGE_TYPE type);