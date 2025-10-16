#include<stdio.h>

#include"field_cell.h"


field_cell create_field_cell(short value) {
    field_cell res;
    
    res.value = value;
    res.is_available = 1;
    res.is_selected = 0;
    res.is_highlited = 0;
    res.is_cursor = 0;
    
    return res;
}

void display_field_cell(field_cell cell) {
    const char *color;

    if (cell.is_highlited)
        color = HIGHLITED_COLOR;
    else if (!cell.is_available)
        color = UNENABLE_COLOR;
    else
        color = ENABLE_COLOR;

    if (cell.is_selected && cell.is_cursor)
        printf(SELECTED_CURSOR_PRINT, color, cell.value);
    else if (cell.is_selected)
        printf(SELECTED_PRINT, color, cell.value);
    else if (cell.is_cursor)
        printf(CURSOR_PRINT, color, cell.value);
    else
        printf(BASE_PRINT, color, cell.value);
}

int check_field_cell_math(field_cell *a, field_cell *b) {
    int res;

    if (a == NULL || b == NULL || a == b ||
        !a->is_available || !b->is_available) {
        res = 0;
    } else if (a->value == b->value || a->value + b->value == 10) {
        res = 1;
    } else {
        res = 0;
    }

    return res;
}


int serialize_field_cell(field_cell *cell, FILE* file) {
    int res;
    short tmp;

    if (cell != NULL && file != NULL) {

        tmp = cell->value +
            cell->is_available * 128 +
            cell->is_highlited * 64 +
            cell->is_selected * 32 +
            cell->is_cursor * 16;
    
        if(fwrite(&tmp, sizeof(short) / 2, 1, file)) {
            res = 1;
        } else {
            res = 0;
        }
    } else {
        res = 0;
    }

    return res;
}

field_cell deserialize_field_cell(FILE* file) {
    field_cell res;
    short tmp;

    res.is_selected = 0;

    if (!fread(&tmp, sizeof(short) / 2, 1, file)) {
        res.value = 0;
        res.is_available = 0;
        res.is_highlited = 0;
    } else {
        res.value = tmp & 15;
        res.is_available = tmp / 128;
        res.is_highlited = tmp / 64 & 1;
        res.is_selected = tmp / 32 & 1;
        res.is_cursor = tmp / 16 & 1;
    }
    
    return res;
}