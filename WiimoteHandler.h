#include "wiimote.h"
#include <vector>
class WiimoteHandler{

private:
	std::vector<wiimote*> wiimotes;
	//void on_state_change(wiimote &remote,
		//state_change_flags  changed,
		//const wiimote_state &new_state);
	//void on_button_state_change(wiimote &remote,
		//state_change_flags  changed,
		//const wiimote_state &new_state);
public:
	void addIRChangeFlags(unsigned wiimotenr);
	void addPresentationFunctions(unsigned);
	void connectFirstWiimote();
	int connectWiimotes();
	void setLEDs(int);
	void setLED(int,unsigned);
	void disconnectWiimotes();
	bool getIRData(unsigned, unsigned, float*);
	void refreshWiimotes();
};