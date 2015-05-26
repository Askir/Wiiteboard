// Wiiteboard.cpp : Defines the entry point for the console application.
//Author: mainly Jascha Beste (ca. 90%)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "QuadrangleMorphing.h"
#include "Point.h"
#include <windows.h>
#include "Rectangle.h"
#include "MouseInputDLL.h"
#include "MorphingController.h"
#include "PenAction.h"
#include <array>
#include "InputHandlingTouchMode.h"
#include <random>
#include "WiimoteHandler.h"
#include <conio.h>
#include "MouseMovement.h"
#include "WiimoteMode.h"
//#include "Matrix.h"

//Initializing global variables for further use
WiimoteHandler wh; //The Handler for all wiimotes
unsigned connected; //A global unsigned int for the connected wiimotes (will be set later max. is 8 atm.)
MorphingController morphcon[8]; //Initializing a morphcontroller for every possible Wiimote.
MouseMovement mouse; //The mousemovement object use to set the mouseposition. THis is actually just an interface to a DLL 
bool cameraenabled = false; //global setting to see if the whiteboard / camera function is even enabled (the programm skips a lot of unnesseccary information if it is disabled)
WiimoteMode modes[8] = { DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, DISABLED_MODE, }; //An Array of all possible Wiimote Modes set to DISABLED at default
bool presentationenabled = false;
CRITICAL_SECTION cs;

