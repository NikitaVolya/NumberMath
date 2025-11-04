
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

void set_mlv_output(struct game_config *config) {

    if (config->output == NULL) {
        config->output = (struct output_config*)malloc(sizeof(struct output_config));
    }

    config->output->display_game = display_mlv_game_screen;
    config->output->update_game = user_console_game_input;
    config->output->end_game_message = end_console_game_message;
    config->output->show_game_menu = show_console_game_menu;;
    config->output->show_game_message = show_console_game_message;
}