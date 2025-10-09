#include<stdio.h>

#include"field_cell.h"

/* 
    Creates a new field cell structure.

    input:    value - the numeric value assigned to the cell
    output:   field_cell structure with the given value, marked as available and unselected
*/
field_cell create_field_cell(short value) {
    field_cell res;
    
    res.value = value;
    res.is_available = 1;
    res.is_selected = 0;
    res.is_highlited = 0;
    
    return res;
}

/* 
    Displays a field cell with appropriate color and formatting.

    input:    cell - field_cell structure to display
    output:   prints the cell to the console with its current visual state
*/
void display_field_cell(field_cell cell) {
    const char *color;

    if (!cell.is_available)
        color = UNENABLE_COLOR;
    else if (cell.is_highlited)
        color = HIGHLITED_COLOR;
    else
        color = ENABLE_COLOR;

    if (cell.is_selected)
        printf(SELECTED_PRINT, color, cell.value);
    else
        printf(BASE_PRINT, color, cell.value);
}

/*  
    Checks if two field cells form a valid match according to NumberMatch rules.

    input:  
        a - pointer to the first field_cell structure  
        b - pointer to the second field_cell structure  

    output:  
        returns 1 if the cells can be matched (their values are equal or sum to 10,  
        and both cells are available and distinct), otherwise returns 0  
*/
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