#include "stdafx.h"
#include <vector>
#include "Rectangle.h"

namespace uschi {

	bool Rectangle::fillVector(std::vector<Point> vector) {
		if (vector.size() != 4) {
			points = vector;
			return true;
		}
		return false;
	}

	std::vector<Point> Rectangle::getVector() {
		return this->points;
	}

	void Rectangle::addPoint(Point point) {
		this->points.push_back(point);
	}

	bool Rectangle::isInsideOf(Point point) {
		if (point.getX() < 0 || point.getY() < 0 ||
			point.getX() > 1 || point.getY() > 1) {
			return false;
		}
		return true;
	}
}