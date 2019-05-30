/*
 * PremiereRecolte.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "01PremiereRecolte.h"
#include "02RecolteChaos.h"

#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

PremiereRecolte premiereRecolte = PremiereRecolte();


float parcourt[][4] = { {DISPLACEMENT,500,450, 1},     // i = 3 : si 1, lidar activé sinon désactivé
						{TURN, 45,0, 0},
						{TURN, 90,0, 0},
						{DISPLACEMENT,500,1100, 1},
						{DISPLACEMENT,500,550, 1},
						{TURN,135,0, 0},
						{TURN,180,0, 0},
						{DISPLACEMENT,200,550, 1}};


float parcourt_yellow[][4] = {  {DISPLACEMENT,2500, 450, 1},
								{TURN, 135,0, 0},
								{TURN, 90,0, 0},
								{DISPLACEMENT,2500,1100, 1},
								{DISPLACEMENT,2500,550, 1},
								{TURN, 45,0, 0},
								{TURN, 0,0, 0},
								{DISPLACEMENT,2800, 550, 1}};


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
	if (tiretteState.get_color() == PURPLE) {
		Serial1.println("On entre dans l'état 1 côté PURPLE ");
		navigator.move_to(parcourt[0][1],parcourt[0][2]);
	}
	else{
		Serial1.println("On entre dans l'état 1 YELLOW ");
		navigator.move_to(parcourt_yellow[0][1],parcourt_yellow[0][2]);
	}

}

void PremiereRecolte::leave() {
	delay(250);
}

void PremiereRecolte::doIt() {

	if(trajectory_index == 8){
					Serial.print("x  :  ");
					Serial.print(Odometry::get_pos_x());
					Serial.print("   y  :  ");
					Serial.println(Odometry::get_pos_y());
					fsmSupervisor.setNextState(&recolteChaos);
					//fsmSupervisor.setNextState(&deadState);
					return;
	}

	if(trajectory_index <= 7){
		angles = zone_observation(parcourt[trajectory_index][3],  parcourt[trajectory_index][0]);
	}

	if(trajectory_index == 4){
		mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
		mandibuleDroite.write(MANDIBULE_DROITE_BAS);
	}

	if(trajectory_index == 7){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
		}


	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		trajectory_index += 1;
		if (tiretteState.get_color() == PURPLE) {
			Serial.print("Entrée dans purple");
			if(parcourt[trajectory_index][0]==DISPLACEMENT  and trajectory_index <= 7){
				navigator.move_to(parcourt[trajectory_index][1],parcourt[trajectory_index][2]);
			}
			else if(parcourt[trajectory_index][0]==TURN){
				navigator.turn_to(parcourt[trajectory_index][1] );
			}
		}
		else{
				Serial.println("Entrée dans yellow");
				if(parcourt_yellow[trajectory_index][0]==DISPLACEMENT and trajectory_index <= 7){
					navigator.move_to(parcourt_yellow[trajectory_index][1],parcourt_yellow[trajectory_index][2]);
				}
				else if(parcourt_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(parcourt_yellow[trajectory_index][1] );
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

