// PCInput.h

#ifdef PCINPUT_EXPORTS
#define PCINPUT_API __declspec(dllexport) 
#else
#define PCINPUT_API __declspec(dllimport) 
#endif

extern "C"{
	 PCINPUT_API void leftArrowDown();
	 PCINPUT_API void leftArrowUp();
	 PCINPUT_API void leftArrowTap();
	 PCINPUT_API void rightArrowDown();
	 PCINPUT_API void rightArrowUp();
	 PCINPUT_API void rightArrowTap();
	 PCINPUT_API void volumeUpDown();
	 PCINPUT_API void volumeUpUp();
	 PCINPUT_API void volumeDownDown();
	 PCINPUT_API void volumeDownUp();
	 PCINPUT_API void spacebarDown();
	 PCINPUT_API void spacebarwUp();
	 PCINPUT_API void spacebarTap();
	 PCINPUT_API void muteTap();
}