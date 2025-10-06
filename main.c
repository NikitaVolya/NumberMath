#include<stdlib.h>
#include<stdio.h>

#include"vector2i.h"

int math_function(int value) {
    return value * 2 + 3;
}

int main() {
    vector2i vector;

    vector = create_vector2i(10, -2);
    print_vector2i(vector);
    
    vector = foreach_vector2i(vector, math_function);
    print_vector2i(vector);

    exit(EXIT_SUCCESS);
}