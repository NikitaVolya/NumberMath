
#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#define MAX_WIDTH 25
#define MAX_HEIGHT 25

#include"field_cell.h"
#include"vector2i.h"


struct game_field {
    field_cell table[MAX_HEIGHT][MAX_WIDTH];
    int score, count;
    short stage, width, height;
};

typedef struct game_field game_field;


game_field* create_new_game_field(short width);
void add_values_game_field(game_field *field, short *values, int number);
int remove_game_field_row(game_field *field, int index);

int get_game_field_row_size(game_field *field, int index);
field_cell* get_game_field_cell(game_field *field, vector2i pos);

int get_available_game_field_cell(game_field *field, vector2i pos);
    
int set_highlight_game_field_cell(game_field *field, vector2i pos, int value);
int set_selection_game_field_cell(game_field *field, vector2i pos, int value);
int set_available_game_field_cell(game_field *field, vector2i pos, int value);

int find_match(game_field *field, vector2i *start_p, vector2i *end_p);

int check_match(game_field *field, vector2i start_p, vector2i end_p);
int check_game_row_is_clear(game_field *field, int index);
int check_game_field_is_clear(game_field *field);

void display_game_field(game_field *field);

#endif