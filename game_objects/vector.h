/*
  vector.h - Generic dynamic vector implementation in C

  This header provides macros and inline functions to generate
  type-safe dynamic arrays (vectors) for any data type in C.
  It allows creating, freeing, and manipulating vectors with
  functions like push, pop, insert, remove, map, filter, etc.

  Created by Volianskyi Nikita
  Adapted from the article by David Priver:
  https://www.davidpriver.com/ctemplates.html

  All code in this project is released into the public domain
*/

#ifndef VECTOR_H
#define VECTOR_H

/* 
  Inline functions, #defines and includes that will be
  needed for all instantiations can go up here.
*/

#include <stdlib.h>
#include <stdio.h>

/*
    Combines the given prefix with the specified word to
    create a new identifier.

    Used internally to generate function and type names
    based on a configurable prefix (VECTOR_PREFIX).
*/
#define VECTOR_IMPL(word) VECTOR_COMB1(VECTOR_PREFIX, word)
#define VECTOR_COMB1(pre, word) VECTOR_COMB2(pre, word)
#define VECTOR_COMB2(pre, word) pre##word

/*
    Converts a macro argument into a string literal.
*/
#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#endif /* VECOTR_H */



/*
    NOTE: This section is *not* guarded because it is
    intended to be included multiple times — once per
    type definition of the vector.
*/
#ifndef VECTOR_TYPE
#error "VECTOR_TYPE must be defined before including this file"
#endif /* VECTOR_TYPE */


/*
    Defines the name of the vector data structure to be generated.
    If VECTOR_NAME is not predefined, this macro creates a default
    name in the format `vector_<TYPE>`, for example `vector_int`.
*/
#ifndef VECTOR_NAME
#define VECTOR_NAME VECTOR_COMB1(VECTOR_COMB1(vector,_), VECTOR_TYPE)
#endif /* VECTOR_NAME */


#define VECTOR_NAME_STRING STRINGIZE(VECTOR_NAME)

/*
Prefix for generated functions.
If not given generate like VECTOR_NAME_
'vector_int_' for an 'int'
*/
#ifndef VECTOR_PREFIX
#define VECTOR_PREFIX VECTOR_COMB1(VECTOR_NAME, _)
#endif /* VECTOR_PREFIX */


typedef struct VECTOR_NAME VECTOR_NAME;
struct VECTOR_NAME {
    VECTOR_TYPE* items;
    size_t count, capacity;
};


/*
    Expands the allocated memory of a vector when it becomes full.

    input:
        vector - pointer to the vector of type VECTOR_NAME*

    behavior:
        - Doubles the vector's capacity if it already has allocated memory.
        - If the vector is empty, initializes capacity to 4.
        - Reallocates memory to hold the new number of elements.

    output:
        Updates `vector->items` and `vector->capacity` in place.
*/
#define VECTOR_expand VECTOR_IMPL(expand)

static void VECTOR_expand (VECTOR_NAME* vector) {
    size_t new_capacity;
    
    new_capacity = vector->capacity ? vector->capacity * 2 : 4;
    
    vector->items = (VECTOR_TYPE*) realloc(vector->items, new_capacity * sizeof(VECTOR_TYPE));
    vector->capacity = new_capacity;
}

/*
    Creates and initializes a new vector instance.

    behavior:
        - Allocates memory for a new vector structure.
        - Initializes internal fields:
            * items = NULL
            * count = 0
            * capacity = 0
        - The vector will automatically allocate memory
          when the first element is inserted.

    output:
        returns a pointer to the newly created VECTOR_NAME structure
        or NULL if memory allocation fails.
*/
#define VECTOR_create VECTOR_IMPL(create)

VECTOR_NAME* VECTOR_create () {
    VECTOR_NAME* res = malloc(sizeof(VECTOR_NAME));
    
    if (res != NULL) {
        res->items = NULL;
        res->count = 0;
        res->capacity = 0;
    }

    return res;
}

/*
    Frees all memory used by the vector.

    input:
        vector - pointer to the VECTOR_NAME structure to be freed

    behavior:
        - Releases the memory allocated for the vector's items.
        - Frees the vector structure itself.
        - Does nothing if the given pointer is NULL.
*/
#define VECTOR_free VECTOR_IMPL(free)

void VECTOR_free (VECTOR_NAME *vector) {
    if (vector != NULL) {
        free(vector->items);
        free(vector);
    }
}

/*
    Retrieves an element from the vector at a specified index.

    input:
        vector - pointer to the VECTOR_NAME structure
        index  - position of the element to retrieve (0-based)

    behavior:
        - If the index is within bounds, returns the element at that position.
        - If the index is out of bounds, prints an error message and terminates the program.

    output:
        returns the element of type VECTOR_TYPE stored at the given index
*/
#define VECTOR_get VECTOR_IMPL(get)

VECTOR_TYPE VECTOR_get (VECTOR_NAME *vector, size_t index) {
    if (index >= vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error in get index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    }
    
    return vector->items[index];
}

/*
    Sets the value of an element in the vector at a specified index.

    input:
        vector - pointer to the VECTOR_NAME structure
        value  - new value of type VECTOR_TYPE to assign
        index  - position in the vector to modify (0-based)

    behavior:
        - If the index is within bounds, replaces the current value at that position.
        - If the index is out of bounds, prints an error message and terminates the program.
*/
#define VECTOR_set VECTOR_IMPL(set)

