#include<stdlib.h>
#include<stdio.h>

#include"game_field.h"


game_field* create_new_game_field(short width) {
    game_field *res;

    if (width > 0) {
        res = (game_field*)malloc(sizeof(game_field));

        res->score = 0;
        res->count = 0;
        res->stage = 1;
    
        res->width = width;
        res->height = 0;

        res->hints_max = 5;
        res->hints_available = res->hints_max;

        res->additions_max = 5;
        res->additions_available = res->additions_max;
    } else {
        res = NULL;
    }

    return res;
}

void add_values_game_field(game_field *field, short *values, int number) {
    int i, y, x;

    if (field != NULL) {
        for(i = 0; i < number; i++) {
            y = field->count / field->width;
            x = field->count % field->width;

            field->table[y][x] = create_field_cell(values[i]);
            field->count++;
        }
    
        field->height = field->count / field->width +
            (field->count % field->width > 0 ? 1 : 0);
    } else {
        printf("Warning! add_values_game_field argument field is NULL\n");
    }
}

int remove_game_field_row(game_field *field, int index) {
    int res, i, j, next_row_size;

    if (index < 0 || index >= field->height) {
        res = 0;
    } else if (index == field->height - 1) {
        res = 1;
        field->count -= get_game_field_row_size(field, index);
        field->height -= 1;
    } else {
        res = 1;

        for(i = index; i < field->height; i++) {
            next_row_size = get_game_field_row_size(field, i + 1);

            for(j = 0; j < next_row_size; j++) {
                field->table[i][j] = field->table[i + 1][j];
            }
        }

        field->height -= 1;
        field->count -= field->width;
    }
    
    return res;
}

int get_game_field_row_size(game_field *field, int index) {
    int res;

    if (index < 0 || field->height <= index) {
        res = 0;
    } else if (index == field->height - 1) {
        res = field->count - field->width * index;
    } else {
        res = field->width;
    }

    return res;
}

field_cell* get_game_field_cell(game_field *field, vector2i pos) {
    field_cell *res;
    int row_size;

    row_size = get_game_field_row_size(field, pos.y);

    if (pos.x < 0 || pos.x >= row_size ||
        pos.y < 0 || pos.y >= field->height)
        res = NULL;
    else
        res = &(field->table[pos.y][pos.x]);

    return res;
}

int get_available_game_field_cell(game_field *field, vector2i pos) {
    field_cell *cell;
    int res;

    cell = get_game_field_cell(field, pos);

    if (cell == NULL) {
        res = 0;
    }
    else {
        res = cell->is_available;
    }
    
    return res;
}

int set_highlight_game_field_cell(game_field *field, vector2i pos, int value) {
    field_cell *cell;
    int res;

    cell = get_game_field_cell(field, pos);

    if (cell == NULL) {
        res = 0;
    }
    else {
        cell->is_highlited = value;
        res = 1;
    }
    
    return res;
}

int set_selection_game_field_cell(game_field *field, vector2i pos, int value) {
    field_cell *cell;
    int res;

    cell = get_game_field_cell(field, pos);

    if (cell == NULL) {
        res = 0;
    }
    else {
        cell->is_selected = value;
        res = 1;
    }
    
    return res;
}

int set_available_game_field_cell(game_field *field, vector2i pos, int value) {
    field_cell *cell;
    int res;

    cell = get_game_field_cell(field, pos);

    if (cell == NULL) {
        res = 0;
    }
    else {
        cell->is_available = value;
        res = 1;
    }
    
    return res;
}

int set_cursor_game_field_cell(game_field *field, vector2i pos, int value) {
    field_cell *cell;
    int res;

    cell = get_game_field_cell(field, pos);

    if (cell == NULL) {
        res = 0;
    }
    else {
        cell->is_cursor = value;
        res = 1;
    }
    
    return res;
}

