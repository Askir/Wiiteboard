#include "InputHandlingTouchMode.h"
#include <math.h>

bool InputHandlingTouchMode::onReceiveIRPoint(Coord* point) {
	double deltaX = abs( point->x - referencedCoord->x );
	double deltaY = abs( point->y - referencedCoord->y );
	if (deltaX < 0.025 && deltaY < 0.025) {
		return true;
	}
	//delete referencedCoord;
	referencedCoord = point;
	resetCounter();
	return false;	
}

PenAction InputHandlingTouchMode::onReceiveData(bool data) {
	if (data) {
		if (receivedDataCounter == 150) {
			isRightClick = true;
		}
		receivedDataCounter++;
	}
	else {
		if (receivecZerosCounter == 3) {
			if (isRightClick) {
				resetCounter();
				return RIGHT_CLICK;
			}
			else {
				resetCounter();
				return LEFT_CLICK_UP;
			}
		}
		receivecZerosCounter++;
	}
	return LEFT_CLICK_DOWN;
}

void InputHandlingTouchMode::resetCounter() {
	receivedDataCounter = 0;
	receivecZerosCounter = 0;
	isRightClick = false;
}

InputHandlingTouchMode::~InputHandlingTouchMode()
{
}
InputHandlingTouchMode::InputHandlingTouchMode()
{
	referencedCoord = new Coord(0, 0);
}

