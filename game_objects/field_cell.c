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