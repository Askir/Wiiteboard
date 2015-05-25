#ifndef InputHandling_h
#define InputHandling_h
#include "PenAction.h"
#include <array>

class InputHandling
{
public:
	InputHandling() { };
	bool InputHandling::receiveBit(bool bit);
	PenAction InputHandling::getPenAction();
	PenAction InputHandling::analyzePenAction();
	~InputHandling();
private:
	std::array<bool, 12> InputHandling::byte;
	std::array<bool, 4> InputHandling::nibble;
	std::array<bool, 6> InputHandling::translatedBits;
	bool startBitReceived = false;
	bool lastBit = false;
	int InputHandling::bitCounter = 0;
	bool InputHandling::readFrame();
	PenAction InputHandling::penAction;

};

#endif