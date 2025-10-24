
#ifndef FIELD_CELL_H
#define FIELD_CELL_H

#define ENABLE_COLOR    "\033[0m"
#define UNENABLE_COLOR  "\033[35m"
#define HIGHLITED_COLOR "\033[36m"

#define BASE_PRINT "%s %d \033[0m"
#define SELECTED_PRINT "%s[%d]\033[0m"
#define CURSOR_PRINT "%s\033[4m %d \033[0m"
#define SELECTED_CURSOR_PRINT "%s\033[4m[%d]\033[0m"


/*  
    Represents a single cell in the game field.

    value         - numeric value of the cell (from 1 to 9)  
    is_available  - indicates whether the cell is active and can be used (1 = true, 0 = false)  
    is_selected   - marks the cell as currently selected by the player  
    is_highlited  - shows that the cell is highlighted
    is_cursor     - indicates that the cursor is currently pointing to this cell  
*/
struct field_cell {
    short value;
    int is_available;
    int is_selected;
    int is_highlited;
    int is_cursor;
};

typedef struct field_cell field_cell;

/* 
    Creates a new field cell structure.

    input:    value - the numeric value assigned to the cell
    output:   field_cell structure with the given value, marked as available and unselected
*/
field_cell create_field_cell(short value);
/* 
    Displays a field cell with appropriate color and formatting.

    input:    cell - field_cell structure to display
    output:   prints the cell to the console with its current visual state
*/
void display_field_cell(field_cell cell);
/*  
    Checks if two field cells form a valid match according to NumberMatch rules.

    input:  
        a - pointer to the first field_cell structure  
        b - pointer to the second field_cell structure  

    output:  
        returns 1 if the cells can be matched (their values are equal or sum to 10,  
        and both cells are available and distinct), otherwise returns 0  
*/
int check_field_cell_math(field_cell *a, field_cell *b);

#endif