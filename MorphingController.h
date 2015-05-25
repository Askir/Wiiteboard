#ifndef MorphingController_h
#define MorphingController_h

#include "QuadrangleMorphing.h"
#include "MouseMovement.h"
#include "InputHandling.h"
#include "Rectangle.h"
#include "Point.h"
#include <cmath>
#include "Perspective.h"
#include "Coord.h"

class MorphingController
{
public:
	unsigned MorphingController::counter = 0;
	MorphingController();
	bool MorphingController::addCalibrationPoint(float x, float y);
	bool MorphingController::finalCalibration();
	bool MorphingController::deleteLastCalibrationPoint();
	void MorphingController::resetCalibration();
	PenAction MorphingController::getNewData(bool bitValue);
	Coord* MorphingController::getNewIRPoint(float x, float y);
	static const int WIIMOTE_CAMERA_HEIGHT = 768;
	static const int WIIMOTE_CAMERA_WIDTH = 1024;
	double calibrationPoints[4][2];
	Perspective pers;
	~MorphingController();
private:
	QuadrangleMorphing MorphingController::quadrangleMorphing;
	MouseMovement MorphingController::mouseMovement;
	void MorphingController::executeMouseAction(Point mousePoint, PenAction penAction);
	InputHandling MorphingController::inputHandling;
	uschi::Rectangle MorphingController::calibrationRectangle;
	std::vector<int> MorphingController::lastBitValues;
	void doMouseAction(Point point, PenAction penAction);
	Point MorphingController::lastPoint;
};

#endif