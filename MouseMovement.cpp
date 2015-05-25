#include "stdafx.h"
#include "MouseMovement.h"
#include <MouseInputDLL.h>




void MouseMovement::leftClickUp() {
	leftUp();
}
void MouseMovement::leftClickDown() {
	leftDown();
}
void MouseMovement::rightClickUp() {
	rightUp();
}

void MouseMovement::rightClickDown() {
	rightDown();
}

void MouseMovement::setMousePosition(int x, int y) {
	setPosition(x, 65665 - y);
}

void MouseMovement::executePenAction(PenAction penAction) {
	switch (penAction) {
		case RIGHT_CLICK_DOWN:
			rightClickDown();
			break;
		case RIGHT_CLICK_UP:
			rightClickUp();
			break;
		case LEFT_CLICK_DOWN:
			leftClickDown();
			break;
		case LEFT_CLICK_UP:
			leftClickUp();
			break;
	}
}