/**
 * @file serializer.h
 * @brief Functions for saving and loading NumberMatch game state to and from files.
 *
 * This header provides functions to serialize and deserialize individual field cells
 * as well as the entire game field. It allows saving the current game progress to a file
 * and loading it back to resume the game.
 *
 * @see @ref game_field.h , @ref field_cell.h
 */

#include<stdlib.h>
#include<stdio.h>

#include"game_objects/game_field.h"
#include"game_objects/field_cell.h"

/**
 * @brief Saves a single field cell to a file in a compact format.
 *
 * @param[in] cell Pointer to the field_cell structure to serialize.
 * @param[in,out] file Pointer to an already opened file for writing.
 *
 * @return int Returns 1 if the cell was successfully written, 0 otherwise.
 */
int serialize_field_cell(field_cell *cell, FILE* file);

/**
 * @brief Loads a single field cell from a file.
 *
 * @param[in,out] file Pointer to an already opened file for reading.
 *
 * @return field_cell Returns a field_cell structure with values read from the file,
 *                    or a default empty cell if reading fails.
 */
field_cell deserialize_field_cell(FILE* file);

/**
 * @brief Saves the current game field state to a file.
 *
 * @param[in] field Pointer to the game_field structure to save.
 * @param[in] file_name Path to the output file.
 *
 * @return int Returns 1 if serialization succeeded, 0 if the file could not be opened for writing.
 */
int serialize_game_field(game_field* field, const char* file_name);

/**
 * @brief Loads a saved game field state from a file.
 *
 * @param[in] file_name Path to the file containing serialized game data.
 *
 * @return game_field* Returns a pointer to the newly allocated game_field structure
 *                     if loading succeeds, otherwise returns NULL.
 */
game_field* deserialize_game_field(const char* file_name);