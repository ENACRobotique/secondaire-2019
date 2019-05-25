/*
 * RetractArmButtonState.cpp
 *
 *  Created on: 18 avr. 2018
 *      Author: Maxime
 */

#include "RetractArmButtonState.h"
#include "TiretteState.h"
#include "DeadState.h"
#include "MoveToCubeState.h"
#include "TiretteState.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../Navigator.h"
#include "../lib/USManager.h"

RetractArmButtonState retractArmButtonState = RetractArmButtonState();

RetractArmButtonState::RetractArmButtonState() {
	time_start = 0;
	time_servo = 0;
}

RetractArmButtonState::~RetractArmButtonState() {
	// TODO Auto-generated destructor stub
}

void RetractArmButtonState::enter() {
	Serial.println("Etat rangement du bras");
	time_start = millis();
	if(tiretteState.get_color() == GREEN){
		navigator.move_to(400,1125);
	}
	else{
		navigator.move_to(400,1875);
	}
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

void RetractArmButtonState::leave() {

}

void RetractArmButtonState::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(time_servo ==0){
			time_servo = millis();
			//arm.write(RETRACTED_ARM);
		}
		if (millis() - time_servo > SERVO_MOVEMENT_DURATION) {
			fsmSupervisor.setNextState(&deadState);
		}
	}

}

void RetractArmButtonState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(navigator.isTrajectoryFinished()){
		//arm.write(RETRACTED_ARM);
	}
	else{
		if(tiretteState.get_color() == GREEN){
			navigator.move_to(400,1125);
		}
		else{
			navigator.move_to(400,1875);
		}
	}
}

void RetractArmButtonState::forceLeave(){

}