void VECTOR_set (VECTOR_NAME *vector, VECTOR_TYPE value, size_t index) {
    if (index >= vector->count) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Error in set index %ld out of bounds\n", index);
        exit(EXIT_FAILURE);
    }
    
    vector->items[index] = value;
}

/*
    Adds a new element to the end of the vector.

    input:
        vector - pointer to the VECTOR_NAME structure
        value  - element of type VECTOR_TYPE to append

    behavior:
        - If the vector is full, automatically expands its capacity.
        - Stores the new value at the next available position.
        - Increments the element count.
*/
#define VECTOR_push VECTOR_IMPL(push)

void VECTOR_push (VECTOR_NAME *vector, VECTOR_TYPE value) {

    if (vector->count == vector->capacity) {
        VECTOR_expand(vector);
    }
    vector->items[vector->count++] = value;
}

/*
    Inserts an element into the vector at the specified index.

    input:
        vector - pointer to the VECTOR_NAME structure
        value  - element of type VECTOR_TYPE to insert
        index  - position where the new element should be placed

    behavior:
        - If the index is greater than the current element count, 
          prints an error message and terminates the program.
        - If the index equals the last element position, 
          the function behaves like VECTOR_push().
        - If the vector is full, expands its capacity.
        - Shifts existing elements to the right from the given index 
          to make room for the new element.
        - Increments the element count.
*/
#define VECTOR_insert VECTOR_IMPL(insert)

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

/*
    Removes and returns the last element from the vector.

    input:
        vector - pointer to the VECTOR_NAME structure

    behavior:
        - If the vector is empty (count == 0), 
          prints an error message and terminates the program.
        - Otherwise, decreases the element count by one 
          and returns the last stored element.

    output:
        returns the last element of type VECTOR_TYPE from the vector
*/
#define VECTOR_pop VECTOR_IMPL(pop)

VECTOR_TYPE VECTOR_pop (VECTOR_NAME *vector) {
    if (vector->count == 0) {
        fprintf(stderr, "Segmentation fault " VECTOR_NAME_STRING " : Cant pop in clear vector\n");
        exit(EXIT_FAILURE);
    }

    vector->count--;
    return vector->items[vector->count];
}

/*
    Removes and returns an element from the vector at a specific index.

    input:
        vector - pointer to the VECTOR_NAME structure
        index  - position of the element to remove (0-based)

    behavior:
        - If the index is out of bounds (>= count), 
          prints an error message and terminates the program.
        - Shifts all elements after the removed one to fill the gap.
        - Decreases the total element count by one.

    output:
        returns the removed element of type VECTOR_TYPE
*/
#define VECTOR_remove VECTOR_IMPL(remove)

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

/*
    Iterates over all elements in the vector and applies a given function.

    input:
        vector - pointer to the VECTOR_NAME structure
        func   - pointer to a function that takes an element of type VECTOR_TYPE
*/
#define VECTOR_foreach VECTOR_IMPL(foreach)

void VECTOR_foreach (VECTOR_NAME *vector, void (*func)(VECTOR_TYPE value)) {
    size_t i;

    for (i = 0; i < vector->count; i++) {
        func(vector->items[i]);
    }
}

/*
    Applies a transformation function to all elements in the vector,
    replacing each element with the result.

    input:
        vector - pointer to the VECTOR_NAME structure
        func   - pointer to a function that takes a VECTOR_TYPE element
                 and returns a transformed VECTOR_TYPE element
*/
#define VECTOR_map VECTOR_IMPL(map)

void VECTOR_map (VECTOR_NAME *vector, VECTOR_TYPE (*func)(VECTOR_TYPE value)) {
    size_t i;

    for (i = 0; i < vector->count; i++) {
        vector->items[i] = func(vector->items[i]);
    }
}

/*
    Checks if at least one element in the vector satisfies a given predicate.

    input:
        vector    - pointer to the VECTOR_NAME structure
        predicate - pointer to a function that takes a VECTOR_TYPE element
                    and returns non-zero if the condition is met, 0 otherwise
*/
#define VECTOR_any VECTOR_IMPL(any)

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

/*
    Returns the first element in the vector that satisfies a given predicate,
    or a default value if no element satisfies it.

    input:
        vector        - pointer to the VECTOR_NAME structure
        predicate     - pointer to a function that takes a VECTOR_TYPE element
                        and returns non-zero if the condition is met, 0 otherwise
        default_value - value to return if no element satisfies the predicate

    output:
        VECTOR_TYPE value from the vector or `default_value` if none found
*/
#define VECTOR_first_or_default VECTOR_IMPL(first_or_default)

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

/*
    Removes all elements from the vector that do not satisfy a given predicate.

    input:
        vector    - pointer to the VECTOR_NAME structure
        predicate - pointer to a function that takes a VECTOR_TYPE element
                    and returns non-zero if the element should be kept,
                    0 if it should be removed
                    
    output:
        modifies the vector in-place, removing elements that do not satisfy the predicate
*/
#define VECTOR_filter VECTOR_IMPL(filter)

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