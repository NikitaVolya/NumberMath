
#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#define MAX_WIDTH 10
#define MAX_HEIGHT 51 * 5 + 1

#include"field_cell.h"
#include"vector2i.h"


/*  
    Represents the game field for NumberMatch.

    table                - 2D array of field_cell structures (cells of the field) 
    count                - total number of cells currently in use
    
    score                - current game score 
    stage                - current stage/level of the game
    
    width                - number of cells in each row  
    height               - number of rows currently in the field

    hints_available      - number of hints the player currently has  
    hints_max            - maximum hints allowed

    additions_available  - number of additions the player currently has
    additions_max        - maximum number of additions allowed  
*/
struct game_field {
    field_cell table[MAX_HEIGHT][MAX_WIDTH];
    int score, count;
    unsigned short stage, width, height,
        hints_available, hints_max,
        additions_available, additions_max;
};

typedef struct game_field game_field;


/*  
    Creates and initializes a new game field with the specified width.

    input:  
        width - desired field width (must be greater than 0)

    output:  
        returns a pointer to the newly allocated game_field structure  
        with initialized default values (score, count, stage, height = 0);  
        returns NULL if the provided width is invalid  
*/
game_field* create_new_game_field(short width);

/*  
    Adds new cell values to the game field sequentially.

    input:  
        field  - pointer to the game_field structure to modify  
        values - array of short integers containing the values to insert  
        number - number of values to add from the array  

    output:  
        fills the field’s internal table with the provided values  
        starting from the next available cell position,  
        updates the total count of cells and recalculates the field height  
        (does nothing if field is NULL)  
*/
void add_values_game_field(game_field *field, short *values, int number);

/*  
    Removes a row from the game field by its index.

    input:  
        field - pointer to the game_field structure  
        index - row index to remove  

    output:  
        returns 1 if the row was successfully removed,  
        0 if the index is invalid  
*/
int remove_game_field_row(game_field *field, int index);

/*  
    Expands the game field by duplicating all available cells  
    and appending their values to the end of the field.

    input:  
        field - pointer to the game_field structure to expand
*/
void expand_game_field(game_field *field);


/*  
    Saves the current game field state to a file.

    input:  
        field     - pointer to the game_field structure to save  
        file_name - path to the output file  

    output:  
        returns 1 if serialization succeeded,  
        0 if the file could not be opened for writing  
*/
int serialize_game_field(game_field* field, const char* file_name);

/*  
    Loads a saved game field state from a file.

    input:  
        file_name - path to the file containing serialized game data  

    output:  
        returns a pointer to the newly allocated game_field structure  
        if loading succeeds, otherwise returns NULL  
*/
game_field* deserialize_game_field(const char* file_name);



/*  
    Returns the number of cells in a specific row of the game field.

    input:  
        field - pointer to the game_field structure  
        index - row index  

    output:  
        returns the number of cells in the given row,  
        or 0 if the index is invalid  
*/
int get_game_field_row_size(game_field *field, int index);

/*  
    Checks whether a specific cell in the game field is available.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  

    output:  
        returns 1 if the cell exists and is available,  
        otherwise returns 0  
*/
field_cell* get_game_field_cell(game_field *field, vector2i pos);

/*  
    Checks whether a specific cell in the game field is available.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  

    output:  
        returns 1 if the cell exists and is available,  
        otherwise returns 0  
*/
int get_available_game_field_cell(game_field *field, vector2i pos);



/*  
    Sets the highlight state of a specific cell in the game field.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  
        value - new highlight state (non-zero to highlight, 0 to remove)  

    output:  
        returns 1 if the cell exists and was updated,  
        otherwise returns 0  
*/
int set_highlight_game_field_cell(game_field *field, vector2i pos, int value);

/*  
    Sets the selection state of a specific cell in the game field.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  
        value - new selection state (non-zero to select, 0 to deselect)  

    output:  
        returns 1 if the cell exists and was updated,  
        otherwise returns 0  
*/
int set_selection_game_field_cell(game_field *field, vector2i pos, int value);

/*  
    Sets the availability state of a specific cell in the game field.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  
        value - new availability state (non-zero for available, 0 for unavailable)  

    output:  
        returns 1 if the cell exists and was updated,  
        otherwise returns 0  
*/
int set_available_game_field_cell(game_field *field, vector2i pos, int value);

/*  
    Sets the cursor state of a specific cell in the game field.

    input:  
        field - pointer to the game_field structure  
        pos   - cell position (x, y) within the field  
        value - new cursor state (non-zero if the cursor is on this cell, 0 otherwise)  

    output:  
        returns 1 if the cell exists and was updated,  
        otherwise returns 0  
*/
int set_cursor_game_field_cell(game_field *field, vector2i pos, int value);

/*  
    Searches the game field for a valid match according to NumberMatch rules.

    input:  
        field   - pointer to the game_field structure  
        start_p - pointer to a vector2i that will store the start cell of the match  
        end_p   - pointer to a vector2i that will store the end cell of the match  

    output:  
        returns 1 if a matching pair is found and updates start_p and end_p,  
        otherwise returns 0  
*/
int find_match(game_field *field, vector2i *start_p, vector2i *end_p);


/*  
    Verifies if two cells in the game field form a valid match path.

    input:  
        field   - pointer to the game_field structure  
        start_p - start cell position (x, y)  
        end_p   - end cell position (x, y)  

    output:  
        returns 1 if a valid match exists between the two cells,  
        0 if the cells cannot be matched according to the rules  
*/
int check_match(game_field *field, vector2i start_p, vector2i end_p);

/*  
    Checks if a specific row in the game field is completely cleared.

    input:  
        field - pointer to the game_field structure  
        index - row index to check  

    output:  
        returns 1 if all cells in the row are unavailable,  
        0 if any cell is still available or index is invalid  
*/
int check_game_row_is_clear(game_field *field, int index);

/*  
    Checks if the entire game field is cleared (no available cells).

    input:  
        field - pointer to the game_field structure  

    output:  
        returns 1 if all cells are unavailable,  
        0 if any cell is still available  
*/
int check_game_field_is_clear(game_field *field);

/*  
    Checks if the game is over.

    input:  
        field - pointer to the game_field structure  

    output:  
        returns 1 if the game is over (field is cleared or no matches left),  
        0 otherwise  
*/
int check_game_is_over(game_field *field);

/*  
    Displays the entire game field on the console.

    input:  
        field - pointer to the game_field structure  

    output:  
        prints each cell of the field with proper formatting and colors  
*/
void display_game_field(game_field *field);

#endif