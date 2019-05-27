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
									{TURN,-192,0},
									{DISPLACEMENT,205,550},
};


float traj_rangement1_yellow[][3] = { {DISPLACEMENT,2500,570},
									{TURN,-32,0},
									{TURN,4,0},
									{DISPLACEMENT,2795,550},
};




PremierRangement::PremierRangement() {
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = US_RANGE;
	usDistances.rear_right = US_RANGE;
	angles.angleA = 80;
	angles.angleB = 100;
	trajectory_index = 0;
}

PremierRangement::~PremierRangement() {
	// TODO Auto-generated destructor stub
}

void PremierRangement::enter() {

	//Serial.println("Etat premiere recolte");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_rangement1_purple[0][1],traj_rangement1_purple[0][2]);
	}
	else{
		navigator.move_to(traj_rangement1_yellow[0][1],traj_rangement1_yellow[0][2]);
	}
}

void PremierRangement::leave() {

}

void PremierRangement::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(trajectory_index == 3){
			fsmSupervisor.setNextState(&premierRecalage);
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
			if(traj_rangement1_purple[trajectory_index][0]==DISPLACEMENT)
				/*if(trajectory_index == 4){
					usDistances.rear_left = 0;
					usDistances.rear_right = 0;
				}
				else*/
				navigator.move_to(traj_rangement1_purple[trajectory_index][1],traj_rangement1_purple[trajectory_index][2]);
			else if(traj_rangement1_purple[trajectory_index][0]==TURN)
				navigator.turn_to(traj_rangement1_purple[trajectory_index][1] );
		}
		else{
			trajectory_index += 1;
			if(traj_rangement1_yellow[trajectory_index][0]==DISPLACEMENT)
				navigator.move_to(traj_rangement1_yellow[trajectory_index][1],traj_rangement1_yellow[trajectory_index][2]);
			else if(traj_rangement1_yellow[trajectory_index][0]==TURN)
				navigator.turn_to(traj_rangement1_yellow[trajectory_index][1] );
		}
	}
}

void PremierRangement::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	/*if(digitalRead(COLOR) == GREEN){
		navigator.move_to(POS_X_WATER,POS_Y_WATER_GREEN);
	}
	else{
		navigator.move_to(POS_X_WATER,POS_Y_WATER_ORANGE);
	}*/
	Serial.println("reenter");
	/*if(digitalRead(COLOR) == GREEN){
		navigator.move_to(1500,-10000);
	}
	else{
		navigator.move_to(1500,-10000);
	}*/
}

void PremierRangement::forceLeave(){

}

