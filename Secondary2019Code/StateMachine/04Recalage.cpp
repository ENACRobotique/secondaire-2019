/*
 * Recalage.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "04Recalage.h"
#include "05Balance.h"


#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

Recalage recalage = Recalage();


float traj_recalage_purple[][4] = { {DISPLACEMENT, 350, 1050, 1},
									  {TURN, -45,0, 0},
									  {TURN, 0,0, 0},
									  {DISPLACEMENT,-75, 1050, 0},
									  {DISPLACEMENT, 175, 1050, 1},
									  {TURN, 45,0, 0},
									  {TURN, 90,0, 0}};


float traj_recalage_yellow[][4] = { {DISPLACEMENT, 2650, 1050, 1},
									  {TURN, -135,0, 0},
									  {TURN, -180,0, 0},
									  {DISPLACEMENT,3075, 1050, 0},
									  {DISPLACEMENT, 2825, 1050, 1},
									  {TURN, 135,0, 0},
									  {TURN, 90,0, 0}};

Recalage::Recalage() {
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

Recalage::~Recalage() {
	// TODO Auto-generated destructor stub
}

void Recalage::enter() {
	has_reentered = 0;
	Serial1.println("Etat 03 : Rangement chaos");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recalage_purple[0][1],traj_recalage_purple[0][2]);
	}
	else{
		navigator.move_to(traj_recalage_yellow[0][1],traj_recalage_yellow[0][2]);
	}

}

void Recalage::leave() {

}

void Recalage::doIt() {
	if(trajectory_index <= 6){
			angles = zone_observation(traj_recalage_purple[trajectory_index][3],  traj_recalage_purple[trajectory_index][0]);
	}

	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 6){
			fsmSupervisor.setNextState(&balance);
		}
		else{
			trajectory_index += 1;
			if(trajectory_index == 4){
				if(tiretteState.get_color() == PURPLE){
					Odometry::set_pos(0, 1050, 0);
				}
				else{
					Odometry::set_pos(3000, 1050, 180);
				}
			}
			if(tiretteState.get_color() == PURPLE){
				if(traj_recalage_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_recalage_purple[trajectory_index][1],traj_recalage_purple[trajectory_index][2]);
				}
				else if(traj_recalage_purple[trajectory_index][0]==TURN){
					navigator.turn_to(traj_recalage_purple[trajectory_index][1]);
				}

			}
			else{
				if(traj_recalage_yellow[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_recalage_yellow[trajectory_index][1],traj_recalage_yellow[trajectory_index][2]);
				}
				else if(traj_recalage_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(traj_recalage_yellow[trajectory_index][1] );
				}
			}
		}
	}
}

void Recalage::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		angles.angleA = lidar_ar1;
		angles.angleB = lidar_ar2;
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(traj_recalage_purple[trajectory_index][1],traj_recalage_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(traj_recalage_yellow[trajectory_index][1],traj_recalage_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void Recalage::forceLeave(){

}

