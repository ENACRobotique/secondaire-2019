/*
 * MoveToButtonState.cpp
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#include "MoveToButtonState.h"
#include "MoveLaunchButtonState.h"
#include "TurnToButtonState.h"
#include "TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"

MoveToButtonState moveToButtonState = MoveToButtonState();

float traj_button_green[][2] = {	{500,850},
									{500,1150}
};
float traj_button_orange[][2] = {	{500,2150},
									{500,1850}
};


MoveToButtonState::MoveToButtonState() {
	time_start = 0;
	trajectory_index = 0;
	time_servo = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
}

MoveToButtonState::~MoveToButtonState() {
	// TODO Auto-generated destructor stub
}

void MoveToButtonState::enter() {
	Serial.println("Etat déplacement vers le bouton");
	if(tiretteState.get_color() == GREEN){
		navigator.move_to(traj_button_green[0][0],traj_button_green[0][1]);
	}
	else{
		navigator.move_to(traj_button_orange[0][0],traj_button_orange[0][1]);
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
	time_start = millis();
}

void MoveToButtonState::leave() {

}

void MoveToButtonState::doIt() {
	if(navigator.isTrajectoryFinished()){
			if(trajectory_index == 1){
				fsmSupervisor.setNextState(&turnToButtonState);
			}
			else{
				trajectory_index+=1;
				if(tiretteState.get_color() == GREEN){
					navigator.move_to(traj_button_green[trajectory_index][0],traj_button_green[trajectory_index][1]);
				}
				else{
					navigator.move_to(traj_button_orange[trajectory_index][0],traj_button_orange[trajectory_index][1]);
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
		}
}

void MoveToButtonState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(tiretteState.get_color() == GREEN){
		navigator.move_to(traj_button_green[trajectory_index][0],traj_button_green[trajectory_index][1]);
	}
	else{
		navigator.move_to(traj_button_orange[trajectory_index][0],traj_button_orange[trajectory_index][1]);
	}
}

void MoveToButtonState::forceLeave(){

}
