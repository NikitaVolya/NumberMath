#include"game.h"


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

void user_game_select(vector2i *cursor,
                      vector2i *selected_pos,
                      game_field *field) {
    MATCH_TYPE match_res;
    
    if (selected_pos->x != -1) {
        set_selection_game_field_cell(field, *selected_pos, 0);

        match_res = check_match(field, *selected_pos, *cursor);
        if (cursor->x == selected_pos->x && cursor->y == selected_pos->y) {
            selected_pos->x = -1;
        } else if (match_res) {
            set_available_game_field_cell(field, *selected_pos, 0);
            set_available_game_field_cell(field, *cursor, 0);

            if (check_game_row_is_clear(field, cursor->y)) {
                remove_game_field_row(field, cursor->y);
                field->score += CLEAR_LINE_MATCH;
                
                if (selected_pos->y >= cursor->y)
                    selected_pos->y--;
                
                cursor->y--;
            }
            
            if (check_game_row_is_clear(field, selected_pos->y)) {
                remove_game_field_row(field, selected_pos->y);
                field->score += CLEAR_LINE_MATCH;
                cursor->y--;
            }

            if (cursor->y < 0) cursor->y = 0;

            selected_pos->x = -1;

            field->score += match_res;
            serialize_game_field(field, "save.bin");
        } else {
            *selected_pos = *cursor;
            set_selection_game_field_cell(field, *selected_pos, 1);         
        }
    } else {
        *selected_pos = *cursor;
        set_selection_game_field_cell(field, *selected_pos, 1);
    }
}

void game_cycle(game_field* field) {
    vector2i cursor, selected_pos;

    serialize_game_field(field, "save.bin");
    
    selected_pos.x = -1;

    cursor = create_vector2i(0, 0);

    do {
        
        set_cursor_game_field_cell(field, cursor, 1);
        
        display_console_game_screen(field);

        set_cursor_game_field_cell(field, cursor, 0);
        
        user_console_game_input(&cursor, &selected_pos, field);

        if (check_game_field_is_clear(field)) {
            field->stage++;
            field->score += CLEAR_FIELD_MATCH;
            
            field->additions_available = field->additions_max;
            field->hints_available = field->hints_max;
            
            init_game_field(field);

            serialize_game_field(field, "save.bin");
        }

     } while (!check_game_is_over(field));

    end_console_game_message(field);
    
    free(field);
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

void load_game() {
    game_field *field;

    field = deserialize_game_field("save.bin");
    if (field == NULL) {
        show_console_game_message("Error while loading game!");
    } else {
        game_cycle(field);
    }
}

void start_game() {
    
    game_field *field;

    field = create_new_game_field(9);

    init_game_field(field);

    game_cycle(field);
}