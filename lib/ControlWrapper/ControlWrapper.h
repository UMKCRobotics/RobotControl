#ifndef CONTROLWRAPPER_H
#define CONTROLWRAPPER_H
#include "Arduino.h"


class ControlWrapper {
	private:
		int deadzone = 12000;
		int min_pwm = 80;
		int max_pwm = 255;
		int leftPWM = 0;
		int rightPWM = 0;
	public:
		long rawLeftY = 0;
		long rawRightY = 0;
		ControlWrapper();
		void updateMotorPWM();
		int getLeftPWM() { return leftPWM; };
		int getRightPWM() { return rightPWM; };
		void setRawLeftY(long val) { rawLeftY = val; };
		void setRawRightY(long val) { rawRightY = val; };
};

class SerialControlWrapper {
	private:

	public:
		SerialControlWrapper() {};


};


#endif
