/*
 * deuxiemeRecalage.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */



#include "05RangementChaos.h"
#include "06DeuxiemeRecalage.h"
#include "00TiretteState.h"
#include "07AtomeMontee.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "../odometry.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "DeadState.h"

DeuxiemeRecalage deuxiemeRecalage = DeuxiemeRecalage();


float traj_recalage2_purple[][4] = { {DISPLACEMENT,600,400, 1},
									{TURN,135,0, 1},
									{TURN,90,0, 1},
									{DISPLACEMENT,600,1330, 1},
									{DISPLACEMENT,600,1400, 0},

};

float traj_recalage2_yellow[][4] = { {DISPLACEMENT,2400,400, 1},
									{TURN,135,0, 1},
									{TURN,90,0, 1},
									{DISPLACEMENT,2400,1330, 1},
									{DISPLACEMENT,2400,1400, 0},

};



DeuxiemeRecalage::DeuxiemeRecalage() {
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

DeuxiemeRecalage::~DeuxiemeRecalage() {
	// TODO Auto-generated destructor stub
}

void DeuxiemeRecalage::enter() {
	has_reentered = 0;
	//Serial.println("Etat premiere recalage");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recalage2_purple[0][1],traj_recalage2_purple[0][2]);
	}
	else{
		navigator.move_to(traj_recalage2_yellow[0][1],traj_recalage2_yellow[0][2]);
	}
}

void DeuxiemeRecalage::leave() {

}

void DeuxiemeRecalage::doIt() {
	if(trajectory_index <= 4){
		angles = zone_observation(traj_recalage2_purple[trajectory_index][3],  traj_recalage2_purple[trajectory_index][0]);
	}
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 4){
			if(tiretteState.get_color() == PURPLE){
				Odometry::set_pos(600,1378,90);
			}
			else{
				Odometry::set_pos(2400,1378,90); //TODO regarder les mesure du cote jaune
			}
			//fsmSupervisor.setNextState(&deadState);
			fsmSupervisor.setNextState(&atomeMontee);
		}
		else{
			if(tiretteState.get_color() == PURPLE){
				trajectory_index += 1;
				if(traj_recalage2_purple[trajectory_index][0]==DISPLACEMENT){
					/*if(trajectory_index == 3){
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}
					else if(trajectory_index == 4){
						angles.angleA = 0;
						angles.angleB = 0;
					}*/
					navigator.move_to(traj_recalage2_purple[trajectory_index][1],traj_recalage2_purple[trajectory_index][2]);
				}
				else if(traj_recalage2_purple[trajectory_index][0]==TURN){
					/*angles.angleA = 0;
					angles.angleB = 0;*/
					navigator.turn_to(traj_recalage2_purple[trajectory_index][1] );
				}

			}
			else{
				trajectory_index += 1;
				if(traj_recalage2_yellow[trajectory_index][0]==DISPLACEMENT){
					/*if(trajectory_index == 3){
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}
					else if(trajectory_index == 4){
						angles.angleA = 0;
						angles.angleB = 0;
					}*/
					navigator.move_to(traj_recalage2_yellow[trajectory_index][1],traj_recalage2_yellow[trajectory_index][2]);
				}
				else if(traj_recalage2_yellow[trajectory_index][0]==TURN){
					//angles.angleA = 0;
					//angles.angleB = 0;
					navigator.turn_to(traj_recalage2_yellow[trajectory_index][1] );
				}
			}
		}
	}
}

void DeuxiemeRecalage::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(traj_recalage2_purple[trajectory_index][1],traj_recalage2_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(traj_recalage2_yellow[trajectory_index][1],traj_recalage2_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}

}

void DeuxiemeRecalage::forceLeave(){

}

