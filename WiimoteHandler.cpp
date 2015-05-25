#include "WiimoteHandler.h"
#include "wiimote.h"
#include "PCInputDLL.h"


//warning a lot of test code in this .cpp dont touch anything im glad that it works ^^
//Author: Jascha Beste
//Most code is copied out of the Demo.cpp from WiiYourself


//this function is entirely copied from WiiYourself i don't know if i need it later so I'll just leave it in although it is disabled atm.
	void on_state_change(wiimote &remote,
		state_change_flags  changed,
		const wiimote_state &new_state)
	{
		// we use this callback to set report types etc. to respond to key events
		//  (like the wiimote connecting or extensions (dis)connecting).

		// NOTE: don't access the public state from the 'remote' object here, as it will
		//		  be out-of-date (it's only updated via RefreshState() calls, and these
		//		  are reserved for the main application so it can be sure the values
		//		  stay consistent between calls).  Instead query 'new_state' only.

		// the wiimote just connected
		if (changed & CONNECTED)
		{
			// ask the wiimote to report everything (using the 'non-continous updates'
			//  default mode - updates will be frequent anyway due to the acceleration/IR
			//  values changing):

			// note1: you don't need to set a report type for Balance Boards - the
			//		   library does it automatically.

			// note2: for wiimotes, the report mode that includes the extension data
			//		   unfortunately only reports the 'BASIC' IR info (ie. no dot sizes),
			//		   so let's choose the best mode based on the extension status:
			if (new_state.ExtensionType != wiimote::BALANCE_BOARD)
			{
				if (new_state.bExtension)
					remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
				else
					remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots
			}
		}
		// a MotionPlus was detected
		if (changed & MOTIONPLUS_DETECTED)
		{
			// enable it if there isn't a normal extension plugged into it
			// (MotionPlus devices don't report like normal extensions until
			//  enabled - and then, other extensions attached to it will no longer be
			//  reported (so disable the M+ when you want to access them again).
			if (remote.ExtensionType == wiimote_state::NONE) {
				bool res = remote.EnableMotionPlus();
				_ASSERT(res);
			}
		}
		// an extension is connected to the MotionPlus
		else if (changed & MOTIONPLUS_EXTENSION_CONNECTED)
		{
			// We can't read it if the MotionPlus is currently enabled, so disable it:
			if (remote.MotionPlusEnabled())
				remote.DisableMotionPlus();
		}
		// an extension disconnected from the MotionPlus
		else if (changed & MOTIONPLUS_EXTENSION_DISCONNECTED)
		{
			// enable the MotionPlus data again:
			if (remote.MotionPlusConnected())
				remote.EnableMotionPlus();
		}
		// another extension was just connected:
		else if (changed & EXTENSION_CONNECTED)
		{

			// switch to a report mode that includes the extension data (we will
			//  loose the IR dot sizes)
			// note: there is no need to set report types for a Balance Board.
			if (!remote.IsBalanceBoard())
				remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
		}
		// extension was just disconnected:
		else if (changed & EXTENSION_DISCONNECTED)
		{

			// use a non-extension report mode (this gives us back the IR dot sizes)
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
		}
	} 

	//button state change methond a lot of copy and paste also using the original authors comments.
	//This methid basically maps wiimote keys to useful presentation function

	void on_IR_change(wiimote &remote, state_change_flags changed, const wiimote_state &new_state){

		remote.RefreshState();
		wiimote_state::ir::dot &dot = remote.IR.Dot[0];
		_tprintf(_T("IR STATE %d"), dot.bVisible);

	}
	void on_button_state_change(wiimote &remote,
	state_change_flags  changed,
	const wiimote_state &new_state){
	remote.RefreshState();
	static bool last_A = false, last_B = false, last_Two = false, last_One = false, last_Plus, last_Minus;
		bool pressedA = remote.Button.A();						
			if (pressedA){ /* just pressed? */						
				if (!last_A) rightArrowTap();
			}
			else if (last_A){ /* just released */
				//_tprintf(_T("A not pressed anymore"));
			}
			/* remember the current button state for next time */			
			last_A = pressedA;

		bool pressedB = remote.Button.B();
			if (pressedB){ /* just pressed? */
				if (!last_B) leftArrowTap();
			}
			else if (last_B){ /* just released */
				//_tprintf(_T("B not pressed anymore"));
			}
			/* remember the current button state for next time */
			last_B = pressedB;

		bool pressedOne = remote.Button.One();
			if (pressedOne){ /* just pressed? */
				if (!last_One) {
					volumeUpDown();
					volumeUpUp();
				}
			}
			else if (last_One){ /* just released */
			}
			/* remember the current button state for next time */
			last_One = pressedOne;

		bool pressedTwo = remote.Button.Two();
			if (pressedTwo){ /* just pressed? */
				if (!last_Two){ 
					volumeDownDown(); 
					volumeDownUp();
				}
			}
			else if (last_Two){ /* just released */
			}
			/* remember the current button state for next time */
			last_Two = pressedTwo;
		bool pressedPlus = remote.Button.Plus();
			if (pressedPlus){ /* just pressed? */
				if (!last_Plus) spacebarTap();
			}
			else if (last_Plus){ /* just released */
			}
			/* remember the current button state for next time */
			last_Plus = pressedPlus;
		bool pressedMinus = remote.Button.Minus();
			if (pressedMinus){ /* just pressed? */
				if (!last_Minus)muteTap();
			}
			else if (last_Minus){ /* just released */
			}
			/* remember the current button state for next time */
			last_Minus = pressedMinus;
}

	//adding the callback for Presentationfunctions
	void WiimoteHandler::addIRChangeFlags(unsigned wiimotenr){
		wiimotes[wiimotenr]->ChangedCallback = on_button_state_change;
		wiimotes[wiimotenr]->CallbackTriggerFlags = (state_change_flags)(IR_CHANGED);
	}
	void WiimoteHandler::addPresentationFunctions(unsigned wiimotenr){
		wiimotes[wiimotenr]->ChangedCallback = on_button_state_change;
		wiimotes[wiimotenr]->CallbackTriggerFlags = (state_change_flags)(BUTTONS_CHANGED);

	}
	//my first test function i still use it for testing the WiiYourself methods
	void WiimoteHandler::connectFirstWiimote(){

		wiimote remote;
		//remote.ChangedCallback = &on_state_change;
		//remote.CallbackTriggerFlags = (state_change_flags)(CONNECTED |
		//EXTENSION_CHANGED |
		//MOTIONPLUS_CHANGED);
		while (!remote.Connect(wiimote::FIRST_AVAILABLE)) {
			Sleep(1000);
		}
		printf("Succeeded!\n");
		remote.SetLEDs(0x0f);
		//for (unsigned i = 0; i < 5; i++){
		//_tprintf(_T("%u\n"),i);
		//}
		//for (unsigned i = 0; i < 10000; i++)
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
		while (true){
			/**_tprintf(_T("       IR:"));

			_tprintf(_T("  Mode %s  "),
			((remote.IR.Mode == wiimote_state::ir::OFF) ? _T("OFF  ") :
			(remote.IR.Mode == wiimote_state::ir::BASIC) ? _T("BASIC") :
			(remote.IR.Mode == wiimote_state::ir::EXTENDED) ? _T("EXT. ") :
			_T("FULL ")));
			// IR dot sizes are only reported in EXTENDED IR mode (FULL isn't supported yet)
			bool dot_sizes = (remote.IR.Mode == wiimote_state::ir::EXTENDED);

			for (unsigned index = 0; index < 4; index++)
			{
			wiimote_state::ir::dot &dot = remote.IR.Dot[index];

			if (!remote.IsBalanceBoard())
			_tprintf(_T("%u: "), index);

			if (dot.bVisible) {
			_tprintf(_T("Seen       "));
			}
			else{
			_tprintf(_T("Not seen   "));
			}

			_tprintf(_T("Size"));
			if (dot_sizes)
			_tprintf(_T("%3d "), dot.Size);
			else{
			_tprintf(_T(" n/a"));
			}

			_tprintf(_T("  X %.3f  Y %.3f\n"), dot.X, dot.Y);

			if (index < 3)
			_tprintf(_T("                        "));
			}**/
			remote.RefreshState();
			for (unsigned index = 0; index < 4; index++)
			{
				wiimote_state::ir::dot &dot = remote.IR.Dot[index];

				if (dot.bVisible){
					printf("yey");
					_tprintf(_T("  X %.3f  Y %.3f\n"), dot.X, dot.Y);

					//if (index < 3)
					//	_tprintf(_T("                        "));
				}
				else{
					//_tprintf(_T("Dot %u is not visible \n"), index);
				}

			}
			Sleep(10);

		}
		Sleep(10000);
	}

	//connecting all Wiimotes and saving them in an Arayy filled with pointers
	int WiimoteHandler::connectWiimotes(){
		unsigned detected = 0;
		while (detected < 8)
		{
			wiimote *next = new wiimote;
			if (!next->Connect(wiimote::FIRST_AVAILABLE))
				break;
			detected++;
			next->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
			wiimotes.push_back(next);
		}
		return detected;
	}
	//setting all LEDs to a sepcific value
	void WiimoteHandler::setLEDs(int on){
		for (unsigned i = 0; i < wiimotes.size();i++){
			wiimotes[i]->SetLEDs(on);
		}
	}
	//setting the LED of one specific Wiimote to a specific value
	void WiimoteHandler::setLED(int on, unsigned wiimote){
		wiimotes[wiimote]->SetLEDs(on);
	}
	//disconnect the wiimote. This function is not used atm. but might be usefull in the future if we try to expand the project
	void WiimoteHandler::disconnectWiimotes(){
		for (int i = 0; i < wiimotes.size();i++){
			wiimotes[i]->Disconnect();
			delete wiimotes[i];
			wiimotes[i] = NULL;
		}
		wiimotes.clear();
	}
	//Funtion used to get the WiimoteData self explaining I guess.
	bool WiimoteHandler::getIRData(unsigned wiimoteNo, unsigned dotNo, float* point){
		if (wiimotes.size()>wiimoteNo){
			wiimote_state::ir::dot &dot = wiimotes[wiimoteNo]->IR.Dot[dotNo];
			if (dot.bVisible){
				point[0] = dot.X;
				point[1] = dot.Y;
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
	//WOW you put a for loop into a function congratz Jascha :)
	void WiimoteHandler::refreshWiimotes(){
		for (int i = 0; i < wiimotes.size(); i++){
			wiimotes[i]->RefreshState();
		}
	}


