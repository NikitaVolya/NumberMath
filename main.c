#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

#include"game_config.h"

int select_output_function(void (**init_game_output_func)(struct game_config*), char *key) {
    int res;

    res = 1;
    if (strcmp(key, "mlv") == 0) {
        *init_game_output_func = set_mlv_output;
    } else if (strcmp(key, "console") == 0) {
        *init_game_output_func = set_console_output;
    } else {
        fprintf(stderr, "Argument error: \"%s\" output is not existe\n", key);
        res = 0;
    }

    return res;
}

int main(int argc, char **argv) {
    struct game_config *config;
    void (*init_game_output_func)(struct game_config*);
    
    srand(time(NULL));

    config = create_game_config();

    if (argc >= 2) {
        if (!select_output_function(&init_game_output_func, argv[1])) {
            exit(EXIT_FAILURE);
        }
    } else {
        init_game_output_func = set_mlv_output;
    }

    init_game_output_func(config);

    execute_game(config);

    free_game_config(config);
    exit(EXIT_SUCCESS);
}