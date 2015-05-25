#ifndef MouseMovement_h
#define MouseMovement_h
#include "PenAction.h"
class MouseMovement
{
public:
	MouseMovement() {};
	void MouseMovement::leftClick();
	void MouseMovement::leftClickUp();
	void MouseMovement::leftClickDown();
	void MouseMovement::rightClick();
	void MouseMovement::rightClickUp();
	void MouseMovement::rightClickDown();
	void MouseMovement::setMousePosition(int x, int y);
	void MouseMovement::executePenAction(PenAction penAction);
	~MouseMovement() {};
};

#endif MouseMovement_h;