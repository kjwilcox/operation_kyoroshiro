#include "defines.h"
#include "kvector.h"


X_y Vector_to_X_y(Vector vec)
{
	X_y temp;
	temp.x = vec.magnitude * cos(vec.heading * (M_PI / 180.0)); // convert to radians for cos()
	temp.y = vec.magnitude * sin(vec.heading * (M_PI / 180.0)); // convert to radians for sin()

	return temp;
}

Vector X_y_to_vector(X_y split)
{
	Vector temp;

	if (is_null_X_y(split))
	{
		temp.magnitude = 0;
		temp.heading = 0;
	}
	else
	{
		temp.magnitude = sqrt(pow(split.x, 2) + pow(split.y, 2));
		temp.heading = atan2(split.y, split.x) * (180.0 / M_PI); // convert to degrees
	}

	return temp;
}

X_y add_X_y(X_y a, X_y b)
{
	X_y temp;
	temp.x = a.x + b.x;
	temp.y = a.y + b.y;

	return temp;
}

Vector add_Vector(Vector a, Vector b)
{
	X_y temp_a;
	X_y temp_b;

	temp_a = Vector_to_X_y(a);
	temp_b = Vector_to_X_y(b);

	return X_y_to_vector(add_X_y(temp_a, temp_b));
}

int is_null_X_y(X_y a)
{
	if (a.x == 0 && a.y == 0)
		return TRUE;
	return FALSE;
}

int is_neg_Vector(Vector v)
{
	if (v.magnitude < 0)
		return TRUE;
	return FALSE;
}
