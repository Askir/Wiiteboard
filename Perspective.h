#ifndef Perspective_h
#define Perspective_h

#include "Coord.h"
#include "Vector.h"
#include "Matrix.h"


class Perspective {
	private:
		double Perspective::fov;
		Vector* Perspective::view;
		Matrix* Perspective::morph;

		void Perspective::calibrate(Vector* a, Vector* b, Vector* c, Vector* d);
		Vector* Perspective::calcCenter(Vector* a, Vector* b, Vector* c, Vector* d);

	public:
		Perspective::Perspective();
		Perspective::Perspective(Coord a, Coord b, Coord c, Coord d);
		void Perspective::calibrate(Coord a, Coord b, Coord c, Coord d);
		Coord* Perspective::transform(Coord pen);
		Perspective::~Perspective();
		
};
#endif