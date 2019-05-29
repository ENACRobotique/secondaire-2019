/*
 * Homologationy.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "01Homologationy.h"
#include "01HomoRangement.h"
#include "00TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"

Homologationy homologationy = Homologationy();


float parcout_homoy[][3] = { {DISPLACEMENT,2850,1300},
									{TURN,135,0},
									{TURN,178,0},
									{DISPLACEMENT,2500,1300},
									{TURN,-140,0},
									{TURN,-94,0},
									{DISPLACEMENT,2500,350}
};


Homologationy::Homologationy() {
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

Homologationy::~Homologationy() {
	// TODO Auto-generated destructor stub
}

void Homologationy::enter() {
	has_reentered = 0;

	Serial.println("On entre dans l'état 1");

	Serial2.println("On entre dans l'état 1 YELLOW ");
	navigator.move_to(parcout_homoy[0][1],parcout_homoy[0][2]);


}

void Homologationy::leave() {
	delay(250);
}


void Homologationy::doIt() {

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 7){
			Serial.print("x  :  ");
			Serial.print(Odometry::get_pos_x());
			Serial.print("   y  :  ");
			Serial.println(Odometry::get_pos_y());
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			fsmSupervisor.setNextState(&homoRangement);
			return;
		}
		trajectory_index += 1;
		Serial.println("Entrée dans yellow");

		if(parcout_homoy[trajectory_index][0]==DISPLACEMENT and trajectory_index < 7){
			angles.angleA = lidar_av1;
			angles.angleB = lidar_av2;
			Serial.println("Entrée dans yellow DISPLACEMENT");
			Serial.print("consx  :  ");
			Serial.print(parcout_homoy[trajectory_index][1]);
			Serial.print("   consy  :  ");
			Serial.println(parcout_homoy[trajectory_index][2]);
			navigator.move_to(parcout_homoy[trajectory_index][1],parcout_homoy[trajectory_index][2]);
		}
		else if(parcout_homoy[trajectory_index][0]==TURN){
			angles.angleA = 0;
			angles.angleB = 359;
			navigator.turn_to(parcout_homoy[trajectory_index][1] );
		}
		if(trajectory_index == 3){
			Odometry::set_pos(2850,1300,180);
		}
		else if(trajectory_index == 6){
			Odometry::set_pos(2500,1300,-90);
		}

	}
}

void Homologationy::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
		angles.angleA = lidar_av1;
		angles.angleB = lidar_av2;
		navigator.move_to(parcout_homoy[0][1],parcout_homoy[0][2]);
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void Homologationy::forceLeave(){

}

