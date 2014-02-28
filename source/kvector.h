#ifndef KYLE_VECTOR_H
#define KYLE_VECTOR_H

#include "defines.h"
// msvc needs this, don't know about other compilers
#define _USE_MATH_DEFINES
#include <math.h>

// vector functions
X_y Vector_to_X_y(Vector a);
Vector X_y_to_vector(X_y a);
X_y add_X_y(X_y a, X_y b);
Vector add_Vector(Vector a, Vector b);
int is_null_X_y(X_y a);
int is_neg_Vector(Vector v);

#endif