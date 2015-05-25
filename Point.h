#ifndef Point_h
#define Point_h

class Point {
	private:
		double Point::x;
		double Point::y;
		double Point::z;
	public:
		Point() { };
		Point::Point(double x, double y, double z);
		void Point::setX(double x);
		double Point::getX();
		void Point::setY(double y);
		double Point::getY();
		void Point::setZ(double z);
		double Point::getZ();
		Point::~Point();
};
#endif
