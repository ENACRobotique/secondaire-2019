/*
 * RetractArmBeeState.cpp
 *
 *  Created on: 18 avr. 2018
 *      Author: Maxime
 */

#include "RetractArmBeeState.h"

#include "MoveToCubeState.h"
#include "TiretteState.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"

RetractArmBeeState retractArmBeeState = RetractArmBeeState();

RetractArmBeeState::RetractArmBeeState() {
	time_start = 0;

}

RetractArmBeeState::~RetractArmBeeState() {
	// TODO Auto-generated destructor stub
}

void RetractArmBeeState::enter() {
	Serial.println("Etat rangement du bras");
	time_start = millis();
	//arm.write(RETRACTED_ARM);
}

void RetractArmBeeState::leave() {

}

void RetractArmBeeState::doIt() {
	if (millis() - time_start > SERVO_MOVEMENT_DURATION) {
		fsmSupervisor.setNextState(&moveToCubeState);
	}

}

void RetractArmBeeState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	arm.write(170);
}

void RetractArmBeeState::forceLeave(){

}
