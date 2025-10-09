
#ifndef FIELD_CELL_H
#define FIELD_CELL_H

#define ENABLE_COLOR "\033[0m"
#define UNENABLE_COLOR "\033[35m"
#define HIGHLITED_COLOR "\033[36m"

#define BASE_PRINT "%s %d \033[0m"
#define SELECTED_PRINT "%s[%d]\033[0m"


struct field_cell {
    short value;
    int is_available;
    int is_selected;
    int is_highlited;
};

typedef struct field_cell field_cell;


field_cell create_field_cell(short value);

void display_field_cell(field_cell cell);

int check_field_cell_math(field_cell *a, field_cell *b);

#endif