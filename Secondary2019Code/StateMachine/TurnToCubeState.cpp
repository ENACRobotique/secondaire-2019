 /*
* TurnToCubeState.cpp
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#include "TurnToCubeState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "DeadState.h"
#include "FSMSupervisor.h"

TurnToCubeState turnToCubeState = TurnToCubeState();

TurnToCubeState::TurnToCubeState() {
	time_start = 0;
}

TurnToCubeState::~TurnToCubeState() {
	// TODO Auto-generated destructor stub
}

void TurnToCubeState::enter() {
	Serial.println("Etat rotation vers les cubes");
	navigator.turn_to(0);
	time_start = millis();
}

void TurnToCubeState::leave() {

}

void TurnToCubeState::doIt() {
	if(navigator.isTrajectoryFinished()){
		fsmSupervisor.setNextState(&deadState);
	}
}

void TurnToCubeState::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	navigator.turn_to(0);
}

void TurnToCubeState::forceLeave(){

}
