/**
 * @file vector.h
 * @brief Generic dynamic vector implementation in C.
 *
 * This header provides macros and inline functions to generate
 * type-safe dynamic arrays (vectors) for any data type in C.
 * It supports creating, freeing, and manipulating vectors with
 * operations such as push, pop, insert, remove, map, filter, etc.
 *
 * @author Volianskyi Nikita
 * @note Adapted from the article by David Priver:
 *       https://www.davidpriver.com/ctemplates.html
 *
 * @attention All code in this project is released into the public domain.
 *
 * @note
 * This section is intentionally left unguarded so that it can
 * be included multiple times—once for each vector type instantiation.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>

/**
 * @name Name-Combining and String-Conversion Macros
 * @brief Internal utility macros used to generate vector function names and string literals.
 *
 * These macros allow the generic vector system to:
 * - concatenate identifiers to form function names (`pre##word`)
 * - convert macro values into string literals
 *
 * They are internal but documented because they explain how automatic
 * function names like `vector_int_set` are generated.
 */
/** @{ */

/**
 * @def VECTOR_COMB1(pre, word)
 * @brief Indirect macro expansion before token concatenation.
 *
 * This macro forces the full expansion of @c pre and @c word before passing
 * them to `VECTOR_COMB2()`, ensuring the resulting name is correctly formed.
 *
 * @note  
 * C’s `##` operator does **not** expand macro arguments automatically.  
 * Wrapping it inside two levels fixes that:
 * - `VECTOR_COMB1(A, B)` → expands A and B, then → `VECTOR_COMB2(A, B)`
 * - `VECTOR_COMB2(A, B)` → performs `A##B`
 */
#define VECTOR_COMB1(pre, word) VECTOR_COMB2(pre, word)

/**
 * @def VECTOR_COMB2(pre, word)
 * @brief Concatenates two identifiers.
 *
 * After full expansion from `VECTOR_COMB1`, this macro joins the tokens:
 *
 * @code
 * VECTOR_COMB2(Vector_, int)  // → Vector_int
 * VECTOR_COMB2(foo, bar)      // → foobar
 * @endcode
 *
 * This is the core mechanism behind automatically generated vector function
 * names such as `Vector_int_set`, `Vector_float_push`, etc.
 */
#define VECTOR_COMB2(pre, word) pre##word

/**
 * @def STRINGIZE(s)
 * @brief Converts a macro argument into a string literal (after expansion).
 *
 * This macro stringifies @c s but only *after* expanding its macro value,
 * thanks to the two-step approach using `STRINGIZE2()`.
 *
 * @code
 * #define TYPE int
 * STRINGIZE(TYPE)  // → "int"
 * @endcode
 */
#define STRINGIZE(s) STRINGIZE2(s)

/**
 * @def STRINGIZE2(s)
 * @brief Direct stringification without expansion.
 *
 * Used internally by `STRINGIZE()`—this macro applies the @c # operator
 * directly, without expanding @c s first.
 */
#define STRINGIZE2(s) #s
/** @} */

#endif /* VECOTR_H */

/**
 * @def VECTOR_TYPE
 * @brief User-defined macro defining the type stored in the vector.
 *
 * This macro must be defined before including the vector header.  
 * It specifies the data type of the vector elements.
 *
 * @note Example:
 * @code
 * #define VECTOR_TYPE int
 * #include "vector.h"
 * @endcode
 */
#ifndef VECTOR_TYPE
#error "VECTOR_TYPE is required! Define VECTOR_TYPE before including this header."
#endif /* VECTOR_TYPE */


#ifndef VECTOR_NAME
/**
 * @brief
 * Defines the name of the vector data structure to be generated.
 *
 * If VECTOR_NAME is not predefined, a default name will be created
 * in the format `vector_VECTOR_TYPE`, e.g., `vector_int`.
 */
#define VECTOR_NAME VECTOR_COMB1(VECTOR_COMB1(vector,_), VECTOR_TYPE)
#endif /* VECTOR_NAME */

