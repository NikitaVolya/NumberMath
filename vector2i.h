
struct vector2i
{
    int x, y;
};
typedef struct vector2i vector2i;

/* 
    Creates a 2D integer vector.
    input:    x, y - integer coordinates
    output:   vector2i structure with given coordinates
*/
vector2i create_vector2i(int x, int y) {
    vector2i vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

/* 
    Creates a 2D integer vector with both coordinates set to zero.

    input:    none
    output:   vector2i(0, 0)
*/
vector2i create_zero_vector2i() {
    return create_vector2i(0, 0);
}

/* 
    Adds two 2D integer vectors.

    input:    a, b - vectors to add
    output:   sum of the two vectors
*/
vector2i add_vector2i(vector2i a, vector2i b) {
    vector2i rep;
    rep.x = a.x + b.x;
    rep.y = a.y + b.y;
    return rep;
}

/* 
    Subtracts one 2D integer vector from another.

    input:    a, b - vectors to subtract (a - b)
    output:   result of subtraction
*/
vector2i sub_vector2i(vector2i a, vector2i b) {
    vector2i rep;
    rep.x = a.x - b.x;
    rep.y = a.y - b.y;
    return rep;
}

/* 
    Multiplies a 2D integer vector by an integer scalar.

    input:    vector - source vector
              value  - scalar multiplier
    output:   scaled vector
*/
vector2i mult_vector2i(vector2i vector, int value) {
    vector2i rep;
    rep.x = vector.x * value;
    rep.y = vector.y * value;
    return rep;
}

/* 
    Returns a vector with absolute values of both coordinates.

    input:    vector - source vector
    output:   vector with absolute coordinates
*/
vector2i abs_vector2i(vector2i vector) {
    vector2i rep;
    rep.x = (vector.x < 0 ? -vector.x : vector.x);
    rep.y = (vector.y < 0 ? -vector.y : vector.y);
    return rep;
}

/* 
    Applies a function to each component of a 2D integer vector and returns the result.

    input:    vector - source vector
              fptr   - pointer to a function that takes and returns an int
    output:   new vector with transformed components
*/
vector2i foreach_vector2i(vector2i vector, int(*fptr)(int)) {
    vector2i rep;
    rep.x = fptr(vector.x);
    rep.y = fptr(vector.y);
    return rep;
}

/* 
    Prints the 2D integer vector to the console.

    input:    vector - vector to print
    output:   none
*/
void print_vector2i(vector2i vector) {
    printf("vector2i[%d:%d]\n", vector.x, vector.y);
}