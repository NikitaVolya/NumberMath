/**
 * @file game_field.h
 * @brief Data structures representing the NumberMatch game field.
 *
 * This header defines the core structures used to store and manage
 * the NumberMatch game state. It includes the definition of individual
 * cells within the grid as well as the full game field containing
 * score tracking, dimensions, and available actions.
 *
 * Structures:
 * - field_cell — represents a single cell of the game grid.
 * - game_field — stores the entire game state including the grid,
 *   score, tools, and field dimensions.
 */
 
#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include<stdlib.h>
#include<stdio.h>
#include"field_cell.h"
#include"vector2i.h"

struct field_row {
    field_cell** items;              /**< Dynamic array of cell pointers within a single row. */
    size_t count;                    /**< Current number of cells in the row. */
    size_t capacity;                 /**< Allocated capacity of the row. */
};
typedef struct field_row field_row;

struct field_table {
    field_row** items;              /**< Dynamic array of row pointers forming the grid. */
    size_t count;                   /**< Current number of rows in the table. */
    size_t capacity;                /**< Allocated capacity for rows. */
};
typedef struct field_table field_table;

/**
 * @brief Represents the complete NumberMatch game field and its runtime state.
 *
 * This structure holds the dynamic grid of cells along with all gameplay-related
 * counters such as score, stage progression, available hints, and additions.
 */
struct game_field {
    field_table *table;                 /**< 2D grid of game cells. */

    int score;                          /**< Current player score. */
    int count;                          /**< Number of active (non-empty) cells. */

    unsigned short stage;               /**< Current game stage (level). */
    unsigned short width;               /**< Number of columns in the field. */

    unsigned short hints_available;     /**< Remaining hints available to the player. */
    unsigned short hints_max;           /**< Maximum number of hints allowed. */

    unsigned short additions_available; /**< Remaining additions (expansions) available. */
    unsigned short additions_max;       /**< Maximum number of additions allowed. */
};

typedef struct game_field game_field;


/**
 * @brief Describes the type of match found between two game cells.
 *
 * These values represent different match categories used in the
 * NumberMatch game logic when verifying whether two cells form a valid match
 * and what additional effects should occur on the field after matching.
 */
enum MATCH_TYPE {
    DIRECTE_MATCH = 1,       /**< Direct horizontal, vertical, or diagonal match. */
    NEXT_LINE_MATCH = 2,     /**< Match continues on the next line. */
    DISTANCE_MATCH = 4,      /**< Match with one or more empty spaces between cells. */
    CLEAR_LINE_MATCH = 10,   /**< Successful match that triggers clearing of a full line. */
    CLEAR_FIELD_MATCH = 150, /**< Successful match that triggers clearing of the entire field. */
    NOT_MATCH = 0,           /**< Cells do not form a valid match. */
    NONE_MATCH = -1          /**< Default uninitialized state before checking. */
};

typedef enum MATCH_TYPE MATCH_TYPE;


/**  
 * @brief Creates and initializes a new game field with the specified width.
 *
 * This function allocates memory for a new game_field structure and sets
 * initial default values such as score, count, stage, and height. The table
 * is empty initially.
 *
 * @param width Desired field width (must be greater than 0)
 * @return Pointer to the newly allocated game_field structure with initialized
 *         default values; returns NULL if the provided width is invalid
 */
game_field* create_new_game_field(short width);

/**
 * @brief Initializes the table of the game field.
 *
 * Allocates an empty field_table and assigns it to the given game_field.
 * The table is created with zero initial rows.
 *
 * @param[in,out] field Pointer to the game_field to initialize.
 */
void init_game_field_table(game_field *field);

/**
 * @brief Returns the current height of the game field.
 *
 * The height corresponds to the number of rows stored in the field table.
 *
 * @param[in] field Pointer to the game_field.
 * @return int Number of rows in the field.
 */
int get_game_field_height(game_field *field);

/**
 * @brief Adds a new cell to the game field, automatically creating rows as needed.
 *
 * This function appends a cell to the last row of the game field.  
 * If the field has no rows yet, a new row is created.  
 * If the last row is already full (based on field->width), a new row is created
 * and the cell is added there.
 *
 * @param[in,out] field Pointer to the game_field structure being modified.
 * @param[in]     cell  Pointer to the cell to add to the field.
 *
 * @note Rows grow automatically; the function ensures the field maintains a
 *       rectangular structure defined by `field->width`.
 */
void add_cell_game_field(game_field *field, field_cell *cell);

/**  
 * @brief Adds new cell values to the game field sequentially.
 *
 * Inserts values from the provided array into the next available positions
 * in the field’s internal table, updating the total cell count and
 * recalculating the field height.
 *
 * @param field[in/out] Pointer to the game_field structure to modify
 * @param values[in] Array of short integers containing the values to insert
 * @param number[in] Number of values to add from the array
 *
 * @note Does nothing if the provided field pointer is NULL.
 */
void add_values_game_field(game_field *field, short *values, int number);

