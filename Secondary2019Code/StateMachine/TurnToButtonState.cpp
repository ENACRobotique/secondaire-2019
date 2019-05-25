 /*
* TurnToButtonState.cpp
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#include "TurnToButtonState.h"
#include "MoveLaunchButtonState.h"
#include "DeadState.h"
#include "TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"

TurnToButtonState turnToButtonState = TurnToButtonState();

TurnToButtonState::TurnToButtonState() {
	time_start = 0;
	time_servo = 0;
}

TurnToButtonState::~TurnToButtonState() {
	// TODO Auto-generated destructor stub
}

void TurnToButtonState::enter() {
	Serial.println("Etat rotation vers l'interrupteur");
	navigator.turn_to(180);
	time_start = millis();
}

void TurnToButtonState::leave() {

}

void TurnToButtonState::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(time_servo == 0){
			time_servo = millis();
			//arm.write(EXTENDED_ARM);
		}
		if(millis() - time_servo > SERVO_MOVEMENT_DURATION){
			fsmSupervisor.setNextState(&moveLaunchButtonState);
		}
	}
}

void TurnToButtonState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	navigator.turn_to(180);
}

void TurnToButtonState::forceLeave(){

}
