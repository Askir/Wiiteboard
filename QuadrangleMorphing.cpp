#include "stdafx.h"
#include "QuadrangleMorphing.h"
#include "Rectangle.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace uschi;

	QuadrangleMorphing::QuadrangleMorphing(Rectangle quadrangle) {
		this->morphableQuadrangle = quadrangle;
	}

	void QuadrangleMorphing::setMorphableQuadrangle(Rectangle quadrangle) {
		this->morphableQuadrangle = quadrangle;
	}

	double QuadrangleMorphing::deformation(double x) {
		Point pointD = quadrangleHistory.at(2).getVector().at(3);
		return (x * (pointD.getY() / pointD.getX()));
	}

	double QuadrangleMorphing::scaleX(double y) {
		Point pointC = quadrangleHistory.at(3).getVector().at(2);
		Point pointD = quadrangleHistory.at(3).getVector().at(3);
		return y * ((pointC.getX() - pointD.getX()) / pointC.getY()) + pointD.getX();
	}

	double QuadrangleMorphing::scaleY(double x) {
		Point pointB = quadrangleHistory.at(3).getVector().at(1);
		Point pointC = quadrangleHistory.at(3).getVector().at(2);
		return x * ((pointC.getY() - pointB.getY()) / pointC.getX()) + pointB.getY();
	}

	void QuadrangleMorphing::addQuadrangle(Rectangle rectangle) {
		this->quadrangleHistory.push_back(rectangle);
	}

	double QuadrangleMorphing::getRotate() {
		Point pointB = quadrangleHistory.at(1).getVector().at(1);
		double root = sqrt(pow(pointB.getX(), 2) + pow(pointB.getY(), 2));
		if (pointB.getY() < 0) {
			return 3.14159265358979323846f - (asin(pointB.getX() / root));
		}
		else {
			return (asin(pointB.getX() / root));
		}
	}



	void QuadrangleMorphing::startQuadrangleTransformation() {
		addQuadrangle(morphableQuadrangle);
		transformation(morphableQuadrangle);
	}

	Point QuadrangleMorphing::startPointTransformation(Point point) {
		return pointTransformation(point);
	}

	Rectangle QuadrangleMorphing::getFinalSquare() {
		return this->finalSquare;
	}


	void QuadrangleMorphing::transformation(Rectangle rectangle) {
		Rectangle newRectangle = Rectangle();
		Point rectanglePointA = rectangle.getVector().at(0);
		Point aPoint = Point();
		for (int i = 0; i < 4; ++i) {
			aPoint.setX(rectangle.getVector().at(i).getX() - rectanglePointA.getX());
			aPoint.setY(rectangle.getVector().at(i).getY() - rectanglePointA.getY());
			newRectangle.addPoint(aPoint);
		}
		addQuadrangle(newRectangle);
		rotation(newRectangle);
	}

	Point QuadrangleMorphing::pointTransformation(Point point) {
		Rectangle rectangle = quadrangleHistory.at(0);
		Point newPoint = Point();
		newPoint.setX(point.getX() - rectangle.getVector().at(0).getX());
		newPoint.setY(point.getY() - rectangle.getVector().at(0).getY());
		return pointRotation(newPoint);
	}

	void QuadrangleMorphing::rotation(Rectangle rectangle) {
		Rectangle newRectangle = Rectangle();
		newRectangle.addPoint(Point(0,0,0));
		Point aPoint = Point();
		double rotate = getRotate();
		for (int i = 1; i < 4; ++i) {
			double x = rectangle.getVector().at(i).getX();
			double y = rectangle.getVector().at(i).getY();
			aPoint.setX(x * cos(rotate) - y * sin(rotate));
			aPoint.setY(y * cos(rotate) + x * sin(rotate));
			newRectangle.addPoint(aPoint);
		}
		addQuadrangle(newRectangle);
		deformation(newRectangle);
	}

	Point QuadrangleMorphing::pointRotation(Point point) {
		Point newPoint = Point();
		double rotate = getRotate();
		newPoint.setX(point.getX() * cos(rotate) - point.getY() * sin(rotate));
		newPoint.setY(point.getY() * cos(rotate) - point.getX() * sin(rotate));
		return pointDeformation(newPoint);
	}

	void QuadrangleMorphing::deformation(Rectangle rectangle) {
		Rectangle newRectangle = Rectangle();
		newRectangle.addPoint(Point(0, 0, 0));
		Point aPoint = Point();
		for (int i = 1; i < 4; ++i) {
			double x = rectangle.getVector().at(i).getX();
			double y = rectangle.getVector().at(i).getY();
			aPoint.setX(x);
			aPoint.setY(y - deformation(x));
			newRectangle.addPoint(aPoint);
		}
		addQuadrangle(newRectangle);
		normalisation(newRectangle);
	}

	Point QuadrangleMorphing::pointDeformation(Point point) {
		Point newPoint = Point();
		newPoint.setX(point.getX());
		newPoint.setY(point.getY() - deformation(point.getX()));
		return pointNormalisation(newPoint);
	}

	void QuadrangleMorphing::normalisation(Rectangle rectangle) {
		Rectangle newRectangle = Rectangle();
		newRectangle.addPoint(Point(0, 0, 0));
		Point aPoint = Point();
		for (int i = 1; i < 4; ++i) {
			double x = rectangle.getVector().at(i).getX();
			double y = rectangle.getVector().at(i).getY();
			aPoint.setX(x / scaleX(y));
			aPoint.setY(y / scaleY(x));
			newRectangle.addPoint(aPoint);
		}
		this->finalSquare = newRectangle;
		addQuadrangle(newRectangle);
	}

	Point QuadrangleMorphing::pointNormalisation(Point point) {
		Point newPoint = Point();
		newPoint.setX(point.getX() / scaleX(point.getY()));
		newPoint.setY(point.getY() / scaleY(point.getX()));
		return newPoint;
	}
