/*
 * PremiereRecolte.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "01PremiereRecolte.h"
#include "02PremierRangement.h"
#include "00TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"

PremiereRecolte premiereRecolte = PremiereRecolte();

float parcourt[][3] = { {DISPLACEMENT,150,1300},
									{TURN,44,0},
									{TURN,-2,0},
									{DISPLACEMENT,500,1300},
									{TURN,-45,0},
									{TURN,-90,0},
									{DISPLACEMENT,500,350}
};


PremiereRecolte::PremiereRecolte() {
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

PremiereRecolte::~PremiereRecolte() {
	// TODO Auto-generated destructor stub
}

void PremiereRecolte::enter() {
	has_reentered = 0;

	Serial.println("On entre dans l'état 1 côté PURPLE ");
	navigator.move_to(parcourt[0][1],parcourt[0][2]);

}

void PremiereRecolte::leave() {
	delay(250);
}

void PremiereRecolte::doIt() {

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
		Serial.print("couleur : ");
		Serial.println(tiretteState.get_color());
		trajectory_index += 1;
		if(parcourt[trajectory_index][0]==DISPLACEMENT  and trajectory_index < 7){
			angles.angleA = lidar_av1;
			angles.angleB = lidar_av2;
			navigator.move_to(parcourt[trajectory_index][1],parcourt[trajectory_index][2]);
		}
		else if(parcourt[trajectory_index][0]==TURN){
			angles.angleA = 0;
			angles.angleB = 0;
			navigator.turn_to(parcourt[trajectory_index][1] );
			if(trajectory_index == 3){
				Odometry::set_pos(150, 1300, 0);
			}
		}

	}
}

void PremiereRecolte::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
			angles.angleA = lidar_av1;
			angles.angleB = lidar_av2;
			navigator.move_to(parcourt[0][1],parcourt[0][2]);
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void PremiereRecolte::forceLeave(){

}