#ifndef VECTOR_PREFIX
/**
 * @brief Prefix for all generated vector functions.
 *
 * If VECTOR_PREFIX is not predefined, it defaults to VECTOR_NAME_
 * For example, for VECTOR_NAME = vector_int, the prefix becomes vector_int_
*/
#define VECTOR_PREFIX VECTOR_COMB1(VECTOR_NAME, _)
#endif /* VECTOR_PREFIX */

/**
 * @brief Concatenates a prefix and a word to generate a new identifier.
 *
 * This macro is used internally to create type- and function-specific
 * names for vector instances based on the configured @ref VECTOR_PREFIX .
 *
 * @param prefix The prefix to use (typically @ref VECTOR_PREFIX)
 * @param word   The word to append to the prefix
 */
#define VECTOR_IMPL(word) VECTOR_COMB1(VECTOR_PREFIX, word)

/**
 * @brief Converts the VECTOR_NAME macro into a string literal.
 *
 * This macro produces a string version of the generated vector's name.
 * Internally it uses the @ref STRINGIZE macro to transform the
 * @c VECTOR_NAME identifier into a string constant.
 *
 * @note
 * This is useful for debugging, logging, or generating readable error messages.
 *
 * @see VECTOR_NAME
 * @see STRINGIZE
 */
#define VECTOR_NAME_STRING STRINGIZE(VECTOR_NAME)


/**
 * @name Vector Function Name Generator Macros
 * @brief Expands generic function names into type-specific ones.
 *
 * These macros automatically generate the final function names for the
 * vector implementation by combining the user-defined @ref VECTOR_PREFIX
 * with a functional suffix such as `push`, `set`, `remove`, etc.
 *
 * For example, if:
 * @code
 * #define VECTOR_PREFIX vector_int_
 * @endcode
 *
 * then:
 * @code
 * VECTOR_push  →  vector_int_push
 * VECTOR_set   →  vector_int_set
 * VECTOR_map   →  vector_int_map
 * @endcode
 *
 * This allows the entire vector module to be generated automatically
 * for any data type simply by defining:
 * @code
 * #define VECTOR_TYPE int
 *
 * or
 *
 * #define VECTOR_TYPE int
 * #define VECTOR_PREFIX vectori_
 * @endcode
 */

/** @{ */


/** @def VECTOR_create   
 *  @brief Creates a new vector instance. 
 */
#define VECTOR_create VECTOR_IMPL(create)

/** @def VECTOR_expand   
 *  @brief Expands vector capacity if needed. 
 */
#define VECTOR_expand VECTOR_IMPL(expand)

/** @def VECTOR_free     
 *  @brief Frees all vector memory. 
 */
#define VECTOR_free VECTOR_IMPL(free)

/** @def VECTOR_get      
 *  @brief Retrieves an element by index. 
 */
#define VECTOR_get VECTOR_IMPL(get)

/** @def VECTOR_set      
 *  @brief Sets an element at a given index. 
 */
#define VECTOR_set VECTOR_IMPL(set)

/** @def VECTOR_push    
 *  @brief Appends a new value to the end of the vector. 
 */
#define VECTOR_push VECTOR_IMPL(push)

/** @def VECTOR_insert   
 *  @brief Inserts a value at a specific index. 
 */
#define VECTOR_insert VECTOR_IMPL(insert)

/** @def VECTOR_pop      
 *  @brief Removes and returns the last element. 
 */
#define VECTOR_pop VECTOR_IMPL(pop)

/** @def VECTOR_remove   
 *  @brief Removes an element at a given index. 
 */
#define VECTOR_remove VECTOR_IMPL(remove)

/** @def VECTOR_foreach  
 *  @brief Iterates through each element in the vector. 
 */
#define VECTOR_foreach VECTOR_IMPL(foreach)

/** @def VECTOR_map      
 *  @brief Applies a mapping function to each element. 
 */
#define VECTOR_map VECTOR_IMPL(map)

/** @def VECTOR_any 
 *  @brief Returns true if any element satisfies a predicate. 
 */
