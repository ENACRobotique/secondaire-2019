/*
 * HomoRangement.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "01HomoRangement.h"
#include "00TiretteState.h"
#include "DeadState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

HomoRangement homoRangement = HomoRangement();

float trajp[][3] = { {DISPLACEMENT,500,570},
									{TURN,-141,0},
									{TURN,-192,0},
									{DISPLACEMENT,205,550},
};


float trajy[][3] = { {DISPLACEMENT,2500,570},
									{TURN,-32,0},
									{TURN,4,0},
									{DISPLACEMENT,2795,550},
};




HomoRangement::HomoRangement() {
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = US_RANGE;
	usDistances.rear_right = US_RANGE;
	angles.angleA = lidar_ar1;
	angles.angleB = lidar_ar2;
	trajectory_index = 0;
}

HomoRangement::~HomoRangement() {
	// TODO Auto-generated destructor stub
}

void HomoRangement::enter() {
	has_reentered = 0;
	//digitalWrite(13, HIGH);

	//Serial.println("Etat homoe recolte");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(trajp[0][1],trajp[0][2]);
	}
	else{
		navigator.move_to(trajy[0][1],trajy[0][2]);
	}
}

void HomoRangement::leave() {

}

void HomoRangement::doIt() {
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 3){
			fsmSupervisor.setNextState(&deadState);
			//fsmSupervisor.setNextState(&deadState);
			return;
		}
		if(trajectory_index == 2){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
			if(tiretteState.get_color() == PURPLE){
				Odometry::set_pos(500, 550, 180);
			}
			else{
				Odometry::set_pos(2500, 550, 0);
			}
		}
		if(tiretteState.get_color() == PURPLE){
			trajectory_index += 1;
			if(trajp[trajectory_index][0]==DISPLACEMENT){
				if(trajectory_index == 4){
					usDistances.rear_left = 0;
					usDistances.rear_right = 0;
				}
				else{
					angles.angleA = 0;
					angles.angleB = 359;
					navigator.move_to(trajp[trajectory_index][1],trajp[trajectory_index][2]);
				}
			}
			else if(trajp[trajectory_index][0]==TURN){
				angles.angleA = 0;
				angles.angleB = 359;
				navigator.turn_to(trajp[trajectory_index][1] );
			}
		}
		else{
			trajectory_index += 1;
			if(trajy[trajectory_index][0]==DISPLACEMENT){
				if(trajectory_index != 4){
					angles.angleA = 0;
					angles.angleB = 359;
					navigator.move_to(trajy[trajectory_index][1],trajy[trajectory_index][2]);
				}
			}
			else if(trajy[trajectory_index][0]==TURN){
				angles.angleA = 0;
				angles.angleB = 359;
				navigator.turn_to(trajy[trajectory_index][1]);
			}
		}
	}
}

void HomoRangement::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		angles.angleA = lidar_ar1;
		angles.angleB = lidar_ar2;
		if(tiretteState.get_color() == PURPLE){
			navigator.move_to(trajp[0][1],trajp[0][2]);
		}
		else{
			navigator.move_to(trajy[0][0],trajy[0][1]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}

}

void HomoRangement::forceLeave(){

}

