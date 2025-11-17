
#ifndef _OUTPUT_CONFIG_H
#define _OUTPUT_CONFIG_H

#include"../game_config.h"

struct game_config;
struct output_config;

#include"console/console_game_strategy.h"
#include"mlv/mlv_game_strategy.h"


/*  
    Holds function pointers for different output actions in the game.

    fields:
        display_game       - function to render the current game state
        update_game        - function to handle user input and update game state
        end_game_message   - function to display the end-of-game message
        show_game_menu     - function to display the main menu
        show_game_message  - function to display a temporary message (e.g., hints, errors)
*/
struct output_config {
    void (*display_game)(struct game_config *config);
    void (*update_game)(struct game_config *config);
    void (*end_game_message)(struct game_config *config);
    void (*show_game_menu)(struct game_config *config);
    void (*show_game_message)(const char *text);
};

/*  
    Sets up the console-based output functions for the game.

    input:
        config - pointer to a game_config structure
*/
void set_console_output(struct game_config *config);

/*  
    Sets up the mlv-based output functions for the game.

    input:
        config - pointer to a game_config structure
*/
void set_mlv_output(struct game_config *config);

#endif /* _OUTPUT_CONFIG_H */