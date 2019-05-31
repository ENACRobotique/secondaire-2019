/*
 * BasculePalets.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "04BasculePalets.h"
#include "05MonteeAtome.h"

#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

BasculePalets basculePalets = BasculePalets();


float bascule_purple[][4] = {{DISPLACEMENT,175,1835, 0},
							 {DISPLACEMENT, 175, 1700, 0},
							 {DISPLACEMENT,175, 2050, 0},
							 {DISPLACEMENT, 175, 1800, 0},
							 {TURN, 135, 0, 0},
							 {TURN, 180, 0, 0}};


float bascule_yellow[][4] = {{DISPLACEMENT,2500, 450, 2}};


BasculePalets::BasculePalets() {
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

BasculePalets::~BasculePalets() {
	// TODO Auto-generated destructor stub
}

void BasculePalets::enter() {
	has_reentered = 0;

	if (tiretteState.get_color() == PURPLE) {
		mandibuleGauche.write(MANDIBULE_GAUCHE_INTER);
		mandibuleDroite.write(MANDIBULE_DROITE_INTER);
		Serial1.println("On entre dans l'état 4 côté PURPLE ");
		navigator.move_to(bascule_purple[0][1],bascule_purple[0][2]);
	}
	else{
		Serial1.println("On entre dans l'état 4 YELLOW ");
		navigator.move_to(bascule_yellow[0][1],bascule_yellow[0][2]);
	}

}

void BasculePalets::leave() {
	delay(250);
}

void BasculePalets::doIt() {
	if(trajectory_index <= 5){
				angles = zone_observation(bascule_purple[trajectory_index][3],  bascule_purple[trajectory_index][0]);
	}

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 1){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
		}
		if(trajectory_index == 5){
			fsmSupervisor.setNextState(&monteeAtome);
		}
		else{
			trajectory_index += 1;
			if(tiretteState.get_color() == PURPLE){
				if(bascule_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(bascule_purple[trajectory_index][1],bascule_purple[trajectory_index][2]);
				}
				else if(bascule_purple[trajectory_index][0]==TURN){
					navigator.turn_to(bascule_purple[trajectory_index][1]);
				}

			}
			else{
				if(bascule_yellow[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(bascule_yellow[trajectory_index][1],bascule_yellow[trajectory_index][2]);
				}
				else if(bascule_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(bascule_yellow[trajectory_index][1] );
				}
			}
		}
	}
}

void BasculePalets::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(bascule_purple[trajectory_index][1],bascule_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(bascule_yellow[trajectory_index][1],bascule_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void BasculePalets::forceLeave(){

}

