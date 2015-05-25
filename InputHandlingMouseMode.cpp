#include "stdafx.h"
#include <iostream>
#include "InputHandlingMouseMode.h"
#include <array>


std::array<bool, 4> LEFT_CLICK_DOWN_NIBBLE = { 0, 0, 0, 1 };
std::array<bool, 4> LEFT_CLICK_UP_NIBBLE = { 0, 1, 0, 0 };
std::array<bool, 4> RIGHT_CLICK_DOWN_NIBBLE = { 0, 0, 1, 0 };
std::array<bool, 4> RIGHT_CLICK_UP_NIBBLE = { 0, 0, 1, 1 };
std::array<bool, 4> MOUSE_DISCONNECT_NIBBLE = { 0, 0, 0, 0 };
std::array<bool, 4> MOVE_MOUSE_NIBBLE = { 0, 1, 1, 1 };
std::array<bool, 2> CORRECT_START_BITS = { 0, 1 };

//returns true if the 8 Bit limit is reached
bool InputHandlingMouseMode::receiveBit(bool bit) {
	if (!startBitReceived) {
		startBitReceived = !bit;
	}
	if (startBitReceived) {
		byte[bitCounter] = bit;
		if (bitCounter >= 11) {
			bitCounter = 0;
			readFrame();
			lastBit = false;
			startBitReceived = false;
			return true;
		}
		bitCounter++;
	}
	else {
		lastBit = bit;
	}
	return false;
}

bool InputHandlingMouseMode::readFrame() {
	lastBit = byte[0];
	int counter = 0;
	int byteIndex = 0;
	int startBitIndex = 0;
	int index = 0;
	for (bool b : byte) {
		if (b == lastBit && byteIndex < 11) {
			counter++;
		}
		else if (byteIndex == 4) {
			if (translatedBits[0] != CORRECT_START_BITS[0] || translatedBits[1] != CORRECT_START_BITS[1]) {
				return false;
			}
		}
		else {
			if (byteIndex == 11) {
				counter++;
			}
			if (lastBit) {
				counter = (counter - (counter % 2)) / 2;
			}
			else {
				counter = (counter + (counter % 2)) / 2;
			}
			int curIndex = index;
			for (; index < curIndex + counter; ++index) {
				//std::cout << "Index: " << index;
				translatedBits[index] = lastBit;
			}
			if (lastBit && byteIndex % 2 == 1) {
				counter = 1;
			}
			else if (byteIndex % 2 == 1) {
				counter = 0;
			}
			else {
				counter = 1;
			}
		}
		byteIndex++;
		lastBit = b;
	}
	if (translatedBits[0] == CORRECT_START_BITS[0] && translatedBits[1] == CORRECT_START_BITS[1]) {
		for (int i = 2; i < translatedBits.size(); ++i) {
			nibble[i - 2] = translatedBits[i];
		}
		return true;
	}
	nibble = MOVE_MOUSE_NIBBLE;
	return false;
}

PenAction InputHandlingMouseMode::analyzePenAction() {
	for (bool b : nibble) {
		//std::cout << "\n" << "Nibble: " << b << "\n";
	}
	if (nibble == LEFT_CLICK_DOWN_NIBBLE) {
		penAction = LEFT_CLICK_DOWN;
	}
	else if (nibble == LEFT_CLICK_UP_NIBBLE) {
		penAction = LEFT_CLICK_UP;
	}
	else if (nibble == RIGHT_CLICK_DOWN_NIBBLE) {
		penAction = RIGHT_CLICK_DOWN;
	}
	else if (nibble == RIGHT_CLICK_UP_NIBBLE) {
		penAction = RIGHT_CLICK_UP;
	}
	else if (nibble == MOUSE_DISCONNECT_NIBBLE) {
		penAction = MOUSE_DISCONNECT;
	}
	else {
		penAction = MOVE_MOUSE;
	}
	return penAction;
}

PenAction InputHandlingMouseMode::getPenAction() {
	return analyzePenAction();
}



InputHandlingMouseMode::~InputHandlingMouseMode()
{
}