#include "stdafx.h"
#include "Point.h"



	Point::Point(double x, double y, double z)
	{
			this->x = x;
			this->y = y;
			this->z = z;
	}

	void Point::setX(double x) {
		this->x = x;
	}

	double Point::getX() {
		return this->x;
	}

	void Point::setY(double y) {
		this->y = y;
	}

	double Point::getY() {
		return this->y;
	}

	void Point::setZ(double z) {
		this->z = z;
	}

	double Point::getZ() {
		return this->z;
	}

	
	Point::~Point()
	{
	}
