/*
 * FSMSupervisor.cpp
 *
 *  Created on: 11 avr. 2018
 *      Author: robot
 */

#include "FSMSupervisor.h"
#include "Arduino.h"
#include "AbstractState.h"
#include "00TiretteState.h"
#include "DeadState.h"
#include "PauseState.h"
#include "../params.h"
#include "../lib/USManager.h"
#include "../motorControl.h"
#include "../Navigator.h"


struct Angles zone_observation(int activation, float type_mouvement){

	struct Angles angles;
	if(type_mouvement == DISPLACEMENT){
		if(activation == 1 && MotorControl::get_cons_speed() > 0){
			angles.angleA = lidar_av1;
			angles.angleB = lidar_av2;
		}
		else if(activation == 1 && MotorControl::get_cons_speed() < 0){
			angles.angleA = lidar_ar1;
			angles.angleB = lidar_ar2;
		}
		else{
			angles.angleA = 0;
			angles.angleB = 0;
		}
	}
	else if(type_mouvement == TURN){
		if(activation == 1 && MotorControl::get_cons_omega() != 0){
			angles.angleA = lidar_turn1;
			angles.angleB = lidar_turn2;
		}
		else{
			angles.angleA = 0;
			angles.angleB = 0;
		}
	}

	return(angles);
}



FSMSupervisor fsmSupervisor = FSMSupervisor();

FSMSupervisor::FSMSupervisor() : isLidarSet(false), lidarPWM(25), lidarAsservTime(500){
	// TODO Auto-generated constructor stub
	nextState = NULL;
	currentState = NULL;
	previousState = NULL;
	time_obstacle_left = 0;
	deb = millis();

}

FSMSupervisor::~FSMSupervisor() {
	// TODO Auto-generated destructor stub
}

void FSMSupervisor::setNextState(AbstractState* state) {
	nextState = state;
}

void FSMSupervisor::update() {
	if (millis() - tiretteState.get_time_start() > TIME_RACE){
		fsmSupervisor.setNextState(&deadState);
	}

	if(nextState != NULL && nextState != currentState){
		currentState->leave();
		nextState->enter();
		currentState = nextState;
		nextState = NULL;
		Serial2.println("ON CHANGE D'ÉTAT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}

	currentState->doIt();


	if(Serial2.available()){
		lidarManager.update();
	}

	if((millis() - deb > 75)){
		deb = millis();
		//Serial2.print("Speed :    ");
		//Serial2.println(MotorControl::get_cons_speed());

		unsigned int angleA = currentState->getAngles().angleA;
		unsigned int angleB = currentState->getAngles().angleB;
		/*Serial1.print("A   ");
		Serial1.print(angleA);
		Serial1.print("    B   ");
		Serial1.println(angleB);*/
		//Serial.println(lidarManager.obstacleDetected(angleA, angleB));
		if(lidarManager.obstacleDetected(angleA, angleB)){
			time_obstacle_left = 0;
			Serial1.println("Obstacle detected");
			if(currentState != &pauseState){			// on va dans l'état pause
				currentState->forceLeave();
				previousState = currentState;
				currentState = &pauseState;
				pauseState.enter();
			}

		}
		else {
			if(currentState == &pauseState && previousState != NULL){		// on revient dans l'état précédent !
				if(time_obstacle_left == 0){
					time_obstacle_left = millis();
				}
				if(millis() - time_obstacle_left > DETECTION_STOP_TIME){ //Permet de ne pas repartir directement quand on ne détecte plus d'adversaires
					pauseState.leave();
					previousState->reEnter(pauseState.getPauseTime());
					currentState = previousState;
					previousState = &pauseState;
					time_obstacle_left = 0; //May be useless (just like the primary)
				}
			}
		}
	}
	if (!isLidarSet && lidarAsservTime.check()){
		Serial1.print("Speed:");
		Serial1.println(lidarManager.lidar.getSpeed());
		if (lidarManager.lidar.getSpeed() >= 260){
			lidarPWM -= 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else if (lidarManager.lidar.getSpeed() <= 230){
			lidarPWM += 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else{
			isLidarSet = true;
		}
	}
	//TODO On a besoin de ça pour les ultrasons et pour re rentrer
	/*if(currentState->getFlags() & E_ULTRASOUND){
		usManager.update();
		if(usManager.obstacleDetected()){
			time_obstacle_left = 0;
			if(currentState != &pauseState){			// on va dans l'état pause
				currentState->forceLeave();
				previousState = currentState;
				currentState = &pauseState;
				pauseState.enter();
			}

		}
		else {
			if(currentState == &pauseState && previousState != NULL){		// on revient dans l'état précédent !
				if(time_obstacle_left == 0){
					time_obstacle_left = millis();
				}
				if(millis() - time_obstacle_left > DETECTION_STOP_TIME){ //Permet de ne pas repartir directement quand on ne détecte plus d'adversaires
					pauseState.leave();
					previousState->reEnter(pauseState.getPauseTime());
					currentState = previousState;
					previousState = &pauseState;
					time_obstacle_left = 0; //May be useless (just like the primary)
				}
			}
		}
	}*/
}

void FSMSupervisor::init(AbstractState* state) {
	currentState = state;
	Serial.println("INIT FMS");
	state->enter();
}
