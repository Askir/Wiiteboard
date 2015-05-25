#include "stdafx.h"
#include "Vector.h"

#include <cmath>



Vector::Vector() {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;

	v[3] = 1;
}

Vector::Vector(Coord c) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;

	v[0] = c.x;
	v[1] = c.y;
	v[3] = 1;
}

Vector::Vector(double x, double y, double z) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;

	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector::Vector(double x, double y, double z, bool isPoint) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;

	v[0] = x;
	v[1] = y;
	v[2] = z;
	if (isPoint)
	{
		this->v[3] = 1;
	}
}

Vector::Vector(double x, double y, double z, double w) {
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;

	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}


void Vector::normalize()
{
	double len = length();
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void Vector::scale(double scalar)
{
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;
}

double Vector::length()
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}


Vector* Vector::sum(Vector* vec)
{
	double x, y, z, w;
	x = v[0] + vec->v[0];
	y = v[1] + vec->v[1];
	z = v[2] + vec->v[2];
	w = v[3] + vec->v[3];

	return new Vector(x, y, z, w);
}

double Vector::product(Vector* vec)
{
	double scalar;
	scalar = v[0] * vec->v[0];
	scalar += v[1] * vec->v[1];
	scalar += v[2] * vec->v[2];

	return scalar;
}

double Vector::cos(Vector* vec)
{
	return product(vec) / (length() * vec->length());
}

Vector* Vector::cross(Vector* vec)
{
	double x, y, z;
	x = v[1] * vec->v[2] - v[2] * vec->v[1];
	y = v[2] * vec->v[0] - v[0] * vec->v[2];
	z = v[0] * vec->v[1] - v[1] * vec->v[0];

	return new Vector(x, y, z);
}

Vector* Vector::construct(Vector* end)
{
	double x, y, z;
	x = end->v[0] - v[0];
	y = end->v[1] - v[1];
	z = end->v[2] - v[2];

	return new Vector(x, y, z);
}