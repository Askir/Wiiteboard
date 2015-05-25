#pragma once

#include "PenAction.h"
#include "Coord.h"

class InputHandlingTouchMode
{
public:
	InputHandlingTouchMode() {};
	bool InputHandlingTouchMode::onReceiveIRPoint(Coord* point);
	PenAction InputHandlingTouchMode::onReceiveData(bool data);
	~InputHandlingTouchMode();
private:
	Coord* InputHandlingTouchMode::referencedCoord;
	void InputHandlingTouchMode::resetCounter();
	int InputHandlingTouchMode::receivedDataCounter = 0;
	int InputHandlingTouchMode::receivecZerosCounter = 0;
	bool InputHandlingTouchMode::isRightClick = false;
};


