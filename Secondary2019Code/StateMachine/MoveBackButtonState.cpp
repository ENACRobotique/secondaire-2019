/*
 * MoveBackButtonState.cpp
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#include "MoveBackButtonState.h"
#include "ThrowState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"

MoveBackButtonState moveBackButtonState = MoveBackButtonState();

MoveBackButtonState::MoveBackButtonState() {
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
}

MoveBackButtonState::~MoveBackButtonState() {
	// TODO Auto-generated destructor stub
}

void MoveBackButtonState::enter() {
	Serial.println("Etat déplacement arrière depuis la zone de construction");
	navigator.move_to(400,0);
	time_start = millis();
	if(navigator.moveForward()){
		Serial.println("Forward");
		usDistances.front_left = 30;
		usDistances.front_right = 30;
		usDistances.rear_left = 0;
		usDistances.rear_right = 0;
	}
	else{
		Serial.println("Backwards");
		usDistances.front_left = 0;
		usDistances.front_right = 0;
		usDistances.rear_left = 30;
		usDistances.rear_right = 30;
	}
	usManager.setMinRange(&usDistances);
}

void MoveBackButtonState::leave() {

}

void MoveBackButtonState::doIt() {
	if(navigator.isTrajectoryFinished()){
		fsmSupervisor.setNextState(&throwState);
	}

}

void MoveBackButtonState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	navigator.move_to(400,0);
}

void MoveBackButtonState::forceLeave(){

}
