#include "InputHandlingTouchMode.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>

bool InputHandlingTouchMode::onReceiveIRPoint(Coord* point) {
	double deltaX = abs( point->x - referencedCoord->x );
	double deltaY = abs( point->y - referencedCoord->y );
	/**std::cout << "xCoordPoint: " << point->x;
	std::cout << " | yCoordPoint:" << point->y << "\n";
	std::cout << "xCoordReferencedPoint: " << referencedCoord->x;
	std::cout << " | yCoordReferencedPoint:" << referencedCoord->y << "\n";**/
	if (deltaX < 0.025 && deltaY < 0.025) {
		return true;
	}
	//delete referencedCoord;
	referencedCoord->x = point->x;
	referencedCoord->y = point->y;
	receivedDataCounter = 0;
	return false;	
}

PenAction InputHandlingTouchMode::onReceiveData(bool data) {
	if (data) {
		if (receivedDataCounter == 150) {
			isRightClick = true;
		}
		std::cout << "Counter:" << receivedDataCounter << "\n";
		receivedDataCounter++;
		return LEFT_CLICK_DOWN;
	}
	else {
		receivecZerosCounter++;
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
	}
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

