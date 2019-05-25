/*
 * PauseState.cpp
 *
 *  Created on: 19 avr. 2018
 *      Author: robot
 */

#include "PauseState.h"
#include "Arduino.h"
#include "../Navigator.h"

PauseState pauseState = PauseState();
PauseState::PauseState() {
	pauseStartTime = 0;
	flags = E_ULTRASOUND;
}

PauseState::~PauseState() {
	// TODO Auto-generated destructor stub
}

void PauseState::doIt() {
}

void PauseState::leave() {
}

void PauseState::enter() {
	Serial.println("Etat pause");
	pauseStartTime = millis();
	navigator.forceStop();
}

void PauseState::reEnter(unsigned long interruptTime) {
}

void PauseState::forceLeave() {
}

unsigned long PauseState::getPauseTime() {
	return millis() - pauseStartTime;
}
