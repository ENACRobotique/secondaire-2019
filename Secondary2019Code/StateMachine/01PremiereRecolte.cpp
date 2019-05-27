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
									{TURN,44,0},
									{TURN,-2,0},
								{DISPLACEMENT,500,1300},
								{TURN,-45,0},
								{TURN,-90,0},
								{DISPLACEMENT,500,350}
};


float traj_recolte1_yellow[][2] = { {150,1200},  // TODO
								{500,1200},
								{500,300}
};

float turn_recolte1_yellow[] = {0,-90};


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


	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recolte1_purple[0][1],traj_recolte1_purple[0][2]);
	}
	else{
		navigator.move_to(traj_recolte1_yellow[0][0],traj_recolte1_yellow[0][1]);
	}

}

void PremiereRecolte::leave() {
	delay(250);
}

void PremiereRecolte::doIt() {

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 7){
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			fsmSupervisor.setNextState(&premierRangement);
			return;
		}
		if(tiretteState.get_color() == PURPLE){
			trajectory_index += 1;
			if(traj_recolte1_purple[trajectory_index][0]==DISPLACEMENT){
				angles.angleA = lidar_av1;
				angles.angleB = lidar_av2;
				navigator.move_to(traj_recolte1_purple[trajectory_index][1],traj_recolte1_purple[trajectory_index][2]);
			}
			else if(traj_recolte1_purple[trajectory_index][0]==TURN){
				angles.angleA = 0;
				angles.angleB = 0;
				navigator.turn_to(traj_recolte1_purple[trajectory_index][1] );
				if(trajectory_index == 3){
					Odometry::set_pos(150, 1300, 0);
				}
			}
		}
		else{
			navigator.turn_to(turn_recolte1_yellow[trajectory_index]);
			trajectory_index += 1;
			navigator.move_to(traj_recolte1_yellow[trajectory_index][0],traj_recolte1_yellow[trajectory_index][1]);
		}
	}
}

void PremiereRecolte::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;

	if(trajectory_index == 0){
		if(tiretteState.get_color() == PURPLE){
				angles.angleA = lidar_av1;
				angles.angleB = lidar_av2;
				navigator.move_to(traj_recolte1_purple[0][1],traj_recolte1_purple[0][2]);
			}
			else{
				angles.angleA = lidar_av1;
				angles.angleB = lidar_av2;
				navigator.move_to(traj_recolte1_yellow[0][0],traj_recolte1_yellow[0][1]);
			}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void PremiereRecolte::forceLeave(){

}

