/*
 * PremierRangement.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "01PremiereRecolte.h"
#include "02PremierRangement.h"
#include "00TiretteState.h"
#include "03PremierRecalage.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

PremierRangement premierRangement = PremierRangement();

float traj_rangement1_purple[][3] = { {DISPLACEMENT,500,570},
									{TURN,-141,0},
									{TURN,178,0},
								{DISPLACEMENT,205,550},
};


float traj_rangement1_yellow[][2] = { {150,1200},
								{500,1200},
								{500,300}
};




PremierRangement::PremierRangement() {
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

PremierRangement::~PremierRangement() {
	// TODO Auto-generated destructor stub
}

void PremierRangement::enter() {
	has_reentered = 0;

	//Serial.println("Etat premiere recolte");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_rangement1_purple[0][1],traj_rangement1_purple[0][2]);
	}
	else{
		navigator.move_to(traj_rangement1_yellow[0][0],traj_rangement1_yellow[0][1]);
	}
}

void PremierRangement::leave() {

}

void PremierRangement::doIt() {
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 3){
			fsmSupervisor.setNextState(&premierRecalage);
			//fsmSupervisor.setNextState(&deadState);
			return;
		}
		if(trajectory_index == 2){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
			Odometry::set_pos(500, 550, 180);
		}
		if(tiretteState.get_color() == PURPLE){
			trajectory_index += 1;
			if(traj_rangement1_purple[trajectory_index][0]==DISPLACEMENT){
				if(trajectory_index == 4){
					usDistances.rear_left = 0;
					usDistances.rear_right = 0;
				}
				else{
				angles.angleA = 0;
				angles.angleB = 0;
				navigator.move_to(traj_rangement1_purple[trajectory_index][1],traj_rangement1_purple[trajectory_index][2]);
				}
			}
			else if(traj_rangement1_purple[trajectory_index][0]==TURN){
				angles.angleA = 0;
				angles.angleB = 0;
				navigator.turn_to(traj_rangement1_purple[trajectory_index][1] );
			}
		}
		else{
			navigator.turn_to(traj_rangement1_yellow[trajectory_index][1]);
			trajectory_index += 1;
			navigator.move_to(traj_rangement1_yellow[trajectory_index][0],traj_rangement1_yellow[trajectory_index][1]);
		}
	}
}

void PremierRangement::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		angles.angleA = lidar_ar1;
		angles.angleB = lidar_ar2;
		if(tiretteState.get_color() == PURPLE){
			navigator.move_to(traj_rangement1_purple[0][1],traj_rangement1_purple[0][2]);
		}
		else{
			navigator.move_to(traj_rangement1_yellow[0][0],traj_rangement1_yellow[0][1]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}

}

void PremierRangement::forceLeave(){

}

