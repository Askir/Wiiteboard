// MouseInput.h

#ifdef MOUSEINPUT_EXPORTS
#define MOUSEINPUT_API __declspec(dllexport) 
#else
#define MOUSEINPUT_API __declspec(dllimport) 
#endif

extern "C"{
	 MOUSEINPUT_API void leftClick();
	 MOUSEINPUT_API void setPosition(int x, int y);
	 MOUSEINPUT_API void doubleLeftClick();
	 MOUSEINPUT_API void rightClick();
	 MOUSEINPUT_API void mouseWheel(int movement);
	 MOUSEINPUT_API void leftDown();
	 MOUSEINPUT_API void leftUp();
	 MOUSEINPUT_API void rightDown();
	 MOUSEINPUT_API void rightUp();
}