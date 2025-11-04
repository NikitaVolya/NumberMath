
#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include"game_objects/game_field.h"
#include"game_objects/vector2i.h"
#include"output_strategies/output_config.h"

struct output_config;

/*  
    Holds the current state and configuration of a NumberMatch game.  

    field       - pointer to the current game_field structure  
    output      - pointer to the output strategy used for displaying and updating the game  
    cursor_p    - current position of the cursor in the game field  
    selected_p  - currently selected cell position (-1 indicates no selection)  
*/
struct game_config {
    game_field *field;
    struct output_config *output;
    vector2i cursor_p, selected_p;
};

/*  
    Allocates and initializes a new game_config structure.

    output:
        returns pointer to a newly created game_config with:
        - field set to NULL
        - output set to NULL
        - cursor_p initialized at (0, 0)
        - selected_p initialized at (-1, 0) (no selection)
*/
struct game_config* create_game_config();

/*  
    Frees all resources associated with a game_config structure.

    input:
        config - pointer to the game_config to free

    behavior:
        - Frees the game field if it exists
        - Frees the output configuration if it exists
        - Frees the game_config structure itself
*/
void free_game_config(struct game_config *config);

#endif /* _GAME_CONFIG_H */