#include<stdlib.h>
#include<stdio.h>

#include"game_field.h"


game_field* create_new_game_field(short width) {
    game_field *res;

    if (width > 0) {
        res = (game_field*)malloc(sizeof(game_field));

        res->score = 0;
        res->count = 0;
        res->stage = 0;
    
        res->width = width;
        res->height = 0;
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

void expand_game_field(game_field *field) {
    int i, x, y, old_count = field->count;
    short tmp;

    for (i = 0; i < old_count; i++) {
        x = i % field->width;
        y = i / field->width;

        if (field->table[y][x].is_available) {
            tmp = field->table[y][x].value;
            add_values_game_field(field, &tmp, 1);
        }
    }
    
}

int serialize_game_field(game_field* field, const char* file_name) {
    FILE* file;
    int res, i, x, y;

    if ((file = fopen(file_name, "w")) == NULL) {
        printf("Error while serializing game file\nCant write in file: %s\n", file_name);
        res = 0;
    } else {
        res = 1;
      
        fwrite(&field->width, sizeof(unsigned short), 1, file);
        fwrite(&field->height, sizeof(unsigned short), 1, file);
        fwrite(&field->stage, sizeof(unsigned short), 1, file);
        fwrite(&field->count, sizeof(int) / 2, 1, file);

        for (i = 0; i < field->count; i++) {
            y = i / field->width;
            x = i % field->width;
            
            serialize_field_cell(field->table[y] + x, file);
        }

        fclose(file);
    }

    return res;
}


game_field* deserialize_game_field(const char* file_name) {
    FILE* file;
    game_field* res;
    size_t file_size;
    int i, x, y;

    res = NULL;
    if ((file = fopen(file_name, "r")) == NULL) {
        printf("Error while deserializing game file\nCant read from file: %s\n", file_name);
    } else {

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        if (file_size >= 8) {

            res = (game_field*) malloc(sizeof(game_field));

            if (!fread(&res->width, sizeof(unsigned short), 1, file) ||
                !fread(&res->height, sizeof(unsigned short), 1, file) ||
                !fread(&res->stage, sizeof(unsigned short), 1, file) ||
                !fread(&res->count, sizeof(int) / 2, 1, file) ||
                (size_t)res->count + 8 != file_size) {
                free(res);
                res = NULL;
            } else {
                for (i = 0; i < res->count; i++) {
                    y = i / res->width;
                    x = i % res->width;
                    res->table[y][x] = deserialize_field_cell(file);
                }
            }
        }

        fclose(file);
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
    int res = -1, next_line = 0;
    vector2i current_p, direction, abs_delta, tmp;
    field_cell *start_cell, *end_cell, *current_cell;

    abs_delta = abs_vector2i(get_vector2i_to(start_p, end_p));

    /* switch cells if end point is befor start point */
    if (start_p.y > end_p.y) {
        tmp = start_p;
        start_p = end_p;
        end_p = tmp;
    }
    
    start_cell = get_game_field_cell(field, start_p);
    end_cell = get_game_field_cell(field, end_p);

    /* return 0 if cells can't matching */
    if (!check_field_cell_math(start_cell, end_cell)) {
        res = 0;
    /* check if this diagonal, vertical or horisontal match */
    } else if (abs_delta.x == 0 || abs_delta.y == 0 || abs_delta.x == abs_delta.y) {
        direction = get_direction_to(start_p, end_p);
    /* check if the next line match */
    } else if (start_p.y < end_p.y) {
        direction = create_vector2i(1, 0);
        next_line = 1;
    /* default return 0 */
    } else {
        res = 0;
    }

    /* if res is not 0 start check iteration */
    current_p = start_p;
    while (res == -1) {
        /* move cursor */
        current_p = add_vector2i(current_p, direction);
        current_cell = get_game_field_cell(field, current_p);

        /* return 1 if cursor on end cell */
        if (current_cell == end_cell) {
            res = 1;
        } else if (current_cell == NULL) {
            /* jump on next line */
            if (next_line && current_p.y < field->height - 1) {
                current_p.y += 1;
                current_p.x = -1;
            /* return 0 if cursor is out of bounds and is not next line check */
            } else {
                res = 0;
            }
        /* return 0 if cursor on available cell that is not end cell */
        } else if (current_cell->is_available) {
            res = 0;
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

int check_game_is_over(game_field *field) {
    int res;
    vector2i start, end;

    if (check_game_field_is_clear(field) ||
        !find_match(field, &start, &end)) {
        res = 1;
    } else {
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