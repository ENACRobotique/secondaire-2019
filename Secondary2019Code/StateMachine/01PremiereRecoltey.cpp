/*
 * PremiereRecoltey.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "01PremiereRecoltey.h"
#include "02PremierRangement.h"
#include "00TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"

PremiereRecoltey premiereRecoltey = PremiereRecoltey();


float parcourt_yellow[][3] = { {DISPLACEMENT,2850,1300},
									{TURN,135,0},
									{TURN,178,0},
									{DISPLACEMENT,2500,1300},
									{TURN,-140,0},
									{TURN,-94,0},
									{DISPLACEMENT,2500,350}
};


PremiereRecoltey::PremiereRecoltey() {
	time_start = 0;
	trajectory_index = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = US_RANGE;  //changer selon côté de la table
	usDistances.front_right = US_RANGE;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = lidar_av1;
	angles.angleB = lidar_av2;
}

PremiereRecoltey::~PremiereRecoltey() {
	// TODO Auto-generated destructor stub
}

void PremiereRecoltey::enter() {
	has_reentered = 0;

	Serial.println("On entre dans l'état 1");

	if(tiretteState.get_color() == YELLOW){
		Serial.println("On entre dans l'état 1 YELLOW ");
		navigator.move_to(parcourt_yellow[0][1],parcourt_yellow[0][2]);
	}

}

void PremiereRecoltey::leave() {
	delay(250);
}


void PremiereRecoltey::doIt() {

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 7){
			Serial.print("x  :  ");
			Serial.print(Odometry::get_pos_x());
			Serial.print("   y  :  ");
			Serial.println(Odometry::get_pos_y());
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			fsmSupervisor.setNextState(&premierRangement);
			return;
		}
		trajectory_index += 1;
		Serial.println("Entrée dans yellow");

		if(parcourt_yellow[trajectory_index][0]==DISPLACEMENT and trajectory_index < 7){
			angles.angleA = lidar_av1;
			angles.angleB = lidar_av2;
			Serial.println("Entrée dans yellow DISPLACEMENT");
			Serial.print("consx  :  ");
			Serial.print(parcourt_yellow[trajectory_index][1]);
			Serial.print("   consy  :  ");
			Serial.println(parcourt_yellow[trajectory_index][2]);
			navigator.move_to(parcourt_yellow[trajectory_index][1],parcourt_yellow[trajectory_index][2]);
		}
		else if(parcourt_yellow[trajectory_index][0]==TURN){
			angles.angleA = 0;
			angles.angleB = 0;
			navigator.turn_to(parcourt_yellow[trajectory_index][1] );
		}
		if(trajectory_index == 3){
			Odometry::set_pos(2850,1300,180);
		}
		else if(trajectory_index == 6){
			Odometry::set_pos(2500,1300,-90);
		}

	}
}

void PremiereRecoltey::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
		if(tiretteState.get_color() == YELLOW){
				angles.angleA = lidar_av1;
				angles.angleB = lidar_av2;
				navigator.move_to(parcourt_yellow[0][0],parcourt_yellow[0][1]);
			}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void PremiereRecoltey::forceLeave(){

}

