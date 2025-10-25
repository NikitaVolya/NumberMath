#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"game_config.h"


int main() {
    struct game_config *config;
    
    srand(time(NULL));

    config = create_game_config();

    set_console_output(config);

    execute_game(config);

    free_game_config(config);
    exit(EXIT_SUCCESS);
}