#define VECTOR_any VECTOR_IMPL(any)

/** @def VECTOR_first_or_default
 *  @brief Returns the first element matching a predicate or a default value.
 */
#define VECTOR_first_or_default VECTOR_IMPL(first_or_default)

/** @def VECTOR_filter   
 *  @brief Creates a new vector containing elements that match a predicate. 
 */
#define VECTOR_filter VECTOR_IMPL(filter)

/** @} */

/**
 * @def VECTOR_STRUCT_DEFINED
 * @brief Include guard used to prevent multiple definitions of the vector structure.
 *
 * Ensures the generic VECTOR_NAME struct is defined only once when this header
 * is included in multiple translation units.
 */
#ifndef VECTOR_STRUCT_DEFINED
#define VECTOR_STRUCT_DEFINED

/**
 * @brief Type definition for a generic dynamic vector.
 * 
 * @typedef VECTOR_NAME
 * @struct VECTOR_NAME
 * 
 * @details
 * - `items`    : Pointer to the dynamically allocated array holding the elements.
 * - `count`    : Current number of elements stored in the vector.
 * - `capacity` : Allocated capacity of the vector (number of elements it can hold before expanding).
 */
struct VECTOR_NAME {
    VECTOR_TYPE* items;
    size_t count;
    size_t capacity;
};
typedef struct VECTOR_NAME VECTOR_NAME;

#endif VECTOR_STRUCT_DEFINED

/**
 * @brief Creates and initializes a new vector instance.
 *
 * @param[in] size Init size of vector
 *
 *
 * @return VECTOR_NAME* Pointer to the newly created vector structure,
 *         or NULL if memory allocation fails.
 * 
 */
VECTOR_NAME * VECTOR_create (size_t size) {
    VECTOR_NAME* res = malloc(sizeof(VECTOR_NAME));
    
    if (res != NULL) {

        if (size == 0)
            res->items = NULL;
        else 
            res->items = (VECTOR_TYPE*) calloc(size, sizeof(VECTOR_TYPE));

        res->count = 0;
        res->capacity = size;
    }

    return res;
}

/** 
 * @brief Expands the allocated memory of a vector when it becomes full.
 * 
 * @param[in,out] vector Pointer to the vector of type VECTOR_NAME*.
 * 
 * @details
 * - Doubles the vector's capacity if it already has allocated memory.
 * - If the vector is empty, initializes capacity to 4.
 * - Reallocates memory to hold the new number of elements.
 * 
 * @note
 * Updates `vector->items` and `vector->capacity` in place.
 */
void VECTOR_expand (VECTOR_NAME* vector) {
    size_t new_capacity;
    
    new_capacity = vector->capacity ? vector->capacity * 2 : 4;
    
    vector->items = (VECTOR_TYPE*) realloc(vector->items, new_capacity * sizeof(VECTOR_TYPE));
    vector->capacity = new_capacity;
}

/**
 * @brief Frees all resources used by a dynamic vector.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure to free.
 * 
 * @details
 * - Frees memory allocated for the `items` array.
 * - Frees the vector structure itself.
 * - Safe to call with a NULL pointer (no operation performed).
 */
void VECTOR_free (VECTOR_NAME *vector) {
    if (vector != NULL) {
        free(vector->items);
        free(vector);
    }
}

/**
 * @brief Retrieves an element from the vector at a given index.
 * 
 * @param[in] vector Pointer to the VECTOR_NAME structure.
 * @param[in] index  Zero-based position of the element to retrieve.
 * 
 * @return The element of type VECTOR_TYPE stored at the specified index.
 * 
 * @note
 * - If the index is out of bounds, the function prints an error message
 *   and terminates the program.
 */
VECTOR_TYPE VECTOR_get (VECTOR_NAME *vector, size_t index) {
    if (index >= vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error in get index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    }
    
    return vector->items[index];
}

