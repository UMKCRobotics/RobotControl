#include "ControlWrapper.h"


ControlWrapper::ControlWrapper() {

}

void ControlWrapper::updateMotorPWM() {
	if (rawLeftY == 0)
		leftPWM = 0;
	else {
		leftPWM = map(abs(rawLeftY),deadzone,32768,min_pwm,max_pwm);
		if (rawLeftY < 0)
			leftPWM *= -1;
	}
	if (rawRightY == 0)
		rightPWM = 0;
	else {
		rightPWM = map(abs(rawRightY),deadzone,32768,min_pwm,max_pwm);
		if (rawRightY < 0)
			rightPWM *= -1;
	}
}