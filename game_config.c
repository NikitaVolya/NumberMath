#include"game_config.h"

struct game_config* create_game_config() {
    struct game_config *res;

    res = (struct game_config*)malloc(sizeof(struct game_config));

    res->field = NULL;
    res->output = NULL;
    res->cursor_p = create_vector2i(0, 0);
    res->selected_p = create_vector2i(-1, 0);

    return res;
}

void free_game_config(struct game_config *config) {

    if (config->field != NULL)
        free(config->field);

    if (config->output != NULL)
        free(config->field);

    free(config);
}