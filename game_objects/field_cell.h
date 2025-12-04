/**
 * @file field_cell.h
 * @brief Defines the structure and operations for a single game field cell.
 *
 * This header provides the @ref field_cell structure along with helper
 * functions used to create and validate cells in the NumberMatch-style game.
 *
 */

#ifndef FIELD_CELL_H
#define FIELD_CELL_H


/**
 * @brief Represents a single cell in the game field.
 *
 * Each cell stores a numeric value and several state flags that determine
 * how it should be displayed and whether the player can interact with it.
 */
typedef struct  {
    short value;        /**< Numeric value of the cell (1–9). */
    int is_available;   /**< 1 if the cell is active and can be used; 0 otherwise. */
    int is_selected;    /**< 1 if the cell is currently selected by the player. */
    int is_highlited;   /**< 1 if the cell is visually highlighted. */
    int is_cursor;      /**< 1 if the cursor is currently pointing to this cell. */
} field_cell;

/**
 * @brief Creates and initializes a new field cell.
 *
 * @param value Numeric value to assign to the cell (1–9).
 *
 * @return A field_cell structure initialized with:
 *         - the given value,
 *         - is_available set to 1,
 *         - is_selected, is_highlited and is_cursor set to 0.
 */
field_cell create_field_cell(short value);

/**
 * @brief Checks whether two field cells form a valid NumberMatch pair.
 *
 * Two cells form a valid match if:
 * - Their values are equal OR their values sum to 10.
 * - Both cells are marked as available.
 * - The two pointers refer to different cells.
 *
 * @param a Pointer to the first field_cell structure.
 * @param b Pointer to the second field_cell structure.
 *
 * @return 1 if the cells form a valid match,  
 *         0 otherwise.
 */
int check_field_cell_math(field_cell *a, field_cell *b);

#endif