#ifndef _MLV_MENU_SCREEN_H
#define _MLV_MENU_SCREEN_H

#include <MLV/MLV_all.h>
#include "../../game_objects/vector2i.h"
#include "../../game_config.h"
#include "mlv_game_setup.h"

typedef enum {
    MENU_PLAY = 1,
    MENU_SCOREBOARD = 2,
    MENU_TUTORIAL = 3,
    MENU_QUIT = 4
} MENU_RESULT;

void mlv_show_menu(struct game_config *config);

#endif