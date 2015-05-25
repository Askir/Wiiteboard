#ifndef QuadrangleMorphing_h
#define QuadrangleMorphing_h

#include "Point.h"
#include "Rectangle.h"

using namespace uschi;

class QuadrangleMorphing {
	private:
		uschi::Rectangle QuadrangleMorphing::morphableQuadrangle;
		uschi::Rectangle QuadrangleMorphing::finalSquare;
		std::vector<uschi::Rectangle> QuadrangleMorphing::quadrangleHistory;
		double QuadrangleMorphing::deformation(double x);
		double QuadrangleMorphing::scaleX(double y);
		double QuadrangleMorphing::scaleY(double x);
		void QuadrangleMorphing::addQuadrangle(uschi::Rectangle rectangle);
		double QuadrangleMorphing::getRotate();
		void QuadrangleMorphing::transformation(uschi::Rectangle rectangle);
		Point QuadrangleMorphing::pointTransformation(Point point);
		void QuadrangleMorphing::rotation(uschi::Rectangle rectangle);
		Point QuadrangleMorphing::pointRotation(Point point);
		void QuadrangleMorphing::deformation(uschi::Rectangle rectangle);
		Point QuadrangleMorphing::pointDeformation(Point point);
		void QuadrangleMorphing::normalisation(uschi::Rectangle rectangle);
		Point QuadrangleMorphing::pointNormalisation(Point point);
	public:
		QuadrangleMorphing::QuadrangleMorphing() { };
		QuadrangleMorphing::QuadrangleMorphing(uschi::Rectangle quadrangle);
		uschi::Rectangle QuadrangleMorphing::getFinalSquare();
		void QuadrangleMorphing::setMorphableQuadrangle(uschi::Rectangle quadrangle);
		void QuadrangleMorphing::startQuadrangleTransformation();
		Point QuadrangleMorphing::startPointTransformation(Point point);
		~QuadrangleMorphing() {};
};

#endif