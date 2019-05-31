/*
 * Balance.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "05Balance.h"

#include "00TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

Balance balance = Balance();


float traj_balance_purple[][4] = { {DISPLACEMENT,175,1800, 1},
									  {TURN, 45,0, 0},
									  {TURN, 0,0, 0},
									  {DISPLACEMENT,500, 1800, 0},
									  {DISPLACEMENT, 1300, 1800, 0}
									  };


float traj_balance_yellow[][4] = { {DISPLACEMENT,2825,1800, 1},
								  {TURN, 135,0, 0},
								  {TURN, 180,0, 0},
								  {DISPLACEMENT,2500, 1800, 0},
								  {DISPLACEMENT, 1700, 1800, 0}};

Balance::Balance() {
	trajectory_index = 0;
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = lidar_ar1;
	angles.angleB = lidar_ar2;
}

Balance::~Balance() {
	// TODO Auto-generated destructor stub
}

void Balance::enter() {
	has_reentered = 0;
	Serial1.println("Etat 03 : Rangement chaos");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_balance_purple[0][1],traj_balance_purple[0][2]);
	}
	else{
		navigator.move_to(traj_balance_yellow[0][1],traj_balance_yellow[0][2]);
	}

}

void Balance::leave() {

}

void Balance::doIt() {
	if(trajectory_index <= 4){
			angles = zone_observation(traj_balance_purple[trajectory_index][3],  traj_balance_purple[trajectory_index][0]);
	}

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 3){
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
		}
		if(trajectory_index == 4){
			fsmSupervisor.setNextState(&deadState);
		}
		else{
			trajectory_index += 1;
			if(tiretteState.get_color() == PURPLE){
				if(traj_balance_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_balance_purple[trajectory_index][1],traj_balance_purple[trajectory_index][2]);
				}
				else if(traj_balance_purple[trajectory_index][0]==TURN){
					navigator.turn_to(traj_balance_purple[trajectory_index][1]);
				}

			}
			else{
				if(traj_balance_yellow[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_balance_yellow[trajectory_index][1],traj_balance_yellow[trajectory_index][2]);
				}
				else if(traj_balance_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(traj_balance_yellow[trajectory_index][1] );
				}
			}
		}
	}
}

void Balance::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		angles.angleA = lidar_ar1;
		angles.angleB = lidar_ar2;
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(traj_balance_purple[trajectory_index][1],traj_balance_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(traj_balance_yellow[trajectory_index][1],traj_balance_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void Balance::forceLeave(){

}

