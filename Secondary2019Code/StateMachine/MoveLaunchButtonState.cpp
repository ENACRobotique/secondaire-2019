/*
 * MoveLaunchButtonState.cpp
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#include "MoveLaunchButtonState.h"
#include "RetractArmButtonState.h"
#include "TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"

MoveLaunchButtonState moveLaunchButtonState = MoveLaunchButtonState();

MoveLaunchButtonState::MoveLaunchButtonState() {
	time_start = 0;
	time_servo = 0;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
}

MoveLaunchButtonState::~MoveLaunchButtonState() {
	// TODO Auto-generated destructor stub
}

void MoveLaunchButtonState::enter() {
	Serial.println("Etat activation du panneau");
	time_start = millis();
	if(tiretteState.get_color() == GREEN){
		navigator.move_to(150,1125);
	}
	else{
		navigator.move_to(150,1875);
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

void MoveLaunchButtonState::leave() {

}

void MoveLaunchButtonState::doIt() {
	if(navigator.isTrajectoryFinished()){
		fsmSupervisor.setNextState(&retractArmButtonState);
	}
}

void MoveLaunchButtonState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(millis() - time_start > SERVO_MOVEMENT_DURATION){
		if(tiretteState.get_color() == GREEN){
			navigator.move_to(240,1125);
		}
		else{
			navigator.move_to(240,1875);
		}
	}
	else{
		//arm.write(EXTENDED_ARM);
	}
}

void MoveLaunchButtonState::forceLeave(){

}