/**
 * @brief Sets the value of an element in the vector at a given index.
 *
 * This function's final name is generated by the macro system:
 * it expands to:
 * @code
 * VECTOR_PREFIX(set)
 * @endcode
 * using the @ref VECTOR_IMPL macro.
 *
 * For example, if `VECTOR_PREFIX` is `int_vector_`,
 * the resulting function name becomes:
 * @code
 * int_vector_set
 * @endcode
 *
 * @param[in,out] vector Pointer to the @ref VECTOR_NAME structure.
 * @param[in]     value  New value of type @ref VECTOR_TYPE to assign.
 * @param[in]     index  Zero-based position in the vector to modify.
 *
 * @note
 * - If the index is out of bounds, the function prints an error message
 *   and terminates the program.
 *
 * @see VECTOR_IMPL
 * @see VECTOR_PREFIX
 */
void VECTOR_set (VECTOR_NAME *vector, VECTOR_TYPE value, size_t index) {
    if (index >= vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error in set index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    }
    
    vector->items[index] = value;
}

/**
 * @brief Appends a new element to the end of the vector.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * @param[in]     value  Element of type VECTOR_TYPE to append.
 * 
 * @details 
 * - Automatically expands the vector's capacity if it is full.
 * - Stores the new element at the next available position.
 * - Increments the vector's element count.
 */
void VECTOR_push (VECTOR_NAME *vector, VECTOR_TYPE value) {

    if (vector->count == vector->capacity) {
        VECTOR_expand(vector);
    }
    vector->items[vector->count++] = value;
}

/**
 * @brief Inserts an element into the vector at a specified index.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * @param[in]     value  Element of type VECTOR_TYPE to insert.
 * @param[in]     index  Position in the vector where the element will be inserted (0-based).
 * 
 * @details 
 * - If the index is greater than the current element count, prints an error and exits.
 * - If the index equals the last element position, behaves like VECTOR_push().
 * - Automatically expands the vector if it is full.
 * - Shifts existing elements to the right to make space.
 * - Increments the element count.
 */
void VECTOR_insert (VECTOR_NAME *vector, VECTOR_TYPE value, size_t index) {
    size_t i;

    if (index > vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error insert in index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    } else if (index == vector->count - 1) {
        VECTOR_push(vector, value);
    } else {

        if (vector->count == vector->capacity)
            VECTOR_expand(vector);

        for (i = vector->count; i > index; i -= 1) {
            vector->items[i] = vector->items[i - 1];
        }
        
        vector->items[index] = value;
        vector->count++;
    }
}

/**
 * @brief Removes and returns an element from the vector at the end.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * 
 * @return VECTOR_TYPE The removed element from the vector.
 */
VECTOR_TYPE VECTOR_pop (VECTOR_NAME *vector) {
    if (vector->count == 0) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Cant pop in clear vector\n");
        exit(EXIT_FAILURE);
    }

    vector->count--;
    return vector->items[vector->count];
}

/**
 * @brief Removes and returns an element from the vector at a specific index.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * @param[in] index      Position of the element to remove (0-based).
 * 
 * @details
 * - If the index is out of bounds (>= count), prints an error message and exits.
 * - Shifts all elements after the removed one to fill the gap.
 * - Decreases the total element count by one.
 * 
 * @return VECTOR_TYPE The removed element from the vector.
 */
VECTOR_TYPE VECTOR_remove (VECTOR_NAME *vector, size_t index) {
    VECTOR_TYPE res;
    size_t i;
    
    if (index >= vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error remove in index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    }

    res = vector->items[index];
    
    for (i = index; i < vector->count - 1; i++) {
        vector->items[i] = vector->items[i + 1];
    }
    vector->count--;

    return res;
}

/**
 * @brief Applies a given function to each element of the vector.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * @param[in] func       Function pointer to apply to each element (takes VECTOR_TYPE as argument).
 * 
 * @details
 * Iterates over all elements in the vector and calls `func` for each one.
 */
void VECTOR_foreach (VECTOR_NAME *vector, void (*func)(VECTOR_TYPE value)) {
    size_t i;

    for (i = 0; i < vector->count; i++) {
        func(vector->items[i]);
    }
}