int find_match(game_field *field, vector2i *start_p, vector2i *end_p) {
    int res, i, j, row_size, d;
    vector2i cursor_p, tmp_p;
    field_cell *cursor_cell, *tmp_c;

    res = 0;
    for(i = 0; res == 0 && i < field->height; i++) {
        row_size = get_game_field_row_size(field, i);

        for(j = 0; res == 0 && j < row_size; j++) {

            cursor_p = create_vector2i(j, i);
            cursor_cell = get_game_field_cell(field, cursor_p);


            if (cursor_cell != NULL && cursor_cell->is_available) {

                /* check on column match */
                tmp_p = cursor_p;
                
                do {
                    tmp_p.y++;
                    
                    tmp_c = get_game_field_cell(field, tmp_p);


                    if (tmp_c != NULL && tmp_c->is_available) {
                        if (check_field_cell_math(cursor_cell, tmp_c)) {
                            res = 1;
                            *start_p = cursor_p;
                            *end_p = tmp_p;
                        } else {
                            tmp_c = NULL;
                        } 
                    }
                } while (res == 0 && tmp_c != NULL);

                /* check on diagonal left and right match */
                for (d = -1; d <= 1; d += 2) {

                    tmp_p = cursor_p;

                    do {
                        tmp_p.x += d;
                        tmp_p.y++;
                    
                        tmp_c = get_game_field_cell(field, tmp_p);

                        if (tmp_c != NULL && tmp_c->is_available) {
                            if (check_field_cell_math(cursor_cell, tmp_c)) {
                                res = 1;
                                *start_p = cursor_p;
                                *end_p = tmp_p;
                            } else {
                                tmp_c = NULL;
                            } 
                        }
                    } while (res == 0 && tmp_c != NULL);
                }

                /* check on horisontal and next line check */
                tmp_p = cursor_p;
                
                do {
                    tmp_p.x++;
                    
                    tmp_c = get_game_field_cell(field, tmp_p);

                    if (tmp_c == NULL) {
                        if (tmp_p.y < field->height - 1) {
                            tmp_p.x = -1;
                            tmp_p.y++;

                            /* for continue while cycle */
                            tmp_c = cursor_cell;
                        }
                    } else if (tmp_c != NULL && tmp_c->is_available) {
                        if (check_field_cell_math(cursor_cell, tmp_c)) {
                            res = 1;
                            *start_p = cursor_p;
                            *end_p = tmp_p;
                        } else {
                            tmp_c = NULL;
                        } 
                    }
                } while (res == 0 && tmp_c != NULL);
            }
        }
    }
    
    return res;
}

int check_match(game_field *field, vector2i start_p, vector2i end_p) {
    int next_line = 0, is_direct;
    vector2i current_p, direction, abs_delta, tmp;
    field_cell *start_cell, *end_cell, *current_cell;
    MATCH_TYPE res = NONE_MATCH;


    /* Swap cells if the end point appears before the start point */
    if (start_p.y > end_p.y) {
        tmp = start_p;
        start_p = end_p;
        end_p = tmp;
    }
    
    start_cell = get_game_field_cell(field, start_p);
    end_cell = get_game_field_cell(field, end_p);
    
    abs_delta = abs_vector2i(get_vector2i_to(start_p, end_p));

    /* Return NOT_MATCH if cells cannot be matched */
    if (!check_field_cell_math(start_cell, end_cell)) {
        res = NOT_MATCH;
    
    /* Check for direct horizontal, vertical, or diagonal alignment */
    } else if (abs_delta.x == 0 || abs_delta.y == 0 || abs_delta.x == abs_delta.y) {
        direction = get_direction_to(start_p, end_p);
    /* Check for “next line” match (wrapping to a new line) */
    } else if (start_p.y < end_p.y) {
        direction = create_vector2i(1, 0);
        next_line = 1;
    /* Default: not a valid match */
    } else {
        res = NOT_MATCH;
    }

    /* If still valid, begin match path iteration */
    current_p = start_p;
    is_direct = 1;
    while (res == NONE_MATCH) {
        
        /* Move to the next cell along the direction */
        current_p = add_vector2i(current_p, direction);
        current_cell = get_game_field_cell(field, current_p);

        /* Reached target cell — determine match type */
        if (current_cell == end_cell) {
            if (is_direct && next_line)
                res = NEXT_LINE_MATCH;
            else if (is_direct)
                res = DIRECTE_MATCH;
            else
                res = DISTANCE_MATCH;
        /* Out of field — possibly move to next line */
        } else if (current_cell == NULL) {
            if (next_line && current_p.y < field->height - 1) {
                current_p.y += 1;
                current_p.x = -1;
            } else {
                res = NOT_MATCH;
            }
        /* Found an occupied cell before reaching end — not valid */
        } else if (current_cell->is_available) {
            res = NOT_MATCH;
        /* Passed through empty cells — mark as indirect */
        } else {
            is_direct = 0;
        }
    }

    return res;
}

int check_game_row_is_clear(game_field *field, int index) {
    int res, i, row_size;

    if (index < 0 || field->height <= index) {
        res = 0;
    } else {
        res = 1;
        row_size = get_game_field_row_size(field, index);
        
        for (i = 0; res == 1 && i < row_size; i++) {
            if (get_available_game_field_cell(field, create_vector2i(i, index))) {
                res = 0;
            }
        }
    }

    return res;
}

int check_game_field_is_clear(game_field *field) {
    int res, i, j, rest;
    vector2i tmp;

    res = 1;
    
    for (i = 0; res == 1 && i < field->height - 1; i++) {
        for (j = 0; res == 1 && j < field->width; j++) {

            tmp = create_vector2i(j, i);
            if (get_available_game_field_cell(field, tmp))
                res = 0;
        }
    }

    rest = field->count % field->width;
    for (j = 0; res == 1 && j < rest; j++) {

        tmp = create_vector2i(j, field->height - 1);
        if (get_available_game_field_cell(field, tmp))
                res = 0;
    }

    return res;
}


void display_game_field(game_field *field) {
    int i, j, row_size;

    for(i = 0; i < field->height; i++) {
        row_size = get_game_field_row_size(field, i);
        
        for(j = 0; j < row_size; j++) {
            display_field_cell(field->table[i][j]);
        }

        printf("\n");
    }
}