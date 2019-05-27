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

float traj_recolte1_purple[][3] = { {DISPLACEMENT,150,1300},
									{TURN,45,0},
									{TURN,0,0},
									{DISPLACEMENT,500,1300},
									{TURN,-45,0},
									{TURN,-90,0},
									{DISPLACEMENT,500,350}
};


float traj_recolte1_yellow[][3] = { {DISPLACEMENT,2850,1300},
									{TURN,135,0},
									{TURN,178,0},
									{DISPLACEMENT,2500,1300},
									{TURN,-140,0},
									{TURN,-90,0},
									{DISPLACEMENT,2500,350}
};


PremiereRecolte::PremiereRecolte() {
	time_start = 0;
	trajectory_index = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = US_RANGE;  //changer selon côté de la table
	usDistances.front_right = US_RANGE;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = 80;
	angles.angleB = 100;
}

PremiereRecolte::~PremiereRecolte() {
	// TODO Auto-generated destructor stub
}

void PremiereRecolte::enter() {
	Serial.println("On entre dans l'état 1");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recolte1_purple[0][1],traj_recolte1_purple[0][2]);
		/*Serial.print(trajectory_index);
		Serial.print("    ");
		Serial.print(Odometry::get_pos_x());
		Serial.print("    ");
		Serial.print(Odometry::get_pos_y());
		Serial.print("    ");
		Serial.println(Odometry::get_pos_theta());*/
	}
	else{
		navigator.move_to(traj_recolte1_yellow[0][1],traj_recolte1_yellow[0][2]);
	}

}

void PremiereRecolte::leave() {
	delay(250);
}

void PremiereRecolte::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(trajectory_index == 7){
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			fsmSupervisor.setNextState(&premierRangement);
			return;
		}

		if(tiretteState.get_color() == PURPLE){
			trajectory_index += 1;
			if(traj_recolte1_purple[trajectory_index][0]==DISPLACEMENT)
				navigator.move_to(traj_recolte1_purple[trajectory_index][1],traj_recolte1_purple[trajectory_index][2]);
			else if(traj_recolte1_purple[trajectory_index][0]==TURN)
				navigator.turn_to(traj_recolte1_purple[trajectory_index][1] );
		}
		else{
			trajectory_index += 1;
			if(traj_recolte1_yellow[trajectory_index][0]==DISPLACEMENT)
				navigator.move_to(traj_recolte1_yellow[trajectory_index][1],traj_recolte1_yellow[trajectory_index][2]);
			else if(traj_recolte1_yellow[trajectory_index][0]==TURN)
				navigator.turn_to(traj_recolte1_yellow[trajectory_index][1] );
			if(trajectory_index == 3){
				Odometry::set_pos(2850,1300,180);
			}
			if(trajectory_index == 6){
				Odometry::set_pos(2500,1300,-90);
			}
		}
	}
}

void PremiereRecolte::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

}

void PremiereRecolte::forceLeave(){

}