/**  
 * @brief Removes a row from the game field by its index.
 *
 * Deletes the specified row from the field's internal table, 
 * shifting all subsequent rows up and updating the field height.
 *
 * @param field[in/out] Pointer to the game_field structure
 * @param index[in] Row index to remove
 *
 * @return int Returns 1 if the row was successfully removed, 0 if the index is invalid.
 */
int remove_game_field_row(game_field *field, int index);

/**  
 * @brief Returns the number of cells in a specific row of the game field.
 *
 * @param[in] field Pointer to the game_field structure
 * @param[in] index Row index
 *
 * @return int Number of cells in the given row, or 0 if the index is invalid
 */
int get_game_field_row_size(game_field *field, int index);

/**  
 * @brief Checks whether a specific cell in the game field is available.
 *
 * @param[in] field Pointer to the game_field structure
 * @param[in] pos   Cell position (x, y) within the field
 *
 * @return int Returns 1 if the cell exists and is available, otherwise 0
 */
field_cell* get_game_field_cell(game_field *field, vector2i pos);

/**  
 * @brief Checks whether a specific cell in the game field is available.
 *
 * @param[in] field Pointer to the game_field structure
 * @param[in] pos   Cell position (x, y) within the field
 *
 * @return int Returns 1 if the cell exists and is available, otherwise 0
 */
int get_available_game_field_cell(game_field *field, vector2i pos);

/**  
 * @brief Sets the highlight state of a specific cell in the game field.
 *
 * @param[in,out] field Pointer to the game_field structure
 * @param[in]     pos   Cell position (x, y) within the field
 * @param[in]     value New highlight state (non-zero to highlight, 0 to remove)
 *
 * @return int Returns 1 if the cell exists and was updated, otherwise 0
 */
int set_highlight_game_field_cell(game_field *field, vector2i pos, int value);

/**  
 * @brief Sets the selection state of a specific cell in the game field.
 *
 * @param[in,out] field Pointer to the game_field structure
 * @param[in]     pos   Cell position (x, y) within the field
 * @param[in]     value New selection state (non-zero to select, 0 to deselect)
 *
 * @return int Returns 1 if the cell exists and was updated, otherwise 0
 */
int set_selection_game_field_cell(game_field *field, vector2i pos, int value);

/**  
 * @brief Sets the availability state of a specific cell in the game field.
 *
 * @param[in,out] field Pointer to the game_field structure
 * @param[in]     pos   Cell position (x, y) within the field
 * @param[in]     value New availability state (non-zero for available, 0 for unavailable)
 *
 * @return int Returns 1 if the cell exists and was updated, otherwise 0
 */
int set_available_game_field_cell(game_field *field, vector2i pos, int value);

/**  
 * @brief Sets the cursor state of a specific cell in the game field.
 *
 * @param[in,out] field Pointer to the game_field structure
 * @param[in]     pos   Cell position (x, y) within the field
 * @param[in]     value New cursor state (non-zero if the cursor is on this cell, 0 otherwise)
 *
 * @return int Returns 1 if the cell exists and was updated, otherwise 0
 */
int set_cursor_game_field_cell(game_field *field, vector2i pos, int value);

/**  
 * @brief Searches the game field for a valid match according to NumberMatch rules.
 *
 * @param[in]  field   Pointer to the game_field structure
 * @param[out] start_p Pointer to a vector2i that will store the start cell of the match
 * @param[out] end_p   Pointer to a vector2i that will store the end cell of the match
 *
 * @return int Returns 1 if a matching pair is found and updates start_p and end_p, otherwise 0
 */
int find_match(game_field *field, vector2i *start_p, vector2i *end_p);

/**  
 * @brief Checks whether two cells in the game field can form a valid match.
 *
 * @param[in]  field   Pointer to the game_field structure
 * @param[in]  start_p Starting cell position (x, y)
 * @param[in]  end_p   Ending cell position (x, y)
 *
 * @return MATCH_TYPE Returns one of the MATCH_TYPE values indicating the match type,
 *                    or NOT_MATCH (0) if the cells cannot be matched according to the rules
 */
MATCH_TYPE check_match(game_field *field, vector2i start_p, vector2i end_p);

/**  
 * @brief Checks if a specific row in the game field is completely cleared.
 *
 * @param[in]  field Pointer to the game_field structure
 * @param[in]  index Row index to check
 *
 * @return int Returns 1 if all cells in the row are unavailable,
 *             0 if any cell is still available or index is invalid
 */
int check_game_row_is_clear(game_field *field, int index);

/**  
 * @brief Checks if the entire game field is cleared (no available cells).
 *
 * @param[in]  field Pointer to the game_field structure
 *
 * @return int Returns 1 if all cells are unavailable,
 *             0 if any cell is still available
 */
int check_game_field_is_clear(game_field *field);

/**
 * @brief Frees all memory associated with a game_field structure.
 *
 * @param[in] field Pointer to the game_field structure to free.
 *
 * @note After calling this function, the pointer becomes invalid and must not be used.
 */
void game_field_free(game_field *field);

#endif