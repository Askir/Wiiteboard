#ifndef Vector_h
#define Vector_h

#include "Coord.h"


class Vector {
	friend class Matrix;
	friend class Perspective;

	private:
		double Vector::v[4];

		Vector::Vector(double x, double y, double z, double w);

	public:
		Vector::Vector();
		Vector::Vector(Coord c);
		Vector::Vector(double x, double y, double z);
		Vector::Vector(double x, double y, double z, bool isPoint);

		double Vector::length();
		void Vector::normalize();
		void Vector::scale(double scalar);

		double Vector::cos(Vector* vec);
		Vector* Vector::sum(Vector* vec);
		double Vector::product(Vector* vec);
		Vector* Vector::cross(Vector* vec);
		Vector* Vector::construct(Vector* end);
};
#endif