/*
 * MonteeAtome.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "05MonteeAtome.h"

#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

MonteeAtome monteeAtome = MonteeAtome();


float montee_purple[][4] = {{DISPLACEMENT,600,1700, 0},
							 {DISPLACEMENT, 800, 1700, 0},
							 {DISPLACEMENT,600, 1700, 0},
							 {DISPLACEMENT, 1220, 1700, 0}};


float montee_yellow[][4] = {{DISPLACEMENT,2500, 450, 2}};


MonteeAtome::MonteeAtome() {
	time_start = 0;
	trajectory_index = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = US_RANGE;  //changer selon côté de la table
	usDistances.front_right = US_RANGE;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = 0;
	angles.angleB = 0;
}

MonteeAtome::~MonteeAtome() {
	// TODO Auto-generated destructor stub
}

void MonteeAtome::enter() {
	has_reentered = 0;
	if (tiretteState.get_color() == PURPLE) {
		Serial1.println("On entre dans l'état 4 côté PURPLE ");
		navigator.move_to(montee_purple[0][1],montee_purple[0][2]);
	}
	else{
		Serial1.println("On entre dans l'état 4 YELLOW ");
		navigator.move_to(montee_yellow[0][1],montee_yellow[0][2]);
	}

}

void MonteeAtome::leave() {
	delay(250);
}

void MonteeAtome::doIt() {
	if(trajectory_index <= 3){
		angles = zone_observation(montee_purple[trajectory_index][3],  montee_purple[trajectory_index][0]);
	}

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 2){
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
		}
		if(trajectory_index == 3){
			fsmSupervisor.setNextState(&deadState);
		}
		else{
			trajectory_index += 1;
			if(tiretteState.get_color() == PURPLE){
				if(montee_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(montee_purple[trajectory_index][1],montee_purple[trajectory_index][2]);
				}
				else if(montee_purple[trajectory_index][0]==TURN){
					navigator.turn_to(montee_purple[trajectory_index][1]);
				}

			}
			else{
				if(montee_yellow[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(montee_yellow[trajectory_index][1],montee_yellow[trajectory_index][2]);
				}
				else if(montee_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(montee_yellow[trajectory_index][1] );
				}
			}
		}
	}
}

void MonteeAtome::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(montee_purple[trajectory_index][1],montee_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(montee_yellow[trajectory_index][1],montee_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void MonteeAtome::forceLeave(){

}

