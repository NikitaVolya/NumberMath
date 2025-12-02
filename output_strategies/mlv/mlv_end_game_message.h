
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


void mlv_end_game_message(struct game_config *config);