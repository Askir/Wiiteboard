#include "stdafx.h"
#include "MorphingController.h"
#include "MouseMovement.h"
#include <iostream>
#include "Perspective.h"
#include "Coord.h"




MorphingController::MorphingController()
{
	quadrangleMorphing = QuadrangleMorphing();
	mouseMovement = MouseMovement();
	inputHandling = InputHandling();
	calibrationRectangle = Rectangle();
}

bool MorphingController::addCalibrationPoint(float x, float y) {
	
	calibrationPoints[counter][0] = (double)x;
	calibrationPoints[counter][1] = (double)y;
	counter++;
	return true;
	/**if (calibrationRectangle.getVector().size() <= 4) {
		calibrationRectangle.addPoint(Point(x * MorphingController::WIIMOTE_CAMERA_WIDTH, y * MorphingController::WIIMOTE_CAMERA_HEIGHT, 0));
		return true;
	}
	return false;**/
}

void MorphingController::resetCalibration() {
	calibrationRectangle = Rectangle();
}

bool MorphingController::deleteLastCalibrationPoint() {
	std::vector<Point> rectanglePoints = calibrationRectangle.getVector();
	if (rectanglePoints.size() > 0) {
		calibrationRectangle.getVector().pop_back();
		return true;
	}
	return false;
}

bool MorphingController::finalCalibration() {

	pers = Perspective(Coord(calibrationPoints[0][0], calibrationPoints[0][1]), Coord(calibrationPoints[1][0], calibrationPoints[1][1]), Coord(calibrationPoints[2][0], calibrationPoints[2][1]), Coord(calibrationPoints[3][0], calibrationPoints[3][1]));
	return true;

	/**
	if (calibrationRectangle.getVector().size() == 4) {
		quadrangleMorphing.setMorphableQuadrangle(calibrationRectangle);
		quadrangleMorphing.startQuadrangleTransformation();
		return true;
	} 
	return false;
	**/

}
PenAction MorphingController::getNewData(bool bitValue) {
	PenAction penAction = NO_ACTION;
	if (inputHandling.receiveBit(bitValue)) {
		penAction = inputHandling.getPenAction();
	}
	return penAction;
}
Coord* MorphingController::getNewIRPoint(float x, float y) {
	Coord coord((double)x, (double)y);
	Coord* point = pers.transform(coord);
	return point;
	/**
	Point point = quadrangleMorphing.startPointTransformation(Point(x * MorphingController::WIIMOTE_CAMERA_WIDTH, y * MorphingController::WIIMOTE_CAMERA_HEIGHT, 0));
	//std::cout << quadrangleMorphing.getFinalSquare().isInsideOf(point);
	if (quadrangleMorphing.getFinalSquare().isInsideOf(point)) {
		return new Point(point.getX() * 65665, point.getY() * 65665, 0);
	}
	else{
		return new Point(-100,-100,0);
	}**/
	
}

void MorphingController::executeMouseAction(Point mousePoint, PenAction penAction) {
	mouseMovement.setMousePosition(mousePoint.getX(), mousePoint.getY());
	mouseMovement.executePenAction(penAction);
}

MorphingController::~MorphingController()
{
}
