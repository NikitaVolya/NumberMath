#include"game.h"

short randshort(short start, short end) {
    return start + rand() % (end - start + 1);
}

void expand_game_field(struct game_config *config) {
    int i, x, y, old_count;
    game_field *field;
    short tmp;

    field = config->field;

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
        config->output->show_game_message("No addditions available");
    }
    serialize_game_field(field, "save.bin");
}

void update_stage(game_field *field) {
    
    field->stage++;
    field->score += CLEAR_FIELD_MATCH;
            
    field->additions_available = field->additions_max;
    field->hints_available = field->hints_max;
            
    init_game_field(field);

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

void show_game_hints(struct game_config *config) {
    vector2i pos1, pos2;
    game_field *field;

    field = config->field;

    if (field->hints_available <= 0) {
        config->output->show_game_message("No hints available ");
    } else if (find_match(field, &pos1, &pos2)) {
        set_highlight_game_field_cell(field, pos1, 1);
        set_highlight_game_field_cell(field, pos2, 1);
        field->hints_available--;
    } else {
        config->output->show_game_message("No match finded");
    }
    serialize_game_field(field, "save.bin");
}

void user_game_select(struct game_config *config) {
    game_field *field;
    vector2i *cursor_p, *selected_p;
    MATCH_TYPE match_res;

    field = config->field;
    cursor_p = &config->cursor_p;
    selected_p = &config->selected_p;
    
    /* - If a cell is already selected */
    if (selected_p->x != -1) {
        /* - Deselects it temporarily. */ 
        set_selection_game_field_cell(field, *selected_p, 0);

        /* - Checks if the current cursor position matches the previously selected cell. */
        match_res = check_match(field, *selected_p, *cursor_p);
        
        /* - If the same cell is clicked again, cancels the selection. */
        if (cursor_p->x == selected_p->x && cursor_p->y == selected_p->y) {
            selected_p->x = -1;
        /* - If a valid match is found, removes both cells, updates the score,  
           clears completed rows, and repositions the cursor.  */
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
        /* - If is a not valide match select current cursor position */
        } else {
            *selected_p = *cursor_p; 
            set_selection_game_field_cell(field, *selected_p, 1);         
        }
    /* - If no cell is currently selected */
    } else {
        *selected_p = *cursor_p;
        set_selection_game_field_cell(field, *selected_p, 1);
    }
}

void game_cycle(struct game_config *config) {

    serialize_game_field(config->field, "save.bin");
    
    config->selected_p.x = -1;
    config->cursor_p = create_vector2i(0, 0);

    do {
        set_cursor_game_field_cell(config->field, config->cursor_p, 1);

        config->output->display_game(config);

        set_cursor_game_field_cell(config->field, config->cursor_p, 0);

        config->output->update_game(config);

        if (check_game_field_is_clear(config->field)) {
            update_stage(config->field);
        }

     } while (!check_game_is_over(config->field));

    config->output->end_game_message(config);
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

void load_game(struct game_config *config) {

    /* free any previously allocated field to avoid memory leaks */
    if (config->field != NULL) {
        free(config->field);
    }

    /* attempt to load saved game data */
    if ((config->field = deserialize_game_field("save.bin")) == NULL) {
        config->output->show_game_message("Error while loading game!");
    } else {
        game_cycle(config);
        
        free(config->field);
        config->field = NULL;
    }
}

void start_game(struct game_config *config) {
    
    /* free any previously allocated field to avoid memory leaks */
    if (config->field != NULL) {
        free(config->field);
    }
    
    /* create a new game field with width 9 */
    config->field = create_new_game_field(9);

    init_game_field(config->field);
    game_cycle(config);

    free(config->field);
    config->field = NULL;
}

void execute_game(struct game_config *config) {

    if (config == NULL) {
        printf("Error config is NULL");
    } else if (config->output == NULL) {
        printf("Error! Output strategy not selected\n");
    } else {
        config->output->show_game_menu(config);
    }

}