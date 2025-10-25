#include"game.h"

game_config* create_game_config() {
    game_config *res;

    res = (game_config*)malloc(sizeof(game_config));

    res->field = NULL;
    res->cursor_p = create_vector2i(0, 0);
    res->selected_p = create_vector2i(-1, 0);

    return res;
}

void free_game_config(game_config *config) {

    if (config->field != NULL)
        free(config->field);

    free(config);
}

short randshort(short start, short end) {
    return start + rand() % (end - start + 1);
}

void expand_game_field(game_field *field) {
    int i, x, y, old_count;
    short tmp;

    if (field->additions_available > 0) {

        old_count = field->count;

        for (i = 0; i < old_count; i++) {
            x = i % field->width;
            y = i / field->width;

            if (field->table[y][x].is_available) {
                tmp = field->table[y][x].value;
                add_values_game_field(field, &tmp, 1);
            }
        }

        field->additions_available--;
    } else {
        show_console_game_message("No addditions available");
    }
    serialize_game_field(field, "save.bin");
}

int check_game_is_over(game_field *field) {
    int res;
    vector2i start, end;

    if (check_game_field_is_clear(field) ||
        (!find_match(field, &start, &end) && field->additions_available <= 0)) {
        res = 1;
    } else {
        res = 0;
    }

    return res;
}

void show_game_hints(game_field *field) {
    vector2i pos1, pos2;

    if (field->hints_available <= 0) {
        show_console_game_message("No hints available ");
    } else if (find_match(field, &pos1, &pos2)) {
        set_highlight_game_field_cell(field, pos1, 1);
        set_highlight_game_field_cell(field, pos2, 1);
        field->hints_available--;
    } else {
        show_console_game_message("No match finded");
    }
    serialize_game_field(field, "save.bin");
}

void user_game_select(game_config *config) {
    game_field *field;
    vector2i *cursor_p, *selected_p;
    MATCH_TYPE match_res;

    field = config->field;
    cursor_p = &config->cursor_p;
    selected_p = &config->selected_p;
    
    if (selected_p->x != -1) {
        set_selection_game_field_cell(field, *selected_p, 0);

        match_res = check_match(field, *selected_p, *cursor_p);
        if (cursor_p->x == selected_p->x && cursor_p->y == selected_p->y) {
            selected_p->x = -1;
        } else if (match_res) {
            set_available_game_field_cell(field, *selected_p, 0);
            set_available_game_field_cell(field, *cursor_p, 0);

            if (check_game_row_is_clear(field, cursor_p->y)) {
                remove_game_field_row(field, cursor_p->y);
                field->score += CLEAR_LINE_MATCH;
                
                if (selected_p->y >= cursor_p->y)
                    selected_p->y--;
                
                cursor_p->y--;
            }
            
            if (check_game_row_is_clear(field, selected_p->y)) {
                remove_game_field_row(field, selected_p->y);
                field->score += CLEAR_LINE_MATCH;
                cursor_p->y--;
            }

            if (cursor_p->y < 0) cursor_p->y = 0;

            selected_p->x = -1;

            field->score += match_res;
            serialize_game_field(field, "save.bin");
        } else {
            *selected_p = *cursor_p;
            set_selection_game_field_cell(field, *selected_p, 1);         
        }
    } else {
        *selected_p = *cursor_p;
        set_selection_game_field_cell(field, *selected_p, 1);
    }
}

void game_cycle(game_config *config) {

    serialize_game_field(config->field, "save.bin");
    
    config->selected_p.x = -1;

    config->cursor_p = create_vector2i(0, 0);

    do {
        
        set_cursor_game_field_cell(config->field, config->cursor_p, 1);
        
        display_console_game_screen(config);

        set_cursor_game_field_cell(config->field, config->cursor_p, 0);
        
        user_console_game_input(config);

        if (check_game_field_is_clear(config->field)) {
            config->field->stage++;
            config->field->score += CLEAR_FIELD_MATCH;
            
            config->field->additions_available = config->field->additions_max;
            config->field->hints_available = config->field->hints_max;
            
            init_game_field(config->field);

            serialize_game_field(config->field, "save.bin");
        }

     } while (!check_game_is_over(config->field));

    end_console_game_message(config);
}

void init_game_field(game_field *field) {
    short values[INIT_CELLS_COUNT];
    int i;

    for(i = 0; i < INIT_CELLS_COUNT; i++) {
        values[i] = randshort(1, 9);
    }

    while (field->height > 0) remove_game_field_row(field, 0);
    
    add_values_game_field(field, values, INIT_CELLS_COUNT);
}

void load_game(game_config *config) {

    if (config->field != NULL) {
        free(config->field);
    }
    
    config->field = deserialize_game_field("save.bin");
    if (config->field == NULL) {
        show_console_game_message("Error while loading game!");
    } else {
        game_cycle(config);
        
        free(config->field);
        config->field = NULL;
    }
}

void start_game(game_config *config) {

    if (config->field != NULL) {
        free(config->field);
    }

    config->field = create_new_game_field(9);

    init_game_field(config->field);
    game_cycle(config);

    free(config->field);
    config->field = NULL;
}