//The callback funtion for the wiiteboard is called every tick (parameters are necessary because of the used API but are unused)
void CALLBACK intervallMouseControl(
	_In_ PVOID   lpParameter,
	_In_ BOOLEAN TimerOrWaitFired
	){
	
	
	EnterCriticalSection(&cs);
	//every printf is debug code atm.
	//_tprintf(_T("------TICK START------\n"));
	DWORD tID = GetCurrentThreadId();
	//_tprintf(_T("%d \n threadID \n"), tID);
	//Matrix* m = new Matrix();
	//refreshing wiimote data every Tick to get the newest Data
	wh.refreshWiimotes();
	//Initializing some needed Arrays for later us
	static Coord* lastPoints[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	static int lastPointcounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	Coord* seenPoints[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	PenAction action[8] = { NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION, NO_ACTION };
	//getting the IRDATA from every single camera mode wiimote and processing it with the linked morphcontroller
	for (unsigned i = 0; i < connected; i++){
		if (modes[i] == CAMERA_MODE){
			bool visible = false;
			float data[2];
			int k = 0;
			while (!visible && k < 4){
				visible = wh.getIRData(i, k, data);
				k++;
			}
			action[i] = morphcon[i].getNewData(visible);
			if (visible){
				
				//_tprintf(_T("Wiimotenr: %i seenPoint x,y %f %f \n"),i, data[0], data[1]);
				seenPoints[i] = (morphcon[i].getNewIRPoint(data[0], data[1]));
				//_tprintf(_T("Wiimotenr: %i calculated Point, X: %.6f, Y: %.6f \n"), i, seenPoints[i]->getX(), seenPoints[i]->getY());
				seenPoints[i]->x = seenPoints[i]->x * 65635;
				seenPoints[i]->y = seenPoints[i]->y * 65635;
			}
			_tprintf(_T("Wiimotenr: %i has seen? %d \n"),i, visible);
		}
	}
	//selfexplaining variable initializing. Why it is here and not earlier in the code? - I don't know I just felt that way
	static int lastPointNr = 0;
	int pointNr=0;
	int xValue=0;
	int yValue=0;
	double maxValue = 65635;
	double minValue = 0;

	//Adding the single IRValues together to get the average value later


	for (int i = 0; i < 8; i++){
		
		if ((seenPoints[i] != NULL) && (seenPoints[i]->x >= minValue) && (seenPoints[i]->x <= maxValue) && (seenPoints[i]->x >= minValue) && (seenPoints[i]->x <= maxValue)) {
			pointNr++;
		//	_tprintf(_T("ADDED 1 to pointNR, pointNR is now %i \n"),pointNr);
			xValue += seenPoints[i]->x;
			yValue += seenPoints[i]->y;
		}
		else{
			//if (lastPoints[i] != NULL){
				lastPointcounter[i] += 1;
			//}
			if ((lastPoints[i] != NULL) && (lastPoints[i]->x >= minValue) && (lastPoints[i]->x <= maxValue) && (lastPoints[i]->x >= minValue) && (lastPoints[i]->x <= maxValue) && lastPointcounter[i] < 4){
				pointNr++;
				xValue += lastPoints[i]->x;
				yValue += lastPoints[i]->y;

			}
		}
	}
	//setting the postiong to the average calculated value
	/**
	TODO:
	ADD calculation of last value of the other wiimotes if their data is missing for this tick.
	**/
	if (pointNr > 0){
			//_tprintf(_T("seenPoint %i %i \n" ), xValue/pointNr,yValue/pointNr);
			mouse.setMousePosition(xValue / pointNr, yValue / pointNr);
			
	}
	else{
		//_tprintf(_T("DID NOT SET MOUSEPOSITION"));
	}
	//executing the calculated PenAction if necessary
	for (unsigned i = 0; i < connected; i++){
		if (action[i] != NO_ACTION){
			if (action[i] != MOUSE_DISCONNECT){
				//mouse.executePenAction(action[i]);
				break;
			}
		}
	}
	//deleting the seenPOints to prevent a memory leak
	for (int i=0; i < 8; i++){
		if (lastPointcounter[i]>3){
			delete lastPoints[i];
			lastPoints[i] = NULL;
			lastPointcounter[i] = 0;
		}
		if (seenPoints[i] != NULL){
			delete lastPoints[i];
			lastPoints[i] = seenPoints[i];
			lastPointcounter[i] = 0;
		}
		else{
			delete seenPoints[i];
			seenPoints[i] = NULL;
		}
		
	}
	
	LeaveCriticalSection(&cs);
//	_tprintf(_T("------TICK END------\n"));
}

//Just using a nice Winows API function to call the earlier described function every 10 ms
void startMouseControl(){
	HANDLE hTQ = CreateTimerQueue();            
	if (hTQ == NULL){
		printf("%x\n", GetLastError());
	}
	else{
		HANDLE hnewTimer;
		CreateTimerQueueTimer(&hnewTimer,
			hTQ,
			&intervallMouseControl,
			NULL,
			0, 
			10,
			WT_EXECUTEDEFAULT);
		std::cout << " press a to abort! ";
		char input = _getch();
		if (input == 'a'){
			DeleteTimerQueueTimer(hTQ, hnewTimer, NULL);
			DeleteTimerQueueEx(hTQ, NULL);
		}
	}

}



int _tmain(int argc, _TCHAR* argv[])
{

	
	InitializeCriticalSection(&cs);
	//connecting all available wiimotes they have to be connected with bluetooth before the start of the programm (i might change this later but i am to lazy/busy atm)
	connected = wh.connectWiimotes();
	_tprintf(_T("%u Wiimotes connected \n"), connected);
/** wh.addPresentationFunctions(0);
	while (true){
		wh.refreshWiimotes();
		static float data[2];
		float data2[2];
		wh.getIRData(0, 0,data2);
		if (data[0]!=data2[0])
			_tprintf(_T("%f,%f \n"), data2[0], data2[1]);
		data[0] = data2[0];
		data[1] = data2[1];
	} **/
	//setting Wiimote Mode and getting calibration data. This is functional but very ugly atm. gonna change it ASAP.
	for (unsigned i = 0; i < connected; i++){
		wh.setLED(i, i);
		bool defined = false;
		while (!defined){
			_tprintf(_T("Choose mode for wiimote Number %u \n"), i);
			_tprintf(_T("[w] for wiitboard camera \n[p} for presentation tool \n"));
			char input = _getch();
			if (input == 'w'){
				modes[i] = CAMERA_MODE;
				cameraenabled = true;
				defined = true;
			}
			else if (input == 'p'){
				modes[i] = PRESENTATION_MODE;
				defined = true;
				presentationenabled = true;
			}
			else{
				_tprintf(_T("Wrong Button try again:"));
			}
		}

	}
	//adding presentationfunctions to choosen wiimotes
	if (presentationenabled){
		for (int i = 0; i < 8; i++){
			if (modes[i] == PRESENTATION_MODE){
				wh.addPresentationFunctions(i);
			}
		}
	}
restart:

	if (cameraenabled){
		
		for (int i = 0; i < 4; i++){
restartPoint:
			_tprintf(_T("Please tap int the Corner Number %i \n"), i+1);
		
			
			for (unsigned i2 = 0; i2 < connected; i2++){
				if (modes[i2] == CAMERA_MODE){
					bool visible = false;
					float data[2];
					while (!visible){
						wh.refreshWiimotes();
						visible = wh.getIRData(i2, 0, data);
						if (visible){
							morphcon[i2].addCalibrationPoint(data[0], data[1]);
							_tprintf(_T("Found point %i for wiimote number: %u \n"), i + 1,i2);
							if (i == 3){
								morphcon[i2].finalCalibration();
							}
						}
					}
				}
				
			}
wrongButton:
			_tprintf(_T("Press 'r' to restart the progress \n Press 'd' to do the last point again \n Press 'a' if you are happy with the result \n"));
			char input = _getch();
			switch (input){
			case 'r':
				for (int i = 0; i < 8; i++){
					if (modes[i] == CAMERA_MODE){
						morphcon[i].resetCalibration();
					}
				}
				goto restart;
				break;
			case 'd':
				for (int i = 0; i < 8; i++){
					if (modes[i] == CAMERA_MODE){
						morphcon[i].deleteLastCalibrationPoint();
					}
				}
				goto restartPoint;
				break;
			case 'a':
				_tprintf(_T("Point %i validated \n"), i+1);
				break;
			default :
				goto wrongButton;
			}
			//waiting a second so windows doesn't explode and no point does get interpretated twice or even more
			Sleep(1000);
		}
		//starting the mosuecontroll
		startMouseControl();
	}
	
	//while (true){
		//wh.refreshWiimotes();
	//}
	//just some test code down here. Delete it if you want to

	/**_tprintf(_T("to start calibrating press 'c'"));
	char input = _getch();
	if (input == 'c'){
		bool visible = false;
		while (!visible){
			visible = wh.getIRData
		}
	}**/
	/**while (true){
		float data[2];
		wh.getIRData(0, 0, data);
	}**/

	/**MorphingController morphCon = MorphingController();
	morphCon.addCalibrationPoint(0.0, 0.0);
	morphCon.addCalibrationPoint(0.0, 1.0);
	morphCon.addCalibrationPoint(1.0, 1.0);
	morphCon.addCalibrationPoint(1.0, 0.0);
	morphCon.finalCalibration();
	std::array<bool, 8> leftClickDown = { 0, 0, 0, 0, 0, 1, 1, 1 };
	//{0, 0, 0, 1}
	std::array<bool, 8> leftClickUp = { 0, 1, 1, 1, 0, 0, 0, 0 };
	//{0, 1, 0, 0}
	std::array<bool, 8> rightClickDown = { 0, 0, 0, 1, 1, 1, 0, 0 };
	//{0, 0, 1, 0}
	std::array<bool, 8> rightClickUp = { 0, 0, 0, 1, 1, 1, 1, 1 };
	//{0, 0, 1, 1}
	float x;
	float y;
	for (int i = 0; i < 15; ++i) {
		x = rand() / (float) RAND_MAX;
		std::cout << "X: " << x << "\n";
		y = rand() / (float) RAND_MAX;
		std::cout << "Y: " << y << "\n";
		morphCon.getNewIRPoint(x, y);
		for (int i = 0; i < 8; ++i) {
			std::cout << "RightClickDown@i -> " << rightClickDown.at(i) << "\n";
			morphCon.getNewData(rightClickDown.at(i));
		}
		Sleep(10);
		for (int j = 0; j < 8; ++j) {
			std::cout << "RightClickUp@i -> " << rightClickUp.at(j) << "\n";
			morphCon.getNewData(rightClickUp.at(j));
		}
		Sleep(1000);
		morphCon.getNewIRPoint(x, y);
		for (int i = 0; i < 8; ++i) {
			std::cout << "LeftClickDown@i -> " << leftClickDown.at(i) << "\n";
			morphCon.getNewData(leftClickDown.at(i));
		}
		Sleep(10);
		for (int i = 0; i < 8; ++i) {
			std::cout << "LeftClickUp@i -> " << leftClickUp.at(i) << "\n";
			morphCon.getNewData(leftClickUp.at(i));
		}
		Sleep(1000);
	}
	**\
	/**	InputHandling input = InputHandling();
	std::array<bool, 12> test = { 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1 };
	for (bool b : test) {
	if (input.receiveBit(b)) {
	std::cout << "InputHandling -> PenAction: " << input.analyzePenAction() << "\n";
	}
	} **/


	/**	PenAction penA = LEFT_CLICK_DOWN;
	if (penA == LEFT_CLICK_DOWN) {
	penA = MOUSE_DISCONNECT;
	}
	std::cout << penA;
	Point pointA = Point(27, 18, 0);
	Point pointB = Point(25, 381, 0);
	Point pointC = Point(1023, 50, 0);
	Point pointD = Point(700, 1, 0);
	uschi::Rectangle rectangle = uschi::Rectangle();
	rectangle.addPoint(pointA);
	rectangle.addPoint(pointB);
	rectangle.addPoint(pointC);
	rectangle.addPoint(pointD);
	QuadrangleMorphing morphingOfD00M = QuadrangleMorphing(rectangle);
	morphingOfD00M.startQuadrangleTransformation();
	std::cout << morphingOfD00M.getFinalSquare().getVector().at(2).getX();
	Point point = morphingOfD00M.startPointTransformation(Point(500, 19, 0));
	setPosition(point.getX() * 65565, (1 - point.getY()) * 65565);
	std::cout << point.getX() << " " << point.getY(); **/
	Sleep(300000);
	_CrtDumpMemoryLeaks();
}


