
#include "output_config.h"


void set_console_output(struct game_config *config) {

    if (config->output == NULL) {
        config->output = (struct output_config*)malloc(sizeof(struct output_config));
    }

    config->output->display_game = display_console_game_screen;
    config->output->update_game = user_console_game_input;
    config->output->end_game_message = end_console_game_message;
    config->output->show_game_menu = show_console_game_menu;
    config->output->show_game_message = show_console_game_message;
}

/* tmp game menu function for creating game screen */
void show_mlv_game_menu(struct game_config *config) {

    MLV_create_window("NumberMatch | Game",
                      "NumberMatch",
                      GAME_WINDOW_WIDTCH,
                      GAME_WINDOW_HEIGHT);

    MLV_ctext_animations_start();
    
    MLV_change_frame_rate(FRAME_RATE);

    show_console_game_menu(config);

    MLV_ctext_animations_end();

    MLV_free_window();
}

void set_mlv_output(struct game_config *config) {

    if (config->output == NULL) {
        config->output = (struct output_config*)malloc(sizeof(struct output_config));
    }

    config->output->display_game = display_mlv_game_screen;
    config->output->update_game = user_mlv_game_input;
    config->output->end_game_message = end_console_game_message;
    config->output->show_game_menu = show_mlv_game_menu;
    config->output->show_game_message = show_console_game_message;
}