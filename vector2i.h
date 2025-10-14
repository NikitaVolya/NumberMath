
#ifndef VECTOR2I_H
#define VECTOR2I_H

struct vector2i
{
    int x, y;
};
typedef struct vector2i vector2i;


vector2i create_vector2i(int x, int y);
vector2i create_zero_vector2i();

vector2i add_vector2i(vector2i a, vector2i b);
vector2i sub_vector2i(vector2i a, vector2i b);
vector2i mult_vector2i(vector2i vector, int value);

vector2i abs_vector2i(vector2i vector);

vector2i foreach_vector2i(vector2i vector, int(*fptr)(int));

vector2i get_vector2i_to(vector2i a, vector2i b);
vector2i get_direction(vector2i vector);
vector2i get_direction_to(vector2i a, vector2i b);

#endif