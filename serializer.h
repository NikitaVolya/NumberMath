#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include<stdlib.h>
#include<stdio.h>

#include"game_objects/game_field.h"
#include"game_objects/field_cell.h"

/*  
    Saves a single field cell to a file in a compact format.

    input:  
        cell - pointer to the field_cell structure to serialize  
        file - pointer to an open file for writing  

    output:  
        returns 1 if the cell was successfully written,  
        0 otherwise  
*/
int serialize_field_cell(field_cell *cell, FILE* file);

/*  
    Loads a single field cell from a file.

    input:  
        file - pointer to an open file for reading  

    output:  
        returns a field_cell structure with values read from the file,  
        or a default empty cell if reading fails  
*/
field_cell deserialize_field_cell(FILE* file);

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



#endif /* _SERIALIZER_H */