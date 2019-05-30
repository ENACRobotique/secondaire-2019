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


void PauseState::enter_bis(int angleA, int angleB) {
	Serial.println("Etat pause");
	angles.angleA= angleA;
	angles.angleB = angleB;
	pauseStartTime = millis();
	navigator.forceStop();
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
