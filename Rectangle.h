#ifndef Rectangle_h
#define Rectangle_h

#include "Point.h"
#include <vector>

namespace uschi {
	class Rectangle
	{
	private:
		std::vector<Point> points;
	public:
		bool Rectangle::fillVector(std::vector<Point> vector);
		void Rectangle::addPoint(Point point);
		std::vector<Point> Rectangle::getVector();
		bool Rectangle::isInsideOf(Point point);
		Rectangle::~Rectangle() { };
	};
}
#endif