/**
 * @brief Transforms each element of the vector using a provided function.
 * 
 * @param[in,out] vector Pointer to the VECTOR_NAME structure.
 * @param[in] func       Function pointer that takes a VECTOR_TYPE element and returns a transformed VECTOR_TYPE element.
 * 
 * @details
 * Iterates over all elements in the vector, applies `func` to each element,
 * and replaces the original element with the returned value.
 */
void VECTOR_map (VECTOR_NAME *vector, VECTOR_TYPE (*func)(VECTOR_TYPE value)) {
    size_t i;

    for (i = 0; i < vector->count; i++) {
        vector->items[i] = func(vector->items[i]);
    }
}

/**
 * @brief Tests whether any element in the vector satisfies a given predicate.
 * 
 * @param[in] vector    Pointer to the VECTOR_NAME structure.
 * @param[in] predicate Function pointer that takes a VECTOR_TYPE element and returns non-zero if the condition is met.
 * 
 * @return int Returns 1 if at least one element satisfies the predicate, 0 otherwise.
 * 
 * @details
 * Iterates through all elements of the vector and applies `predicate`.
 * Stops at the first element that satisfies the condition.
 */
int VECTOR_any (VECTOR_NAME *vector, int (*predicate)(VECTOR_TYPE value)) {
    int res;
    size_t i;

    res = 0;

    for (i = 0; i < vector->count && res == 0; i++) {
        if (predicate(vector->items[i]))
            res = 1;
    }

    return res;
}

/**
 * @brief Finds the first element in the vector satisfying a predicate.
 * 
 * @param[in] vector        Pointer to the VECTOR_NAME structure.
 * @param[in] predicate     Function pointer that takes a VECTOR_TYPE element and returns non-zero if the condition is met.
 * @param[in] default_value Value to return if no element satisfies the predicate.
 * 
 * @return VECTOR_TYPE Returns the first element that satisfies `predicate`, or `default_value` if none is found.
 * 
 * @details
 * Iterates through the vector elements in order and applies `predicate`.
 * Returns immediately when a matching element is found.
 */
VECTOR_TYPE VECTOR_first_or_default (VECTOR_NAME *vector,
                                     int (*predicate)(VECTOR_TYPE value),
                                     VECTOR_TYPE default_value) {
    VECTOR_TYPE res;
    size_t i;

    i = 0;
    while (i < vector->count && !predicate(vector->items[i])) {
        i++;
    }

    if (i == vector->count)
        res = default_value;
    else
        res = vector->items[i];

    return res;
}

/**
 * @brief Removes all elements from the vector that do not satisfy a given predicate.
 * 
 * @param[in,out] vector    Pointer to the VECTOR_NAME structure to modify.
 * @param[in]     predicate Function pointer that takes a VECTOR_TYPE element and returns non-zero if the element should be kept, 0 if it should be removed.
 * 
 * @details
 * Iterates through all elements of the vector and removes those for which `predicate` returns 0.
 * The vector is modified in-place, and the count of elements is updated accordingly.
 */
void VECTOR_filter (VECTOR_NAME *vector, int (*predicate)(VECTOR_TYPE value)) {
    size_t i, j;

    j = 0;
    for (i = 0; i < vector->count; i++) {

        if (predicate(vector->items[i])) {
            vector->items[j++] = vector->items[i];
        }
    }

    vector->count = j;
}

#undef VECTOR_TYPE
#undef VECTOR_PREFIX
#undef VECTOR_NAME
#undef VECTOR_NAME_STRING
#undef VECTOR_LINKAGE

#undef VECTOR_expand
#undef VECTOR_create
#undef VECTOR_free
#undef VECTOR_get
#undef VECTOR_set
#undef VECTOR_push
#undef VECTOR_insert
#undef VECTOR_pop
#undef VECTOR_remove
#undef VECTOR_foreach
#undef VECTOR_map
#undef VECTOR_any
#undef VECTOR_first_or_default
#undef VECTOR_filter