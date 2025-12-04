/**
 * @file vector2i.h
 * @brief Utility functions and data structures for working with 2D integer vectors.
 *
 * This header defines the `vector2i` structure and a set of helper functions
 * for performing arithmetic operations, distance calculations, transformations,
 * and direction computations on 2D integer vectors.
 *
 * The module is lightweight and does not require dynamic memory allocation,
 * making it suitable for game development, grid-based systems, and general math utilities.
 */

#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <math.h>

/**
 * @brief A simple 2D integer vector.
 *
 * Represents a point or direction in a 2D integer grid using:
 * - `x` — horizontal coordinate
 * - `y` — vertical coordinate
 */
typedef struct vector2i
{
    int x; /**< X coordinate */
    int y; /**< Y coordinate */
} vector2i;


/**
 * @brief Creates a 2D vector with the given coordinates.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 *
 * @return A `vector2i` with fields set to (`x`, `y`).
 */
vector2i create_vector2i(int x, int y);


/**
 * @brief Creates a zero vector (0, 0).
 *
 * @return A `vector2i` with both coordinates equal to zero.
 */
vector2i create_zero_vector2i();


/**
 * @brief Adds two integer vectors.
 *
 * @param a First vector.
 * @param b Second vector.
 *
 * @return A vector representing the sum `a + b`.
 */
vector2i add_vector2i(vector2i a, vector2i b);


/**
 * @brief Subtracts one vector from another.
 *
 * @param a Minuend vector.
 * @param b Subtrahend vector.
 *
 * @return A vector representing the difference `a - b`.
 */
vector2i sub_vector2i(vector2i a, vector2i b);


/**
 * @brief Multiplies a vector by an integer scalar.
 *
 * @param vector The vector to scale.
 * @param value  The scalar multiplier.
 *
 * @return A scaled vector.
 */
vector2i mult_vector2i(vector2i vector, int value);


/**
 * @brief Returns a vector whose coordinates are absolute values of the input.
 *
 * @param vector Input vector.
 *
 * @return A vector with absolute X and Y components.
 */
vector2i abs_vector2i(vector2i vector);


/**
 * @brief Applies a function to each component of a vector.
 *
 * @param vector Input vector.
 * @param fptr   Pointer to a function that accepts and returns an int.
 *
 * @return A new vector where each component is the result of `fptr`.
 */
vector2i foreach_vector2i(vector2i vector, int(*fptr)(int));


/**
 * @brief Computes the vector difference from point `a` to point `b`.
 *
 * Equivalent to `b - a`.
 *
 * @param a Starting point.
 * @param b Ending point.
 *
 * @return A vector representing the difference.
 */
vector2i get_vector2i_to(vector2i a, vector2i b);


/**
 * @brief Converts a vector into a normalized grid direction.
 *
 * Each component becomes:
 * - `-1` if negative
 * - `0`  if zero
 * - `1`  if positive
 *
 * @param vector Input vector.
 *
 * @return A direction vector with components in {-1, 0, 1}.
 */
vector2i get_direction(vector2i vector);


/**
 * @brief Computes the direction from point `a` to point `b`.
 *
 * Equivalent to calling `get_direction(b - a)`.
 *
 * @param a Starting point.
 * @param b Target point.
 *
 * @return A step vector with values in {-1, 0, 1}.
 */
vector2i get_direction_to(vector2i a, vector2i b);


/**
 * @brief Computes the Euclidean length of a vector.
 *
 * @param vector Input vector.
 *
 * @return Length as a floating-point value.
 */
float vector2i_get_length(vector2i vector);


/**
 * @brief Computes the Euclidean distance between two points.
 *
 * @param a First point.
 * @param b Second point.
 *
 * @return Distance between `a` and `b`.
 */
float vector2i_get_distance(vector2i a, vector2i b);

#endif /* VECTOR2I_H */
