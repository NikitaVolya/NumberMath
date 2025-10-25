#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"output_strategies/console/console_game_strategy.h"


int main() {
    game_config *config;
    
    srand(time(NULL));

    config = create_game_config();
    
    show_console_game_menu(config);

    free_game_config(config);
    exit(EXIT_SUCCESS);
}